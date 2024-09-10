#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "eval.hpp"
#include "move_generator.hpp"
#include "position.hpp"
#include <chrono>
class Search {
public:
  Search(Position *position_ptr);
  int iterative_deepening(int time, int moves_remaining);
  int negamax(int alpha, int beta, int depth);
  int negamax_root(int depth);
  int quiescence(int alpha, int beta);

  inline Move get_best_move() { return best_move_overall; };

private:
  Move best_move_overall;
  Move best_move;
  MoveGenerator move_gen;
  Evaluator eval;
  Position *pos;
  int time_limit;
  std::chrono::time_point<std::chrono::high_resolution_clock> search_start;

  //debug messages
  size_t nodes_searched;
  size_t time_searched;
  size_t depth_searched;
  bool search_done;

};

#endif
