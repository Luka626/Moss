#include "utils.hpp"
#include "datatypes.hpp"

namespace Utils {
bitboard IN_BETWEEN[64][64];
std::vector<TT_Entry> TT;

}
void Utils::init() {
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

