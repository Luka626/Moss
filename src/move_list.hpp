#ifndef MOVE_LIST_HPP_
#define MOVE_LIST_HPP_
#include "datatypes.hpp"

class MoveList {
private:
  Move moves[128];
  size_t count;

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
  void score_moves(Move TT_move);
  void sort_moves();
};

struct CompareMoves {
  inline bool operator()(Move mv1, Move mv2) const {
    return mv1.sort_score > mv2.sort_score;
  }
};
#endif
