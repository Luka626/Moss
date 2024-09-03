#include "utils.hpp"
#include "datatypes.hpp"

namespace Utils{    
int knight_jumps[8];
bitboard knight_attacks[64];

int king_jumps[8];
bitboard king_attacks[64];
}

void Utils::init() {
    generate_knight_jumps();
    generate_knight_attacks();

    generate_king_jumps();
    generate_king_attacks();

}

void Utils::generate_knight_jumps(){
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

void Utils::generate_king_jumps(){
  king_jumps[0] = SW;
  king_jumps[1] = S;
  king_jumps[2] = SE;
  king_jumps[3] = W;
  king_jumps[4] = E;
  king_jumps[5] = NE;
  king_jumps[6] = N;
  king_jumps[7] = NW;

}

void Utils::generate_knight_attacks(){
  // generate a bitboard array containing all valid knight moves from all
  // squares
  for (size_t i = a1; i <= NSQUARES; i++) {
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

void Utils::generate_king_attacks(){
  for (size_t i = a1; i <= NSQUARES; i++) {
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
