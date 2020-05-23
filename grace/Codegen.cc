#include "Context.hh"
#include "iostream"
#include "llvm/IR/Verifier.h"

using namespace llvm;
using namespace grace;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          LLVMContext &TheContext,
                                          const std::string &Name,
                                          llvm::Type *T) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(T, nullptr, Name);
}

Value *BlockNode::codegen(Context &C) {
  for (auto &Stmt : Stmts)
    Stmt->codegen(C);

  return nullptr;
}

Value *LiteralIntNode::codegen(Context &C) {
  return ConstantInt::get(C.getContext(), APInt(INT_SIZE, IVal));
}

Value *LiteralBoolNode::codegen(Context &C) {
  LLVMContext &TheContext = C.getContext();
  return ConstantInt::get(TheContext, APInt(BOOL_SIZE, BVal ? 1 : 0));
}

Value *IfThenElseNode::codegen(Context &C) {

  auto &Builder = C.getBuilder();
  auto &TheContext = C.getContext();

  auto TheFunction = Builder.GetInsertBlock()->getParent();

  auto ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  auto MergeBB = BasicBlock::Create(TheContext, "merge", TheFunction);
  auto LastBB =
      Else ? BasicBlock::Create(TheContext, "else", TheFunction) : MergeBB;

  auto CondV = Condition->codegen(C);
  auto CondVTy = Type::from(CondV->getType());

  if (!CondVTy->isBoolTy()) {
    Log::error(Condition->loc.begin) << "'" << CondVTy->str() << "' is not convertible to '"
                     << Type::boolTy()->str() << "'\n";
    return nullptr;
  }

  Builder.CreateCondBr(CondV, ThenBB, LastBB);

  Builder.SetInsertPoint(ThenBB);
  C.ST.enterScope();
  Then->codegen(C);
  C.ST.leaveScope();

  Builder.CreateBr(MergeBB);

  if (Else) {
    Builder.SetInsertPoint(LastBB);
    C.ST.enterScope();
    Else->codegen(C);
    C.ST.leaveScope();

    Builder.CreateBr(MergeBB);
  }

  Builder.SetInsertPoint(MergeBB);

  return nullptr;
}

Value *FuncDeclNode::codegen(Context &C) {
  auto Sym = dynamic_cast<FuncSymbol *>(C.ST.get(Name));

  // Check if function is already defined.
  if (Sym) {
    Log::error(loc.begin) << "function " << Name << " already defined\n";
    return nullptr;
  }

  // Create vector with llvm types for args.
  std::vector<llvm::Type *> ArgsType;
  ArgsType.reserve(Args->size());
  for (auto Arg : *Args)
    ArgsType.push_back(Arg->Ty->emit(C));

  // Create function signature.
  FunctionType *FT = FunctionType::get(ReturnTy->emit(C), ArgsType, false);
  auto F = Function::Create(FT, GlobalValue::LinkageTypes::ExternalLinkage,
                            Name, &C.getModule());

  // set args
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName((*Args)[Idx++]->Id);

  BasicBlock *BB = BasicBlock::Create(C.getContext(), "entry", F);
  C.getBuilder().SetInsertPoint(BB);

  std::vector<Type *> ArgsTy;
  for (auto Arg : *Args)
    ArgsTy.push_back(Arg->Ty);

  C.ST.set(Name, new FuncSymbol(F, ReturnTy, ArgsTy));
  C.ST.enterScope();

  // insert args into scope
  Idx = 0;
  for (auto &Arg : F->args()) {
    AllocaInst *Alloca =
        CreateEntryBlockAlloca(F, C.getContext(), Arg.getName(), Arg.getType());

    C.getBuilder().CreateStore(&Arg, Alloca);

    C.ST.set(Arg.getName(), new VariableSymbol(Alloca, (*Args)[Idx++]->Ty));
  }

  // generate function body
  C.ReturnFound = false;
  Body->codegen(C);

  C.ST.leaveScope();

  if (!C.ReturnFound) {
    Log::warning(Body->loc.end) << "expected a return statement inside function body, "
                          "but none was found.\n";
    return nullptr;
  }

  return nullptr;
}

