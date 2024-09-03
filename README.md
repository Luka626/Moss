# Lchess -  A Homemade Chess Engine

## Features
- Bitboard representation
- Full FEN notation support

## To-Do Big Picture
- [x] Refactor code, implementing a better object-oriented approach and RAII
- [x] Set up a move encoding system (quiet moves, captures, promotions, etc.)
- [ ] Finish move generation for:
  - [x] Knights
  - [x] King
  - [ ] Queen
  - [ ] Bishops
  - [ ] Rooks
  - [ ] Pawns


## To-Do Immediate Work
- [ ] Implement pawn attack tables *EASY*
- [ ] Handle en-passant ?? *MEDIUM*
- [ ] magic bitboards?? how do you do sliding piece attacks lol *HARD*
- [ ] add a legality checker to prune pseudo legal moves down to legal moves *MEDIUM*
- [ ] handle checks *EASY*
- [ ] handle pins *MEDIUM*
- [ ] implement captures in make_move *EASY*

### Longer term
- [ ] Zoboroist hashing??? idk
- [ ] Endgame transposition table
- [ ] Implement a recursive minimax search
- [ ] Try alpha-beta pruning
- [ ] Implement a perft function, since we're kinda close to testing move generation

## Helpful Links
- https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
- https://www.chessprogramming.org/Main_Page
