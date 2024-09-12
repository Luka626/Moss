#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "datatypes.hpp"
#include "eval.hpp"
#include "move_generator.hpp"
#include "position.hpp"
#include <chrono>
#include <vector>
class Search {
public:
  Search(Position *position_ptr);
  int iterative_deepening(int time, int moves_remaining);
  int negamax(int alpha, int beta, int depth);
  int negamax_root(int depth);
  int quiescence(int alpha, int beta);
  bool update_TT(zobrist_key z_key, size_t depth, int evaluation, bool exact,
                 bool upper_bound, bool lower_bound, Move best_move);
  TT_Entry probe_TT(zobrist_key z_key, size_t depth, bool &was_found);

  inline Move get_best_move() { return best_move_overall; };

private:
  Move best_move_overall;
  Move best_move;
  MoveGenerator move_gen;
  Evaluator eval;
  Position *pos;
  int time_limit;
  std::chrono::time_point<std::chrono::high_resolution_clock> search_start;
  std::vector<TT_Entry> transposition_table;
  size_t hashsize;

  // debug messages
  int nodes_searched;
  size_t time_searched;
  size_t depth_searched;
  bool search_done;
  size_t hashfull;
};

#endif
