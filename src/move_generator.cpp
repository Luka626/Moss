#include "move_generator.hpp"
#include <iomanip>

MoveGenerator::MoveGenerator(Position *position_ptr) {
  position = position_ptr;
  initiate_rank_attacks();
};

double MoveGenerator::divide(size_t depth) {
  std::cout << std::fixed;
  std::cout << std::setprecision(0);
  MoveList move_list = MoveList();
  double nodes;
  double all_nodes;

  move_list = generate_legal_moves();
  for (size_t i = 0; i < move_list.size(); i++) {
    position->make_move(move_list.at(i));
    nodes = perft(depth - 1);
    all_nodes += nodes;
    if (nodes > 0) {
      std::cout << move_list.at(i) << ": " << nodes << "\n";
    }
    position->undo_move(move_list.at(i));
  }
  std::cout << "Nodes searched: " << all_nodes << std::endl;
  std::cout << std::scientific;
  return all_nodes;
}

double MoveGenerator::perft(size_t depth) {
  MoveList move_list = MoveList();
  double nodes = 0;

  if (!validate_gamestate()) {
    return 0ULL;
  }

  if (depth == 0) {
    return 1ULL;
  }

  move_list = generate_legal_moves();

  for (size_t i = 0; i < move_list.size(); i++) {
    position->make_move(move_list.at(i));
    nodes += perft(depth - 1);
    position->undo_move(move_list.at(i));
  }
  return nodes;
}

bool MoveGenerator::validate_gamestate() {
  return !king_in_check(~position->side_to_play);
}
bool MoveGenerator::king_in_check(Colors side){
    bitboard king_bb = position->get_bitboard(side, Pieces::KING);
    Square king_square = Utils::pop_bit(king_bb);
    return generate_attackers(king_square) & position->color_bitboards[~side]; 
}

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

bitboard MoveGenerator::generate_attackers(Square sq) {
  bitboard wpawn_bb = position->get_bitboard(Colors::WHITE, Pieces::PAWN);
  bitboard bpawn_bb = position->get_bitboard(Colors::BLACK, Pieces::PAWN);
  bitboard knights_bb = position->pieces_bitboards[Pieces::KNIGHT];
  bitboard bishops_bb = position->pieces_bitboards[Pieces::BISHOP];
  bitboard rooks_bb = position->pieces_bitboards[Pieces::ROOK];
  bitboard queens_bb = position->pieces_bitboards[Pieces::QUEEN];
  bitboard kings_bb = position->pieces_bitboards[Pieces::KING];

  bitboard rect_attacks =
      generate_rectilinear_attacks(position->get_occupied(), sq);
  bitboard diag_attacks =
      generate_diagonal_attacks(position->get_occupied(), sq);

  return ((Utils::wpawn_attacks[sq] & bpawn_bb) |
          (Utils::bpawn_attacks[sq] & wpawn_bb) |
          (Utils::knight_attacks[sq] & knights_bb) |
          (Utils::king_attacks[sq] & kings_bb) | (diag_attacks & bishops_bb) |
          (rect_attacks & rooks_bb) |
          ((diag_attacks | rect_attacks) & queens_bb));
}

bitboard MoveGenerator::generate_pinned_pieces() {
  bitboard pinned = 0ULL;
  bitboard king = position->get_bitboard(Pieces::KING);
  bitboard opp_rooks =
      position->get_bitboard(~position->side_to_play, Pieces::ROOK);
  bitboard opp_queens =
      position->get_bitboard(~position->side_to_play, Pieces::QUEEN);
  bitboard opp_rect_attackers = opp_rooks | opp_queens;

  bitboard pinners = xray_rectilinear_attacks(
      position->get_occupied(),
      position->color_bitboards[position->side_to_play], Utils::pop_bit(king));
  pinners &= opp_rect_attackers;
  while (pinners) {
    Square sq = Utils::pop_bit(pinners);
    pinned |= Utils::IN_BETWEEN[sq][Utils::pop_bit(king)] &
              position->color_bitboards[position->side_to_play];
  }

  bitboard opp_bishops =
      position->get_bitboard(~position->side_to_play, Pieces::BISHOP);
  bitboard opp_diag_attackers = opp_queens | opp_bishops;
  pinners = xray_diagonal_attacks(
      position->get_occupied(),
      position->color_bitboards[position->side_to_play], Utils::pop_bit(king));
  pinners &= opp_diag_attackers;
  while (pinners) {
    Square sq = Utils::pop_bit(pinners);
    pinned |= Utils::IN_BETWEEN[sq][Utils::pop_bit(king)] &
              position->color_bitboards[position->side_to_play];
  }
  return pinned;
}

