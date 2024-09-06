#include "search.hpp"
#include "datatypes.hpp"
#include "eval.hpp"
#include "move_generator.hpp"
#include <limits.h>

Search::Search(Position *position_ptr)
    : move_gen(position_ptr), eval(position_ptr) {
  pos = position_ptr;
}

int Search::negamax_root(int depth) {
  int best_value = INT_MIN;
  int alpha = -999999;
  int beta = 999999;
  int eval = 0;

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
  }
  return alpha;
}

int Search::negamax(int alpha, int beta, int depth) {
  // at leaf node -> return evaluation of resulting position

  if (depth == 0) {
    return eval.evaluate();
  }

  MoveList moves = move_gen.generate_legal_moves();

  if (moves.empty()) {
    if (!move_gen.validate_gamestate()) {
      return -100000 + pos->ply;
    } else {
      return 0;
    }
  }

  int eval = 0;
  int best_eval = -999999;

  for (int i = 0; i < moves.size(); i++) {
    // make move on board
    Move mv = moves.at(i);
    pos->make_move(mv);

    // recursive call
    eval = -negamax(-beta, -alpha, depth - 1);

    // undo to reset board
    pos->undo_move(mv);

    if (eval >= beta) {
      return beta;
    }

    if (eval > best_eval) {
      best_eval = eval;
      if (eval > alpha) {
          alpha = eval;
      }
    }
  }

  return alpha;
}
