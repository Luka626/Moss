#include "search.hpp"
#include "datatypes.hpp"
#include <chrono>
#include <limits.h>

Search::Search(Position *position_ptr)
    : move_gen(position_ptr), eval(position_ptr) {
  pos = position_ptr;
  depth_searched = 0;
  nodes_searched = 0;
  time_searched = 1;
  search_start = std::chrono::high_resolution_clock::now();
  search_done = false;
  time_limit = 0;
}

int Search::iterative_deepening(int time, int moves_remaining) {
  int depth = 1;
  int eval = 0;

  time_limit = 0.95 * time / moves_remaining;
  search_start = std::chrono::high_resolution_clock::now();
  search_done = false;

  while (!search_done) {
    eval = negamax_root(depth);
    depth++;
  }

  return eval;
}

int Search::negamax_root(int depth) {
  auto start_time = std::chrono::high_resolution_clock::now();

  int alpha = -INT_MAX;
  int beta = INT_MAX;
  int eval = 0;
  best_move = {};
  depth_searched = depth;

  MoveList moves = move_gen.generate_legal_moves();

  for (int i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);
    pos->make_move(mv);

    // recursive call
    eval = -negamax(-beta, -alpha, depth - 1);

    // reset board before making different move
    pos->undo_move(mv);

    // if a recursive search returns a stronger move
    // we save the parent of that subtree as best
    if (eval > alpha) {
      alpha = eval;
      best_move = moves.at(i);
    }

    if (search_done) {
      break;
    }
  }
  if (!search_done) {
    best_move_overall = best_move;
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  time_searched = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - start_time)
                      .count();
  time_searched = (time_searched > 0) ? time_searched : 1;
  double nps = nodes_searched / (time_searched / 1000.0);

  std::cout << "info depth " << depth_searched << " score cp " << alpha / 10
            << " nodes " << nodes_searched << " nps " << nps << " time "
            << time_searched << " pv " << best_move_overall << std::endl;
  return alpha;
}

int Search::negamax(int alpha, int beta, int depth) {
  // at leaf node -> return evaluation of resulting position

  if (nodes_searched & 1023) {
    auto curr_time = std::chrono::high_resolution_clock::now();
    auto search_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(curr_time -
                                                              search_start);

    if (search_duration.count() > time_limit) {
      search_done = true;
      return quiescence(alpha, beta);
    }
  }

  if (depth == 0) {
    nodes_searched++;
    return quiescence(alpha, beta);
  }

  int best_eval = -INT_MAX;
  MoveList moves = move_gen.generate_legal_moves();

  if (moves.empty()) {
    if (move_gen.king_in_check(pos->side_to_play)) {
      nodes_searched++;
      return -INT_MAX + pos->ply;
    } else {
      nodes_searched++;
      return 0;
    }
  }

  int eval = 0;

  for (int i = 0; i < moves.size(); i++) {
    // make move on board
    Move mv = moves.at(i);
    pos->make_move(mv);

    // recursive call
    eval = -negamax(-beta, -alpha, depth - 1);

    // undo to reset board
    pos->undo_move(mv);

    if (eval > best_eval) {
      best_eval = eval;
      if (eval > alpha) {
        alpha = eval;
      }
    }
    if (eval >= beta) {
      return beta;
    }
  }

  return best_eval;
}

int Search::quiescence(int alpha, int beta) {
  int stand_pat = eval.evaluate();

  if (stand_pat >= beta) {
    return beta;
  }
  if (stand_pat > alpha) {
    alpha = stand_pat;
  }

  MoveList moves = move_gen.generate_legal_moves();
  for (int i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);
    if (!mv.is_capture) {
      continue;
    }
    pos->make_move(mv);

    int eval = -quiescence(-beta, -alpha);

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
