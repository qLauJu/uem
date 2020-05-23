//
// Created by Guilherme Souza on 12/6/18.
//

#ifndef GRACE_ERROR_HH
#define GRACE_ERROR_HH

#include <string>

class Error {
public:
  enum TypeID {
    TypeMismatch,
    VarAlreadyDeclared,
    FuncAlreadyDeclared,
  };

private:
  std::string Message;

  explicit Error(const std::string &Message) : Message(Message) {}

public:
  static Error typeMismatch(const std::string &TypeA,
                            const std::string &TypeB) {
    return Error("cannot convert value of type '" + TypeA +
                 "' to specified type '" + TypeB + "'\n");
  }

  const std::string &getMessage() const { return Message; }
};

#endif // GRACE_ERROR_HH
