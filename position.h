#ifndef POSITION_H
#define POSITION_H

#include <bitset>
#include <cstddef>
#include "types.h"

class Position {
public:
    Position();

    int legalMoves(int moves[81]) const;
    void makeMove(int bitpos);
    int result() const;

    bool isLegal(int bitPos) const;
    bool boardClosed(int b) const;

    const std::bitset<81>& xState() const { return Xstate; }
    const std::bitset<81>& oState() const { return Ostate; }
    const std::bitset<9>& bigXstate() const { return bigX; }
    const std::bitset<9>& bigOstate() const { return bigO; }
    int getNextBoard() const { return nextBoard; }
    int getTurn() const { return turn; }
private:
    static bool wins[512];
    static void initWins();

    int nextBoard; // -1 => any
    int turn;

    std::bitset<81> Xstate;
    std::bitset<81> Ostate;

    std::bitset<9> bigX;
    std::bitset<9> bigO;

    static int boardMask(const std::bitset<81>& state, int b);
};

#endif