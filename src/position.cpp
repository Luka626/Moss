#include "position.hpp"
#include "datatypes.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

// default constructor of a starting position
Position::Position() { set_board(Utils::STARTING_FEN_POSITION); };

Position::Position(const std::string &fen) { set_board(fen); }

int Position::set_board(const std::string &fen) {
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
  castling_flags = fen_token;

  std::getline(iss, fen_token, ' ');
  if (fen_token != "-") {
    en_passant_square = fen_token;
  };

  std::getline(iss, fen_token, ' ');
  halfmove_clock = std::stoi(fen_token);

  std::getline(iss, fen_token, ' ');
  ply = std::stoi(fen_token);

  return 0;
};

void Position::make_move(Move &move) {
  ply++;
  bitboard from_bitboard = 1ULL << move.from;
  bitboard to_bitboard = 1ULL << move.to;
  bitboard from_to_bitboard = from_bitboard ^ to_bitboard;

  pieces_bitboards[move.piece] ^= from_to_bitboard;
}

// overrides << operator to "pretty" print chess position
std::ostream &operator<<(std::ostream &os, const Position &pos) {
  os << "\nMove: " << pos.ply << "\n\n";
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
