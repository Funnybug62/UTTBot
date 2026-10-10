#ifndef ENGINE_H
#define ENGINE_H

#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "position.h"

struct Node {
    int move;
    int parent;
    int mover;
    std::vector<int> children;
    int untried[81];
    int nUntried;
    int visits = 0;
    float wins = 0;
};

class Engine {
public:
    explicit Engine(unsigned seed = std::random_device{}());
    int bestMove(const Position& pos);
private:
    static constexpr double C = 1.41;
    const int ITERATIONS = 10000;

    int playout(Position pos);
    int UCB(int cur);
    std::mt19937 rng;

    std::vector<Node> tree;

    int randomMove(const Position& pos);
    int expand(int cur, Position& p);
    void backprop(int cur, int r);
    int mostVisitedChild();
};

#endif