Value *VarDeclNode::codegen(Context &C) {
  if (C.ST.get(Id)) {
    Log::error(loc.begin) << "variable " << Id << " already declared.\n";
    return nullptr;
  }

  Function *TheFunction = C.getBuilder().GetInsertBlock()->getParent();

  AllocaInst *Alloca =
      CreateEntryBlockAlloca(TheFunction, C.getContext(), Id, Ty->emit(C));

  C.ST.set(Id, new VariableSymbol(Alloca, Ty));

  if (Assign)
    Assign->codegen(C);

  return nullptr;
}

Value *ReturnNode::codegen(Context &C) {
  C.ReturnFound = true;

  IRBuilder<> &Builder = C.getBuilder();

  if (expr)
    return Builder.CreateRet(expr->codegen(C));

  return Builder.CreateRetVoid();
}

Value *SkipNode::codegen(Context &C) {
  auto Sym = dynamic_cast<BlockSymbol *>(C.ST.get("skip"));

  if (!Sym)
    Log::error(loc.begin) << "skip command can appear only inside loops.\n";
  else
    C.getBuilder().CreateBr(Sym->BB);

  return nullptr;
}

Value *StopNode::codegen(Context &C) {
  auto Sym = dynamic_cast<BlockSymbol *>(C.ST.get("stop"));

  if (!Sym)
    Log::error(loc.begin) << "stop command can appear only inside loops.\n";
  else
    C.getBuilder().CreateBr(Sym->BB);

  return nullptr;
}

Value *ForNode::codegen(Context &C) {
  auto &Builder = C.getBuilder();
  auto &TheContext = C.getContext();
  auto TheFunction = Builder.GetInsertBlock()->getParent();

  auto BeforeLoopBB =
      BasicBlock::Create(TheContext, "before_loop", TheFunction);
  auto LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
  auto StepBB = BasicBlock::Create(TheContext, "step", TheFunction);
  auto AfterLoopBB = BasicBlock::Create(TheContext, "after_loop", TheFunction);

  C.ST.set("skip", new BlockSymbol(StepBB));
  C.ST.set("stop", new BlockSymbol(AfterLoopBB));

  Start->codegen(C);

  Builder.CreateBr(BeforeLoopBB);

  Builder.SetInsertPoint(BeforeLoopBB);

  auto CondV = End->codegen(C);
  if (!CondV)
    return nullptr;

  auto CondTy = Type::from(CondV->getType());
  if (!CondTy->isBoolTy()) {
    Log::error(End->loc.begin) << "'" << CondTy->str() << "' is not convertible to '"
                     << Type::boolTy()->str() << "'\n";
    return nullptr;
  }

  Builder.CreateCondBr(CondV, LoopBB, AfterLoopBB);

  Builder.SetInsertPoint(LoopBB);

  C.ST.enterScope();
  Body->codegen(C);
  C.ST.leaveScope();

  Builder.SetInsertPoint(StepBB);
  Step->codegen(C);

  Builder.CreateBr(BeforeLoopBB);

  Builder.SetInsertPoint(AfterLoopBB);

  return nullptr;
}

Value *WhileNode::codegen(Context &C) {
  auto &Builder = C.getBuilder();
  auto &TheContext = C.getContext();

  auto TheFunction = Builder.GetInsertBlock()->getParent();

  auto BeforeLoopBB =
      BasicBlock::Create(TheContext, "before_loop", TheFunction);
  auto LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
  auto AfterLoopBB = BasicBlock::Create(TheContext, "after_loop", TheFunction);

  C.ST.set("skip", new BlockSymbol(LoopBB));
  C.ST.set("stop", new BlockSymbol(AfterLoopBB));

  Builder.CreateBr(BeforeLoopBB);
  Builder.SetInsertPoint(BeforeLoopBB);

  auto CondV = Condition->codegen(C);
  auto CondTy = Type::from(CondV->getType());
  if (!CondTy->isBoolTy()) {
    Log::error(Condition->loc.begin) << "'" << CondTy->str() << "' is not convertible to '"
                     << Type::boolTy()->str() << "'\n";
    return nullptr;
  }

  Builder.CreateCondBr(CondV, LoopBB, AfterLoopBB);

  Builder.SetInsertPoint(LoopBB);

  C.ST.enterScope();
  Block->codegen(C);
  C.ST.leaveScope();

  Builder.CreateBr(BeforeLoopBB);

  Builder.SetInsertPoint(AfterLoopBB);

  return nullptr;
}

