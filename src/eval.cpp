#include "eval.hpp"
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

    while (white_pieces) {
      Square w_square = Utils::pop_bit(white_pieces);
      switch (piece) {
      case PAWN:
        white_eval += Utils::WPAWN_POSITIONAL[w_square];
        break;
      case KNIGHT:
        white_eval += Utils::WKNIGHT_POSITIONAL[w_square];
        break;
      case BISHOP:
        white_eval += Utils::WBISHOP_POSITIONAL[w_square];
        break;
      case ROOK:
        white_eval += Utils::WROOK_POSITIONAL[w_square];
        break;
      case QUEEN:
        white_eval += Utils::WQUEEN_POSITIONAL[w_square];
        break;
      case KING:
        white_eval += Utils::WKING_POSITIONAL[w_square];
        break;
      }
    }

    while (black_pieces) {
      Square b_square = Utils::pop_bit(black_pieces);
      b_square = (Square)(b_square ^ 56);
      switch (piece) {
      case PAWN:
        black_eval -= Utils::WPAWN_POSITIONAL[b_square];
        break;
      case KNIGHT:
        black_eval -= Utils::WKNIGHT_POSITIONAL[b_square];
        break;
      case BISHOP:
        black_eval -= Utils::WBISHOP_POSITIONAL[b_square];
        break;
      case ROOK:
        black_eval -= Utils::WROOK_POSITIONAL[b_square];
        break;
      case QUEEN:
        black_eval -= Utils::WQUEEN_POSITIONAL[b_square];
        break;
      case KING:
        black_eval -= Utils::WKING_POSITIONAL[b_square];
        break;
      }
    }

  }
  int perspective = (pos->side_to_play == WHITE) ? 1 : -1;

  return (white_eval + black_eval) * perspective;
}

