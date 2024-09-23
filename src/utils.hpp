#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "datatypes.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Utils {

/////////////////
/* Global Vars */
/////////////////
static const size_t MAX_DEPTH = 64;
static const size_t MAX_PLY = 512;
static const std::string STARTING_FEN_POSITION =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

///////////////////////////////
/* [Square] <-> [Rank, File] */
///////////////////////////////
inline size_t rank(Square sq) { return sq >> 3; }
inline size_t file(Square sq) { return sq & 7; }
inline Square get_square(size_t rank, size_t file) {
  return (Square)(8 * rank + file);
}

//////////////////////////////
/* [Square] --> {Ray Masks} */
//////////////////////////////
constexpr bitboard RANK_MASK[8] = {
    0xFF,         0xFF00,         0xFF0000,         0xFF000000,
    0xFF00000000, 0xFF0000000000, 0xFF000000000000, 0xFF00000000000000};
constexpr bitboard FILE_MASK[8] = {0x0101010101010101, 0x202020202020202,
                                   0x404040404040404,  0x808080808080808,
                                   0x1010101010101010, 0x2020202020202020,
                                   0x4040404040404040, 0x8080808080808080};
inline bitboard rank_mask(Square square) { return RANK_MASK[rank(square)]; }
inline bitboard file_mask(Square square) { return FILE_MASK[file(square)]; }
inline bitboard diagonal_mask(Square square) {
  bitboard main_diag = 0x8040201008040201;
  int diag = (square & 7) - (square >> 3);
  return diag >= 0 ? main_diag >> diag * 8 : main_diag << -diag * 8;
}
inline bitboard anti_diagonal_mask(Square square) {
  bitboard main_diag = 0x0102040810204080;
  int diag = 7 - (square & 7) - (square >> 3);
  return diag >= 0 ? main_diag >> diag * 8 : main_diag << -diag * 8;
}

/////////////////////
/** ATTACK TABLES **/
/////////////////////
extern int knight_jumps[8];
extern int king_jumps[8];
extern int wpawn_jumps[2];
extern int bpawn_jumps[2];

extern bitboard wpawn_attacks[64];
extern bitboard bpawn_attacks[64];
extern bitboard king_attacks[64];
extern bitboard knight_attacks[64];

extern bitboard IN_BETWEEN[64][64];

/////////////////////////
/* Transposition Table */
/////////////////////////
const size_t HASHSIZE = 128 * 1024 * 1024 / sizeof(TT_Entry);
extern std::vector<TT_Entry> TT;

///////////////////////////////
/* Bitboard Helper Functions */
///////////////////////////////
inline bitboard get_bit(const bitboard bitboard, const int index) {
  return bitboard & (1ULL << index);
}
inline void set_bit(bitboard &bitboard, const int index) {
  bitboard |= (1ULL << index);
}
inline bitboard set_bit(const int index) { return 1ULL << index; }
inline Square lsb(const bitboard bb) {
  return static_cast<Square>(__builtin_ctzll(bb));
}
inline size_t pop_count(bitboard bb) {
  size_t count = 0;
  while (bb) {
    count++;
    bb &= bb - 1;
  }
  return count;
}
inline bitboard reverse(bitboard bb) {
  bb =
      ((bb >> 8) & 0x00FF00FF00FF00FFULL) | ((bb << 8) & 0xFF00FF00FF00FF00ULL);
  bb = ((bb >> 16) & 0x0000FFFF0000FFFFULL) |
       ((bb << 16) & 0xFFFF0000FFFF0000ULL);
  bb = ((bb >> 32) & 0x00000000FFFFFFFFULL) |
       ((bb << 32) & 0xFFFFFFFF00000000ULL);
  return bb;
}
inline Square pop_bit(bitboard &bb) {
  Square output = lsb(bb);
  bb &= bb - 1;
  return output;
}
inline void print_bitboard(const bitboard bitboard) {
  std::cout << "\n";
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << " " << rank + 1 << "  ";
    for (int file = 0; file <= 7; file++) {
      int square = rank * 8 + file;
      std::cout << " " << (bitboard & (1ULL << square) ? 1 : 0);
    }
    std::cout << "\n";
  }
  std::cout << "\n     a b c d e f g h\n";
}

//////////////////////////////
/* Initialize Static Tables */
//////////////////////////////
void init();
void clear_TT();
void generate_knight_jumps();
void generate_king_jumps();
void generate_wpawn_jumps();
void generate_bpawn_jumps();
void generate_bpawn_attacks();
void generate_wpawn_attacks();
void generate_knight_attacks();
void generate_king_attacks();
void generate_in_between();

} // namespace Utils
#endif
