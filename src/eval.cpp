#include "eval.hpp"
#include "datatypes.hpp"
#include "utils.hpp"
#include <memory>

Evaluator::Evaluator(std::shared_ptr<Position> position_ptr)
    : pos(position_ptr) {}

// Evaluation entry point
int Evaluator::evaluate() const {
  int eval = 0;
  for (int i = PAWN; i < NPIECES; i++) {
    Pieces piece = static_cast<Pieces>(i);
    eval += piece_evaluation(Colors::WHITE, piece);
    eval -= piece_evaluation(Colors::BLACK, piece);
  }
  // For our negamax implementation we evaluate with
  // with respect to the side to play.
  eval = (pos->side_to_play == WHITE) ? eval : -eval;
  return eval;
}

// Simplified piece-based evaluation
// Adds material and PST evals
int Evaluator::piece_evaluation(const Colors side, const Pieces piece) const {
  bitboard pieces_bb = pos->get_bitboard(side, piece);
  return material_evaluation(piece, pieces_bb) +
         pst_evaluation(side, piece, pieces_bb);
}

// Sums material value of a piece bitboard
int Evaluator::material_evaluation(const Pieces piece,
                                   const bitboard bb) const {
  return Utils::pop_count(bb) * MATERIAL_VALUE[piece];
}

// Returns piece-square table evaluation of a piece bitboard
int Evaluator::pst_evaluation(const Colors side, const Pieces piece,
                              bitboard bb) const {
  int evaluation = 0;
  while (bb) {
    Square square = Utils::pop_bit(bb);
    square = (side == WHITE) ? square : static_cast<Square>(square ^ 56);
    evaluation += PST[piece][square];
  }
  return evaluation;
}
