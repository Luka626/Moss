# Moss - C++ Chess Engine

## Features
- Bitboard representation
- Full FEN notation support
- ~5 million node/second move generation
- Hyperbolic quintessence sliding piece attack generation
- Negamax depth-first search with alpha/beta pruning
- Piece-square evaluation
- Quiesence search
- Move ordering: PV move -> MVV-LVA
- Zobrist hashing and transposition table


## To-Do, Priority:
- [x] Zobrist hashing
- [x] Transposition tables
- [x] iterative deepening
- [x] move ordering
- [x] quiscense search
- [x] piece mobility
- [x] PSQ tables blending based on total material
- [ ] speed up move gen a little further

## To-Do Search
- [x] hash positions into trans table (https://www.chessprogramming.org/Zobrist_Hashing, https://www.chessprogramming.org/Transposition_Table)
- [x] quiscense search
- [ ] search extension
- [ ] killer moves

## To-Do Performance
- [x] hashing
- [x] fix legal_move_generation (right now it loops over all moves)
- [ ] encode moves in a single INT
- [x] perform another round of profiling

## To-Do Evaluation
- [x] positional piece values
- [ ] piece mobility calculation
- [ ] early game v. late game piece positions


## Helpful Links
- https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
- https://www.chessprogramming.org/Main_Page

#### Done (to-do graveyard)
- [x] cutechess-cli for version testing
- [x] piece-square evaluation
- [x] implement piece values
- [x] Implement a recursive minimax search
- [x] Try alpha-beta pruning
- [x] Minimax -> Negamax
- [x] Set up some way to play against it (through command line)
- [x] create new eval class (random)
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
