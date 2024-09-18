#include "move_list.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>

void MoveList::score_moves(Move TT_move) {
  for (size_t i = 0; i < this->size(); i++) {
    Move *move = &this->moves[i];
    move->sort_score = move->piece;

    if (*move == TT_move) {
      move->sort_score = INT_MAX/8;
    } else if (move->is_capture) {
      move->sort_score += Utils::MVV_LVA[move->captured_piece][move->piece];
    }

    if (move->promotion){
        move->sort_score += 75;
    }
  }
}

void MoveList::sort_moves() {
  std::sort(this->moves, this->moves + this->size(), CompareMoves());
}
