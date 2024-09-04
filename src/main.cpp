#include "datatypes.hpp"
#include "move_generator.hpp"
#include "position.hpp"
#include <chrono>
#include <iostream>
#include <memory>

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  std::shared_ptr<Position> test_position = std::make_shared<Position>(
      Position("8/6K1/3Rb3/8/3pr3/1P1nPNPk/pB1p3P/6Q1 w - - 0 1"));
  std::shared_ptr<MoveGenerator> test_move_generator =
      std::make_shared<MoveGenerator>(MoveGenerator(test_position));

  std::cout << *test_position;
  std::vector<Move> moves_list =
      test_move_generator->generate_pseudo_legal_moves();
  std::cout << "Perft, depth 0: " << moves_list.size() << std::endl;

  ////*
  for (const auto &move : moves_list) {
  //  std::cout << move.piece << " " << move.from << " " << move.to << std::endl;
    if (move.from == e3){
        Move test_move = move;
        test_position->make_move(test_move);
    }
  }
  //*/
  //
  std::cout << *test_position;

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "\nRuntime: " << duration.count() << " ms" << std::endl;

  return 0;
}
