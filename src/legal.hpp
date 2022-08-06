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

/*
inline uint64_t calc_legal(const uint64_t player, const uint64_t opponent){
    uint64_t res = 0ULL;
    u64_4 masked_opponent, tmp, shift;
    for (int i = 0; i  < N_FLIP_DIRECTION_4; i += 4){
        masked_opponent.set(flip_mask[i], flip_mask[i + 1], flip_mask[i + 2], flip_mask[i + 3]);
        masked_opponent = masked_opponent & opponent;
        tmp.set(player);
        shift.set(flip_shift[i], flip_shift[i + 1], flip_shift[i + 2], flip_shift[i + 3]);
        tmp = masked_opponent & (tmp << shift);
        tmp = tmp | (masked_opponent & (tmp << shift));
        res |= all_or(tmp << shift);
        tmp.set(player);
        tmp = masked_opponent & (tmp >> shift);
        tmp = tmp | (masked_opponent & (tmp >> shift));
        res |= all_or(tmp >> shift);
    }
    return res & ~(player | opponent);
}
*/