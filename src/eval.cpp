#include "eval.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

Evaluator::Evaluator(Position *position_ptr) { pos = position_ptr; }

int Evaluator::evaluate() {
  int white_eval = 0;
  int black_eval = 0;

  for (int i = PAWN; i < NPIECES; i++) {
    Pieces piece = (Pieces)i;
    bitboard white_pieces = pos->get_bitboard(Colors::WHITE, piece);
    bitboard black_pieces = pos->get_bitboard(Colors::BLACK, piece);
    white_eval +=
        (Utils::pop_count(white_pieces) * Utils::MATERIAL_VALUE[piece]);
    black_eval -=
        (Utils::pop_count(black_pieces) * Utils::MATERIAL_VALUE[piece]);
  }

  int perspective = (pos->side_to_play == WHITE) ? 1 : -1;
  return (white_eval + black_eval) * perspective;
}
