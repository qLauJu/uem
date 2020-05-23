//
// Created by Guilherme Souza on 11/22/18.
//

#include <Context.hh>

using namespace grace;

void Context::insertPrintfAndScanf() {
  auto Int8PtrTy = llvm::Type::getInt8PtrTy(getContext());
  auto Int32Ty = llvm::Type::getInt32Ty(getContext());

  auto FnType = llvm::FunctionType::get(Int32Ty, Int8PtrTy);

  auto Printf = llvm::Function::Create(
      FnType, llvm::GlobalValue::ExternalLinkage, "printf", &getModule());
  auto Scanf = llvm::Function::Create(
      FnType, llvm::GlobalValue::ExternalLinkage, "scanf", &getModule());

  ST.set("printf", new FuncSymbol(Printf, Type::intTy(), {Type::strTy()}));
  ST.set("scanf", new FuncSymbol(Scanf, Type::intTy(), {Type::strTy()}));
}
