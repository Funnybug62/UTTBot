#ifndef ENGINE_H
#define ENGINE_H

#include <random>
#include <iostream>
#include "position.h"

class Engine {
public:
    explicit Engine(unsigned seed = std::random_device{}());
    int bestMove(const Position& pos);
private:
    int playout(Position pos);
    std::mt19937 rng;
};

#endif