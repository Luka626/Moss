#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "eval.hpp"
#include "move_generator.hpp"
#include "position.hpp"
class Search {
public:
  Search(Position *position_ptr);
  int negamax(int alpha, int beta, int depth);
  int negamax_root(int depth);

  inline Move get_best_move() { return best_move; };

private:
  Move best_move;
  MoveGenerator move_gen;
  Evaluator eval;
  Position *pos;
};

#endif
