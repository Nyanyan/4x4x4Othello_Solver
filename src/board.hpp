#pragma once
#include <iostream>
#include "common.hpp"
#include "legal.hpp"
#include "flip.hpp"

using namespace std;

class Board{
    public:
        uint64_t player;
        uint64_t opponent;
    public:
        inline void reset(){
            player = 0x0000042002400000ULL;
            opponent = 0x0000024004200000ULL;
        }

        inline uint64_t get_legal(){
            return calc_legal(player, opponent);
        }

        inline uint64_t get_flip(int pos){
            return calc_flip(player, opponent, pos);
        }

        inline void move(uint64_t flip, int pos){
            player ^= flip;
            opponent ^= flip;
            player ^= 1ULL << pos;
            swap(player, opponent);
        }

        inline void undo(uint64_t flip, int pos){
            swap(player, opponent);
            player ^= 1ULL << pos;
            player ^= flip;
            opponent ^= flip;
        }

        inline void pass(){
            swap(player, opponent);
        }

        inline int evaluate(){
            return 0;
        }

        inline int score(){
            int p = pop_count_ull(player);
            int o = pop_count_ull(opponent);
            int e = N_CELL - p - o;
            if (p > o)
                return p - o + e;
            else if (p < o)
                return p - o - e;
            return p - o;
        }

        inline void print(){
            int z, y, x, coord;
            for (y = 0; y < HW; ++y){
                for (z = 0; z < HW; ++z){
                    for (x = 0; x < HW; ++x){
                        coord = z * HW * HW + y * HW + x;
                        if (1 & (player >> coord))
                            cerr << "X ";
                        else if (1 & (opponent >> coord))
                            cerr << "O ";
                        else
                            cerr << ". ";
                    }
                    cerr << "  ";
                }
                cerr << endl;
            }
            cerr << endl;
        }
};