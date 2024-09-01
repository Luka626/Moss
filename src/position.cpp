#include "position.hpp"
#include "datatypes.hpp"
#include <sstream>
#include <string>

// default constructor of a starting position
Position::Position() {

  side_to_play = WHITE;
  ply = 0;
  for (int i = 0; i < NPIECES; i++) {
    pieces_bitboards[i] = 0ULL;
  }

  for (int i = 0; i < NCOLORS; i++) {
    color_bitboards[i] = 0ULL;
  }

  second_rank = 0ULL;
  seventh_rank = 0ULL;

  // initialize relative movement of all possible knight jumps
  const int knight_jumps[8] = {S + SW, S + SE, SW + W, SE + E,
                               NW + W, NE + E, N + NW, N + NE};
  // generate a bitboard array containing all valid knight moves from all
  // squares
  for (size_t i = a1; i <= NSQUARES; i++) {
    knight_attacks[i] = 0ULL;
    int starting_rank = rank((Square)i);
    int starting_file = file((Square)i);

    for (int j = 0; j < 8; j++) {
      int target_square = i + knight_jumps[j];
      if ((target_square < a1) || (target_square > h8)) {
        continue;
      };
      int end_rank = rank((Square)target_square);
      int end_file = file((Square)target_square);
      if ((abs(end_rank - starting_rank) > 2) ||
          abs(end_file - starting_file) > 2) {
        continue;
      };
      set_bit(knight_attacks[i], target_square);
    };
  };
};

int Position::set_board(const std::string &fen) {
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
      Square square = (Square)get_square(rank, file);

      switch (ch) {
      case 'P': {
        set_bit(pieces_bitboards[PAWN], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'p': {
        set_bit(pieces_bitboards[PAWN], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'N': {
        set_bit(pieces_bitboards[KNIGHT], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'n': {
        set_bit(pieces_bitboards[KNIGHT], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'B': {
        set_bit(pieces_bitboards[BISHOP], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'b': {
        set_bit(pieces_bitboards[BISHOP], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'R': {
        set_bit(pieces_bitboards[ROOK], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'r': {
        set_bit(pieces_bitboards[ROOK], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'Q': {
        set_bit(pieces_bitboards[QUEEN], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'q': {
        set_bit(pieces_bitboards[QUEEN], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      case 'K': {
        set_bit(pieces_bitboards[KING], square);
        set_bit(color_bitboards[WHITE], square);
        break;
      }
      case 'k': {
        set_bit(pieces_bitboards[KING], square);
        set_bit(color_bitboards[BLACK], square);
        break;
      }
      }
      file++;
    }
  }
  std::getline(iss, fen_token, ' ');
  std::cout << fen_token << std::endl;
  if (fen_token == "b") {
    side_to_play = BLACK;
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

std::vector<Move> Position::generate_pseudo_legal_moves(Colors side_to_play) {
  std::vector<Move> output = {};

  // iterates through each piece bb of the person who's turn it is to play
  for (int piece = PAWN + side_to_play; piece < NPIECES; piece += 2) {
    bitboard bb = pieces_bitboards[piece];

    switch (piece) {
      // TODO:
      // [x] double pawn pushes
      // [] pawn captures
      // [] en-passant
    case PAWN: {
      // pushes all the pawns by a single move, prunes collisions
      bitboard single_pawn_push = bb << N;
      single_pawn_push &= ~get_occupied_bitboard();

      bitboard double_pawn_push = 0ULL;
      if ((bb & second_rank) > 0) {
        double_pawn_push = bb << N << N;
        double_pawn_push &= ~get_occupied_bitboard();
      }

      // pops each pawn and adds its move to the list of pseudo-legal moves
      while (single_pawn_push > 0) {
        Move psudeo_legal_move = {};
        psudeo_legal_move.piece = (Pieces)piece;
        psudeo_legal_move.to = pop_bit(single_pawn_push);
        psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S);
        output.push_back(psudeo_legal_move);
      }

      // pops each pawn and adds its move to the list of pseudo-legal moves
      while (double_pawn_push > 0) {
        Move psudeo_legal_move = {};
        psudeo_legal_move.piece = (Pieces)piece;
        psudeo_legal_move.to = pop_bit(double_pawn_push);
        psudeo_legal_move.from = (Square)(psudeo_legal_move.to + S + S);
        output.push_back(psudeo_legal_move);
      }
      break;
    }
      // TODO:
      // [x] double pawn pushes
      // [] pawn captures
      // [] en-passant
    case KING: {
      // pushes all the pawns by a single move, prunes collisions
      bitboard single_pawn_push = bb >> N;
      print_bitboard(single_pawn_push);
      single_pawn_push &= ~get_occupied_bitboard();

      bitboard double_pawn_push = 0ULL;
      if ((bb & seventh_rank) > 0) {
        double_pawn_push = bb >> N >> N;
        double_pawn_push &= ~get_occupied_bitboard();
      }

      // pops each pawn and adds its move to the list of pseudo-legal moves
      while (single_pawn_push != 0ULL) {
        Move psudeo_legal_move = {};
        psudeo_legal_move.piece = (Pieces)piece;
        psudeo_legal_move.to = pop_bit(single_pawn_push);
        psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N);
        output.push_back(psudeo_legal_move);
      }

      // pops each pawn and adds its move to the list of pseudo-legal moves
      while (double_pawn_push > 0) {
        Move psudeo_legal_move = {};
        psudeo_legal_move.piece = (Pieces)piece;
        psudeo_legal_move.to = pop_bit(double_pawn_push);
        psudeo_legal_move.from = (Square)(psudeo_legal_move.to + N + N);
        output.push_back(psudeo_legal_move);
      }
      break;
    }
    case KNIGHT: {

      break;
    };
    }
  }

  return output;
}

// overrides << operator to "pretty" print chess position
std::ostream &operator<<(std::ostream &os, const Position &pos) {
  os << "\nMove: " << pos.ply << "\n\n";
  for (int rank = 7; rank >= 0; rank--) {
    os << " " << rank + 1 << "  ";
    for (int file = 0; file <= 7; file++) {
      int square = get_square(rank, file);

      size_t color_at_square = -1;
      size_t piece_at_square = -1;
      for (size_t color = WHITE; color < NCOLORS; color++) {
        for (size_t piece = PAWN; piece < NPIECES; piece++) {
          if (get_bit(pos.pieces_bitboards[piece] & pos.color_bitboards[color],
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
