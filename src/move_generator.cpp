#include "move_generator.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

MoveGenerator::MoveGenerator(std::shared_ptr<Position> position_ptr) {
  position = position_ptr;
  initiate_rank_attacks();
};

int MoveGenerator::generate_rank_attack(int occupancy, size_t file) {
  int b;
  int x;
  int y = 0;
  for (int x = file - 1; x >= 0; x--) {
    b = 1 << x;
    y |= b;
    if ((occupancy & b) == b)
      break;
  }
  for (x = file + 1; x < 8; x++) {
    b = 1 << x;
    y |= b;
    if ((occupancy & b) == b)
      break;
  }
  return y;
}

void MoveGenerator::initiate_rank_attacks() {
  for (size_t x = 0; x < NSQUARES; x++) {
    for (size_t file = 0; file < 8; file++) {
      RANK_ATTACKS[x * 8ULL + file] = generate_rank_attack(x * 2, file);
    }
  }
}

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
  generate_bishop_moves(pseudo_legal_moves, bishop_bb);
  generate_rook_moves(pseudo_legal_moves, rook_bb);
  generate_queen_moves(pseudo_legal_moves, queen_bb);

  return pseudo_legal_moves;
}

void MoveGenerator::generate_pawn_moves(std::vector<Move> &move_list,
                                        bitboard bb) {

  if (position->side_to_play == WHITE) {
    bitboard single_pawn_push = bb << N;
    single_pawn_push &= ~position->get_occupied();
    bitboard double_pawn_push = 0ULL;
    if (((bb & Utils::RANK_MASK[1]) > 0) & single_pawn_push) {
      double_pawn_push = (bb & Utils::RANK_MASK[1]) << N << N;
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

    std::vector<Move> capture_moves = {};

    while (bb) {
      Square origin_square = Utils::pop_bit(bb);
      bitboard pawn_attacks = Utils::wpawn_attacks[origin_square];

      bitboard capture_moves_bb = pawn_attacks & position->get_enemy();

      add_capture_moves(capture_moves, capture_moves_bb, Pieces::PAWN,
                        origin_square);
    }

    move_list.insert(std::end(move_list), std::begin(capture_moves),
                     std::end(capture_moves));

  } else if (position->side_to_play == BLACK) {
    bitboard single_pawn_push = bb >> N;
    single_pawn_push &= ~position->get_occupied();

    bitboard double_pawn_push = 0ULL;
    if (((bb & Utils::RANK_MASK[6]) > 0) & single_pawn_push) {
      double_pawn_push = (bb & Utils::RANK_MASK[6]) >> N >> N;
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
    std::vector<Move> capture_moves = {};

    while (bb) {
      Square origin_square = Utils::pop_bit(bb);
      bitboard pawn_attacks = Utils::bpawn_attacks[origin_square];

      bitboard capture_moves_bb = pawn_attacks & position->get_enemy();

      add_capture_moves(capture_moves, capture_moves_bb, Pieces::PAWN,
                        origin_square);
    }

    move_list.insert(std::end(move_list), std::begin(capture_moves),
                     std::end(capture_moves));
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
                                        bitboard bb) {
  std::vector<Move> quiet_moves = {};
  std::vector<Move> capture_moves = {};

  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard rook_moves =
        generate_rectilinear_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = rook_moves & position->get_empty();
    bitboard capture_moves_bb = rook_moves & position->get_enemy();

    add_quiet_moves(quiet_moves, quiet_moves_bb, Pieces::ROOK, origin_square);
    add_capture_moves(capture_moves, capture_moves_bb, Pieces::ROOK,
                      origin_square);
  }

  move_list.insert(std::end(move_list), std::begin(quiet_moves),
                   std::end(quiet_moves));
  move_list.insert(std::end(move_list), std::begin(capture_moves),
                   std::end(capture_moves));
}
void MoveGenerator::generate_bishop_moves(std::vector<Move> &move_list,
                                          bitboard bb) {
  std::vector<Move> quiet_moves = {};
  std::vector<Move> capture_moves = {};

  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard bishop_moves = generate_diagonal_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = bishop_moves & position->get_empty();
    bitboard capture_moves_bb = bishop_moves & position->get_enemy();

    add_quiet_moves(quiet_moves, quiet_moves_bb, Pieces::BISHOP, origin_square);
    add_capture_moves(capture_moves, capture_moves_bb, Pieces::BISHOP,
                      origin_square);
  }

  move_list.insert(std::end(move_list), std::begin(quiet_moves),
                   std::end(quiet_moves));
  move_list.insert(std::end(move_list), std::begin(capture_moves),
                   std::end(capture_moves));
}

void MoveGenerator::generate_queen_moves(std::vector<Move> &move_list,
                                         bitboard bb) {
  std::vector<Move> quiet_moves = {};
  std::vector<Move> capture_moves = {};

  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard queen_moves =
        generate_diagonal_attacks(occupancy, origin_square) |
        generate_rectilinear_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = queen_moves & position->get_empty();
    bitboard capture_moves_bb = queen_moves & position->get_enemy();

    add_quiet_moves(quiet_moves, quiet_moves_bb, Pieces::QUEEN, origin_square);
    add_capture_moves(capture_moves, capture_moves_bb, Pieces::QUEEN,
                      origin_square);
  }

  move_list.insert(std::end(move_list), std::begin(quiet_moves),
                   std::end(quiet_moves));
  move_list.insert(std::end(move_list), std::begin(capture_moves),
                   std::end(capture_moves));
}

bitboard MoveGenerator::generate_rectilinear_attacks(bitboard occupancy,
                                                     Square sq) {
  // uses a routine from the CPW to generate rank attacks
  size_t file = Utils::file(sq);
  size_t rank = Utils::rank(sq);
  size_t index = (occupancy >> rank * 8) & 2 * 63;
  bitboard attacks = RANK_ATTACKS[4 * index + file];
  attacks = attacks << (rank * 8);

  // OR rank attakcs with file attakcs from HQ method
  return attacks | hyperbola_quintessence(occupancy, sq, Utils::file_mask(sq));
}
bitboard MoveGenerator::generate_diagonal_attacks(bitboard occupancy,
                                                  Square sq) {
  return hyperbola_quintessence(occupancy, sq, Utils::diagonal_mask(sq)) |
         hyperbola_quintessence(occupancy, sq, Utils::anti_diagonal_mask(sq));
}

// https://www.chessprogramming.org/Subtracting_a_Rook_from_a_Blocking_Piece
bitboard MoveGenerator::hyperbola_quintessence(bitboard occupancy, Square sq,
                                               bitboard mask) {
  // exclude the slider from the mask
  mask = mask & (~Utils::set_bit(sq));
  bitboard x = Utils::set_bit(sq);
  bitboard o = occupancy & mask;
  return ((o - x) ^
          Utils::reverse(Utils::reverse(o) - (0x8000000000000000ull >> sq))) &
         mask;
}

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
