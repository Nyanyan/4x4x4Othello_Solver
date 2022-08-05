#pragma once
#include <iostream>
#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

#define N_FLIP_DIRECTION 14

constexpr uint64_t flip_mask[N_FLIP_DIRECTION] = {
    0x6666666666666666ULL, // same face horizontal
    0x0FF00FF00FF00FF0ULL, // same face vertical
    0x0000FFFFFFFF0000ULL, // 3d vertical
    0x0660066006600660ULL, // A face diagonal small
    0x0660066006600660ULL, // A face diagonal large
    0x0000666666660000ULL, // B face diagonal small
    0x0000666666660000ULL, // B face diagonal large
    0x00000FF00FF00000ULL, // C face diagonal small
    0x00000FF00FF00000ULL, // C face diagonal large
    0x0000042004200000ULL, // diagonal 1
    0x0000042004200000ULL, // diagonal 2
    0x0000024002400000ULL, // diagonal 3
    0x0000024002400000ULL  // diagonal 4
};

constexpr int flip_shift[N_FLIP_DIRECTION] = {
    1,  // same face horizontal
    4,  // same face vertical
    16, // 3d vertical
    3,  // A face diagonal small
    5,  // A face diagonal large
    15, // B face diagonal small
    17, // B face diagonal large
    12, // C face diagonal small
    20, // C face diagonal large
    11, // diagonal 1
    21, // diagonal 2
    13, // diagonal 3
    19  // diagonal 4
};

inline int pop_count_ull(uint64_t x){
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x = (x * 0x0101010101010101ULL) >> 56;
    return x;
}

inline int ntz(uint64_t *x){
    return pop_count_ull((*x & (-(*x))) - 1);
}

inline int first_bit(uint64_t *x){
    return ntz(x);
}

inline int next_bit(uint64_t *x){
    *x &= *x - 1;
    return ntz(x);
}

inline int gather_line1(uint64_t x){
    return _pext_u64(x, 0xF00F00000000F00FULL);
}

inline int gather_line2(uint64_t x){
    return _pext_u64(x, 0x9999000000009999ULL);
}

inline int gather_line3(uint64_t x){
    return _pext_u64(x, 0x9009900990099009ULL);
}
