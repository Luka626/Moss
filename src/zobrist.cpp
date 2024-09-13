#include "zobrist.hpp"
#include "datatypes.hpp"
#include <random>

namespace Zobrist {
zobrist_key PIECES[NPIECES][NCOLORS][NSQUARES];
zobrist_key CASTLING[4];
zobrist_key EN_PASSANT[8];
zobrist_key SIDE;
} // namespace Zobrist



void Zobrist::init() {
  std::random_device rd;
  std::seed_seq sd{rd(), rd(), rd(), rd()};
  std::mt19937_64 rand_engine(sd);

  for (int i = 0; i < NPIECES; i++) {
    for (int j = 0; j < NCOLORS; j++) {
      for (int k = 0; k < NSQUARES; k++) {
        PIECES[i][j][k] = rand_engine();
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    CASTLING[i] = rand_engine();
  }

  for (int i = 0; i < 8; i++) {
    EN_PASSANT[i] = rand_engine();
  }

  SIDE = rand_engine();
}
