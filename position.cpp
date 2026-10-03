#include "position.h"

Position::Position() {
    std::bitset<81> rowBits(0b111);
    
    boardOne   |= (rowBits)      | (rowBits << 9)  | (rowBits << 18);
    boardTwo   |= (rowBits << 3) | (rowBits << 12) | (rowBits << 21);
    boardThree |= (rowBits << 6) | (rowBits << 15) | (rowBits << 24);

    boardFour  |= (rowBits << 27) | (rowBits << 36) | (rowBits << 45);
    boardFive  |= (rowBits << 30) | (rowBits << 39) | (rowBits << 48);
    boardSix   |= (rowBits << 33) | (rowBits << 42) | (rowBits << 51);

    boardSeven |= (rowBits << 54) | (rowBits << 63) | (rowBits << 72);
    boardEight |= (rowBits << 57) | (rowBits << 66) | (rowBits << 75);
    boardNine  |= (rowBits << 60) | (rowBits << 69) | (rowBits << 78);

    std::bitset<81> rowBitsTwo(0b001001001);
    sqOne   |= (rowBitsTwo)      | (rowBitsTwo << 27) | (rowBitsTwo << 54);
    sqTwo   |= (rowBitsTwo << 1) | (rowBitsTwo << 28) | (rowBitsTwo << 55);
    sqThree |= (rowBitsTwo << 2) | (rowBitsTwo << 29) | (rowBitsTwo << 56);

    rowBitsTwo <<= 9;
    sqFour  |= (rowBitsTwo)      | (rowBitsTwo << 27) | (rowBitsTwo << 54);
    sqFive  |= (rowBitsTwo << 1) | (rowBitsTwo << 28) | (rowBitsTwo << 55);
    sqSix   |= (rowBitsTwo << 2) | (rowBitsTwo << 29) | (rowBitsTwo << 56);

    rowBitsTwo <<= 9;
    sqSeven |= (rowBitsTwo)      | (rowBitsTwo << 27) | (rowBitsTwo << 54);
    sqEight |= (rowBitsTwo << 1) | (rowBitsTwo << 28) | (rowBitsTwo << 55);
    sqNine  |= (rowBitsTwo << 2) | (rowBitsTwo << 29) | (rowBitsTwo << 56);

    for (size_t m{0}; m < 512; ++m) {
        wins[m] = false;

        for (int line : lines)
            if ((m & line) == line) { wins[m] = true; break; }
    }

    turn = X;
    nextBoard = -1;
}

void Position::makeMove(std::bitset<81> placed) {
    if (turn == X) Xstate |= placed;
    else Ostate |= placed;

    for (size_t b{0}; b < 9; ++b) {
        if (bigX[b] || bigO[b]) continue;

        if (turn == X) {
            if (wins[boardMask(Xstate, b)]) bigX.set(b);
        } else {
            if (wins[boardMask(Ostate, b)]) bigO.set(b);
        }
    }

    if ((placed & sqOne) != 0) nextBoard = 1;
    if ((placed & sqTwo) != 0) nextBoard = 2;
    if ((placed & sqThree) != 0) nextBoard = 3;
    if ((placed & sqFour) != 0) nextBoard = 4;
    if ((placed & sqFive) != 0) nextBoard = 5;
    if ((placed & sqSix) != 0) nextBoard = 6;
    if ((placed & sqSeven) != 0) nextBoard = 7;
    if ((placed & sqEight) != 0) nextBoard = 8;
    if ((placed & sqNine) != 0) nextBoard = 9;

    if (bigX[nextBoard-1] || bigO[nextBoard-1] || (boardMask((Xstate | Ostate), nextBoard-1) == 0x1FF)) nextBoard = -1;

    turn ^= 1;
}

int Position::boardMask(const std::bitset<81>& state, int b) const {
    const std::bitset<81> three(0b111);
    int base = (b / 3) * 27 + (b % 3) * 3;

    int m = 0;  
    for (size_t i{0}; i < 3; ++i) m |= ((state >> (base + i * 9)) & three).to_ulong() << (i * 3);

    return m;
}

