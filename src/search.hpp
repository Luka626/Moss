#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "datatypes.hpp"
#include "move_generator.hpp"
#include "eval.hpp"
#include "position.hpp"
#include <chrono>
#include <memory>
#include <vector>
class Search {
public:
  Search(std::shared_ptr<Position> position_ptr);
  void new_game();
  int iterative_deepening(const int time, const int moves_remaining);
  void new_search();
  Move get_best_move() const { return best_move; };
  bool update_TT(const zobrist_key z_key, const size_t depth,
                 const int evaluation, const NodeType type, const Move best_move);
  TT_Entry probe_TT(const zobrist_key z_key, const size_t depth,
                    bool &was_found);
  TT_Entry probe_TT(const zobrist_key z_key, const size_t depth);
  
private:
  void info_to_uci(const int eval);
  int negamax(int alpha, int beta, const int depth, bool null_allowed);
  int negamax_root(const int depth);
  int quiescence(int alpha, int beta);
  bool is_search_done();
  void store_killer(Move mv);

  Move best_move;
  std::unique_ptr<MoveGenerator> move_gen;
  std::unique_ptr<Evaluator> eval;
  std::shared_ptr<Position> pos;
  int time_limit;
  std::chrono::time_point<std::chrono::high_resolution_clock> search_start;
  std::chrono::time_point<std::chrono::high_resolution_clock> iteration_start;

  size_t search_age;

  std::vector<KillerMoves> killer_moves;

  const int NULL_MOVE_REDUCTION = 2;
  const int MAX_DEPTH = 64;

  // debug messages
  int nodes_searched;
  size_t time_searched;
  size_t depth_searched;
  bool search_done;
  size_t hashfull;

};

#endif
