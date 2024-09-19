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
  int iterative_deepening(const int time, const int moves_remaining);
  Move get_best_move() const { return best_move_overall; };
  Move inline get_ponder() const {
      if (pv.size() <= 1){
          return Move();
      }
      return pv.at(1);
  };
  

private:
  void info_to_uci(const int eval) const;
  int negamax(int alpha, int beta, const int depth, bool null_allowed);
  int negamax_root(const int depth, int old_best);
  int quiescence(int alpha, int beta);
  bool update_TT(const zobrist_key z_key, const size_t depth,
                 const int evaluation, const NodeType type, const Move best_move);
  TT_Entry probe_TT(const zobrist_key z_key, const size_t depth,
                    bool &was_found);
  TT_Entry probe_TT(const zobrist_key z_key, const size_t depth);
  bool is_search_done();
  void store_killer(Move mv);

  Move best_move_overall;
  Move best_move;
  MoveGenerator move_gen;
  Evaluator eval;
  Position *pos;
  int time_limit;
  std::chrono::time_point<std::chrono::high_resolution_clock> search_start;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

  size_t search_age;

  std::vector<int> repitition_table;
  std::vector<Move> pv;
  std::vector<KillerMoves> killer_moves;

  const int NULL_MOVE_REDUCTION = 2;
  const int MAX_DEPTH = 128;

  // debug messages
  int nodes_searched;
  size_t time_searched;
  size_t depth_searched;
  bool search_done;
  size_t hashfull;

};

#endif
