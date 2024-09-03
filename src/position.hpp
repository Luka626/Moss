#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include <memory>
#include <string>

class Position : public std::enable_shared_from_this<Position> {
public:
  // 6 piece and 2 color bitboards describe piece arrangement
  bitboard pieces_bitboards[NPIECES];
  bitboard color_bitboards[NCOLORS];

  // Encoded in FEN string, needed to establish state of play beyond piece
  // arrangement
  size_t side_to_play;
  std::string en_passant_square;
  std::string castling_flags;
  size_t halfmove_clock;
  size_t ply;

  Position();
  Position(const std::string &fen);

  int set_board(const std::string &fen);

  void make_move(Move &move);
  void undo_move(Move &move);
  void remove_piece(const Pieces piece, const Square location);

  friend std::ostream &operator<<(std::ostream &os, const Position &pos);

  // useful getters
  bitboard inline get_occupied() {
    return color_bitboards[WHITE] | color_bitboards[BLACK];
  }
  bitboard inline get_empty() { return ~get_occupied(); }

  bitboard inline get_enemy(Colors side) {
    return color_bitboards[~side];
  }
  bitboard inline get_enemy() {
    return color_bitboards[~(Colors)side_to_play];
  }

  bitboard inline get_bitboard(Colors color, Pieces piece) {
    return color_bitboards[color] & pieces_bitboards[piece];
  }
  bitboard inline get_bitboard(Pieces piece) {
    return color_bitboards[side_to_play] & pieces_bitboards[piece];
  }
};
#endif
