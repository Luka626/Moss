#ifndef MOVE_LIST_HPP_
#define MOVE_LIST_HPP_
#include "datatypes.hpp"

class MoveList {
private:
  Move moves[128];
  size_t count;

  enum MoveScores : int {
    TTMOVE = 250,
    KILLER1 = 100,
    KILLER2 = 50,
    MVV_LVA_OFFSET = 150,
  };

  // MVV_LVA[victim][attacker]
  static constexpr int MVV_LVA[NPIECES][NPIECES] = {
      // P   N   B   R   Q   K
      {15, 14, 13, 12, 11, 10}, // P
      {25, 24, 23, 22, 21, 20}, // N
      {35, 34, 33, 32, 31, 30}, // B
      {45, 44, 43, 42, 41, 40}, // R
      {55, 54, 53, 52, 51, 50}, // Q
      {0, 0, 0, 0, 0, 0},       // K
  };

public:
  inline MoveList() { count = 0; }
  constexpr size_t clear() {
    count = 0;
    return count;
  }
  inline void push_back(const Move &mv) { moves[count++] = mv; }
  inline Move &at(const size_t index) { return moves[index]; }
  constexpr size_t size() const { return count; }
  constexpr bool empty() const { return count == 0; }
  void score_moves(Move TT_move, Move killer1, Move killer2);
  void sort_moves();
};

struct CompareMoves {
  inline bool operator()(Move mv1, Move mv2) const {
    return mv1.sort_score > mv2.sort_score;
  }
};
#endif
