#pragma once
#include <iostream>
#include <chrono>
#include "common.hpp"

using namespace std;

inline uint64_t tim(){
    return chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
}

inline void bit_print(uint64_t bits){
    int z, y, x, coord;
    for (y = 0; y < HW; ++y){
        for (z = 0; z < HW; ++z){
            for (x = 0; x < HW; ++x){
                coord = z * HW * HW + y * HW + x;
                if (1 & (bits >> coord))
                    cerr << "X ";
                else
                    cerr << ". ";
            }
            cerr << "  ";
        }
        cerr << endl;
    }
    cerr << endl;
}