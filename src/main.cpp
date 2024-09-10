#include "uci.hpp"

int main() {
  Position pos = Position();
  MoveGenerator move_gen = MoveGenerator(&pos);
  Uci uci = Uci(&pos, &move_gen);

  uci.loop();

  return 0;
}
