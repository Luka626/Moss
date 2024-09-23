#include "search.hpp"
#include "datatypes.hpp"
#include "eval.hpp"
#include "move_generator.hpp"
#include "utils.hpp"
#include <chrono>
#include <limits.h>
#include <memory>

Search::Search(std::shared_ptr<Position> position_ptr)
    : move_gen(std::make_unique<MoveGenerator>(position_ptr)),
      eval(std::make_unique<Evaluator>(position_ptr)), pos(position_ptr) {
  search_age = 0;
}

void Search::new_search() {
  search_start = std::chrono::high_resolution_clock::now();
  iteration_start = std::chrono::high_resolution_clock::now();
  search_done = false;

  nodes_searched = 0;
  depth_searched = 0;
  time_searched = 0;
  time_limit = 0;

  hashfull = 0;

  best_move = Move();
  killer_moves.clear();
  killer_moves.resize(Utils::MAX_PLY, {Move(), Move()});
}

void Search::new_game() {
  search_age = 0;
  move_gen->new_game();
  new_search();
}

int Search::iterative_deepening(const int time, const int moves_remaining) {
  depth_searched = 1;
  ++search_age;

  int root_eval = -INT_MAX;
  TT_Entry old_entry = probe_TT(pos->z_key, depth_searched);
  best_move = old_entry.best_move;

  time_limit = time / (moves_remaining + 1);
  search_start = std::chrono::high_resolution_clock::now();

  while (!search_done & (depth_searched < Utils::MAX_DEPTH)) {
    iteration_start = std::chrono::high_resolution_clock::now();

    int alpha = -INT_MAX;
    int beta = INT_MAX;

    MoveList moves = move_gen->generate_pseudo_legal_moves();
    moves.score_moves(best_move, Move(), Move());
    moves.sort_moves();

    for (size_t i = 0; i < moves.size(); i++) {
      Move mv = moves.at(i);
      if (depth_searched >= 10) {
        std::cout << "info currmove " << mv << "\n";
      }

      pos->make_move(mv);
      if (!move_gen->validate_gamestate()) {
        pos->undo_move(mv);
        continue;
      }
      nodes_searched++;

      if (i > 0) {
        root_eval = -negamax(-alpha - 1, -alpha, depth_searched - 1, true);
        if ((root_eval > alpha) & (root_eval < beta)) {
          root_eval = -negamax(-beta, -alpha, depth_searched - 1, true);
        }
      } else {
        root_eval = -negamax(-beta, -alpha, depth_searched - 1, true);
      }

      pos->undo_move(mv);
      if (search_done) {
        break;
      }

      if (root_eval > alpha) {
        alpha = root_eval;
        best_move = mv;
      }
    
    }

    update_TT(pos->z_key, depth_searched, root_eval, NodeType::EXACT, best_move);

    TT_Entry root_entry = probe_TT(pos->z_key, depth_searched);
    best_move = root_entry.best_move;
    root_eval = root_entry.evaluation;
    info_to_uci(root_eval);
    ++depth_searched;
  }

  return root_eval;
}

int Search::negamax(int alpha, int beta, const int depth, bool null_allowed) {
  if ((depth <= 0)) {
    return quiescence(alpha, beta);
  }
  if (pos->is_drawn()) {
    return Scores::DRAW;
  }

  int alpha_old = alpha;
  bool was_found = false;
  TT_Entry entry = probe_TT(pos->z_key, depth, was_found);
  if (was_found) {
    if (entry.type == NodeType::EXACT) {
      return entry.evaluation;
    } else if ((entry.type == NodeType::UPPER) && (entry.evaluation < alpha)) {
      return alpha;
    } else if ((entry.type == NodeType::LOWER) && (entry.evaluation >= beta)) {
      return beta;
    }
  }

  if (!move_gen->king_in_check(pos->side_to_play) & null_allowed &
          (depth >= NULL_MOVE_REDUCTION + 1) &&
      (eval->evaluate() >= beta - 50)) {
    pos->make_null_move();
    int nm_eval =
        -negamax(-beta, -beta + 1, depth - NULL_MOVE_REDUCTION - 1, false);
    pos->undo_null_move();
    if (search_done) {
      return Scores::DRAW;
    }

    if (nm_eval >= beta) {
      return beta;
    }
  }

  zobrist_key move_key = pos->z_key;
  int current_move = 0;
  int best_eval = -INT_MAX;
  Move my_best_move = Move();
  int eval = -INT_MAX;

  MoveList moves = move_gen->generate_pseudo_legal_moves();
  moves.score_moves(entry.best_move, killer_moves.at(pos->ply).killer1,
                    killer_moves.at(pos->ply).killer2);
  moves.sort_moves();

  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);
    pos->make_move(mv);
    if (!move_gen->validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    current_move++;
    nodes_searched++;

    if (i > 0) {
      eval = -negamax(-alpha - 1, -alpha, depth - 1, true);
      if ((alpha < eval) & (eval < beta)) {
        // need to re-search since we failed-high
        eval = -negamax(-beta, -alpha, depth - 1, true);
      }
    }

    else {
      eval = -negamax(-beta, -alpha, depth - 1, true);
    }

    pos->undo_move(mv);

    if (search_done) {
      return Scores::DRAW;
    }
    if (eval >= beta) {
      store_killer(mv);
      update_TT(move_key, depth, eval, NodeType::LOWER, mv);
      return beta;
    }

    if (eval > best_eval) {
      best_eval = eval;
      my_best_move = mv;
      if (eval > alpha) {
        alpha = eval;
      }
    }
  }

  if (current_move <= 0) {
    if (move_gen->king_in_check(pos->side_to_play)) {
      return Scores::CHECKMATE + pos->ply;
    } else {
      return Scores::DRAW;
    }
  }

  if (best_eval < alpha_old) {
    update_TT(move_key, depth, best_eval, NodeType::EXACT, my_best_move);
  } else {
    update_TT(move_key, depth, alpha, NodeType::UPPER, my_best_move);
  }
  return alpha;
}

