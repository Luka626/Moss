#include "search.hpp"
#include "datatypes.hpp"
#include "eval.hpp"
#include "move_generator.hpp"
#include "utils.hpp"
#include <chrono>
#include <limits.h>
#include <memory>

Search::Search(std::shared_ptr<Position> position_ptr)
    : move_gen(std::make_unique<MoveGenerator>(position_ptr)),
      eval(std::make_unique<Evaluator>(position_ptr)), pos(position_ptr) {
  search_age = 0;
}

// Re-sets parameters on a search-by-search basis, TT data is preserved
void Search::new_search() {
  search_start = std::chrono::high_resolution_clock::now();
  iteration_start = std::chrono::high_resolution_clock::now();
  search_done = false;

  nodes_searched = 0;
  depth_searched = 0;
  time_searched = 0;
  time_limit = 0;

  hashfull = 0;

  best_move = Move();
  killer_moves.clear();
  killer_moves.resize(Utils::MAX_PLY, {Move(), Move()});
}

// Re-sets state of object for new games
void Search::new_game() {
  search_age = 0;
  move_gen->new_game();
  new_search();
}

// Main search loop, iteratively searches at increasing depths until timeout
int Search::iterative_deepening(const int time, const int moves_remaining) {
  depth_searched = 1;

  // Aging to prevent old TT entries from lasting forever
  ++search_age;

  int root_eval = -INT_MAX;

  // Probe TT for a PV move from a previous search
  TT_Entry old_entry = probe_TT(pos->z_key, depth_searched);
  best_move = old_entry.best_move;

  // Compute a time limit and begin search
  time_limit = time / (moves_remaining + 1);
  search_start = std::chrono::high_resolution_clock::now();

  // Iterate until timeout or max_depth exceeded
  while (!search_done & (depth_searched < Utils::MAX_DEPTH)) {

    // Time each iteration to report to UCI
    iteration_start = std::chrono::high_resolution_clock::now();

    // Initialize helper variables, generate and sort moves
    int alpha = -INT_MAX;
    int beta = INT_MAX;
    MoveList moves = move_gen->generate_pseudo_legal_moves();
    moves.score_moves(best_move, Move(), Move());
    moves.sort_moves();

    // Main search loop, described better in Negamax()
    for (size_t i = 0; i < moves.size(); i++) {
      Move mv = moves.at(i);
      if (depth_searched >= 10) {
        std::cout << "info currmove " << mv << "\n";
      }

      pos->make_move(mv);
      if (!move_gen->validate_gamestate()) {
        pos->undo_move(mv);
        continue;
      }
      nodes_searched++;

      // Late Move Reductions
      // (https://www.chessprogramming.org/Late_Move_Reductions)
      int LMR = 1;
      // Conditions to reduce (needs tweaks)
      if ((i > 3) & (depth_searched > 2) & (!mv.is_capture) &
          (mv.promotion > 0) & (!move_gen->king_in_check(pos->side_to_play))) {
        // Reduced-depth search
        root_eval = -negamax(-beta, -alpha, depth_searched - 1 - LMR, true);
        // If reduced depth search raises alpha, need to re-search
        if (root_eval > alpha) {
          root_eval = -negamax(-beta, -alpha, depth_searched - 1, true);
        }
        // PVS search based on our current best move
      } else if (i > 0) {
        root_eval = -negamax(-alpha - 1, -alpha, depth_searched - 1, true);
        if ((root_eval > alpha) & (root_eval < beta)) {
          root_eval = -negamax(-beta, -alpha, depth_searched - 1, true);
        }
      } else {
        root_eval = -negamax(-beta, -alpha, depth_searched - 1, true);
      }

      pos->undo_move(mv);
      if (search_done) {
        break;
      }

      if (root_eval > alpha) {
        alpha = root_eval;
        best_move = mv;
      }
    }

    update_TT(pos->z_key, depth_searched, root_eval, NodeType::EXACT,
              best_move);

    TT_Entry root_entry = probe_TT(pos->z_key, depth_searched);
    best_move = root_entry.best_move;
    root_eval = root_entry.evaluation;
    info_to_uci(root_eval);
    ++depth_searched;
  }

  return root_eval;
}

