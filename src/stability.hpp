#pragma once
#include <iostream>
#include "board.hpp"
#include "util.hpp"

using namespace std;

int stable_line[N_4BIT][N_4BIT];

int stable_face[N_16BIT];

int calc_flip_line(int player, int opponent, int place){
    int res = 0;
    int masked_opponent = opponent & 0b0110;
    int m1, m2;
    m1 = place << 1;
    if (m1 & masked_opponent){
        if (((m2 = m1 << 1) & masked_opponent) == 0){
            if (m2 & player)
                res |= m1;
        } else{
            if ((m2 << 1) & player)
                res |= m1 | m2;
        }
    }
    m1 = place >> 1;
    if (m1 & masked_opponent){
        if (((m2 = m1 >> 1) & masked_opponent) == 0){
            if (m2 & player)
                res |= m1;
        } else{
            if ((m2 >> 1) & player)
                res |= m1 | m2;
        }
    }
    return res;
}

int calc_stable_line(int p0, int p1, const int pp0, const int pp1){
    int discs = p0 | p1;
    if (~discs == 0)
        return (p0 & pp0) | (p1 & pp1);
    int res = p0 | p1;
    int flip, place;
    for (int cell = 0; cell < HW; ++cell){
        place = 1 << cell;
        if ((discs & place) == 0){
            flip = calc_flip_line(p0, p1, place);
            p0 ^= place;
            p0 ^= flip;
            p1 ^= flip;
                res &= (p0 & pp0) | (p1 & pp1);
                res &= calc_stable_line(p0, p1, pp0, pp1);
            p1 ^= flip;
            p0 ^= flip;
            p0 ^= place;

            flip = calc_flip_line(p1, p0, place);
            p1 ^= place;
            p0 ^= flip;
            p1 ^= flip;
                res &= (p0 & pp0) | (p1 & pp1);
                res &= calc_stable_line(p0, p1, pp0, pp1);
            p1 ^= flip;
            p0 ^= flip;
            p1 ^= place;
        }
    }
    return res;
}

inline void stability_init(){
    int i, j, stable;
    for (i = 0; i < N_4BIT; ++i){
        for (j = 0; j < N_4BIT; ++j){
            if (i & j)
                continue;
            if (i > j)
                continue;
            stable_line[i][j] = calc_stable_line(i, j, i, j);
            stable_line[j][i] = stable_line[i][j];
        }
    }
}

inline uint64_t calc_edge_stability(Board *board){
    int pedge, oedge, stable;
    uint64_t res = 0ULL;

    pedge = gather_line1(board->player);
    oedge = gather_line1(board->opponent);
    stable =  stable_line[pedge & 0b1111][oedge & 0b1111];
    stable |= stable_line[(pedge >> 4) & 0b1111][(oedge >> 4) & 0b1111] << 4;
    stable |= stable_line[(pedge >> 8) & 0b1111][(oedge >> 8) & 0b1111] << 8;
    stable |= stable_line[(pedge >> 12) & 0b1111][(oedge >> 12) & 0b1111] << 12;
    res |= split_line1(stable);

    pedge = gather_line2(board->player);
    oedge = gather_line2(board->opponent);
    stable =  stable_line[pedge & 0b1111][oedge & 0b1111];
    stable |= stable_line[(pedge >> 4) & 0b1111][(oedge >> 4) & 0b1111] << 4;
    stable |= stable_line[(pedge >> 8) & 0b1111][(oedge >> 8) & 0b1111] << 8;
    stable |= stable_line[(pedge >> 12) & 0b1111][(oedge >> 12) & 0b1111] << 12;
    res |= split_line2(stable);

    pedge = gather_line3(board->player);
    oedge = gather_line3(board->opponent);
    stable =  stable_line[pedge & 0b1111][oedge & 0b1111];
    stable |= stable_line[(pedge >> 4) & 0b1111][(oedge >> 4) & 0b1111] << 4;
    stable |= stable_line[(pedge >> 8) & 0b1111][(oedge >> 8) & 0b1111] << 8;
    stable |= stable_line[(pedge >> 12) & 0b1111][(oedge >> 12) & 0b1111] << 12;
    res |= split_line3(stable);

    return res;
}

inline uint64_t calc_stability(Board *board){
    uint64_t stable = calc_edge_stability(board);
    //cerr << "board" << endl;
    //board->print();
    //bit_print(stable);
    //cerr << endl;
    return stable;
}

inline int stability_cut(Board *board, int *alpha, int *beta){
    if ((board->player | board->opponent) & 0x9009000000009009ULL){
        uint64_t stable = calc_stability(board);
        int stab_player = pop_count_ull(stable & board->player);
        int stab_opponent = pop_count_ull(stable & board->opponent);
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