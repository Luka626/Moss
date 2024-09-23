#ifndef ATTACK_TABLES_HPP_
#define ATTACK_TABLES_HPP_

#include "datatypes.hpp"
#include <cstddef>

namespace AttackTables {
struct Square_Data {
  Square square;
  int rank;
  int file;
};
template <size_t n_dir> struct Attack_Data {
  Pieces PIECE = PAWN;
  int JUMPS[n_dir];
  bitboard ATTACKS[NSQUARES];
};

constexpr size_t PAWN_DIR = 2;
extern Attack_Data<PAWN_DIR> W_PAWN;
extern Attack_Data<PAWN_DIR> B_PAWN;


constexpr size_t KNIGHT_DIR = 8;
extern Attack_Data<KNIGHT_DIR> KNIGHT;

constexpr size_t KING_DIR = 8;
extern Attack_Data<KING_DIR> KING;

void init();
template<size_t n_dir>
auto constexpr compute_attack_bitboard(Attack_Data<n_dir> attack_data,
                                 const Square_Data starting) -> bitboard;
} // namespace AttackTables

#endif
