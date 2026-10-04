#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <bitset>
#include <cstddef>
#include "types.h"

class Position {
public:
    Position();

    int legalMoves(int moves[81]) const;
    void makeMove(std::bitset<81> placed);
    int result() const;

    bool isLegal(int bitPos) const;
    bool boardClosed(int b) const;

    const std::bitset<81>& xState() const { return Xstate; }
    const std::bitset<81>& oState() const { return Ostate; }
    const std::bitset<9>& bigXstate() const { return bigX; }
    const std::bitset<9>& bigOstate() const { return bigO; }
    int getNextBoard() { return nextBoard; }
    int getTurn() { return turn; }
private:
    bool wins[512];

    const int lines[8] = {
        0b000000111, 0b000111000, 0b111000000,   // rows
        0b001001001, 0b010010010, 0b100100100,   // cols
        0b100010001, 0b001010100                 // diagonals
    };

    int nextBoard; // -1 => any
    int turn;

    std::bitset<81> Xstate;
    std::bitset<81> Ostate;

    std::bitset<9> bigX;
    std::bitset<9> bigO;

    std::bitset<81> boardOne;
    std::bitset<81> boardTwo;
    std::bitset<81> boardThree;
    std::bitset<81> boardFour;
    std::bitset<81> boardFive;
    std::bitset<81> boardSix;
    std::bitset<81> boardSeven;
    std::bitset<81> boardEight;
    std::bitset<81> boardNine;

    std::bitset<81> sqOne;
    std::bitset<81> sqTwo;
    std::bitset<81> sqThree;
    std::bitset<81> sqFour;
    std::bitset<81> sqFive;
    std::bitset<81> sqSix;
    std::bitset<81> sqSeven;
    std::bitset<81> sqEight;
    std::bitset<81> sqNine;

    int boardMask(const std::bitset<81>& state, int b) const;
};

#endif