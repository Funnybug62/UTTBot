#ifndef UTT_H
#define UTT_H

#include <raylib.h>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include "position.h"
#include "engine.h"

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

    int screenWidth;
    int screenHeight;
    float margin;
    float boardSize;
    Position p;
    bool AITurn;

    Engine eng;
};

#endif
