# Lchess -  A Homemade Chess Engine

## Features
- Bitboard representation
- Full FEN notation support

## To-Do
- [x] Refactor code, implementing a better object-oriented approach and RAII
- [ ] Set up a move encoding system (quiet moves, captures, promotions, etc.)
- [ ] Finish move generation for:
  - [ ] Knights
  - [ ] King
  - [ ] Queen
  - [ ] Bishops
  - [ ] Rooks
  - [ ] Pawns

### Longer term
- [ ] Zoboroist hashing??? idk
- [ ] Endgame transposition table
- [ ] Implement minimax search with AB-pruning
- [ ] Implement a perft function, since we're kinda close to testing move generation

## Helpful Links
- https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
- https://www.chessprogramming.org/Main_Page
