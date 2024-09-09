#ifndef EVAL_HPP_
#define EVAL_HPP_

#include "position.hpp"
class Evaluator {
public:
  Evaluator(Position *position_ptr);
  int evaluate();

private:
  Position *pos;
};
#endif
