#ifndef MOVE_GENERATOR_HPP_
#define MOVE_GENERATOR_HPP_

#include "datatypes.hpp"
#include "position.hpp"
#include "utils.hpp"
#include <memory>
#include <vector>

class MoveGenerator : public std::enable_shared_from_this<MoveGenerator> {
public:
  MoveGenerator(std::shared_ptr<Position> position_ptr);
  std::vector<Move> generate_pseudo_legal_moves();

  void generate_pawn_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_knight_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_bishop_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_rook_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_queen_moves(std::vector<Move> &moves_list, bitboard bb);
  void generate_king_moves(std::vector<Move> &moves_list, bitboard bb);

private:
  std::shared_ptr<Position> position;
};
#endif
