#ifndef UCI_HPP_
#define UCI_HPP_

#include "move_generator.hpp"
#include "position.hpp"

class Uci {
public:
  Uci(Position *position_ptr, MoveGenerator *movegen_ptr);
  void loop();
  void parse_go(const std::string &go) const;

private:
  Position *pos;
  MoveGenerator *move_gen;

}; // namespace UCI

#endif
