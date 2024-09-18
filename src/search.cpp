#include "search.hpp"
#include "datatypes.hpp"
#include "move_generator.hpp"
#include <chrono>
#include <limits.h>

Search::Search(Position *position_ptr)
    : move_gen((position_ptr)), eval((position_ptr)) {
  pos = position_ptr;
  search_start = std::chrono::high_resolution_clock::now();
  search_done = false;
  nodes_searched = 0;
  best_move = {};
  hashfull = 0;
  hashsize = 128 * 1024 * 1024 / sizeof(TT_Entry);

  transposition_table.reserve(hashsize);
  for (zobrist_key i = 0; i < (hashsize); i++) {
    TT_Entry entry = TT_Entry();
    transposition_table.push_back(entry);
  }
}

int Search::iterative_deepening(const int time, const int moves_remaining) {

  int depth = 1;
  int best_eval = -INT_MAX;

  time_limit = 0.65 * time / moves_remaining;
  search_start = std::chrono::high_resolution_clock::now();
  search_done = false;

  while (!search_done & (depth <= 64)) {
    start_time = std::chrono::high_resolution_clock::now();

    best_eval = negamax_root(depth);
    best_move_overall = best_move;
    depth++;

    auto end_time = std::chrono::high_resolution_clock::now();
    time_searched = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time)
                        .count();
    time_searched = (time_searched > 0) ? time_searched : 1;
    info_to_uci(best_eval);
  }
  return best_eval;
}

int Search::negamax_root(const int depth) {
  if (is_search_done()) {
    return Scores::DRAW;
  }

  TT_Entry entry = probe_TT(pos->z_key, depth);
  best_move = entry.best_move;
  int alpha = -INT_MAX;
  int beta = INT_MAX;
  int eval = -INT_MAX;
  depth_searched = depth;

  MoveList moves = move_gen.generate_pseudo_legal_moves();

  moves.score_moves(best_move);
  moves.sort_moves();

  zobrist_key move_key = pos->z_key;

  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);
    if (depth >= 10 && depth <= 20) {
      std::cout << "info currmove " << mv << std::endl;
    }

    pos->make_move(mv);
    if (!move_gen.validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    nodes_searched++;

    eval = -negamax(-beta, -alpha, depth - 1, true);

    if ((eval > alpha) && ((!search_done) || ((i == 0) && search_done))) {
      alpha = eval;
      best_move = moves.at(i);
    }

    pos->undo_move(mv);
  }
  update_TT(move_key, depth, alpha, NodeType::EXACT, best_move);

  return alpha;
}

int Search::negamax(int alpha, int beta, const int depth, bool null_allowed) {
  if (is_search_done()) {
    return Scores::DRAW;
  }

  if (pos->is_drawn()) {
    return Scores::DRAW;
  }

  if ((depth <= 0)) {
    return quiescence(alpha, beta);
  }

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

  zobrist_key move_key = pos->z_key;
  int alpha_old = alpha;
  int current_move = 0;
  int best_eval = -INT_MAX;
  Move my_best_move = Move();

  if (!move_gen.king_in_check(pos->side_to_play) & null_allowed &
          (depth >= NULL_MOVE_REDUCTION + 1) &&
      (eval.evaluate() >= beta - 50)) {
    pos->make_null_move();
    int nm_eval =
        -negamax(-beta, -beta + 1, depth - NULL_MOVE_REDUCTION-1, false);
    pos->undo_null_move();

    if (nm_eval >= beta) {
      return beta;
    }
  }

  int eval = -INT_MAX;

  MoveList moves = move_gen.generate_pseudo_legal_moves();
  moves.score_moves(entry.best_move);
  moves.sort_moves();

  for (size_t i = 0; i < moves.size(); i++) {
    // make move on board
    Move mv = moves.at(i);

    pos->make_move(mv);
    if (!move_gen.validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    current_move++;
    nodes_searched++;
    // recursive call

    eval = -negamax(-beta, -alpha, depth - 1, true);

    // undo to reset board
    pos->undo_move(mv);

    if (eval >= beta) {
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
    if (move_gen.king_in_check(pos->side_to_play)) {
      return Scores::CHECKMATE + pos->ply;
    } else {
        std::cout << *pos << std::endl;
      return Scores::DRAW;
    }
  }

  if (alpha > alpha_old) {
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

  if (pos->is_drawn()) {
    return Scores::DRAW;
  }

  int stand_pat = eval.evaluate();

  if (stand_pat >= beta) {
    return beta;
  }
  if (alpha < stand_pat) {
    alpha = stand_pat;
  }

  int eval = stand_pat;

  MoveList moves = move_gen.generate_captures();
  moves.score_moves(Move());
  moves.sort_moves();

  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);

    pos->make_move(mv);
    if (!move_gen.validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    nodes_searched++;

    eval = -quiescence(-beta, -alpha);
    pos->undo_move(mv);

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
  zobrist_key idx = z_key % transposition_table.size();

  if ((depth > transposition_table[idx].depth) && (!search_done)) {
    transposition_table[idx].key = z_key;
    transposition_table[idx].depth = depth;
    transposition_table[idx].evaluation = evaluation;
    transposition_table[idx].type = type;
    transposition_table[idx].best_move = best_move;
    hashfull++;
    return true;
  } else {
    return false;
  }
}

TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth) {
  bool dummy = true;
  return probe_TT(z_key, depth, dummy);
}
TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth,
                          bool &was_found) {

  zobrist_key idx = z_key % transposition_table.size();
  TT_Entry entry = transposition_table[idx];

  if ((entry.key == z_key) && (entry.depth > depth)) {
    was_found = true;
  } else {
    was_found = false;
  }
  return entry;
}

void Search::info_to_uci(const int eval) const {
  double nps = nodes_searched / (time_searched / 1000.0);
  std::cout << "info depth " << depth_searched << " score cp " << eval
            << " nodes " << nodes_searched << " nps " << nps << " hashfull "
            << hashfull * 1000 / hashsize << " time " << time_searched << " pv "
            << best_move_overall << std::endl;
}

bool Search::is_search_done() {
  if ((nodes_searched & 2048) == 0) {
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
