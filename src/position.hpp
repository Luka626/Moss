#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "datatypes.hpp"
#include "utils.hpp"
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

  Position();
  Position(const std::string &fen);

  void new_game();
  int set_board(const std::string &fen);

  bool is_drawn() const;
  zobrist_key generate_key() const;
  void make_move(const Move move);
  void undo_move(const Move move);
  void make_null_move();
  void undo_null_move();
  Pieces inline get_piece_type(const Square sq) {
    for (int i = PAWN; i < NPIECES; i++) {
      if (pieces_bitboards[(Pieces)i] & Utils::set_bit(sq)) {
        return (Pieces)i;
      }
    }
    return PAWN;
  }

  void inline remove_piece(Pieces pc, const Square sq) {
    bitboard to_remove = 1ULL << sq;
    pieces_bitboards[pc] &= ~to_remove;
    color_bitboards[~side_to_play] &= ~to_remove;
  }
  void inline remove_pawn(const Square sq) {
    bitboard to_remove = 1ULL << sq;
    pieces_bitboards[Pieces::PAWN] &= ~to_remove;
    color_bitboards[~side_to_play] &= ~to_remove;
  }

  void inline update_castling_rights(const Square from, const Square to,
                                     const bool is_capture) {
    for (int i = 0; i < 4; ++i) {
      if (castling_flags[i] == false) {
        continue;
      }

      switch (i) {
      case 0:
        if ((from == h1) || (from == e1) || (is_capture && (to == h1))) {
          castling_flags[i] = false;
          z_key ^= Zobrist::CASTLING[i];
        }
        break;
      case 1:
        if ((from == a1) || (from == e1) || (is_capture && (to == a1))) {
          castling_flags[i] = false;
          z_key ^= Zobrist::CASTLING[i];
        }
        break;
      case 2:
        if ((from == e8) || (from == h8) || (is_capture && (to == h8))) {
          castling_flags[i] = false;
          z_key ^= Zobrist::CASTLING[i];
        }
        break;
      case 3:
        if ((from == e8) || (from == a8) || (is_capture && (to == a8))) {
          castling_flags[i] = false;
          z_key ^= Zobrist::CASTLING[i];
        }
        break;
      }
    }
  }
  friend std::ostream &operator<<(std::ostream &os, const Position &pos);

  // useful getters
  Move inline get_last_move() const { return last_move; }
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
  std::vector<Undo_Info> undo_info;
  Move last_move;
};
#endif
