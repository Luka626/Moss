#ifndef MOVE_GENERATOR_HPP_
#define MOVE_GENERATOR_HPP_

#include "datatypes.hpp"
#include "position.hpp"
#include <cstdint>
#include <memory>
#include <vector>

class MoveGenerator : public std::enable_shared_from_this<MoveGenerator> {
public:
  MoveGenerator(std::shared_ptr<Position> position_ptr);
  double perft(size_t depth);
  std::vector<Move> generate_pseudo_legal_moves();
  double divide(size_t depth);
  bool validate_gamestate();

  void generate_pawn_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_knight_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_bishop_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_rook_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_queen_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_king_moves(std::vector<Move> &moves_list, bitboard bb);

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
  bitboard hyperbola_quintessence(bitboard bb, Square sq, bitboard mask);

  void add_quiet_moves(std::vector<Move> &moves_list, bitboard bb, Pieces piece,
                       Square origin);
  void add_castling_moves(std::vector<Move> &moves_list);
  void add_capture_moves(std::vector<Move> &moves_list, bitboard bb,
                         Pieces piece, Square origin);

private:
  std::shared_ptr<Position> position;
  std::array<uint8_t, 512> RANK_ATTACKS;
};
#endif
