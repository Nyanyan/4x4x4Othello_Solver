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
    board.player = 0x6678126B3A592402ULL;
    board.opponent = 0x09862494C5A64B64ULL;
    cerr << "conflict " << (board.player & board.opponent) << endl;
    int depth = N_CELL - pop_count_ull(board.player | board.opponent);
    cerr << "depth " << depth << endl;
    int alpha = -N_CELL, beta = N_CELL;
    uint64_t strt = tim();
    n_nodes = 0;
    cerr << "result " << nega_alpha(&board, alpha, beta, false, depth) << endl;
    cerr << "done in " << tim() - strt << " msec " << n_nodes << " nodes" << endl;
    return 0;
}