#include "uci.hpp"
#include <memory>

int main() {
  auto pos = std::make_shared<Position>();

  Uci uci = Uci(pos);

  uci.loop();

  return 0;
}
