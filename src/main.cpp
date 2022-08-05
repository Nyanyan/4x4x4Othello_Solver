#include <iostream>
#include "search.hpp"

using namespace std;

inline void init(){
    stability_init();
    cerr << "initialized" << endl;
}

int main(){
    init();
    Board board;
    // board.reset();
    board.player = 0x6648126B3A592402ULL;
    board.opponent = 0x0986249445264B64ULL;
    board.print();
    cerr << "conflict " << (board.player & board.opponent) << endl;
    int depth = N_CELL - pop_count_ull(board.player | board.opponent);
    cerr << "depth " << depth << endl;
    int alpha = -N_CELL, beta = N_CELL;
    uint64_t strt = tim();
    n_nodes = 0;
    cerr << "result " << nega_alpha(&board, alpha, beta, false, depth) << endl;
    uint64_t elapsed = tim() - strt;
    cerr << "done in " << elapsed << " msec " << n_nodes << " nodes nps " << (n_nodes * 1000 / max(1ULL, elapsed)) << endl;
    return 0;
}