#ifndef UCI_HPP_
#define UCI_HPP_

#include "move_generator.hpp"
#include "position.hpp"
#include "search.hpp"

class Uci {
public:
  Uci(Position *position_ptr, MoveGenerator *movegen_ptr, Search *search_ptr);
  void loop();
  void parse_go(const std::string &go) const;
  void new_game();

private:
  Position *pos;
  MoveGenerator *move_gen;
  Search *search;

}; // namespace UCI

#endif
