#include "datatypes.hpp"
#include "move_generator.hpp"
#include "position.hpp"
#include "utils.hpp"
#include <chrono>
#include <ios>
#include <iostream>
#include <memory>

int main() {

  bool starting = true;
  // test with perft
  size_t depth = 4;

  std::shared_ptr<Position> test_position;
  if (starting) {
    test_position = std::make_shared<Position>(Position());
  } else {
    test_position = std::make_shared<Position>(Position(
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "));
  }
  std::shared_ptr<MoveGenerator> test_move_generator =
      std::make_shared<MoveGenerator>(MoveGenerator(test_position));

  std::cout << *test_position;

  double nodes = 0;
  auto start = std::chrono::high_resolution_clock::now();
  nodes = test_move_generator->divide(depth);
  //nodes = test_move_generator->perft(depth);
  auto stop = std::chrono::high_resolution_clock::now();

  std::cout << std::fixed;
  std::cout << "Perft at depth: " << depth << std::endl;
  std::cout << "Nodes found: " << nodes << std::endl;

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "\nRuntime: " << duration.count() << "ms" << std::endl;
  std::cout << "\nSpeed: " << nodes / (duration.count() * 1e-3) << "nps"
            << std::endl;
  std::cout << std::scientific;

  auto move_list = test_move_generator->generate_pseudo_legal_moves();
  //std::vector<Move> move_list = {};
  for (auto move : move_list) {
      getchar();
    test_position->make_move(move);
    std::cout << *test_position << std::endl;
    if (!test_move_generator->validate_gamestate()) {
      std::cout << "Invalid position!!!";
    }
    test_position->undo_move(move);
    std::cout << *test_position << std::endl;
  }
  std::cout << "DONE!" << std::endl;

  std::cout << "Captures found at final depth: " << test_position->DEBUG_CAPTURES << std::endl;
  std::cout << "En-Passants found at final depth: " << test_position->DEBUG_EP << std::endl;
  std::cout << "Castles found at final depth: " << test_position->DEBUG_CASTLES << std::endl;
  std::cout << "Promotions found at final depth: " << test_position->DEBUG_PROMOTIONS << std::endl;
  return 0;
}
