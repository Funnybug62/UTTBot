#include "engine.h"

Engine::Engine(unsigned seed) : rng(seed) {}

int Engine::bestMove(const Position& pos) {
    int moves[81]{-1};
    int n = pos.legalMoves(moves);

    std::uniform_int_distribution<int> dist(0, n-1);

    return moves[dist(rng)];
}

int Engine::playout(Position pos) {
    while (pos.result() == NONE) pos.makeMove(bestMove(pos));

    return pos.result();
}