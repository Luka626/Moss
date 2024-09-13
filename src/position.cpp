#include "position.hpp"
#include "utils.hpp"
#include "zobrist.hpp"
#include <cstring>
#include <sstream>

// default constructor of a starting position
Position::Position() { set_board(Utils::STARTING_FEN_POSITION); };

Position::Position(const std::string &fen) { set_board(fen); }

zobrist_key Position::generate_key() {
  zobrist_key key = 0ULL;

  // todo : [ ] add castling rights and en passant and side to play hash
  for (int k = 0; k < NCOLORS; k++) {
    Colors side = (Colors)k;
    for (int i = 0; i < NPIECES; i++) {
      Pieces piece = (Pieces)i;
      bitboard piece_bb = get_bitboard(side, piece);
      while (piece_bb) {
        Square sq = Utils::pop_bit(piece_bb);
        key ^= Zobrist::PIECES[piece][side][sq];
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    if (castling_flags[i]) {
      key ^= Zobrist::CASTLING[i];
    }
  }

  if (en_passant_square != -1) {
    key ^= Utils::file(en_passant_square);
  }

  if (side_to_play == BLACK) {
    key ^= Zobrist::SIDE;
  }

  return key;
}

int Position::set_board(const std::string &fen) {
  ply = 1;
  for (int i = 0; i < 4; i++) {
    castling_flags[i] = false;
    castling_history[ply][i] = false;
  };
  Utils::init();
  Zobrist::init();

  for (auto &bb : pieces_bitboards) {
    bb = 0ULL;
  };
  for (auto &bb : color_bitboards) {
    bb = 0ULL;
  };

  std::istringstream iss(fen);
  std::string fen_token;

  std::getline(iss, fen_token, ' ');
  int rank = 7;
  int file = 0;

  for (char ch : fen_token) {
    if (ch == '/') {
      // a slash delimits a rank
      rank--;
      file = 0;
    } else if (std::isdigit(ch)) {
      file += ch - '0';
    } else {
      Square square = (Square)Utils::get_square(rank, file);

      switch (ch) {
      case 'P': {
        Utils::set_bit(pieces_bitboards[PAWN], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'p': {
        Utils::set_bit(pieces_bitboards[PAWN], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'N': {
        Utils::set_bit(pieces_bitboards[KNIGHT], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'n': {
        Utils::set_bit(pieces_bitboards[KNIGHT], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'B': {
        Utils::set_bit(pieces_bitboards[BISHOP], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'b': {
        Utils::set_bit(pieces_bitboards[BISHOP], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'R': {
        Utils::set_bit(pieces_bitboards[ROOK], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'r': {
        Utils::set_bit(pieces_bitboards[ROOK], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'Q': {
        Utils::set_bit(pieces_bitboards[QUEEN], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'q': {
        Utils::set_bit(pieces_bitboards[QUEEN], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'K': {
        Utils::set_bit(pieces_bitboards[KING], square);
        Utils::set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'k': {
        Utils::set_bit(pieces_bitboards[KING], square);
        Utils::set_bit(color_bitboards[BLACK], square);
        break;
      }
      }
      file++;
    }
  }
  std::getline(iss, fen_token, ' ');
  if (fen_token == "b") {
    side_to_play = BLACK;
  } else if (fen_token == "w") {
    side_to_play = WHITE;
  };

  std::getline(iss, fen_token, ' ');
  for (int i = 0; i < std::strlen(fen_token.c_str()); i++) {
    char ch = fen_token[i];
    switch (ch) {
    case ('K'):
      castling_flags[0] = true;
      castling_history[1][0] = true;
      break;
    case ('Q'):
      castling_flags[1] = true;
      castling_history[1][1] = true;
      break;
    case ('k'):
      castling_flags[2] = true;
      castling_history[1][1] = true;
      break;
    case ('q'):
      castling_flags[3] = true;
      castling_history[1][3] = true;
      break;
    }
  }

  std::getline(iss, fen_token, ' ');
  if (fen_token == "-") {
    en_passant_square = (Square)-1;
  } else {
    size_t file = fen_token.at(0) - 'a';
    size_t rank = fen_token.at(1) - '1';
    en_passant_square = Utils::get_square(rank, file);
  };

  std::getline(iss, fen_token, ' ');
  halfmove_clock = std::stoi(fen_token);

  std::getline(iss, fen_token, ' ');
  ply = std::stoi(fen_token);

  z_key = generate_key();

  return 0;
};

void Position::make_move(Move &move) {
  key_history[ply] = z_key;
  en_passant_history[ply] = en_passant_square;
  for (int i = 0; i < 4; i++) {
    castling_history[ply][i] = castling_flags[i];
  }

  ply++;

  bitboard from_bitboard = Utils::set_bit(move.from);
  bitboard to_bitboard = Utils::set_bit(move.to);
  bitboard from_to_bitboard = from_bitboard ^ to_bitboard;

  if (move.is_en_passant) {
    if (side_to_play == WHITE) {
      remove_piece(move.captured_piece, (Square)(move.to + S));
      z_key ^= Zobrist::PIECES[move.captured_piece][!side_to_play][move.to + S];
    } else {
      remove_piece(move.captured_piece, (Square)(move.to + N));
      z_key ^= Zobrist::PIECES[move.captured_piece][!side_to_play][move.to + N];
    }
  }

  // zobrist keys are updated here for castle rights
  update_castling_rights(move.from, move.to, move.is_capture);

  if (move.is_castle) {
    bitboard rook_from = 0ULL;
    bitboard rook_to = 0ULL;
    bitboard rook_from_to = 0ULL;
    Square rook_from_sq = a1;
    Square rook_to_sq = a1;
    if (move.to == g1) {
      rook_from_sq = h1;
      rook_to_sq = f1;
    } else if (move.to == c1) {
      rook_from_sq = a1;
      rook_to_sq = d1;
    } else if (move.to == g8) {
      rook_from_sq = h8;
      rook_to_sq = f8;
    } else if (move.to == c8) {
      rook_from_sq = a8;
      rook_to_sq = d8;
    }
    rook_from = Utils::set_bit(rook_from_sq);
    rook_to = Utils::set_bit(rook_to_sq);
    rook_from_to = rook_from ^ rook_to;
    color_bitboards[side_to_play] ^= rook_from_to;
    pieces_bitboards[Pieces::ROOK] ^= rook_from_to;
    z_key ^= Zobrist::PIECES[Pieces::ROOK][side_to_play][rook_from_sq];
    z_key ^= Zobrist::PIECES[Pieces::ROOK][side_to_play][rook_to_sq];
  }

  if (!move.is_en_passant && move.is_capture) {
    remove_piece(move.captured_piece, move.to);
    z_key ^= Zobrist::PIECES[move.captured_piece][!side_to_play][move.to];
  }
  if (move.is_double_push) {
    en_passant_square = (Square)((move.to + move.from) / 2);
    z_key ^= Zobrist::EN_PASSANT[Utils::file(en_passant_square)];
  } else {
    en_passant_square = (Square)-1;
    if (en_passant_history[ply - 1] != -1) {
      z_key ^= Zobrist::EN_PASSANT[en_passant_history[ply - 1]];
    }
  }

  if (!move.promotion) {
    pieces_bitboards[move.piece] ^= from_to_bitboard;
    z_key ^= Zobrist::PIECES[move.piece][side_to_play][move.from];
    z_key ^= Zobrist::PIECES[move.piece][side_to_play][move.to];
  } else {
    pieces_bitboards[move.piece] &= ~from_bitboard;
    pieces_bitboards[move.promotion] |= to_bitboard;
    z_key ^= Zobrist::PIECES[move.piece][side_to_play][move.from];
    z_key ^= Zobrist::PIECES[move.promotion][side_to_play][move.to];
  }

  color_bitboards[side_to_play] ^= from_to_bitboard;
  side_to_play = ~side_to_play;
}

void Position::undo_move(Move &move) {
  en_passant_square = en_passant_history[ply - 1];
  for (int i = 0; i < 4; i++) {
    castling_flags[i] = castling_history[ply - 1][i];
  }
  z_key = key_history[ply - 1];
  ply--;
  bitboard from_bitboard = Utils::set_bit(move.to);
  bitboard to_bitboard = Utils::set_bit(move.from);
  bitboard from_to_bitboard = from_bitboard ^ to_bitboard;

  if (move.is_castle) {
    bitboard rook_from = 0ULL;
    bitboard rook_to = 0ULL;
    bitboard rook_from_to = 0ULL;
    if (move.to == g1) {
      rook_to = Utils::set_bit(h1);
      rook_from = Utils::set_bit(f1);
    } else if (move.to == c1) {
      rook_to = Utils::set_bit(a1);
      rook_from = Utils::set_bit(d1);
    } else if (move.to == g8) {
      rook_to = Utils::set_bit(h8);
      rook_from = Utils::set_bit(f8);
    } else if (move.to == c8) {
      rook_to = Utils::set_bit(a8);
      rook_from = Utils::set_bit(d8);
    }
    rook_from_to = rook_from ^ rook_to;
    color_bitboards[~side_to_play] ^= rook_from_to;
    pieces_bitboards[Pieces::ROOK] ^= rook_from_to;
  }

  if (!move.promotion) {
    pieces_bitboards[move.piece] ^= from_to_bitboard;
  } else {
    pieces_bitboards[move.piece] ^= to_bitboard;
    pieces_bitboards[move.promotion] ^= from_bitboard;
  }
  color_bitboards[~side_to_play] ^= from_to_bitboard;

  Square captured_square = move.to;
  if (move.is_en_passant) {
    if (side_to_play == BLACK) {
      captured_square = (Square)(captured_square + S);
    } else {
      captured_square = (Square)(captured_square + N);
    }
  }
  if (move.is_capture) {
    Utils::set_bit(pieces_bitboards[move.captured_piece], captured_square);
    Utils::set_bit(color_bitboards[side_to_play], captured_square);
  }
  side_to_play = ~side_to_play;
}

// overrides << operator to "pretty" print chess position
std::ostream &operator<<(std::ostream &os, const Position &pos) {
  os << "\nMove: " << pos.ply << ", " << pos.side_to_play << " to play\n";
  os << pos.castling_flags[0] << pos.castling_flags[1] << pos.castling_flags[2]
     << pos.castling_flags[3] << "\n"
     << pos.en_passant_square << "\n"
     << pos.halfmove_clock << "\n"
     << std::endl;
  for (int rank = 7; rank >= 0; rank--) {
    os << " " << rank + 1 << "  ";
    for (int file = 0; file <= 7; file++) {
      int square = Utils::get_square(rank, file);

      size_t color_at_square = -1;
      size_t piece_at_square = -1;
      for (size_t color = WHITE; color < NCOLORS; color++) {
        for (size_t piece = PAWN; piece < NPIECES; piece++) {
          if (Utils::get_bit(pos.pieces_bitboards[piece] &
                                 pos.color_bitboards[color],
                             square)) {
            piece_at_square = piece;
            color_at_square = color;
            break;
          }
        }
      }

      char identifier = '-';
      switch (piece_at_square) {
      case PAWN:
        identifier = 'P';
        break;
      case KNIGHT:
        identifier = 'N';
        break;
      case BISHOP:
        identifier = 'B';
        break;
      case ROOK:
        identifier = 'R';
        break;
      case QUEEN:
        identifier = 'Q';
        break;
      case KING:
        identifier = 'K';
        break;
      default:
        identifier = '-';
      }
      if (color_at_square == BLACK) {
        identifier = std::tolower(identifier);
      };

      os << " " << identifier;
    }
    os << "\n";
  }
  os << "\n     a b c d e f g h\n\n";
  return os;
}
