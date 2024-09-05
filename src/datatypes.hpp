#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <cstdint>
#include <ostream>

typedef uint64_t bitboard;

const int NCOLORS = 2;
enum Colors : int {
  WHITE,
  BLACK,
};
inline Colors operator~(Colors color) {
  return (color == WHITE) ? BLACK : WHITE;
}

const int NPIECES = 6;
enum Pieces : int { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

const int NDIRECTIONS = 8;
enum Compass : int {
  NW = 7,
  N = 8,
  NE = 9,
  W = -1,
  E = 1,
  SW = -9,
  S = -8,
  SE = -7
};

const int NSQUARES = 64;
enum Square : int {
  a1,
  b1,
  c1,
  d1,
  e1,
  f1,
  g1,
  h1,
  a2,
  b2,
  c2,
  d2,
  e2,
  f2,
  g2,
  h2,
  a3,
  b3,
  c3,
  d3,
  e3,
  f3,
  g3,
  h3,
  a4,
  b4,
  c4,
  d4,
  e4,
  f4,
  g4,
  h4,
  a5,
  b5,
  c5,
  d5,
  e5,
  f5,
  g5,
  h5,
  a6,
  b6,
  c6,
  d6,
  e6,
  f6,
  g6,
  h6,
  a7,
  b7,
  c7,
  d7,
  e7,
  f7,
  g7,
  h7,
  a8,
  b8,
  c8,
  d8,
  e8,
  f8,
  g8,
  h8
};
std::ostream inline &operator<<(std::ostream &os, const Square &sq) {
  int file = sq & 7;
  int rank = sq >> 3;
  char rank_ch = '0' + rank + 1;
  char file_ch = 'a' + file;
  os << file_ch << rank_ch;
  return os;
};
struct Move {
  Pieces piece;
  Square from;
  Square to;
  bool is_capture;
  Pieces captured_piece;
  bool is_double_push;
  bool is_castle;
  bool is_en_passant;
  Pieces promotion;
};
std::ostream inline &operator<<(std::ostream &os, const Move &mv) {
  os << mv.from << mv.to;
  return os;
}

#endif
