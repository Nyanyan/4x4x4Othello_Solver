#pragma once
#include <iostream>
#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

#define N_FLIP_DIRECTION 14
#define N_FLIP_DIRECTION_4 16

constexpr uint64_t flip_mask[N_FLIP_DIRECTION_4] = {
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
    0x0000024002400000ULL, // diagonal 4
    0ULL, 0ULL
};

constexpr int flip_shift[N_FLIP_DIRECTION_4] = {
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
    19, // diagonal 4
    0, 0
};

/*
inline int pop_count_ull(uint64_t x){
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x = (x * 0x0101010101010101ULL) >> 56;
    return x;
}
*/

//#define	pop_count_ull(x) (int)__popcnt64(x)
#define	pop_count_ull(x) (int)__builtin_popcountll(x)

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
    return (_pext_u64(x, 0x8888000000008888ULL) << 8) | _pext_u64(x, 0x1111000000001111ULL);
}

inline int gather_line3(uint64_t x){
    return (_pext_u64(x, 0x8000800080008000ULL) << 12) | (_pext_u64(x, 0x1000100010001000ULL) << 8) | (_pext_u64(x, 0x0008000800080008ULL) << 4) | _pext_u64(x, 0x0001000100010001ULL);
    //return _pext_u64(x, 0x9009900990099009ULL);
}

inline uint64_t split_line1(int x){
    return _pdep_u64(x, 0xF00F00000000F00FULL);
}

inline uint64_t split_line2(int x){
    return _pdep_u64(x >> 8, 0x8888000000008888ULL) | _pdep_u64(x, 0x1111000000001111ULL);
}

inline uint64_t split_line3(int x){
    return _pdep_u64(x >> 12, 0x8000800080008000ULL) | _pdep_u64(x >> 8, 0x1000100010001000ULL) | _pdep_u64(x >> 4, 0x0008000800080008ULL) | _pdep_u64(x, 0x0001000100010001ULL);
    //return _pdep_u64(x, 0x9009900990099009ULL);
}

inline int gather_face1(uint64_t x){
    return _pext_u64(x, 0x000000000000FFFFULL);
}

inline int gather_face2(uint64_t x){
    return _pext_u64(x, 0xFFFF000000000000ULL);
}

inline int gather_face3(uint64_t x){
    return _pext_u64(x, 0x000F000F000F000FULL);
}

inline int gather_face4(uint64_t x){
    return _pext_u64(x, 0xF000F000F000F000ULL);
}

inline int gather_face5(uint64_t x){
    return _pext_u64(x, 0x1111111111111111ULL);
}

inline int gather_face6(uint64_t x){
    return _pext_u64(x, 0x8888888888888888ULL);
}

inline uint64_t next_to_corner(uint64_t bits){
    uint64_t res = 0ULL;
    uint64_t near_corner = (bits & 0x8008000000008008ULL) >> 1;
    res |= near_corner;
    res |= (near_corner & (bits & 0x4004000000004004ULL)) >> 1;
    near_corner = (bits & 0x1001000000001001ULL) << 1;
    res |= near_corner;
    res |= (near_corner & (bits & 0x2002000000002002ULL)) << 1;

    near_corner = (bits & 0x9000000000009000ULL) >> 4;
    res |= near_corner;
    res |= (near_corner & (bits & 0x0900000000000900ULL)) >> 4;
    near_corner = (bits & 0x0009000000000009ULL) << 4;
    res |= near_corner;
    res |= (near_corner & (bits & 0x0090000000000090ULL)) << 4;

    near_corner = (bits & 0x9009000000000000ULL) >> 16;
    res |= near_corner;
    res |= (near_corner & (bits & 0x0000900900000000ULL)) >> 16;
    near_corner = (bits & 0x0000000000009009ULL) << 16;
    res |= near_corner;
    res |= (near_corner & (bits & 0x0000000090090000ULL)) << 16;

    return res;
}

