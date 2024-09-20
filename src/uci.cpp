#include "uci.hpp"
#include "datatypes.hpp"
#include "move_generator.hpp"
#include "search.hpp"
#include "utils.hpp"
#include <cctype>
#include <climits>
#include <memory>
#include <sstream>
#include <string>

Uci::Uci(std::shared_ptr<Position> position_ptr)
    : pos(position_ptr),
      move_gen(std::make_unique<MoveGenerator>(position_ptr)),
      search(std::make_unique<Search>(position_ptr)) {
  new_game();
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

      } else if (word == "fen") {
        std::getline(iss, word);
        pos->set_board(word);
      }
      while (std::getline(iss, word, ' ')) {
        if (word == "moves") {
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
    if (word == "ucinewgame") {
      new_game();
    }
    if (word == "quit") {
      break;
    }
  }
}

void Uci::parse_go(const std::string &go) const {
  std::istringstream iss(go);
  std::string token;
  std::getline(iss, token, ' ');
  int time = INT_MAX;
  int moves_remaining = 40;

  if (isdigit(token[0])) {
    int multiplier = std::stoi(token);
    time = 100 * multiplier;
    moves_remaining = 1;
  } else {
    if (token == "wtime") {
      std::getline(iss, token, ' ');
      if (pos->side_to_play == WHITE) {
        time = std::stoi(token);
      }
      std::getline(iss, token, ' ');
    }
    if (token == "btime") {
      std::getline(iss, token, ' ');
      if (pos->side_to_play == BLACK) {
        time = std::stoi(token);
      }
    std::getline(iss, token, ' ');
    }
    if (token == "movestogo") {
      std::getline(iss, token, ' ');
      moves_remaining = std::stoi(token);
    }
  }
  if (token == "perft") {
    std::getline(iss, token, ' ');
    move_gen->divide(std::stoi(token));
  } else {
    search->new_search();
    search->iterative_deepening(time, moves_remaining);
    std::cout << "bestmove " << search->get_best_move();
    std::cout << std::endl;
  }
}

void Uci::new_game() {
  Zobrist::init();
  Utils::init();
  pos->new_game();
  move_gen->new_game();
  search->new_game();
}
