#include "UTT.h"

UTT::UTT(int screenWidth, int screenHeight, float margin) : screenWidth(screenWidth),screenHeight(screenHeight),margin(margin),boardSize(screenWidth - 2 * margin),XTurn(true),Xstate(0),Ostate(0) {
    std::bitset<81> rowBits(ROW);
    
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
}

void UTT::Run() {
    InitWindow(screenWidth, screenHeight, "UTT");
    SetTargetFPS(60);
    std::ifstream artFile("GameOver.txt");

    while (!WindowShouldClose()) {
        BeginDrawing();
        Draw();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int boardPosX = ((GetMouseX() - margin) * 9) / boardSize;
            int boardPosY = ((GetMouseY() - margin) * 9) / boardSize;

            std::cout << "POS: " << boardPosX << ", " << boardPosY << std::endl;

            int bitPos = boardPosY * 9 + boardPosX;
            std::bitset<81> placed(1);
            placed <<= bitPos;
            
            if (((Xstate | Ostate) & placed) == 0) {
                if (nextBoard == -1) makeMove(placed);
                else if (nextBoard == 1 && ((placed & boardOne) != 0))   makeMove(placed);
                else if (nextBoard == 2 && ((placed & boardTwo) != 0))   makeMove(placed);
                else if (nextBoard == 3 && ((placed & boardThree) != 0)) makeMove(placed);
                else if (nextBoard == 4 && ((placed & boardFour) != 0))  makeMove(placed);
                else if (nextBoard == 5 && ((placed & boardFive) != 0))  makeMove(placed);
                else if (nextBoard == 6 && ((placed & boardSix) != 0))   makeMove(placed);
                else if (nextBoard == 7 && ((placed & boardSeven) != 0)) makeMove(placed);
                else if (nextBoard == 8 && ((placed & boardEight) != 0)) makeMove(placed);
                else if (nextBoard == 9 && ((placed & boardNine) != 0))  makeMove(placed);
            }
        }

        EndDrawing();

        if (wins[(bigX | bigO).to_ulong()] || ((bigX | bigO).to_ulong()) == 0x1FF) CloseWindow();
    }

    std::string line;
    while (std::getline(artFile, line)) { std::cout << line << std::endl; }
    artFile.close();

    CloseWindow();
}

int UTT::boardMask(const std::bitset<81>& state, int b) const {
    const std::bitset<81> three(0b111);
    int base = (b / 3) * 27 + (b % 3) * 3;

    int m = 0;  
    for (size_t i{0}; i < 3; ++i) m |= ((state >> (base + i * 9)) & three).to_ulong() << (i * 3);

    return m;
}

