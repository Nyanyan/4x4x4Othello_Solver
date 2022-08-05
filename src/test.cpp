#include <iostream>
#include "board.hpp"
#include "util.hpp"

using namespace std;

int main(){
    Board board;
    board.reset();
    board.print();
    uint64_t legal = board.get_legal();
    bit_print(legal);
    int pos = 60;
    uint64_t flip = board.get_flip(pos);
    bit_print(flip);
    board.move(flip, pos);
    board.print();
    board.undo(flip, pos);
    board.print();
}