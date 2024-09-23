#include "utils.hpp"
#include "datatypes.hpp"

namespace Utils {
int knight_jumps[8];
bitboard knight_attacks[64];

int king_jumps[8];
bitboard king_attacks[64];

int wpawn_jumps[2];
int bpawn_jumps[2];
bitboard wpawn_attacks[64];
bitboard bpawn_attacks[64];

bitboard IN_BETWEEN[64][64];

std::vector<TT_Entry> TT;

}
void Utils::init() {
  generate_bpawn_jumps();
  generate_wpawn_jumps();
  generate_bpawn_attacks();
  generate_wpawn_attacks();

  generate_knight_jumps();
  generate_knight_attacks();

  generate_king_jumps();
  generate_king_attacks();

  generate_in_between();


  clear_TT();
}

void Utils::generate_in_between() {
  for (size_t i = 0; i < NSQUARES; i++) {
    for (size_t j = 0; j < NSQUARES; j++) {
      bitboard between = 0ULL;

      size_t rank_i = Utils::rank((Square)i);
      size_t rank_j = Utils::rank((Square)j);
      size_t min_rank = std::min(rank_i, rank_j);
      size_t max_rank = std::max(rank_i, rank_j);

      size_t file_i = Utils::file((Square)i);
      size_t file_j = Utils::file((Square)j);
      size_t min_file = std::min(file_i, file_j);
      size_t max_file = std::max(file_i, file_j);

      if (file_i == file_j) {
        for (size_t rank = min_rank; rank <= max_rank; rank++) {
          Square sq = Utils::get_square(rank, file_i);
          between |= Utils::set_bit(sq);
        }
      }
      if (rank_i == rank_j) {
        for (size_t file = min_file; file <= max_file; file++) {
          Square sq = (Square)Utils::get_square(rank_i, file);
          between |= Utils::set_bit(sq);
        }
      }
      if ((max_rank - min_rank) == (max_file - min_file)) {
        bool is_positive_diagonal = (file_j - file_i == rank_j - rank_i);

        size_t file = is_positive_diagonal ? min_file : max_file;
        for (size_t rank = min_rank; rank <= max_rank; rank++) {
          Square sq = Utils::get_square(file, file);
          between |= Utils::set_bit(sq);
          is_positive_diagonal ? file++ : file--;
        }
      }
      IN_BETWEEN[i][j] = between;
    }
  }
}

void Utils::generate_knight_jumps() {
  // initialize relative movement of all possible knight jumps
  knight_jumps[0] = S + SW;
  knight_jumps[1] = S + SE;
  knight_jumps[2] = SW + W;
  knight_jumps[3] = SE + E;
  knight_jumps[4] = NW + W;
  knight_jumps[5] = NE + E;
  knight_jumps[6] = N + NW;
  knight_jumps[7] = N + NE;
}

void Utils::generate_king_jumps() {
  king_jumps[0] = SW;
  king_jumps[1] = S;
  king_jumps[2] = SE;
  king_jumps[3] = W;
  king_jumps[4] = E;
  king_jumps[5] = NE;
  king_jumps[6] = N;
  king_jumps[7] = NW;
}

void Utils::generate_wpawn_jumps() {
  wpawn_jumps[0] = NW;
  wpawn_jumps[1] = NE;
}
void Utils::generate_bpawn_jumps() {
  bpawn_jumps[0] = SW;
  bpawn_jumps[1] = SE;
}
void Utils::generate_knight_attacks() {
  // generate a bitboard array containing all valid knight moves from all
  // squares
  for (size_t i = a1; i < NSQUARES; i++) {
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
}

void Utils::generate_king_attacks() {
  for (size_t i = a1; i < NSQUARES; i++) {
    int starting_rank = rank((Square)i);
    int starting_file = file((Square)i);

    for (int j = 0; j < 8; j++) {
      int target_square = i + king_jumps[j];
      if ((target_square < a1) || (target_square > h8)) {
        continue;
      };
      int end_rank = rank((Square)target_square);
      int end_file = file((Square)target_square);
      if ((abs(end_rank - starting_rank) > 2) ||
          abs(end_file - starting_file) > 2) {
        continue;
      };
      set_bit(king_attacks[i], target_square);
    };
  };
}

void Utils::generate_wpawn_attacks() {
  for (size_t i = a1; i < NSQUARES; i++) {
    int starting_rank = rank((Square)i);
    int starting_file = file((Square)i);

    for (int j = 0; j < 2; j++) {
      int target_square = i + wpawn_jumps[j];
      if ((target_square < a1) || (target_square > h8)) {
        continue;
      };
      int end_rank = rank((Square)target_square);
      int end_file = file((Square)target_square);
      if ((abs(end_rank - starting_rank) > 2) ||
          abs(end_file - starting_file) > 2) {
        continue;
      };
      set_bit(wpawn_attacks[i], target_square);
    };
  };
}

void Utils::generate_bpawn_attacks() {
  for (size_t i = a1; i < NSQUARES; i++) {
    int starting_rank = rank((Square)i);
    int starting_file = file((Square)i);

    for (int j = 0; j < 2; j++) {
      Square target_square = (Square)(i + bpawn_jumps[j]);
      if ((target_square < a1) || (target_square > h8)) {
        continue;
      };
      int end_rank = rank((Square)target_square);
      int end_file = file((Square)target_square);
      if ((abs(end_rank - starting_rank) > 2) ||
          abs(end_file - starting_file) > 2) {
        continue;
      };
      set_bit(bpawn_attacks[i], target_square);
    };
  };
}

void Utils::clear_TT() {
  TT.clear();
  TT.resize(HASHSIZE, TT_Entry());
}
