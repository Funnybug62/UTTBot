#include "UTT.h"

UTT::UTT(int screenWidth, int screenHeight, float margin) : screenWidth(screenWidth),screenHeight(screenHeight),margin(margin),boardSize(screenWidth - 2.0f * margin),p(Position()) {

}

void UTT::Run() {
    InitWindow(screenWidth, screenHeight, "UTT");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const float mx = GetMouseX() - margin;
        const float my = GetMouseY() - margin;
        const bool onBoard = mx >= 0 && my >= 0 && mx < boardSize && my < boardSize;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && onBoard) {
            int boardPosX = (mx * 9) / boardSize;
            int boardPosY = (my * 9) / boardSize;

            std::cout << "POS: " << boardPosX << ", " << boardPosY << std::endl;

            int bitPos = boardPosY * 9 + boardPosX;
            if (p.isLegal(bitPos)) p.makeMove(std::bitset<81>(1) << bitPos);
        }

        BeginDrawing();

        Draw();

        EndDrawing();

        if (p.result() != NONE) CloseWindow();
    }

    CloseWindow();
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
    const int board = (y / 3) * 3 + (x / 3);

    if (!p.isLegal(bitPos)) return;

    if (p.xState()[bitPos] || p.oState()[bitPos]) return;
    if (p.getNextBoard() != -1 && p.getNextBoard() != board) return;

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
    const Color color = p.getTurn() == X ? Color{235, 87, 87, 255} : Color{86, 156, 235, 255};

    if (p.getNextBoard() == -1) {
        DrawRectangleLinesEx({margin - 3.0f, margin - 3.0f, boardSize + 6.0f, boardSize + 6.0f}, 6.0f, color);
        return;
    }

    const float bigCell = boardSize / 3.0f;
    const int k = p.getNextBoard();
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

        if (p.xState()[i]) {
            DrawLineEx({cx - r, cy - r}, {cx + r, cy + r}, 5.0f, xColor);
            DrawLineEx({cx - r, cy + r}, {cx + r, cy - r}, 5.0f, xColor);
        } else if (p.oState()[i]) {
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
        if (!p.boardClosed(b)) continue;

        float ox = margin + (b % 3) * bigCell;
        float oy = margin + (b / 3) * bigCell;
        float cx = ox + bigCell / 2.0f;
        float cy = oy + bigCell / 2.0f;

        DrawRectangleRec({ox + 3.0f, oy + 3.0f, bigCell - 6.0f, bigCell - 6.0f}, dim);

        if (p.bigXstate()[b]) {
            DrawLineEx({cx - r, cy - r}, {cx + r, cy + r}, 14.0f, xColor);
            DrawLineEx({cx - r, cy + r}, {cx + r, cy - r}, 14.0f, xColor);
        } else if (p.bigOstate()[b]) {
            DrawRing({cx, cy}, r - 7.0f, r + 7.0f, 0.0f, 360.0f, 64, oColor);
        } else {
            DrawLineEx({cx - r, cy}, {cx + r, cy}, 14.0f, drawColor);
        }
    }
}
