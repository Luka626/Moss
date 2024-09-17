#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <climits>
#include <cstdint>
#include <ostream>

typedef uint64_t bitboard;
typedef uint64_t zobrist_key;

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
std::ostream inline &operator<<(std::ostream &os, const Pieces &pc) {
  char pc_ch[6] = {'p', 'n', 'b', 'r', 'q', 'k'};
  os << pc_ch[pc];
  return os;
};

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
  bool is_capture = false;
  Pieces captured_piece;
  bool is_double_push;
  bool is_castle;
  bool is_en_passant;
  Pieces promotion;
  int sort_score = 0;
};
std::ostream inline &operator<<(std::ostream &os, const Move &mv) {
  os << mv.from << mv.to;
  if (mv.promotion) {
    os << mv.promotion;
  }
  return os;
}
bool inline operator==(const Move &lhs, const Move &rhs) {
  if (!(lhs.to == rhs.to)) {
    return false;
  }

  if (!(lhs.from == rhs.from)) {
    return false;
  }

  if (!(lhs.piece == rhs.piece)) {
    return false;
  }

  if (!(lhs.is_castle == rhs.is_castle)) {
    return false;
  }
  if (!(lhs.is_capture == rhs.is_capture)) {
    return false;
  }
  if (!(lhs.is_en_passant == rhs.is_en_passant)) {
    return false;
  }
  if (!(lhs.is_double_push == rhs.is_double_push)) {
    return false;
  }

  return true;
}

struct Undo_Info {
  zobrist_key key;
  int halfmove_clock;
  Square en_passant_square;
  bool castling_flags[4];
};

const int NNODETYPES = 3;
enum NodeType : int { EXACT, LOWER, UPPER, NONETYPE };

struct TT_Entry {
  zobrist_key key;
  size_t depth;
  int evaluation;
  NodeType type;
  Move best_move;

  TT_Entry()
      : key(0ULL), depth(0), evaluation(0), type(NONETYPE), best_move(Move()) {}

  TT_Entry(zobrist_key key, size_t depth, int evaluation, NodeType node_type,
           Move best_move)
      : key(key), depth(depth), evaluation(evaluation), type(node_type),
        best_move(best_move) {}
};

enum Scores : int { DRAW = 0, CHECKMATE = -INT_MAX };
#endif
