#include "datatypes.hpp"
#include <iostream>

// BITBOARD HELPER FUNCTIONS
// Helper Functions
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

inline size_t rank(Square sq){
    return sq >> 3;
}

inline size_t file(Square sq){
    return sq & 7;
}

inline size_t get_square(size_t rank, size_t file){
    return 8*rank + file;
}


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