MoveList MoveGenerator::generate_pseudo_legal_moves() {
  MoveList pseudo_legal_moves = MoveList();

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

MoveList MoveGenerator::generate_legal_moves(){
    MoveList ps = generate_pseudo_legal_moves();
    MoveList l = MoveList();
    for (int i = 0; i<ps.size(); i++){
        position->make_move(ps.at(i));
        if (!king_in_check(~position->side_to_play)){
            l.push_back(ps.at(i));
        }
        position->undo_move(ps.at(i));
    }
    return l;
}

void MoveGenerator::generate_pawn_moves(MoveList &move_list, bitboard bb) {

  Pieces promotion_pieces[4] = {QUEEN, KNIGHT, ROOK, BISHOP};
  if (position->side_to_play == WHITE) {
    bitboard single_pawn_push = bb << N;
    single_pawn_push &= ~position->get_occupied();
    bitboard double_pawn_push = single_pawn_push;
    if (single_pawn_push) {
      double_pawn_push = (double_pawn_push & Utils::RANK_MASK[2]) << N;
      double_pawn_push &= ~position->get_occupied();
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (single_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(single_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S);

      if (Utils::rank(psudeo_legal_move.to) == 7) {
        for (const auto &promotion_piece : promotion_pieces) {
          psudeo_legal_move.promotion = promotion_piece;
          move_list.push_back(psudeo_legal_move);
        }
      } else {
        move_list.push_back(psudeo_legal_move);
      }
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (double_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(double_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S + S);
      psudeo_legal_move.is_double_push = true;

      move_list.push_back(psudeo_legal_move);
    }

    bitboard origin_bb = bb;
    while (origin_bb) {
      Square origin_square = Utils::pop_bit(origin_bb);
      bitboard pawn_attacks = Utils::wpawn_attacks[origin_square];
      bitboard capture_moves_bb = pawn_attacks & position->get_enemy();
      while (capture_moves_bb) {
        Square destination_square = Utils::pop_bit(capture_moves_bb);
        Move capture = {};
        capture.piece = PAWN;
        capture.from = origin_square;
        capture.to = destination_square;
        capture.is_capture = true;
        if (Utils::rank(capture.to) == 7) {
          for (const auto &promotion_piece : promotion_pieces) {
            capture.promotion = promotion_piece;
            move_list.push_back(capture);
          }
        } else {
          move_list.push_back(capture);
        }
      }

      add_capture_moves(move_list, capture_moves_bb, Pieces::PAWN,
                        origin_square);
      if (position->en_passant_square > 0) {
        bitboard en_passant_moves_bb =
            pawn_attacks & Utils::set_bit(position->en_passant_square);
        while (en_passant_moves_bb) {
          Square destination_square = Utils::pop_bit(en_passant_moves_bb);
          Move en_passant = {};
          en_passant.piece = PAWN;
          en_passant.from = origin_square;
          en_passant.to = destination_square;
          en_passant.is_capture = true;
          en_passant.is_en_passant = true;
          move_list.push_back(en_passant);
        }
      }
    }

  } else if (position->side_to_play == BLACK) {
    bitboard single_pawn_push = bb >> N;
    single_pawn_push &= ~position->get_occupied();

    bitboard double_pawn_push = single_pawn_push;
    if (single_pawn_push) {
      double_pawn_push = (double_pawn_push & Utils::RANK_MASK[5]) >> N;
      double_pawn_push &= ~position->get_occupied();
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (single_pawn_push != 0ULL) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(single_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N);
      if (Utils::rank(psudeo_legal_move.to) == 0) {
        for (const auto &promotion_piece : promotion_pieces) {
          psudeo_legal_move.promotion = promotion_piece;
          move_list.push_back(psudeo_legal_move);
        }
      } else {
        move_list.push_back(psudeo_legal_move);
      }
    }

    // pops each pawn and adds its move to the list of pseudo-legal moves
    while (double_pawn_push > 0) {
      Move psudeo_legal_move = {};
      psudeo_legal_move.piece = Pieces::PAWN;
      psudeo_legal_move.to = Utils::pop_bit(double_pawn_push);
      psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N + N);
      psudeo_legal_move.is_double_push = true;
      move_list.push_back(psudeo_legal_move);
    }

    bitboard origin_bb = bb;
    while (origin_bb) {
      Square origin_square = Utils::pop_bit(origin_bb);
      bitboard pawn_attacks = Utils::bpawn_attacks[origin_square];
      if (pawn_attacks) {
      };

      bitboard capture_moves_bb = pawn_attacks & position->get_enemy();
      while (capture_moves_bb) {
        Square destination_square = Utils::pop_bit(capture_moves_bb);
        Move capture = {};
        capture.piece = PAWN;
        capture.from = origin_square;
        capture.to = destination_square;
        capture.is_capture = true;

        if (Utils::rank(capture.to) == 0) {
          for (const auto &promotion_piece : promotion_pieces) {
            capture.promotion = promotion_piece;
            move_list.push_back(capture);
          }
        } else {
          move_list.push_back(capture);
        }
      }
      if (position->en_passant_square > 0) {
        bitboard en_passant_moves_bb =
            pawn_attacks & Utils::set_bit(position->en_passant_square);
        while (en_passant_moves_bb) {
          Square destination_square = Utils::pop_bit(en_passant_moves_bb);
          Move en_passant = {};
          en_passant.piece = PAWN;
          en_passant.from = origin_square;
          en_passant.to = destination_square;
          en_passant.is_capture = true;
          en_passant.is_en_passant = true;
          move_list.push_back(en_passant);
        }
      }
    }
  }
}
void MoveGenerator::generate_knight_moves(MoveList &move_list, bitboard bb) {

  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard knight_moves = Utils::knight_attacks[origin_square];

    bitboard quiet_moves_bb = knight_moves & position->get_empty();
    bitboard capture_moves_bb = knight_moves & position->get_enemy();

    add_quiet_moves(move_list, quiet_moves_bb, Pieces::KNIGHT, origin_square);
    add_capture_moves(move_list, capture_moves_bb, Pieces::KNIGHT,
                      origin_square);
  }
}

void MoveGenerator::generate_king_moves(MoveList &move_list, bitboard bb) {
  Square origin_square = Utils::pop_bit(bb);
  bitboard king_moves = Utils::king_attacks[origin_square];

  bitboard quiet_moves_bb = king_moves & position->get_empty();
  bitboard capture_moves_bb = king_moves & position->get_enemy();

  add_castling_moves(move_list);
  add_quiet_moves(move_list, quiet_moves_bb, Pieces::KING, origin_square);
  add_capture_moves(move_list, capture_moves_bb, Pieces::KING, origin_square);
}

void MoveGenerator::generate_rook_moves(MoveList &move_list, bitboard bb) {
  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard rook_moves =
        generate_rectilinear_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = rook_moves & position->get_empty();
    bitboard capture_moves_bb = rook_moves & position->get_enemy();

    add_quiet_moves(move_list, quiet_moves_bb, Pieces::ROOK, origin_square);
    add_capture_moves(move_list, capture_moves_bb, Pieces::ROOK, origin_square);
  }
}
void MoveGenerator::generate_bishop_moves(MoveList &move_list, bitboard bb) {
  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard bishop_moves = generate_diagonal_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = bishop_moves & position->get_empty();
    bitboard capture_moves_bb = bishop_moves & position->get_enemy();

    add_quiet_moves(move_list, quiet_moves_bb, Pieces::BISHOP, origin_square);
    add_capture_moves(move_list, capture_moves_bb, Pieces::BISHOP,
                      origin_square);
  }
}

