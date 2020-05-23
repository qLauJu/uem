
#pragma once

#include "BinOp.hh"
#include "llvm/IR/Value.h"
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <location.hh>

namespace grace {

class Context;

class Type;

static std::string NestedLevel(unsigned level) {
  std::string str(level * 4, ' ');
  return str;
}

static std::string to_string(const BinOp &Op) {
  switch (Op) {
  case BinOp::PLUS:
    return "+";
  case BinOp::MINUS:
    return "-";
  case BinOp::TIMES:
    return "*";
  case BinOp::DIV:
    return "/";
  case BinOp::MOD:
    return "%";
  case BinOp::LT:
    return "<";
  case BinOp::LTEQ:
    return "<=";
  case BinOp::GT:
    return ">";
  case BinOp::GTEQ:
    return ">=";
  case BinOp::EQ:
    return "==";
  case BinOp::DIFF:
    return "!=";
  case BinOp::AND:
    return "&&";
  case BinOp::OR:
    return "||";
  }
}

class StmtNode;
class VarDeclNode;
class SpecVar;
class LiteralNode;
class ExprNode;

typedef std::vector<StmtNode *> StmtList;
typedef std::vector<VarDeclNode *> VarDeclNodeList;
typedef std::vector<SpecVar *> SpecVarList;

class Param {
public:
    yy::location loc;
  std::string Id;
  Type *Ty;

  Param(const yy::location &loc, std::string Id, Type *Ty) : loc(loc), Id(std::move(Id)), Ty(Ty) {}
};

typedef std::vector<Param *> ParamList;
typedef std::vector<ExprNode *> ExprList;

class Node {
public:
    yy::location loc;

    Node(const yy::location &loc) : loc(loc) {}

    virtual ~Node() = default;

  virtual void dumpAST(std::ostream &os, unsigned level) const = 0;

  virtual llvm::Value *codegen(Context &C) = 0;
};

class StmtNode : virtual public Node {
public:
//    StmtNode(const yy::location &loc) : Node(loc) {}
};

class ExprNode : virtual public Node {
public:
//    ExprNode(const yy::location &loc) : Node(loc) {}
};

class LiteralNode : virtual public ExprNode {
public:
//    LiteralNode(const yy::location &loc) : Node(loc) {}
};

class BlockNode : public Node {
public:
  StmtList Stmts;

    BlockNode(const yy::location &loc) : Node(loc) {

    }

