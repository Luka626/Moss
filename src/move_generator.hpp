#ifndef MOVE_GENERATOR_HPP_
#define MOVE_GENERATOR_HPP_

#include "datatypes.hpp"
#include "position.hpp"
#include "utils.hpp"
#include <array>
#include <sstream>

class MoveGenerator {
public:
  MoveGenerator(Position * position_ptr);
  double perft(size_t depth);
  MoveList generate_pseudo_legal_moves();
  MoveList generate_legal_moves();
  double divide(size_t depth);
  bool validate_gamestate();
  bool king_in_check(Colors color);

  void generate_pawn_moves(MoveList &moves_list, bitboard bb);
  void generate_knight_moves(MoveList &moves_list, bitboard bb);
  void generate_bishop_moves(MoveList &moves_list, bitboard bb);
  void generate_rook_moves(MoveList &moves_list, bitboard bb);
  void generate_queen_moves(MoveList &moves_list, bitboard bb);
  void generate_king_moves(MoveList &moves_list, bitboard bb);

  int generate_rank_attack(int occupancy, size_t file);
  void initiate_rank_attacks();
  bitboard generate_attackers(Square sq);
  bitboard generate_pinned_pieces();

  bitboard inline xray_rectilinear_attacks(bitboard occupancy,
                                           bitboard blockers, Square sq) {
    bitboard attacks = generate_rectilinear_attacks(occupancy, sq);
    blockers &= attacks;
    return attacks ^ generate_rectilinear_attacks(occupancy ^ blockers, sq);
  }
  bitboard inline xray_diagonal_attacks(bitboard occupancy, bitboard blockers,
                                        Square sq) {
    bitboard attacks = generate_diagonal_attacks(occupancy, sq);
    blockers &= attacks;
    return attacks ^ generate_diagonal_attacks(occupancy ^ blockers, sq);
  }

  bitboard generate_rectilinear_attacks(bitboard occupancy, Square sq);
  bitboard generate_diagonal_attacks(bitboard occupancy, Square sq);
// https://www.chessprogramming.org/Subtracting_a_Rook_from_a_Blocking_Piece
bitboard inline hyperbola_quintessence(bitboard occupancy, Square sq,
                                               bitboard mask) {
  // exclude the slider from the mask
  mask = mask & (~Utils::set_bit(sq));
  bitboard x = Utils::set_bit(sq);
  bitboard o = occupancy & mask;
  return ((o - x) ^
          Utils::reverse(Utils::reverse(o) - (0x8000000000000000ull >> sq))) &
         mask;
}
void inline add_quiet_moves(MoveList &move_list, bitboard bb,
                                    Pieces piece, Square origin) {
  while (bb) {
    Square destination_square = Utils::pop_bit(bb);
    Move quiet_move = {};
    quiet_move.piece = piece;
    quiet_move.from = origin;
    quiet_move.to = destination_square;
    move_list.push_back(quiet_move);
  }
}

void inline add_capture_moves(MoveList &move_list, bitboard bb,
                                      Pieces piece, Square origin) {
  while (bb) {
    Square destination_square = Utils::pop_bit(bb);
    Move capture = {};
    capture.piece = piece;
    capture.from = origin;
    capture.to = destination_square;
    capture.is_capture = true;
    move_list.push_back(capture);
  }
}
  void add_castling_moves(MoveList &moves_list);

inline Move move_from_string(const std::string &str) {
  MoveList moves = generate_pseudo_legal_moves();
  for (size_t i = 0; i < moves.size(); i++) {
    std::stringstream ss;
    ss << moves.at(i);
    std::string move_str = ss.str();
    if (move_str == str) {
      return moves.at(i);
    }
  }
  return Move();
}

private:
    
  Position *position;
  std::array<uint8_t, 512> RANK_ATTACKS;
};
#endif
