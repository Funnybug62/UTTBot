#include "UTT.h"

UTT::UTT(int screenWidth, int screenHeight, float margin)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      margin(margin),
      boardSize(screenWidth - 2 * margin),
      XTurn(true),
      state(0) {}

void UTT::Run() {
    InitWindow(screenWidth, screenHeight, "UTT");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        Draw();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int boardPosX = ((GetMouseX() - 40) * 9) / boardSize;
            int boardPosY = ((GetMouseY() - 40) * 9) / boardSize;

            std::cout << "POS: " << boardPosX << ", " << boardPosY << std::endl;

            int bitPos = boardPosY * 9 + boardPosX;
            state |= (std::bitset<81>(1) << bitPos);
            std::cout << state << std::endl;
        }
        
        EndDrawing();
    }

    CloseWindow();
}

void UTT::Draw() const {
    ClearBackground(Color{33, 33, 33, 255});
    DrawBoard();
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
