#include "move_list.hpp"
#include <algorithm>

void MoveList::score_moves(Move TT_move, Move killer1, Move killer2) {
  for (size_t i = 0; i < this->size(); i++) {
    Move *move = &this->moves[i];
    move->sort_score = 0;

    if (move->is_capture) {
      move->sort_score = MVV_LVA[move->captured_piece][move->piece];
    } else {
      if (*move == killer1) {
        move->sort_score = MoveScores::KILLER1;
      }
      if (*move == killer2) {
        move->sort_score = MoveScores::KILLER2;
      }
    }

    if (*move == TT_move) {
      move->sort_score = MoveScores::TTMOVE;
    }

    if (move->promotion > 0) {
      move->sort_score = MVV_LVA[move->promotion][Pieces::PAWN];
    }
  }
}

void MoveList::sort_moves() {
  std::sort(this->moves, this->moves + this->size(), CompareMoves());
}
