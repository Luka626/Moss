#include "search.hpp"
#include "eval.hpp"
#include "move_generator.hpp"

Search::Search(Position *position_ptr) { pos = position_ptr; }

Move Search::simple_search() {
  Move best_move = {};

  MoveGenerator move_gen = MoveGenerator(pos);
  Evaluator eval = Evaluator(pos);

  MoveList moves = MoveList();
  moves = move_gen.generate_pseudo_legal_moves();

  int evaluation = (pos->side_to_play == WHITE) ? -9999 : 9999;

  for (size_t i = 0; i < moves.size(); i++) {
    std::cout << moves.at(i) << pos->side_to_play << std::endl;
    Move mv = moves.at(i);
    pos->make_move(mv);
    if (move_gen.validate_gamestate()) {
      int move_eval = eval.evaluate();
      if (~pos->side_to_play == WHITE) {
        if (move_eval > evaluation) {
          evaluation = move_eval;
          best_move = mv;
        }
      } else if (~pos->side_to_play == BLACK) {
        int move_eval = eval.evaluate();
        if (move_eval < evaluation) {
          evaluation = move_eval;
          best_move = mv;
          std::cout << evaluation << mv << std::endl;
        }
      }
    }
    pos->undo_move(mv);
  }
  return best_move;
}
