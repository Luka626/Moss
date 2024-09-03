#include "move_generator.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

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
  generate_knight_moves(pseudo_legal_moves, knight_bb);
  generate_king_moves(pseudo_legal_moves, king_bb);

  // TODO:
  // [] implement all of these vvv
  generate_bishop_moves(pseudo_legal_moves, bishop_bb);
  generate_rook_moves(pseudo_legal_moves, rook_bb);
  generate_queen_moves(pseudo_legal_moves, queen_bb);

  return pseudo_legal_moves;
}

// TODO:
// [x] double pawn pushes
// [] pawn captures (using capture table)
// [] en-passant (don't worry about this yet?)
void MoveGenerator::generate_pawn_moves(std::vector<Move> &move_list,
                                        bitboard bb) {

  if (position->side_to_play == WHITE) {
    bitboard single_pawn_push = bb << N;
    single_pawn_push &= ~position->get_occupied();
    bitboard double_pawn_push = 0ULL;
    if ((bb & Utils::SECOND_RANK) > 0) {
      double_pawn_push = (bb & Utils::SECOND_RANK) << N << N;
      double_pawn_push &= ~position->get_occupied();
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
    single_pawn_push &= ~position->get_occupied();

    bitboard double_pawn_push = 0ULL;
    if ((bb & Utils::SEVENTH_RANK) > 0) {
      double_pawn_push = (bb & Utils::SEVENTH_RANK) >> N >> N;
      double_pawn_push &= ~position->get_occupied();
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
void MoveGenerator::generate_knight_moves(std::vector<Move> &move_list,
                                          bitboard bb) {

  std::vector<Move> quiet_moves = {};
  std::vector<Move> capture_moves = {};

  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard knight_moves = Utils::knight_attacks[origin_square];

    bitboard quiet_moves_bb = knight_moves & position->get_empty();
    bitboard capture_moves_bb = knight_moves & position->get_enemy();

    add_quiet_moves(quiet_moves, quiet_moves_bb, Pieces::KNIGHT, origin_square);
    add_capture_moves(capture_moves, capture_moves_bb, Pieces::KNIGHT,
                      origin_square);
  }

  move_list.insert(std::end(move_list), std::begin(quiet_moves),
                   std::end(quiet_moves));
  move_list.insert(std::end(move_list), std::begin(capture_moves),
                   std::end(capture_moves));
}

void MoveGenerator::generate_king_moves(std::vector<Move> &move_list,
                                        bitboard bb) {
  std::vector<Move> quiet_moves = {};
  std::vector<Move> capture_moves = {};
  Square origin_square = Utils::pop_bit(bb);
  bitboard king_moves = Utils::king_attacks[origin_square];

  bitboard quiet_moves_bb = king_moves & position->get_empty();
  bitboard capture_moves_bb = king_moves & position->get_enemy();

  add_quiet_moves(quiet_moves, quiet_moves_bb, Pieces::KING, origin_square);
  add_capture_moves(capture_moves, capture_moves_bb, Pieces::KING,
                    origin_square);

  move_list.insert(std::end(move_list), std::begin(quiet_moves),
                   std::end(quiet_moves));
  move_list.insert(std::end(move_list), std::begin(capture_moves),
                   std::end(capture_moves));
}

// TODO: SLIDING PIECES HOW DO I DO THIS LOL
void MoveGenerator::generate_rook_moves(std::vector<Move> &move_list,
                                        bitboard bb) {}
void MoveGenerator::generate_bishop_moves(std::vector<Move> &move_list,
                                          bitboard bb) {}
void MoveGenerator::generate_queen_moves(std::vector<Move> &move_list,
                                         bitboard bb) {}

void MoveGenerator::add_quiet_moves(std::vector<Move> &move_list, bitboard bb,
                                    Pieces piece, Square origin) {
  while (bb) {
    Square destination_square = Utils::pop_bit(bb);
    Move quiet_move = {};
    quiet_move.piece = piece;
    quiet_move.from = origin;
    quiet_move.to = destination_square;
    move_list.push_back(quiet_move);
  }
}

void MoveGenerator::add_capture_moves(std::vector<Move> &move_list, bitboard bb,
                                      Pieces piece, Square origin) {
  while (bb) {
    Square destination_square = Utils::pop_bit(bb);
    Move capture = {};
    capture.piece = piece;
    capture.from = origin;
    capture.to = destination_square;
    capture.is_capture = true;
    move_list.push_back(capture);
  }
}
