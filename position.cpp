#include "position.h"

bool Position::wins[512];

Position::Position() : nextBoard(-1),turn(X) {
    initWins();
}

void Position::initWins() {
        static bool done = false;
        if (done) return;

        const int lines[8] = {
            0b000000111, 0b000111000, 0b111000000,   // rows
            0b001001001, 0b010010010, 0b100100100,   // cols
            0b100010001, 0b001010100                 // diagonals
        };

        for (size_t m{0}; m < 512; ++m) {
            wins[m] = false;

            for (int line : lines)
                if ((m & line) == line) { wins[m] = true; break; }
        }
        done = true;
}

int Position::result() const {
    if (wins[bigX.to_ulong()]) return XWIN;
    if (wins[bigO.to_ulong()]) return OWIN;

    int moves[81];
    if (legalMoves(moves) == 0) return DRAW;
    
    return NONE;
}

bool Position::boardClosed(int b) const {
    return bigX[b] || bigO[b] || boardMask(Xstate | Ostate, b) == 0x1FF;
}

bool Position::isLegal(int bitpos) const {
    int moves[81];
    int n = legalMoves(moves);
    for (int i {0}; i < n; ++i) if (moves[i] == bitpos) return true;
    return false;
}

int Position::legalMoves(int moves[81]) const {
    const std::bitset<81> occupied = (Xstate | Ostate);
    int n = 0;

    for (int b{0}; b < 9; ++b) {
        if (nextBoard != -1 && b != nextBoard) continue;
        if (boardClosed(b)) continue;

        int base = (b / 3) * 27 + (b % 3) * 3;
        for (size_t r{0}; r < 3; ++r)
            for (size_t c{0}; c < 3; ++c) {
                int pos = base + r * 9 + c;
                if (!occupied[pos]) moves[n++] = pos;
            }
    }   

    return n;
}

void Position::makeMove(int bitpos) {
    const int b = (bitpos/27) * 3 + (bitpos % 9) / 3;
    const int cell = (bitpos/9%3) * 3 + bitpos % 3;

    if (turn == X) {
        Xstate.set(bitpos);
        if (!bigX[b] && !bigO[b] && wins[boardMask(Xstate, b)]) bigX.set(b);
    } else {
        Ostate.set(bitpos);
        if (!bigX[b] && !bigO[b] && wins[boardMask(Ostate, b)]) bigO.set(b);
    }

    nextBoard = boardClosed(cell) ? -1 : cell;
    turn ^= 1;  
}

int Position::boardMask(const std::bitset<81>& state, int b) {
    const std::bitset<81> three(0b111);
    int base = (b / 3) * 27 + (b % 3) * 3;

    int m = 0;  
    for (size_t i{0}; i < 3; ++i) m |= ((state >> (base + i * 9)) & three).to_ulong() << (i * 3);

    return m;
}