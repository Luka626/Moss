#include "move_generator.hpp"
#include "datatypes.hpp"
#include <cmath>

MoveGenerator::MoveGenerator(std::shared_ptr<Position> position_ptr) {
  position = position_ptr;
};

std::vector<Move> MoveGenerator::generate_pseudo_legal_moves() {
  std::vector<Move> pseudo_legal_moves = {};

  bitboard pawn_bb = position->get_bitboard(Pieces::PAWN);
  bitboard knight_bb = position->get_bitboard(Pieces::KNIGHT);
  bitboard bishop_bb = position->get_bitboard(Pieces::BISHOP);
  bitboard rook_bb = position->get_bitboard(Pieces::ROOK);
  bitboard queen_bb = position->get_bitboard(Pieces::QUEEN);
  bitboard king_bb = position->get_bitboard(Pieces::KING);

  generate_pawn_moves(pseudo_legal_moves, pawn_bb);


  // TODO:
  // [] implement all of these vvv
  generate_knight_moves(pseudo_legal_moves, knight_bb);
  generate_bishop_moves(pseudo_legal_moves, bishop_bb);
  generate_rook_moves(pseudo_legal_moves, rook_bb);
  generate_queen_moves(pseudo_legal_moves, queen_bb);
  generate_king_moves(pseudo_legal_moves, king_bb);

  return pseudo_legal_moves;
}

// TODO:
// [x] double pawn pushes
// [] separate generating the move bitboard with generating the list of moves
// [] pawn captures (using capture table)
// [] en-passant (don't worry about this yet?)
void MoveGenerator::generate_pawn_moves(std::vector<Move> &move_list,
                                        bitboard bb) {

  if (position->side_to_play == WHITE) {
    bitboard single_pawn_push = bb << N;
    single_pawn_push &= ~position->get_occupied_bitboard();
    bitboard double_pawn_push = 0ULL;
    if ((bb & Utils::SECOND_RANK) > 0) {
      double_pawn_push = bb << N << N;
      double_pawn_push &= ~position->get_occupied_bitboard();
    }
    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (single_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(single_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S);
      move_list.push_back(psudeo_legal_move);
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (double_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(double_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S + S);
      move_list.push_back(psudeo_legal_move);
    }

  } else if (position->side_to_play == BLACK) {
    bitboard single_pawn_push = bb >> N;
    Utils::print_bitboard(single_pawn_push);
    single_pawn_push &= ~position->get_occupied_bitboard();

    bitboard double_pawn_push = 0ULL;
    if ((bb & Utils::SEVENTH_RANK) > 0) {
      double_pawn_push = bb >> N >> N;
      double_pawn_push &= ~position->get_occupied_bitboard();
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (single_pawn_push != 0ULL) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(single_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N);
      move_list.push_back(psudeo_legal_move);
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (double_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(double_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N + N);
      move_list.push_back(psudeo_legal_move);
    }
  }
}
