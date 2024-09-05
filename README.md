# Lchess -  A Homemade Chess Engine

## Features
- Bitboard representation
- Full FEN notation support
- Hyperbolic quintessence move gen

## To-Do Big Picture
    - [ ] 50 move rule
    - [ ] 3-fold repitition

## To-Do Immediate Work
- [x] Handle en-passant ?? *MEDIUM*
- [x] add a legality checker to prune pseudo legal moves down to legal moves *MEDIUM*
- [x] handle checks *EASY*
- [x] handle pins *MEDIUM*

### Longer term
- [ ] Zoboroist hashing??? idk
- [ ] Endgame transposition table
- [ ] Implement a recursive minimax search
- [ ] Try alpha-beta pruning

### Refactors I'd like to do:
- [ ] separate utils into tables/masks and bit magic
- [ ] redo pawn move generation entirely

## Helpful Links
- https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
- https://www.chessprogramming.org/Main_Page

#### Done (to-do graveyard)
- [x] Refactor code, implementing a better object-oriented approach and RAII
- [x] Set up a move encoding system (quiet moves, captures, promotions, etc.)
- [x] Finish move generation for:
  - [x] Knights
  - [x] King
  - [x] Queen
  - [x] Bishops
  - [x] Rooks
  - [x] Pawns
- [x] hyperbolic quintessence to handle sliding piece move generation
- [x] Implement pawn attack tables *EASY*
- [x] implement captures in make_move *EASY*
- [x] handle edge cases:
    - [x] en passant
    - [x] promotion
    - [x] castling
    - [x] checks
    - [x] pins
- [x] implement perft function