/*
Original code: https://github.com/primenumber/issen/blob/72f450256878094ffe90b75f8674599e6869c238/src/move_generator.cpp
modified by Nyanyan
*/
struct u64_4 {
    __m256i data;
    u64_4() = default;
    u64_4(uint64_t val)
        : data(_mm256_set1_epi64x(val)) {}
    u64_4(uint64_t w, uint64_t x, uint64_t y, uint64_t z)
        : data(_mm256_set_epi64x(w, x, y, z)) {}
    //u64_4(u64_2 x, u64_2 y)
    //    : data(_mm256_setr_epi64x(_mm_cvtsi128_si64(y.data), _mm_cvtsi128_si64(_mm_unpackhi_epi64(y.data, y.data)), _mm_cvtsi128_si64(x.data), _mm_cvtsi128_si64(_mm_unpackhi_epi64(x.data, x.data)))) {}
    u64_4(__m256i data) : data(data) {}
    operator __m256i() { return data; }

    void set(uint64_t val){
        data = _mm256_set1_epi64x(val);
    }

    void set(uint64_t w, uint64_t x, uint64_t y, uint64_t z){
        data = _mm256_set_epi64x(w, x, y, z);
    }
};

const u64_4 u64_4_1(1);

inline u64_4 operator>>(const u64_4 lhs, const size_t n) {
    return _mm256_srli_epi64(lhs.data, n);
}

inline u64_4 operator>>(const u64_4 lhs, const u64_4 n) {
    return _mm256_srlv_epi64(lhs.data, n.data);
}

inline u64_4 operator<<(const u64_4 lhs, const size_t n) {
    return _mm256_slli_epi64(lhs.data, n);
}

inline u64_4 operator<<(const u64_4 lhs, const u64_4 n) {
    return _mm256_sllv_epi64(lhs.data, n.data);
}

inline u64_4 operator&(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_and_si256(lhs.data, rhs.data);
}

inline u64_4 operator|(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_or_si256(lhs.data, rhs.data);
}

inline u64_4 operator^(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_xor_si256(lhs.data, rhs.data);
}

inline u64_4 operator+(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_add_epi64(lhs.data, rhs.data);
}

inline u64_4 operator+(const u64_4 lhs, const uint64_t rhs) {
    __m256i r64 = _mm256_set1_epi64x(rhs);
    return _mm256_add_epi64(lhs.data, r64);
}

inline u64_4 operator-(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_sub_epi64(lhs.data, rhs.data);
}

inline u64_4 operator-(const u64_4 lhs) {
    return _mm256_sub_epi64(_mm256_setzero_si256(), lhs.data);
}

inline u64_4 operator*(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_mullo_epi64(lhs.data, rhs.data);
}

inline u64_4 andnot(const u64_4 lhs, const u64_4 rhs) {
    return _mm256_andnot_si256(lhs.data, rhs.data);
}

inline u64_4 operator~(const u64_4 lhs) {
    return _mm256_andnot_si256(lhs.data, _mm256_set1_epi8(0xFF));
}

inline u64_4 nonzero(const u64_4 lhs) {
    return _mm256_cmpeq_epi64(lhs.data, _mm256_setzero_si256()) + u64_4_1;
}

inline uint64_t all_or(const u64_4 lhs) {
    __m128i lhs_xz_yw = _mm_or_si128(_mm256_castsi256_si128(lhs.data), _mm256_extractf128_si256(lhs.data, 1));
    return _mm_extract_epi64(lhs_xz_yw, 0) | _mm_extract_epi64(lhs_xz_yw, 1);
}

inline uint64_t all_and(const u64_4 lhs) {
    __m128i lhs_xz_yw = _mm_and_si128(_mm256_castsi256_si128(lhs.data), _mm256_extractf128_si256(lhs.data, 1));
    return _mm_extract_epi64(lhs_xz_yw, 0) & _mm_extract_epi64(lhs_xz_yw, 1);
}

/*
end of modification
*/