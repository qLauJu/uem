#pragma once

#include "llvm/Support/raw_ostream.h"
#include "location.hh"
#include <ostream>
#include <string>
#include <utility>

using namespace llvm;

class Log {
public:

   static raw_ostream &error(const yy::position &pos) {
       errs() << pos.line << "," << pos.column << ": \033[1;31merror\033[0m: ";
       return errs();
   }

   static raw_ostream &warning(const yy::position &pos) {
       errs() << pos.line << "," << pos.column << ": \033[1;33mwarning\033[0m: ";
       return errs();
   }

};
