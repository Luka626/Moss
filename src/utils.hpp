#ifndef Utils_HPP_
#define Utils_HPP_

#include "datatypes.hpp"
#include <iostream>
#include <string>

namespace Utils {

// Constant masks used in other parts of the engine
const bitboard FIRST_RANK = 0xFF;
const bitboard SECOND_RANK = 0xFF00;
const bitboard THIRD_RANK = 0xFF0000;
const bitboard FOURTH_RANK = 0xFF000000;
const bitboard FIFTH_RANK = 0xFF00000000;
const bitboard SIXTH_RANK = 0xFF0000000000;
const bitboard SEVENTH_RANK = 0xFF000000000000;
const bitboard EIGHTH_RANK = 0xFF00000000000000;

const std::string STARTING_FEN_POSITION =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Extern means declared here but defined in utils.cpp
extern bitboard knight_attacks[64];
extern int knight_jumps[8];

// Init function where we define all the extern attributes
void init();

// Functions to do bit operations on bitboards
inline bitboard get_bit(bitboard bitboard, int index) {
  return bitboard & (1ULL << index);
}

inline void set_bit(bitboard &bitboard, int index) {
  bitboard |= (1ULL << index);
}

inline Square lsb(bitboard bb) {
  return static_cast<Square>(__builtin_ctzll(bb));
}

inline Square pop_bit(bitboard &bb) {
  Square output = lsb(bb);
  bb &= bb - 1;
  return output;
}

// Navigate [Index] <-> [Rank, File]
inline size_t rank(Square sq) { return sq >> 3; }
inline size_t file(Square sq) { return sq & 7; }
inline size_t get_square(size_t rank, size_t file) { return 8 * rank + file; }

// Pretty print a single bitboard
inline void print_bitboard(bitboard bitboard) {
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
