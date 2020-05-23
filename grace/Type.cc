//
// Created by Guilherme Souza on 12/7/18.
//

#include "llvm/IR/Type.h"
#include "Context.hh"
#include <Type.hh>

using namespace grace;

llvm::Type *IntType::emit(Context &C) {
  return llvm::Type::getInt32Ty(C.getContext());
}

llvm::Type *BoolType::emit(Context &C) {
  return llvm::Type::getInt1Ty(C.getContext());
}

llvm::Type *StringType::emit(Context &C) {
  llvm::errs() << "unimplemented\n";
  return nullptr;
}

grace::Type *grace::Type::from(llvm::Type *Ty) {
  if (Ty->isIntegerTy(32))
    return intTy();

  if (Ty->isIntegerTy(1))
    return boolTy();

  return nullptr;
}

bool grace::Type::isIntTy() const {
  return dynamic_cast<const IntType *>(this) != nullptr;
}

bool grace::Type::isStringTy() const {
  return dynamic_cast<const StringType *>(this) != nullptr;
}

bool grace::Type::isBoolTy() const {
  return dynamic_cast<const BoolType *>(this) != nullptr;
}

bool grace::Type::operator==(const grace::Type &Other) {
  if (isIntTy() && Other.isIntTy())
    return true;
  if (isBoolTy() && Other.isBoolTy())
    return true;
  if (isStringTy() && Other.isStringTy())
    return true;
  return false;
}

bool grace::Type::operator!=(const grace::Type &Other) {
  return !(*this == Other);
}

grace::Type *grace::Type::boolTy() { return new BoolType(); }

grace::Type *grace::Type::intTy() { return new IntType(); }

grace::Type *grace::Type::strTy() { return new StringType(); }
