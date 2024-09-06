#include "datatypes.hpp"
#include "move_generator.hpp"
#include "position.hpp"
#include <chrono>
#include <ios>
#include <iostream>
#include <memory>

int main() {

  bool starting = true;
  // test with perft
  size_t depth = 4;

  Position test_position;
  if (starting) {
    test_position = Position();
  } else {
    test_position = Position(
        "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/P3P1P1/1pN2Q1p/1PPBBP1P/R3K2R b KQkq - 0 1");
  }
  std::shared_ptr<MoveGenerator> test_move_generator =
      std::make_shared<MoveGenerator>(MoveGenerator(&test_position));

  //std::cout << test_position;

  double nodes = 0;
  auto start = std::chrono::high_resolution_clock::now();
  //nodes = test_move_generator->divide(depth);
  nodes = test_move_generator->perft(depth);
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

  /*
  auto move_list = test_move_generator->generate_pseudo_legal_moves();
  move_list = MoveList();
  for (size_t ind = 0; ind < move_list.size(); ind++) {
      Move move = move_list.at(ind);
      getchar();
    test_position.make_move(move);
    std::cout << test_position << std::endl;
    if (!test_move_generator->validate_gamestate()) {
      std::cout << "Invalid position!!!";
    }
      getchar();
    test_position.undo_move(move);
    std::cout << test_position << std::endl;
  }
  */
  std::cout << "DONE!" << std::endl;

  return 0;
}
