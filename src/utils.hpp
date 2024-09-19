#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "datatypes.hpp"
#include <array>
#include <iostream>
#include <string>

namespace Utils {

// Constant masks used in other parts of the engine
constexpr bitboard RANK_MASK[8] = {
    0xFF,         0xFF00,         0xFF0000,         0xFF000000,
    0xFF00000000, 0xFF0000000000, 0xFF000000000000, 0xFF00000000000000};
constexpr bitboard FILE_MASK[8] = {0x0101010101010101, 0x202020202020202,
                                   0x404040404040404,  0x808080808080808,
                                   0x1010101010101010, 0x2020202020202020,
                                   0x4040404040404040, 0x8080808080808080};
//
// Navigate [Index] <-> [Rank, File]
inline size_t rank(Square sq) { return sq >> 3; }
inline size_t file(Square sq) { return sq & 7; }
inline Square get_square(size_t rank, size_t file) {
  return (Square)(8 * rank + file);
}

// Functions to return sliding piece masks from a given square
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

inline bitboard rank_mask(Square square) { return RANK_MASK[rank(square)]; }
inline bitboard file_mask(Square square) { return FILE_MASK[file(square)]; }

const std::string STARTING_FEN_POSITION =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Extern means declared here but defined in utils.cpp
extern bitboard knight_attacks[64];
extern int knight_jumps[8];

extern int king_jumps[8];
extern bitboard king_attacks[64];

extern int wpawn_jumps[2];
extern int bpawn_jumps[2];
extern bitboard wpawn_attacks[64];
extern bitboard bpawn_attacks[64];

extern bitboard IN_BETWEEN[64][64];

const size_t HASHSIZE = 128 * 1024 * 1024 / sizeof(TT_Entry);
extern TT_Entry TT[HASHSIZE];

extern int MVV_LVA[NPIECES][NPIECES];

extern int WPAWN_POSITIONAL[NSQUARES];
extern int WKNIGHT_POSITIONAL[NSQUARES];
extern int WBISHOP_POSITIONAL[NSQUARES];
extern int WROOK_POSITIONAL[NSQUARES];
extern int WQUEEN_POSITIONAL[NSQUARES];
extern int WKING_POSITIONAL[NSQUARES];

extern int MATERIAL_VALUE[NPIECES];
// Init function where we define all the extern attributes
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

// Functions to do bit operations on bitboards
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
  // Brian Kernighan's Way
  size_t count = 0;
  while (bb) {
    count++;
    bb &= bb - 1; // reset LS1B
  }
  return count;
}

// reverse bitboard by rotation, credit Antonin GAVREL on stackoverflow
// https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
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

// Pretty print a single bitboard
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

} // namespace Utils
#endif
