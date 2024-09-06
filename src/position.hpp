#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include <string>

class Position {
public:
  // 6 piece and 2 color bitboards describe piece arrangement
  bitboard pieces_bitboards[NPIECES];
  bitboard color_bitboards[NCOLORS];

  // Encoded in FEN string, needed to establish state of play beyond piece
  // arrangement
  Colors side_to_play;
  Square en_passant_history[256];
  Square en_passant_square;
  bool castling_history[256][4];
  bool castling_flags[4];
  size_t halfmove_clock;
  size_t ply;


  Position();
  Position(const std::string &fen);

  int set_board(const std::string &fen);

  void make_move(Move &move);
  void undo_move(Move &move);
Pieces inline remove_piece(Square sq) {
  bitboard to_remove = 1ULL << sq;
  for (size_t piece = 0; piece < NPIECES; piece++) {
    bitboard piece_bb = pieces_bitboards[piece];
    if (piece_bb & to_remove) {
      pieces_bitboards[piece] &= ~to_remove;
      color_bitboards[~side_to_play] &= ~to_remove;
      return (Pieces)piece;
    };
    // todo: add NOPIECE piecetype and return it here instead of a PAWN
  }
  return Pieces::PAWN;
}


void inline update_castling_rights(Square from, Square to, bool is_capture) {
  if (from == a1) {
    castling_flags[1] = false;
  } else if (from == h1) {
    castling_flags[0] = false;
  } else if (from == a8) {
    castling_flags[3] = false;
  } else if (from == h8) {
    castling_flags[2] = false;
  } else if (from == e1) {
    castling_flags[0] = false;
    castling_flags[1] = false;
  } else if (from == e8) {
    castling_flags[2] = false;
    castling_flags[3] = false;
  }

  if (is_capture) {
    if (to == a1) {
      castling_flags[1] = false;
    } else if (to == h1) {
      castling_flags[0] = false;
    } else if (to == a8) {
      castling_flags[3] = false;
    } else if (to == h8) {
      castling_flags[2] = false;
    }
  }
}
  friend std::ostream &operator<<(std::ostream &os, const Position &pos);

  // useful getters
  bitboard inline get_occupied() {
    return color_bitboards[WHITE] | color_bitboards[BLACK];
  }
  bitboard inline get_empty() { return ~get_occupied(); }

  bitboard inline get_enemy(Colors side) { return color_bitboards[~side]; }
  bitboard inline get_enemy() { return color_bitboards[~(Colors)side_to_play]; }

  bitboard inline get_bitboard(Colors color, Pieces piece) {
    return color_bitboards[color] & pieces_bitboards[piece];
  }
  bitboard inline get_bitboard(Pieces piece) {
    return color_bitboards[side_to_play] & pieces_bitboards[piece];
  }
};
#endif
