#include "search.hpp"
#include "uci.hpp"

int main() {
  Position pos = Position();

  MoveGenerator move_gen = MoveGenerator(&pos);
  Search sch = Search(&pos);

  Uci uci = Uci(&pos, &move_gen, &sch);


  uci.loop();

  return 0;
}