    void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(Body" << std::endl;
    for (auto Stmt : Stmts) {
      Stmt->dumpAST(os, level + 1);
    }
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class AssignNode : public StmtNode {
protected:
  std::string Id;
  ExprNode *Assign;

public:

  AssignNode(const yy::location &loc, std::string Id, ExprNode *Assign)
      : Node(loc), Id(std::move(Id)), Assign(Assign) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(Assign id: " << Id
       << "; value: " << std::endl;
    Assign->dumpAST(os, level + 1);
    os << std::endl << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class CompoundAssignNode : public AssignNode {
public:
  BinOp Op;

  CompoundAssignNode(const yy::location &loc, std::string id, BinOp Op, ExprNode *Assign)
      : Node(loc), AssignNode(loc, id, Assign), Op(Op) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(assing id: " << Id
       << "; value: " << std::endl;
    os << to_string(Op) << std::endl;
    Assign->dumpAST(os, level + 1);
  }

  llvm::Value *codegen(Context &C) override;
};

class LiteralIntNode : public LiteralNode {
public:
  int IVal;

  LiteralIntNode(const yy::location &loc, int value) : Node(loc), IVal(value) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(literal value: " << IVal << ")";
  }

  llvm::Value *codegen(Context &C) override;
};

class LiteralStringNode : public LiteralNode {
public:
  std::string Str;

  LiteralStringNode(const yy::location &loc, const std::string &Str) : Node(loc), Str(Str) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(literal value: " << Str << ")";
  }

  llvm::Value *codegen(Context &C) override;
};

class LiteralBoolNode : public LiteralNode {
public:
  bool BVal;

  LiteralBoolNode(const yy::location &loc, bool BVal) : Node(loc), BVal(BVal) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(literal value: " << std::boolalpha << BVal
       << ")";
  }

  llvm::Value *codegen(Context &C) override;
};

class SpecVar {
public:
    yy::location loc;
    std::string Id;
  AssignNode *Assign;

  SpecVar(const yy::location &loc, std::string Id, AssignNode *Assign)
      : loc(loc), Id(std::move(Id)), Assign(Assign) {}
};

class VarDeclNode : public StmtNode {
protected:
  std::string Id;
  AssignNode *Assign;
  Type *Ty;

public:
  VarDeclNode(const yy::location &loc, std::string Id, AssignNode *Assign, Type *Ty)
      : Node(loc), Id(std::move(Id)), Assign(Assign), Ty(Ty) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(varDecl id: " << Id << "; type: " << Ty;

    if (Assign) {
      os << std::endl;
      Assign->dumpAST(os, level + 1);
      os << NestedLevel(level);
    }

    os << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class VarDeclNodeListStmt : public StmtNode {
public:
  VarDeclNodeList varDeclList;

    VarDeclNodeListStmt(const yy::location &loc) : Node(loc) {

    }

    void dumpAST(std::ostream &os, unsigned level) const override {
    for (const auto &varDecl : varDeclList)
      varDecl->dumpAST(os, level);
  }

  llvm::Value *codegen(Context &C) override;
};

class FuncDeclNode : public StmtNode {
  std::string Name;
  Type *ReturnTy;
  ParamList *Args;
  BlockNode *Body;

public:
  FuncDeclNode(const yy::location &loc, std::string Name, Type *ReturnTy, ParamList *Args,
               BlockNode *Body)
      : Node(loc), Name(std::move(Name)), ReturnTy(ReturnTy), Args(Args), Body(Body) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(function Name: " << Name
       << "; ReturnType: " << ReturnTy << std::endl;
    Body->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ProcDeclNode : public StmtNode {
  std::string Name;
  ParamList *Args;
  BlockNode *Body;

public:
  ProcDeclNode(const yy::location &loc, std::string Name, ParamList *Args, BlockNode *Body)
      : Node(loc), Name(std::move(Name)), Args(Args), Body(Body) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(function Name: " << Name << "; ReturnType: "
       << "void" << std::endl;
    Body->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class IfThenElseNode : public StmtNode {
  ExprNode *Condition;
  BlockNode *Then, *Else;

public:
  IfThenElseNode(const yy::location &loc, ExprNode *Condition, BlockNode *Then, BlockNode *Else)
      : Node(loc), Condition(Condition), Then(Then), Else(Else) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(if" << std::endl;
    Condition->dumpAST(os, level + 1);
    os << std::endl;
    Then->dumpAST(os, level + 1);
    if (Else != nullptr)
      Else->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class VariableExprNode : public ExprNode {
protected:
  std::string Id;

public:
    VariableExprNode(const yy::location &loc, std::string Id) : Node(loc), Id(std::move(Id)) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(var " << Id << " )" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ExprNegativeNode : public ExprNode {
  ExprNode *RHS;

public:
   ExprNegativeNode(const yy::location &loc, ExprNode *RHS) : Node(loc), RHS(RHS) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(-" << std::endl;
    RHS->dumpAST(os, level + 1);
    os << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ExprNotNode : public ExprNode {
  ExprNode *RHS;

public:
  ExprNotNode(const yy::location &loc, ExprNode *RHS) : Node(loc), RHS(RHS) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(NOT " << std::endl;
    RHS->dumpAST(os, level + 1);
    os << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ExprOperationNode : public ExprNode {
  ExprNode *LHS, *RHS;
  BinOp Op;

public:
  ExprOperationNode(const yy::location &loc, ExprNode *LHS, BinOp Op, ExprNode *RHS)
      : Node(loc), LHS(LHS), Op(Op), RHS(RHS) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(expr" << std::endl;
    LHS->dumpAST(os, level + 1);
    os << to_string(Op) << std::endl;
    RHS->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class CallExprNode : public ExprNode {
  std::string Callee;
  ExprList *Args;

public:
  CallExprNode(const yy::location &loc, std::string Callee, ExprList *Args)
      : Node(loc), Callee(std::move(Callee)), Args(Args) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(call " << Callee << std::endl
       << NestedLevel(level + 1) << "args: " << std::endl;

    for (auto Arg : *Args) {
      Arg->dumpAST(os, level + 2);
      os << "," << std::endl;
    }

    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class WhileNode : public StmtNode {
  ExprNode *Condition;
  BlockNode *Block;

public:
  WhileNode(const yy::location &loc, ExprNode *Condition, BlockNode *Block)
      : Node(loc), Condition(Condition), Block(Block) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(while" << std::endl;
    Condition->dumpAST(os, level + 1);
    os << std::endl;
    Block->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ForNode : public StmtNode {
  AssignNode *Start;
  ExprNode *End;
  AssignNode *Step;
  BlockNode *Body;

public:
  ForNode(const yy::location &loc, AssignNode *Start, ExprNode *End, AssignNode *Step, BlockNode *Body)
      : Node(loc), Start(Start), End(End), Step(Step), Body(Body) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(for" << std::endl;
    Start->dumpAST(os, level + 1);
    os << std::endl;
    End->dumpAST(os, level + 1);
    os << std::endl;
    Step->dumpAST(os, level + 1);
    os << std::endl;
    Body->dumpAST(os, level + 1);
    os << NestedLevel(level) << ")" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class ReturnNode : public StmtNode {
  ExprNode *expr;

public:
    ReturnNode(const yy::location &loc, ExprNode *expr) : Node(loc), expr(expr) {}

  void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(return ";

    if (expr != nullptr) {
      os << std::endl;
      expr->dumpAST(os, level + 1);
      os << std::endl << NestedLevel(level) << ")" << std::endl;
    } else {
      os << ")" << std::endl;
    }
  }

  llvm::Value *codegen(Context &C) override;
};

class StopNode : public StmtNode {
public:

    StopNode(const yy::location &loc) : Node(loc) {

    }

    void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(stop)" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class SkipNode : public StmtNode {
public:

    SkipNode(const yy::location &loc) : Node(loc) {

    }

    void dumpAST(std::ostream &os, unsigned level) const override {
    os << NestedLevel(level) << "(skip)" << std::endl;
  }

  llvm::Value *codegen(Context &C) override;
};

class WriteNode : public StmtNode {
  ExprList *Exprs;

public:
   WriteNode(const yy::location &loc, ExprList *Exprs) : Node(loc), Exprs(Exprs) {}

  void dumpAST(std::ostream &os, unsigned level) const override {}

  llvm::Value *codegen(Context &C) override;
};

}; // namespace grace