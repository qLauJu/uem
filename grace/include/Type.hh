//
// Created by Guilherme Souza on 12/7/18.
//

#pragma once

#include "AST.hh"
#include <string>

namespace llvm {
class Type;
};

namespace grace {
class Context;

class Type {
public:
  virtual ~Type() = default;

  virtual llvm::Type *emit(Context &C) = 0;
  virtual std::string str() const = 0;

  static Type *from(llvm::Type *Ty);
  static Type *boolTy();
  static Type *intTy();
  static Type *strTy();

  bool operator==(const Type &Other);
  bool operator!=(const Type &Other);

  bool isIntTy() const;
  bool isBoolTy() const;
  bool isStringTy() const;
};

class IntType : public Type {
public:
  llvm::Type *emit(Context &C) override;
  std::string str() const override { return "int"; }
};

class BoolType : public Type {
public:
  llvm::Type *emit(Context &C) override;
  std::string str() const override { return "bool"; }
};

class StringType : public Type {
public:
  llvm::Type *emit(Context &C) override;
  std::string str() const override { return "string"; }
};

}; // namespace grace
