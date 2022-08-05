#pragma once
#include <iostream>
#include "bit.hpp"

using namespace std;

inline uint64_t calc_legal(const uint64_t player, const uint64_t opponent){
    uint64_t masked_opponent, res = 0ULL, tmp;
    for (int i = 0; i  < N_FLIP_DIRECTION; ++i){
        masked_opponent = opponent & flip_mask[i];
        tmp =  masked_opponent & (player << flip_shift[i]);
        tmp |= masked_opponent & (tmp << flip_shift[i]);
        res |= tmp << flip_shift[i];
        tmp =  masked_opponent & (player >> flip_shift[i]);
        tmp |= masked_opponent & (tmp >> flip_shift[i]);
        res |= tmp >> flip_shift[i];
    }
    return res & ~(player | opponent);
}
