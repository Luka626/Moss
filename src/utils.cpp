#include "utils.hpp"

void Utils::init() {
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
}
