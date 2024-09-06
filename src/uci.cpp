#include "uci.hpp"
#include "datatypes.hpp"
#include "move_generator.hpp"
#include "search.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

Uci::Uci(Position *position_ptr, MoveGenerator *movegen_ptr) {
  pos = position_ptr;
  move_gen = movegen_ptr;
}

void Uci::loop() {
  std::string command;
  std::string word;

  std::cout.setf(std::ios::unitbuf);

  while (std::getline(std::cin, command)) {
    std::istringstream iss(command);

    std::getline(iss, word, ' ');

    if (word == "uci") {
      std::cout << "id name lChess 0.1" << std::endl;
      std::cout << "id author Luka Andjelic" << std::endl;
      std::cout << "uciok" << std::endl;
    }

    if (word == "isready") {
      std::cout << "readyok" << std::endl;
    }

    if (word == "position") {
      std::getline(iss, word, ' ');
      if (word == "startpos") {
        pos->set_board(Utils::STARTING_FEN_POSITION);
      } else if (word == "fen"){
          pos->set_board(word);
      }
      while (std::getline(iss, word, ' ')){
              if (word == "moves"){
              continue;
              }
              Move mv = move_gen->move_from_string(word);
              pos->make_move(mv);
      }

    }
    if (word == "go") {
      std::getline(iss, word);
      parse_go(word);
    }
    if (word == "print") {
      std::cout << *pos << std::endl;
    }
    if (word == "quit") {
      break;
    }
  }
}

void Uci::parse_go(const std::string &go){
    std::istringstream iss(go);

    Search search = Search(pos);
    Move best_move = search.simple_search();

    std::cout << "bestmove " << best_move << std::endl;
}