int Search::negamax(int alpha, int beta, const int depth, bool null_allowed) {
  // If we have reached a leaf node, drop into QSearch
  if ((depth <= 0)) {
    return quiescence(alpha, beta);
  }

  // Check for draws, as per 50-move and 3-fold repitition rules
  if (pos->is_drawn()) {
    return Scores::DRAW;
  }

  // Probe Transposition Table for a TT-cutoff and/or a hash move
  bool was_found = false;
  TT_Entry entry = probe_TT(pos->z_key, depth, was_found);
  if (was_found) {
    if (entry.type == NodeType::EXACT) {
      return entry.evaluation;
    } else if ((entry.type == NodeType::UPPER) && (entry.evaluation < alpha)) {
      return alpha;
    } else if ((entry.type == NodeType::LOWER) && (entry.evaluation >= beta)) {
      return beta;
    }
  }
  // entry.best_move will still contain the hash move from now on

  // Null-Move Reduction -
  // (https://www.chessprogramming.org/Null_Move_Reductions) Pass the turn and
  // see if a reduced-depth search still produces a beta cutoff
  if (!move_gen->king_in_check(pos->side_to_play) & null_allowed &
          (depth >= NULL_MOVE_REDUCTION + 1) &&
      (eval->evaluate() >= beta - 50)) {

    pos->make_null_move();
    int nm_eval =
        -negamax(-beta, -beta + 1, depth - NULL_MOVE_REDUCTION - 1, false);
    pos->undo_null_move();

    if (search_done) {
      return Scores::DRAW;
    }

    if (nm_eval >= beta) {
      return beta;
    }
  }

  // Initialize some helper variables for our main search
  zobrist_key move_key = pos->z_key;
  int alpha_old = alpha;
  int current_move = 0;
  int best_eval = -INT_MAX;
  Move my_best_move = Move();
  int eval = -INT_MAX;

  // Generate, score, and order moves.
  MoveList moves = move_gen->generate_pseudo_legal_moves();
  moves.score_moves(entry.best_move, killer_moves.at(pos->ply).killer1,
                    killer_moves.at(pos->ply).killer2);
  moves.sort_moves();

  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);
    pos->make_move(mv);

    // When making a move, we lose some time by having to check for legality.
    // We save time overall since more moves are pruned than played,
    // and psuedo_legal move gen is faster than strictly legal move gen.
    if (!move_gen->validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    current_move++;
    nodes_searched++;

    // Late Move Reductions
    // (https://www.chessprogramming.org/Late_Move_Reductions)
    int LMR = 1;
    // Conditions for LMR (needs tweaking)
    if ((i > 3) & (depth_searched > 2) & (!mv.is_capture) & (!mv.promotion) &
        (!move_gen->king_in_check(pos->side_to_play))) {
      // Reduced-depth search
      eval = -negamax(-beta, -alpha, depth - 1 - LMR, true);
      // Need to re-search if our reduced-depth search still raised alpha
      if (eval > alpha) {
        eval = -negamax(-beta, -alpha, depth - 1, true);
      }

      // Principal Variation Search
      // (https://www.chessprogramming.org/Principal_Variation_Search)
      // Only search the first move (PV node from TT) at full depth
      // Search subsequent nodes with a null window to test if they could
      // represent an improvement.
    } else if (i > 0) {
      eval = -negamax(-alpha - 1, -alpha, depth - 1, true);
      // Identified a move that may be better than our PV, need to re-search
      if ((alpha < eval) & (eval < beta)) {
        eval = -negamax(-beta, -alpha, depth - 1, true);
      }
    } else {
      eval = -negamax(-beta, -alpha, depth - 1, true);
    }

    pos->undo_move(mv);
    if (search_done) {
      return Scores::DRAW;
    }

    // If a move is too good to be true, we return beta
    if (eval >= beta) {
      // Store the move as a killer and in our TT
      store_killer(mv);
      update_TT(move_key, depth, eval, NodeType::LOWER, mv);
      return beta;
    }

    // If we found a new best evaluation, update our best_move and see if we
    // raise alpha
    if (eval > best_eval) {
      best_eval = eval;
      my_best_move = mv;
      if (eval > alpha) {
        alpha = eval;
      }
    }
  }

  // If we played none of our moves, we are either in stalemate or checkmate
  if (current_move <= 0) {
    if (move_gen->king_in_check(pos->side_to_play)) {
      return Scores::CHECKMATE + pos->ply;
    } else {
      return Scores::DRAW;
    }
  }

  // Update TT with this position as a PV node if we never raised alpha
  if (best_eval < alpha_old) {
    update_TT(move_key, depth, best_eval, NodeType::EXACT, my_best_move);
  } else {
    update_TT(move_key, depth, alpha, NodeType::UPPER, my_best_move);
  }

  // Return alpha as our evaluation of the position
  return alpha;
}