void UTT::makeMove(std::bitset<81> placed) {
    if (XTurn) Xstate |= (placed);
    else Ostate |= (placed);

    for (size_t b{0}; b < 9; ++b) {
        if (bigX[b] || bigO[b]) continue;

        if (XTurn) {
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

    XTurn ^= true;

    std::cout << "XSTATE: " << Xstate << std::endl;
    std::cout << "OSTATE: " << Ostate << std::endl;
}

void UTT::Draw() const {
    ClearBackground(Color{33, 33, 33, 255});
    DrawHover();
    DrawBoard();
    DrawNextBoard();
    DrawPieces();
    DrawBigPieces();
}

void UTT::DrawHover() const {
    const float mx = GetMouseX() - margin;
    const float my = GetMouseY() - margin;
    if (mx < 0 || my < 0 || mx >= boardSize || my >= boardSize) return;

    const float cell = boardSize / 9.0f;
    const int x = mx / cell;
    const int y = my / cell;
    const int bitPos = y * 9 + x;
    const int board = (y / 3) * 3 + (x / 3) + 1;

    if (Xstate[bitPos] || Ostate[bitPos]) return;
    if (nextBoard != -1 && nextBoard != board) return;

    DrawRectangleRec({margin + x * cell, margin + y * cell, cell, cell}, Color{60, 60, 60, 255});
}

void UTT::DrawBoard() const {
    const float x = margin;
    const float y = margin;
    const float size = boardSize;

    const float bigCell = size / 3.0f;
    const float smallCell = bigCell / 3.0f;
    const float inset = smallCell * 0.12f;

    const Color smallLine = Color{120, 120, 120, 255};
    const Color bigLine = Color{230, 230, 230, 255};

    for (int by = 0; by < 3; by++) {
        for (int bx = 0; bx < 3; bx++) {
            float ox = x + bx * bigCell;
            float oy = y + by * bigCell;
            for (int i = 1; i < 3; i++) {
                DrawLineEx({ox + i * smallCell, oy + inset}, {ox + i * smallCell, oy + bigCell - inset}, 2.0f, smallLine);
                DrawLineEx({ox + inset, oy + i * smallCell}, {ox + bigCell - inset, oy + i * smallCell}, 2.0f, smallLine);
            }
        }
    }

    for (int i = 1; i < 3; i++) {
        DrawLineEx({x + i * bigCell, y}, {x + i * bigCell, y + size}, 6.0f, bigLine);
        DrawLineEx({x, y + i * bigCell}, {x + size, y + i * bigCell}, 6.0f, bigLine);
    }

    DrawRectangleLinesEx({x - 3.0f, y - 3.0f, size + 6.0f, size + 6.0f}, 6.0f, bigLine);
}

void UTT::DrawNextBoard() const {
    const Color color = XTurn ? Color{235, 87, 87, 255} : Color{86, 156, 235, 255};

    if (nextBoard == -1) {
        DrawRectangleLinesEx({margin - 3.0f, margin - 3.0f, boardSize + 6.0f, boardSize + 6.0f}, 6.0f, color);
        return;
    }

    const float bigCell = boardSize / 3.0f;
    const int k = nextBoard - 1;
    const float ox = margin + (k % 3) * bigCell;
    const float oy = margin + (k / 3) * bigCell;

    DrawRectangleLinesEx({ox - 3.0f, oy - 3.0f, bigCell + 6.0f, bigCell + 6.0f}, 6.0f, color);
}

void UTT::DrawPieces() const {
    const float cell = boardSize / 9.0f;
    const float r = cell * 0.28f;

    const Color xColor = Color{235, 87, 87, 255};
    const Color oColor = Color{86, 156, 235, 255};

    for (int i = 0; i < 81; i++) {
        float cx = margin + (i % 9 + 0.5f) * cell;
        float cy = margin + (i / 9 + 0.5f) * cell;

        if (Xstate[i]) {
            DrawLineEx({cx - r, cy - r}, {cx + r, cy + r}, 5.0f, xColor);
            DrawLineEx({cx - r, cy + r}, {cx + r, cy - r}, 5.0f, xColor);
        } else if (Ostate[i]) {
            DrawRing({cx, cy}, r - 2.5f, r + 2.5f, 0.0f, 360.0f, 36, oColor);
        }
    }
}

void UTT::DrawBigPieces() const {
    const float bigCell = boardSize / 3.0f;
    const float r = bigCell * 0.34f;

    const Color xColor = Color{235, 87, 87, 255};
    const Color oColor = Color{86, 156, 235, 255};
    const Color drawColor = Color{150, 150, 150, 255};
    const Color dim = Color{33, 33, 33, 190};

    for (int b = 0; b < 9; b++) {
        const bool full = boardMask(Xstate | Ostate, b) == 0x1FF;
        if (!bigX[b] && !bigO[b] && !full) continue;

        float ox = margin + (b % 3) * bigCell;
        float oy = margin + (b / 3) * bigCell;
        float cx = ox + bigCell / 2.0f;
        float cy = oy + bigCell / 2.0f;

        DrawRectangleRec({ox + 3.0f, oy + 3.0f, bigCell - 6.0f, bigCell - 6.0f}, dim);

        if (bigX[b]) {
            DrawLineEx({cx - r, cy - r}, {cx + r, cy + r}, 14.0f, xColor);
            DrawLineEx({cx - r, cy + r}, {cx + r, cy - r}, 14.0f, xColor);
        } else if (bigO[b]) {
            DrawRing({cx, cy}, r - 7.0f, r + 7.0f, 0.0f, 360.0f, 64, oColor);
        } else {
            DrawLineEx({cx - r, cy}, {cx + r, cy}, 14.0f, drawColor);
        }
    }
}
