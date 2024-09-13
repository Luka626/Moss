#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include "zobrist.hpp"
#include <string>

class Position {
public:
  // 6 piece and 2 color bitboards describe piece arrangement
  bitboard pieces_bitboards[NPIECES];
  bitboard color_bitboards[NCOLORS];

  // Encoded in FEN string, needed to establish state of play beyond piece
  // arrangement
  Colors side_to_play;
  Square en_passant_square;
  bool castling_flags[4];
  size_t halfmove_clock;
  size_t ply;
  zobrist_key z_key;
  bool was_capture;

  Position();
  Position(const std::string &fen);

  int set_board(const std::string &fen);

  zobrist_key generate_key();
  void make_move(Move &move);
  void undo_move(Move &move);
  void inline remove_piece(Pieces pc, Square sq) {
    bitboard to_remove = 1ULL << sq;
    pieces_bitboards[pc] &= ~to_remove;
    color_bitboards[~side_to_play] &= ~to_remove;
  }

  void inline update_castling_rights(Square from, Square to, bool is_capture) {
    if (from == a1) {
      castling_flags[1] = false;
      z_key ^= Zobrist::CASTLING[1];
    } else if (from == h1) {
      castling_flags[0] = false;
      z_key ^= Zobrist::CASTLING[0];
    } else if (from == a8) {
      castling_flags[3] = false;
      z_key ^= Zobrist::CASTLING[3];
    } else if (from == h8) {
      castling_flags[2] = false;
      z_key ^= Zobrist::CASTLING[2];
    } else if (from == e1) {
      castling_flags[0] = false;
      castling_flags[1] = false;
      z_key ^= Zobrist::CASTLING[0];
      z_key ^= Zobrist::CASTLING[1];
    } else if (from == e8) {
      castling_flags[2] = false;
      castling_flags[3] = false;
      z_key ^= Zobrist::CASTLING[2];
      z_key ^= Zobrist::CASTLING[3];
    }

    if (is_capture) {
      if (to == a1) {
        castling_flags[1] = false;
        z_key ^= Zobrist::CASTLING[1];
      } else if (to == h1) {
        castling_flags[0] = false;
        z_key ^= Zobrist::CASTLING[0];
      } else if (to == a8) {
        castling_flags[3] = false;
        z_key ^= Zobrist::CASTLING[3];
      } else if (to == h8) {
        castling_flags[2] = false;
        z_key ^= Zobrist::CASTLING[2];
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

private:
  zobrist_key key_history[256];
  Square en_passant_history[256];
  bool castling_history[256][4];
};
#endif