// Quiesence Search - (https://www.chessprogramming.org/Quiescence_Search)
// Continue to search all forcing moves once depth = 0.
// Prevents mis-evaluating position due to the horizon effect.
int Search::quiescence(int alpha, int beta) {

  // If we have timed out, set the search_done flag and begin unwinding
  if ((search_done = is_search_done())) {
    return Scores::DRAW;
  }

  // Set the static evaluation as our initial evaluation
  int stand_pat = eval->evaluate();

  if (stand_pat >= beta) {
    return beta;
  }
  if (alpha < stand_pat) {
    alpha = stand_pat;
  }

  int eval = stand_pat;

  // Generate, score, and sort captures only
  MoveList moves = move_gen->generate_captures();
  moves.score_moves(Move(), killer_moves.at(pos->ply).killer1,
                    killer_moves.at(pos->ply).killer2);
  moves.sort_moves();

  // Recursively search all forcing moves until quiet moves remain
  for (size_t i = 0; i < moves.size(); i++) {
    Move mv = moves.at(i);

    // Same AlphaBeta pattern as in negamax/negamax_root
    pos->make_move(mv);
    if (!move_gen->validate_gamestate()) {
      pos->undo_move(mv);
      continue;
    }
    nodes_searched++;
    eval = -quiescence(-beta, -alpha);
    pos->undo_move(mv);

    if (eval >= beta) {
      return beta;
    }

    if (eval > alpha) {
      alpha = eval;
    }
  }

  return alpha;
}

// Age -> Depth replacement scheme Transposition Table
bool Search::update_TT(const zobrist_key z_key, const size_t depth,
                       const int evaluation, const NodeType type,
                       const Move best_move) {
  zobrist_key idx = z_key % Utils::TT.size();

  // Do not update TT with junk from a cancelled search
  if (search_done) {
    return false;
  }

  // younger means our search is from a new root position
  bool younger = (search_age > Utils::TT.at(idx).age);

  // Do not overwrite deeper searches unless we are younger
  if (younger | (Utils::TT.at(idx).depth <= depth)) {
    Utils::TT.at(idx).key = z_key;
    Utils::TT.at(idx).depth = depth;
    Utils::TT.at(idx).evaluation = evaluation;
    Utils::TT.at(idx).type = type;
    Utils::TT.at(idx).best_move = best_move;
    Utils::TT.at(idx).age = search_age;
    hashfull++;
    return true;
  }

  return false;
}

// Probe our TT for an entry containing move and evaluation data
TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth,
                          bool &was_found) {

  // Hash into table
  zobrist_key idx = z_key % Utils::TT.size();
  TT_Entry entry = Utils::TT.at(idx);

  // If the entry does not exist or is a collision
  if (entry.key != z_key) {
    // Return a null entry
    was_found = false;
    return TT_Entry();
  }

  // If the entry was from a shallower search
  // Return entry for hash move purposes, do
  // not use this position for cutoffs or as PV node
  if ((entry.depth < depth)) {
    was_found = false;
    return entry;
  }

  // If entry exists and from a deeper search
  // It is viable for cutoffs and TT moves
  was_found = true;
  return entry;
}

// Overload of probe_TT if we aren't performing cut-offs
// AKA if we only care about the best move found at this pos.
TT_Entry Search::probe_TT(const zobrist_key z_key, const size_t depth) {
  bool dummy = true;
  return probe_TT(z_key, depth, dummy);
}

// Check is our search has timed out
bool Search::is_search_done() const {
  if (search_done) {
    return true;
  }
  bool times_up = false;
  // Only calculate elapsed time every 1024 nodes to save cycles
  if ((nodes_searched & 1023) == 0) {
    auto curr_time = std::chrono::high_resolution_clock::now();
    auto search_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(curr_time -
                                                              search_start);
    if (search_duration.count() > time_limit) {
      times_up = true;
    }
  }
  return times_up;
}

// Store quiet moves that fail high as "killers"
void Search::store_killer(Move mv) {
  if (mv.is_capture) {
    return;
  }
  if (mv == killer_moves.at(pos->ply).killer1) {
    return;
  }

  // Treats killer moves as a Queue of size 2
  Move tmp = killer_moves.at(pos->ply).killer1;
  killer_moves.at(pos->ply).killer2 = tmp;
  killer_moves.at(pos->ply).killer1 = mv;
}

// Print iterative deepening information to UCI as an "info" message
void Search::info_to_uci(const int eval) {
  auto end_time = std::chrono::high_resolution_clock::now();
  time_searched = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - iteration_start)
                      .count();
  time_searched = (time_searched > 0) ? time_searched : 1;
  double nps = nodes_searched / (time_searched / 1000.0);
  std::cout << "info";
  std::cout << " depth " << depth_searched;
  std::cout << " score cp " << eval;
  std::cout << " nodes " << nodes_searched;
  std::cout << " nps " << nps;
  std::cout << " hashfull " << hashfull * 1000 / Utils::TT.size();
  std::cout << " time " << time_searched;
  std::cout << " pv ";

  std::vector<Move> pv = {};
  TT_Entry entry = probe_TT(pos->z_key, 0);
  while (entry.best_move && pv.size() < depth_searched) {
    pv.push_back(entry.best_move);
    std::cout << entry.best_move << "  ";
    pos->make_move(entry.best_move);
    entry = probe_TT(pos->z_key, 0);
  }
  for (auto riter = pv.rbegin(); riter.base() != pv.begin(); ++riter) {
    Move mv = *riter;
    if (mv) {
      pos->undo_move(mv);
    }
  }
  std::cout << "\n";
}
