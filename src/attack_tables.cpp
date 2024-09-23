#include "attack_tables.hpp"
#include "utils.hpp"

namespace AttackTables {

// Pre-computes attack tables for each non-sliding piece
void init() {
  for (Square square = a1; square < NSQUARES; ++square) {

    // Fold data about the current square into a struct
    Square_Data starting = {
        square,
        static_cast<int>(Utils::rank(square)),
        static_cast<int>(Utils::file(square)),
    };

    // Initialize ATTACKS bitboard-array of each non-slider at current the square
    W_PAWN.ATTACKS[starting.square] = compute_attack_bitboard(W_PAWN, starting);
    B_PAWN.ATTACKS[starting.square] = compute_attack_bitboard(B_PAWN, starting);
    KNIGHT.ATTACKS[starting.square] = compute_attack_bitboard(KNIGHT, starting);
    KING.ATTACKS[starting.square] = compute_attack_bitboard(KING, starting);
  }
}

// Generates an attack bitboard for a given non-sliding piece at a given square
template <size_t n_dir>
auto constexpr compute_attack_bitboard(Attack_Data<n_dir> attack_data,
                                       const Square_Data starting)  -> bitboard {
  bitboard attack_bb = 0ULL;

  // Iterate over the non-slider's possible jumps
  for (auto jump : attack_data.JUMPS) {
    Square_Data target = {};
    target.square = static_cast<Square>(starting.square + jump);

    // Don't add a move that does not belong to [a1, h8]
    if ((target.square < a1) | (target.square > h8)) {
      continue;
    }
    target.rank = Utils::rank(target.square);
    target.file = Utils::file(target.square);

    // Don't add a move that causes piece to wrap around row/col
    if ((abs(target.rank - starting.rank) > 2) |
        (abs(target.file - starting.file) > 2)) {
      continue;
    }

    // Toggle the target square's bit on the attack bitboard
    Utils::set_bit(attack_bb, target.square);
  }
  return attack_bb;
}

//////////////////////////////////////////////////
/* Attack_Data declarations for each non-slider */
//////////////////////////////////////////////////
Attack_Data<PAWN_DIR> W_PAWN = {
    Pieces::PAWN,
    {
        NW,
        NE,
    },
    {},
};
Attack_Data<PAWN_DIR> B_PAWN = {
    Pieces::PAWN,
    {
        SW,
        SE,
    },
    {},
};
Attack_Data<KNIGHT_DIR> KNIGHT = {
    Pieces::KNIGHT,
    {
        S + SW,
        S + SE,
        SW + W,
        SE + E,
        NW + W,
        NE + E,
        N + NW,
        N + NE,
    },
    {},
};

Attack_Data<KING_DIR> KING = {
    Pieces::KING,
    {
        SW,
        S,
        SE,
        W,
        E,
        NE,
        N,
        NW,
    },
    {},
};
} // namespace AttackTables