void MoveGenerator::generate_queen_moves(MoveList &move_list, bitboard bb) {
  while (bb) {
    Square origin_square = Utils::pop_bit(bb);
    bitboard occupancy = position->get_occupied();
    bitboard queen_moves =
        generate_diagonal_attacks(occupancy, origin_square) |
        generate_rectilinear_attacks(occupancy, origin_square);

    bitboard quiet_moves_bb = queen_moves & position->get_empty();
    bitboard capture_moves_bb = queen_moves & position->get_enemy();

    add_quiet_moves(move_list, quiet_moves_bb, Pieces::QUEEN, origin_square);
    add_capture_moves(move_list, capture_moves_bb, Pieces::QUEEN,
                      origin_square);
  }
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

void MoveGenerator::add_castling_moves(MoveList &move_list) {
  if (position->side_to_play == WHITE) {
    if (position->castling_flags[0]) {
      Square rook_square = h1;
      // Square rook_target_square = f1;
      Square king_target_square = g1;

      bitboard between = Utils::IN_BETWEEN[e1][rook_square];
      bitboard between_ex = between ^ Utils::set_bit(rook_square);
      between_ex = between_ex ^ Utils::set_bit(e1);
      if ((between_ex & position->get_occupied()) != 0) {
        between = 0ULL;
      } else {
        between = between ^ Utils::set_bit(rook_square);
      }

      bool is_path_attacked = false;
      bitboard potential_attacked = Utils::IN_BETWEEN[king_target_square][e1];

      while (potential_attacked) {
        Square intermediate_square = Utils::pop_bit(potential_attacked);
        if ((generate_attackers(intermediate_square) &
             position->color_bitboards[~position->side_to_play]) > 0) {
          is_path_attacked = true;
          break;
        }
      }

      if (!is_path_attacked && ((between_ex & position->get_occupied()) == 0)) {
        Move w_kingside_castle = {};
        w_kingside_castle.piece = KING;
        w_kingside_castle.from = e1;
        w_kingside_castle.to = king_target_square;
        w_kingside_castle.is_castle = true;
        move_list.push_back(w_kingside_castle);
      }
    }
    if (position->castling_flags[1]) {
      Square rook_square = a1;
      Square rook_target_square = d1;
      Square king_target_square = c1;
      bitboard between = Utils::IN_BETWEEN[e1][rook_square];
      bitboard between_ex = between ^ Utils::set_bit(rook_square);
      between_ex = between_ex ^ Utils::set_bit(e1);
      if ((between_ex & position->get_occupied()) != 0) {
        between = 0ULL;
      } else {
        between = between ^ Utils::set_bit(rook_square);
      }

      bool is_path_attacked = false;
      bitboard potential_attacked = Utils::IN_BETWEEN[king_target_square][e1];

      while (potential_attacked) {
        Square intermediate_square = Utils::pop_bit(potential_attacked);
        if ((generate_attackers(intermediate_square) &
             position->color_bitboards[~position->side_to_play]) > 0) {
          is_path_attacked = true;
          break;
        }
      }

      if (!is_path_attacked && ((between_ex & position->get_occupied()) == 0)) {
        Move w_kingside_castle = {};
        w_kingside_castle.piece = KING;
        w_kingside_castle.from = e1;
        w_kingside_castle.to = king_target_square;
        w_kingside_castle.is_castle = true;
        move_list.push_back(w_kingside_castle);
      }
    }

  } else {
    if (position->castling_flags[2]) {
      Square rook_square = h8;
      Square rook_target_square = f8;
      Square king_target_square = g8;
      bitboard between = Utils::IN_BETWEEN[e8][rook_square];
      bitboard between_ex = between ^ Utils::set_bit(rook_square);
      between_ex = between_ex ^ Utils::set_bit(e8);
      if ((between_ex & position->get_occupied()) != 0) {
        between = 0ULL;
      } else {
        between = between ^ Utils::set_bit(rook_square);
      }

      bool is_path_attacked = false;
      bitboard potential_attacked = Utils::IN_BETWEEN[king_target_square][e8];

      while (potential_attacked) {
        Square intermediate_square = Utils::pop_bit(potential_attacked);
        if ((generate_attackers(intermediate_square) &
             position->color_bitboards[~position->side_to_play]) > 0) {
          is_path_attacked = true;
          break;
        }
      }

      if (!is_path_attacked && ((between_ex & position->get_occupied()) == 0)) {
        Move w_kingside_castle = {};
        w_kingside_castle.piece = KING;
        w_kingside_castle.from = e8;
        w_kingside_castle.to = king_target_square;
        w_kingside_castle.is_castle = true;
        move_list.push_back(w_kingside_castle);
      }
    }
    if (position->castling_flags[3]) {
      Square rook_square = a8;
      Square rook_target_square = d8;
      Square king_target_square = c8;
      bitboard between = Utils::IN_BETWEEN[e8][rook_square];
      bitboard between_ex = between ^ Utils::set_bit(rook_square);
      between_ex = between_ex ^ Utils::set_bit(e8);
      if ((between_ex & position->get_occupied()) != 0) {
        between = 0ULL;
      } else {
        between = between ^ Utils::set_bit(rook_square);
      }

      bool is_path_attacked = false;
      bitboard potential_attacked = Utils::IN_BETWEEN[king_target_square][e8];

      while (potential_attacked) {
        Square intermediate_square = Utils::pop_bit(potential_attacked);
        if ((generate_attackers(intermediate_square) &
             position->color_bitboards[~position->side_to_play]) > 0) {
          is_path_attacked = true;
          break;
        }
      }

      if (!is_path_attacked && ((between_ex & position->get_occupied()) == 0)) {
        Move w_kingside_castle = {};
        w_kingside_castle.piece = KING;
        w_kingside_castle.from = e8;
        w_kingside_castle.to = king_target_square;
        w_kingside_castle.is_castle = true;
        move_list.push_back(w_kingside_castle);
      }
    }
  }
}
