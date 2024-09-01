#include <chrono>
#include <iostream>


#include "position.cpp"
#include "datatypes.hpp"

//


// https://www.codeproject.com/Articles/5313417/Worlds-Fastest-Bitboard-Chess-Movegenerator
//
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    Position test_position = Position();
    test_position.set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");


    std::cout << test_position;
    std::vector<Move> moves_list = test_position.generate_pseudo_legal_moves(WHITE);
    std::cout << "Perft, depth 0: " << moves_list.size() << std::endl;

    ///*
    for (const auto& move : moves_list){
        std::cout << move.piece << move.from << move.to << std::endl;
    }
    //*/


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "\nRuntime: " << duration.count() << " ms" << std::endl;

    return 0;
}