Value *VariableExprNode::codegen(Context &C) {
  auto Sym = dynamic_cast<VariableSymbol *>(C.ST.get(Id));
  if (!Sym) {
    Log::error(loc.begin) << "variable '" << Id << "' not declared.\n";
    return nullptr;
  }

  return C.getBuilder().CreateLoad(Sym->Alloca, Id);
}

Value *LiteralStringNode::codegen(Context &C) {
  return C.getBuilder().CreateGlobalStringPtr(Str);
}

llvm::Value *AssignNode::codegen(Context &C) {
  auto Sym = dynamic_cast<VariableSymbol *>(C.ST.get(Id));
  if (!Sym) {
    Log::error(loc.begin) << "variable '" << Id << "' not declared.\n";
    return nullptr;
  }


  Value *Store = Assign->codegen(C);
  if (!Store)
    return nullptr;

  auto AssignTy = Type::from(Store->getType());
  auto DeclaredTy = Sym->Ty;

  if (*AssignTy != *DeclaredTy) {
    Log::error(Assign->loc.begin) << "cannot assign value of type '" << AssignTy->str()
                     << "', expected '" << DeclaredTy->str() << "'\n";
    return nullptr;
  }

  C.getBuilder().CreateStore(Store, Sym->Alloca);

  return Store;
}

Value *VarDeclNodeListStmt::codegen(Context &C) {
  for (auto VarDecl : varDeclList)
    VarDecl->codegen(C);

  return nullptr;
}

Value *ExprNegativeNode::codegen(Context &C) {
  Value *RHSV = RHS->codegen(C);
  return C.getBuilder().CreateNeg(RHSV);
}

Value *ExprNotNode::codegen(Context &C) {
  Value *RHSV = RHS->codegen(C);
  auto Ty = Type::from(RHSV->getType());

  if (!Ty->isBoolTy()) {
    Log::error(RHS->loc.begin) << "'" << Ty->str() << "' is not convertible to '"
                     << Type::boolTy()->str() << "'\n";
    return nullptr;
  }

  return C.getBuilder().CreateNot(RHSV);
}

Value *ExprOperationNode::codegen(Context &C) {
  Value *LHSV = LHS->codegen(C);
  Value *RHSV = RHS->codegen(C);

  switch (Op) {
  case BinOp::PLUS:
    return C.getBuilder().CreateAdd(LHSV, RHSV);
  case BinOp::MINUS:
    return C.getBuilder().CreateSub(LHSV, RHSV);
  case BinOp::TIMES:
    return C.getBuilder().CreateMul(LHSV, RHSV);
  case BinOp::DIV:
    return C.getBuilder().CreateUDiv(LHSV, RHSV);
  case BinOp::MOD:
    return C.getBuilder().CreateURem(LHSV, RHSV);
  case BinOp::LT:
    return C.getBuilder().CreateICmpSLT(LHSV, RHSV);
  case BinOp::LTEQ:
    return C.getBuilder().CreateICmpSLE(LHSV, RHSV);
  case BinOp::GT:
    return C.getBuilder().CreateICmpSGT(LHSV, RHSV);
  case BinOp::GTEQ:
    return C.getBuilder().CreateICmpSGE(LHSV, RHSV);
  case BinOp::EQ:
    return C.getBuilder().CreateICmpEQ(LHSV, RHSV);
  case BinOp::DIFF:
    return C.getBuilder().CreateICmpNE(LHSV, RHSV);
  case BinOp::AND:
    return C.getBuilder().CreateAnd(LHSV, RHSV);
  case BinOp::OR:
    return C.getBuilder().CreateOr(LHSV, RHSV);
  }
}

