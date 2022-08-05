#pragma once
#include <iostream>
#include "bit.hpp"

using namespace std;

inline uint64_t calc_flip(const uint64_t player, const uint64_t opponent, const int pos){
    uint64_t masked_opponent, m1, m2, place = 1ULL << pos, res = 0ULL;
    for (int i = 0; i < N_FLIP_DIRECTION; ++i){
        masked_opponent = opponent & flip_mask[i];
        m1 = place << flip_shift[i];
        if (m1 & masked_opponent){
            if (((m2 = m1 << flip_shift[i]) & masked_opponent) == 0ULL){
                if (m2 & player)
                    res |= m1;
            } else{
                if ((m2 << flip_shift[i]) & player)
                    res |= m1 | m2;
            }
        }
        m1 = place >> flip_shift[i];
        if (m1 & masked_opponent){
            if (((m2 = m1 >> flip_shift[i]) & masked_opponent) == 0ULL){
                if (m2 & player)
                    res |= m1;
            } else{
                if ((m2 << flip_shift[i]) & player)
                    res |= m1 | m2;
            }
        }
    }
    return res;
}
