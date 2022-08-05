#pragma once
#include <iostream>
#include "board.hpp"
#include "util.hpp"

using namespace std;

constexpr int stable_line_4bit[N_4BIT] = {
    0, 1, 0, 2, 
    0, 1, 0, 3, 
    1, 2, 1, 3, 
    2, 3, 3, 4
};

int stable_line_16bit[N_16BIT];

inline void stability_init(){
    int i, j;
    for (i = 0; i < N_16BIT; ++i){
        stable_line_16bit[i] = 0;
        for (j = 0; j < 4; ++j)
            stable_line_16bit[i] += stable_line_4bit[(i >> (j * 4)) & 0xF];
    }
}

inline int calc_stability(uint64_t bits){
    int res = 0;
    res += stable_line_16bit[gather_line1(bits)];
    res += stable_line_16bit[gather_line2(bits)];
    res += stable_line_16bit[gather_line3(bits)];
    return res;
}

inline int stability_cut(Board *board, int *alpha, int *beta){
    if ((board->player | board->opponent) & 0x9009000000009009ULL){
        int stab_player = calc_stability(board->player);
        int stab_opponent = calc_stability(board->opponent);
        int n_alpha = 2 * stab_player - N_CELL;
        int n_beta = N_CELL - 2 * stab_opponent;
        if (*beta <= n_alpha)
            return n_alpha;
        if (n_beta <= *alpha)
            return n_beta;
        if (n_beta <= n_alpha)
            return n_alpha;
        *alpha = max(*alpha, n_alpha);
        *beta = min(*beta, n_beta);
    }
    return SCORE_UNDEFINED;
}