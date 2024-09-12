#ifndef ZOBRIST_HPP_
#define ZOBRIST_HPP_

#include "datatypes.hpp"
namespace Zobrist {

extern zobrist_key PIECES[NPIECES][NCOLORS][NSQUARES];
extern zobrist_key CASTLING[4];
extern zobrist_key EN_PASSANT[8];
extern zobrist_key SIDE;

void init();

} // namespace Zobrist

#endif
