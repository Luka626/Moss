#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include <memory>
#include <stack>
#include <string>
#include <vector>

class Position : public std::enable_shared_from_this<Position> {
public:
  // 6 piece and 2 color bitboards describe piece arrangement
  bitboard pieces_bitboards[NPIECES];
  bitboard color_bitboards[NCOLORS];

  // Encoded in FEN string, needed to establish state of play beyond piece
  // arrangement
  Colors side_to_play;
  std::stack<Square> en_passant_history;
  Square en_passant_square;
  std::stack<std::vector<bool>> castling_history;
  std::vector<bool> castling_flags;
  size_t halfmove_clock;
  size_t ply;

  size_t DEBUG_PROMOTIONS;
  size_t DEBUG_CASTLES;
  size_t DEBUG_EP;
  size_t DEBUG_CAPTURES;

  Position();
  Position(const std::string &fen);

  int set_board(const std::string &fen);

  void make_move(Move &move);
  void undo_move(Move &move);
  Pieces remove_piece(const Square location);


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
