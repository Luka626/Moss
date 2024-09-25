#include "uci.hpp"
#include <memory>

int main() {
  auto uci = std::make_unique<Uci>(Uci());
  uci->loop();
  return 0;
}
