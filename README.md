# Lchess -  A Homemade Chess Engine

## Features
- Bitboard representation
- Full FEN notation support
- ~5 million node/second move generation
- Hyperbolic quintessence sliding piece attack generation

## To-Do Immediate Work
- [ ] Set up some way to play against it (through command line)
- [ ] create new eval class (random)
- [ ] hash positions into trans table
- [ ] implement piece values
- [ ] positional piece values?

## To-Do Big Picture
- [ ] implement zobrist hashing (faster and necessary for trans. tables)
- [ ] 50 move rule
- [ ] 3-fold repitition
- [ ] encoding moves into a single int (faster move gen)
- [ ] replace undo_move with a copy from hash


### Longer term
- [ ] Zoboroist hashing??? idk
- [ ] Endgame transposition table
- [ ] Implement a recursive minimax search
- [ ] Try alpha-beta pruning

## Helpful Links
- https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
- https://www.chessprogramming.org/Main_Page

#### Done (to-do graveyard)
- [x] ~5,000,000 node per second move search
- [x] use valgrind/kcachegrind for profiling and optimization, for 17x speed up
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
- [x] implement divide function and test against stockfish
- [x] Handle en-passant ?? *MEDIUM*
- [x] add a legality checker to prune pseudo legal moves down to legal moves *MEDIUM*
- [x] handle checks *EASY*
- [x] handle pins *MEDIUM*