int Search::quiescence(int alpha, int beta) {
  if (is_search_done()) {
    return Scores::DRAW;
  }

  int stand_pat = eval->evaluate();

  if (stand_pat >= beta) {
    return beta;
  }
  if (alpha < stand_pat) {
    alpha = stand_pat;
  }

  int eval = stand_pat;

  MoveList moves = move_gen->generate_captures();
  moves.score_moves(Move(), killer_moves.at(pos->ply).killer1,
                    killer_moves.at(pos->ply).killer2);
  moves.sort_moves();

  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);

    pos->make_move(mv);
    if (!move_gen->validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    nodes_searched++;

    eval = -quiescence(-beta, -alpha);
    pos->undo_move(mv);
    if (search_done) {
      return Scores::DRAW;
    }

    if (eval >= beta) {
      return beta;
    }

    if (eval > alpha) {
      alpha = eval;
    }
  }

  return alpha;
}

bool Search::update_TT(const zobrist_key z_key, const size_t depth,
                       const int evaluation, const NodeType type,
                       const Move best_move) {
  zobrist_key idx = z_key % Utils::TT.size();

  if (search_done) {
    return false;
  }

  bool younger = (search_age > Utils::TT.at(idx).age);
  /*
  bool force = (type == NodeType::EXACT) & (Utils::TT.at(idx).type != EXACT);
  if (force) {
    Utils::TT.at(idx).key = z_key;
    Utils::TT.at(idx).depth = depth;
    Utils::TT.at(idx).evaluation = evaluation;
    Utils::TT.at(idx).type = type;
    Utils::TT.at(idx).best_move = best_move;
    Utils::TT.at(idx).age = search_age;
    hashfull++;
    return true;
  }
*/
  if (younger | (Utils::TT.at(idx).depth <= depth)) {
    Utils::TT.at(idx).key = z_key;
    Utils::TT.at(idx).depth = depth;
    Utils::TT.at(idx).evaluation = evaluation;
    Utils::TT.at(idx).type = type;
    Utils::TT.at(idx).best_move = best_move;
    Utils::TT.at(idx).age = search_age;
    hashfull++;
    return true;
  }

  return false;
}

TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth) {
  bool dummy = true;
  return probe_TT(z_key, depth, dummy);
}
TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth,
                          bool &was_found) {

  zobrist_key idx = z_key % Utils::TT.size();
  TT_Entry entry = Utils::TT.at(idx);

  if (entry.key != z_key) {
    was_found = false;
    return TT_Entry();
  }

  if ((entry.depth < depth)) {
    was_found = false;
    return entry;
  }

  was_found = true;
  return entry;
}

bool Search::is_search_done() {
  if ((nodes_searched & 1023) == 0) {
    auto curr_time = std::chrono::high_resolution_clock::now();
    auto search_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(curr_time -
                                                              search_start);
    if (search_duration.count() > time_limit) {
      search_done = true;
    }
  }
  return search_done;
}

void Search::store_killer(Move mv) {
  if (mv.is_capture) {
    return;
  }
  if (mv == killer_moves.at(pos->ply).killer1) {
    return;
  }

  Move tmp = killer_moves.at(pos->ply).killer1;
  killer_moves.at(pos->ply).killer2 = tmp;
  killer_moves.at(pos->ply).killer1 = mv;
}

void Search::info_to_uci(const int eval) {
  auto end_time = std::chrono::high_resolution_clock::now();
  time_searched = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - iteration_start)
                      .count();
  time_searched = (time_searched > 0) ? time_searched : 1;
  double nps = nodes_searched / (time_searched / 1000.0);
  std::cout << "info";
  std::cout << " depth " << depth_searched;
  std::cout << " score cp " << eval;
  std::cout << " nodes " << nodes_searched;
  std::cout << " nps " << nps;
  std::cout << " hashfull " << hashfull * 1000 / Utils::TT.size();
  std::cout << " time " << time_searched;
  std::cout << " pv ";

  std::vector<Move> pv = {};
  TT_Entry entry = probe_TT(pos->z_key, 0);
  while (entry.best_move && pv.size() < depth_searched) {
    pv.push_back(entry.best_move);
    std::cout << entry.best_move << "  ";
    pos->make_move(entry.best_move);
    entry = probe_TT(pos->z_key, 0);
  }
  for (auto riter = pv.rbegin(); riter.base() != pv.begin(); ++riter) {
    Move mv = *riter;
    if (mv) {
      pos->undo_move(mv);
    }
  }
  std::cout << "\n";
}