Value *CallExprNode::codegen(Context &C) {
  auto Sym = dynamic_cast<FuncSymbol *>(C.ST.get(Callee));

  if (!Sym) {
    Log::error(loc.begin) << "function '" << Callee << "' not found.\n";
    return nullptr;
  }

  if (Sym->Function->arg_size() != Args->size()) {
    Log::error(loc.begin) << "incorrect number of arguments passed to function "
                     << Callee << "\n";
    return nullptr;
  }

  std::vector<Value *> ArgsV;
  for (auto Arg : *Args) {
    ArgsV.push_back(Arg->codegen(C));
    if (!ArgsV.back())
      return nullptr;
  }

  bool ErrorFound = false;
  for (unsigned i = 0; i < Args->size(); ++i) {

    auto DeclaredTy = Sym->Args[i];
    auto PassedTy = Type::from(ArgsV[i]->getType());

    if (*DeclaredTy != *PassedTy) {
      Log::error((*Args)[i]->loc.begin) << "wrong param type passed to function '" << Callee
                       << "' at index '" << std::to_string(i) << "', expected '"
                       << DeclaredTy->str() << "', but found '"
                       << PassedTy->str() << "'\n";

      ErrorFound = true;
    }
  }

  if (ErrorFound)
    return nullptr;

  return C.getBuilder().CreateCall(Sym->Function, ArgsV);
}

Value *ProcDeclNode::codegen(Context &C) {
  std::cout << "ProcDeclNode unimplemented" << std::endl;
  return nullptr;
}

Value *WriteNode::codegen(Context &C) {

  auto Sym = dynamic_cast<FuncSymbol *>(C.ST.get("printf"));
  assert(Sym && "forgot to insert printf function");

  auto StrFormat = C.getBuilder().CreateGlobalStringPtr("%s");
  auto IntFormat = C.getBuilder().CreateGlobalStringPtr("%d");
  auto NewLineFormat = C.getBuilder().CreateGlobalStringPtr("\n");

  for (auto Expr : *Exprs) {
    auto Value = Expr->codegen(C);
    auto Ty = Type::from(Value->getType());

    if (Ty->isIntTy() || Ty->isBoolTy()) {
      C.getBuilder().CreateCall(Sym->Function, {IntFormat, Value});
    } else if (Ty->isStringTy()) {
      C.getBuilder().CreateCall(Sym->Function, {StrFormat, Value});
    }
  }

  C.getBuilder().CreateCall(Sym->Function, {NewLineFormat});

  return nullptr;
}

Value *CompoundAssignNode::codegen(Context &C) {
  auto Sym = dynamic_cast<VariableSymbol *>(C.ST.get(Id));

  if (!Sym) {
    Log::error(loc.begin) << "variable '" << Id << "' not declared.\n";
    return nullptr;
  }

  auto Alloca = Sym->Alloca;
  Value *Store = Assign->codegen(C);

  auto AllocatedTy = Sym->Ty;
  auto AssigningTy = Type::from(Store->getType());

  if (*AllocatedTy != *AssigningTy) {
    Log::error(Assign->loc.begin) << "cannot assign value of type '" << AssigningTy->str()
                     << "', expected '" << AllocatedTy->str() << "'\n";
    return nullptr;
  }

  Value *AllocaValue = C.getBuilder().CreateLoad(Alloca, Id);
  Value *Result = nullptr;

  switch (Op) {
  case BinOp::PLUS:
    Result = C.getBuilder().CreateAdd(Store, AllocaValue);
    break;
  case BinOp::MINUS:
    Result = C.getBuilder().CreateSub(Store, AllocaValue);
    break;
  case BinOp::TIMES:
    Result = C.getBuilder().CreateMul(Store, AllocaValue);
    break;
  case BinOp::DIV:
    Result = C.getBuilder().CreateUDiv(Store, AllocaValue);
    break;
  }

  C.getBuilder().CreateStore(Result, Alloca);

  return nullptr;
}
