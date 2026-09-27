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
    void DrawBoard() const;

    int screenWidth;
    int screenHeight;
    float margin;
    float boardSize;

    bool XTurn;

    std::bitset<81> state;
};

#endif
