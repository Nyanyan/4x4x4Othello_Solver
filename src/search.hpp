#pragma once
#include <iostream>
#include "board.hpp"
#include "stability.hpp"
#include "util.hpp"

using namespace std;

#define N_CELL_TYPE 3

uint64_t n_nodes;

constexpr uint64_t cell_type_mask[N_CELL_TYPE] = {
    0x9009000000009009ULL, // corner
    0x6996900990096996ULL, // C
    0x0660699669960660ULL  // inner
};

int nega_alpha(Board *board, int alpha, int beta, bool passed, int depth){
    ++n_nodes;
    if (pop_count_ull(board->player | board->opponent) == N_CELL)
        return board->score();
    //if (depth == 0)
    //    return board->score();
    int v = stability_cut(board, &alpha, &beta);
    if (v != SCORE_UNDEFINED)
        return v;
    uint64_t legal = board->get_legal();
    if (legal == 0ULL){
        if (passed)
            return board->score();
        board->pass();
            v = -nega_alpha(board, -beta, -alpha, true, depth);
        board->pass();
        return v;
    }
    uint64_t masked_legal, flip;
    int g, cell, i;
    v = -INF;
    /*
    for (cell = first_bit(&legal); legal; cell = next_bit(&legal)){
        flip = board->get_flip(cell);
        board->move(flip, cell);
            g = -nega_alpha(board, -beta, -alpha, false, depth - 1);
        board->undo(flip, cell);
        if (v < g){
            v = g;
            alpha = max(alpha, g);
            if (beta <= alpha)
                break;
        }
    }
    */
    for (i = 0; i < N_CELL_TYPE; ++i){
        masked_legal = legal & cell_type_mask[i];
        if (masked_legal && alpha <= beta){
            for (cell = first_bit(&masked_legal); masked_legal; cell = next_bit(&masked_legal)){
                flip = board->get_flip(cell);
                board->move(flip, cell);
                    g = -nega_alpha(board, -beta, -alpha, false, depth - 1);
                board->undo(flip, cell);
                if (v < g){
                    v = g;
                    alpha = max(alpha, g);
                    if (beta <= alpha)
                        break;
                }
            }
        }
    }
    return v;
}