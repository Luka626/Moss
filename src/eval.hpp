#ifndef EVAL_HPP_
#define EVAL_HPP_

#include "position.hpp"
#include <memory>
class Evaluator {
public:
  Evaluator(std::shared_ptr<Position> position_ptr);
  int evaluate() const;

private:
  std::shared_ptr<Position> pos;
};
#endif
