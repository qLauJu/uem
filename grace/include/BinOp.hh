//
// Created by Guilherme Souza on 11/28/18.
//

#ifndef GRACE_OPERATORS_HH
#define GRACE_OPERATORS_HH

#include <ostream>

namespace grace {
enum class BinOp {
  PLUS,
  MINUS,
  TIMES,
  DIV,
  MOD,
  LT,
  LTEQ,
  GT,
  GTEQ,
  EQ,
  DIFF,
  AND,
  OR,
};
}; // namespace grace

#endif // GRACE_OPERATORS_HH
