#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include "utils.hpp"
#include "zobrist.hpp"
#include <string>
#include <vector>

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
  std::vector<zobrist_key> position_history;

  Position();
  Position(const std::string &fen);

  int set_board(const std::string &fen);

  zobrist_key generate_key() const;
  void make_move(const Move move);
  void undo_move(const Move move);
  Pieces inline get_piece_type(const Square sq) {
    for (int i = PAWN; i < NPIECES; i++) {
      if (pieces_bitboards[(Pieces)i] & Utils::set_bit(sq)) {
        return (Pieces)i;
      }
    }
    return PAWN;
  }

  void inline remove_piece(const Square sq) {
    bitboard to_remove = 1ULL << sq;
    Pieces pc = pieces_bitboards[pc] &= ~to_remove;
    color_bitboards[~side_to_play] &= ~to_remove;
  }

  void inline update_castling_rights(const Square from, const Square to,
                                     const bool is_capture) {
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
  bitboard inline get_occupied() const {
    return color_bitboards[WHITE] | color_bitboards[BLACK];
  }
  bitboard inline get_empty() const { return ~get_occupied(); }

  bitboard inline get_enemy(const Colors &side) const {
    return color_bitboards[~side];
  }
  bitboard inline get_enemy() const {
    return color_bitboards[~(Colors)side_to_play];
  }

  bitboard inline get_bitboard(const Colors color, const Pieces piece) const {
    return color_bitboards[color] & pieces_bitboards[piece];
  }
  bitboard inline get_bitboard(const Pieces piece) const {
    return color_bitboards[side_to_play] & pieces_bitboards[piece];
  }

private:
  zobrist_key key_history[256];
  Square en_passant_history[256];
  bool castling_history[256][4];
};
#endif
