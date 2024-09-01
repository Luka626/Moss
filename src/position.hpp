#include "datatypes.hpp"
#include "utils.hpp"
#include <vector>

class Position {
public:
  bitboard pieces_bitboards[NPIECES];
  bitboard color_bitboards[NCOLORS];
  size_t ply;
  size_t side_to_play;
  std::string en_passant_square;
  std::string castling_flags;
  size_t halfmove_clock;

  bitboard second_rank;
  bitboard seventh_rank;
  bitboard knight_attacks[64];
  int knight_jumps[8];

  Position();

  int set_board(const std::string& fen);
  std::vector<Move> generate_pseudo_legal_moves(Colors side_to_play);
  void make_move(Move &move);
  void undo_move(Move &move);
  void remove_piece(const Pieces piece, const Square location);

  friend std::ostream &operator<<(std::ostream &os, const Position &pos);

  bitboard inline get_occupied_bitboard() {
    bitboard occupied = 0ULL;
    for (const auto &bb : pieces_bitboards) {
      occupied |= bb;
    }
    return occupied;
  };
};
