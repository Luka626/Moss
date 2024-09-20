#ifndef UCI_HPP_
#define UCI_HPP_

#include "move_generator.hpp"
#include "position.hpp"
#include "search.hpp"
#include <memory>

class Uci {
public:
  Uci(std::shared_ptr<Position> position_ptr);
  void loop();
  void parse_go(const std::string &go) const;
  void new_game();

private:
  std::shared_ptr<Position> pos;
  std::unique_ptr<MoveGenerator> move_gen;
  std::unique_ptr<Search> search;

}; // namespace UCI

#endif
