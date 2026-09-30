#ifndef UTT_H
#define UTT_H

#include <raylib.h>
#include <bitset>
#include <iostream>

class UTT {
public:
    UTT(int screenWidth, int screenHeight, float margin);

    void Run();

private:
    void Draw() const;
    void DrawHover() const;
    void DrawBoard() const;
    void DrawNextBoard() const;
    void DrawPieces() const;
    void DrawBigPieces() const;
    void makeMove(std::bitset<81> placed);
    int boardMask(const std::bitset<81>& state, int b) const;

    const int lines[8] = {
        0b000000111, 0b000111000, 0b111000000,   // rows
        0b001001001, 0b010010010, 0b100100100,   // cols
        0b100010001, 0b001010100                 // diagonals
    };

    int screenWidth;
    int screenHeight;
    float margin;
    float boardSize;

    const int ROW = 0b111;
    bool wins[512];

    bool XTurn;

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

    int nextBoard = -1;
};

#endif
