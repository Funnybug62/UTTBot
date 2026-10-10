#include "engine.h"

Engine::Engine(unsigned seed) : rng(seed) {}

int Engine::UCB(int cur) {
    const double logN = std::log(tree[cur].visits);
    int best = -1;
    float bestScore = -1e9;

    for (int c : tree[cur].children) {
        const Node& ch = tree[c];

        float score = ch.wins / ch.visits + C * std::sqrt(logN / ch.visits);

        if (score > bestScore) {
            bestScore = score;
            best = c;
        }
    }

    return best;
}

int Engine::expand(int cur, Position& p) {
    std::uniform_int_distribution<int> dist(0, tree[cur].nUntried-1);
    int i = dist(rng);
    int m = tree[cur].untried[i];

    tree[cur].untried[i] = tree[cur].untried[tree[cur].nUntried - 1];
    tree[cur].nUntried--;

    Node child;
    child.move = m;
    child.parent = cur;
    child.mover = p.getTurn();
    p.makeMove(m);
    child.nUntried = p.legalMoves(child.untried);
    tree.push_back(child);
    int idx = tree.size()-1;
    tree[cur].children.push_back(idx);

    return idx;
}

void Engine::backprop(int cur, int r) {
    while (cur != -1) {
        Node& n = tree[cur];
        n.visits++;

        if (r == DRAW) n.wins += 0.5;
        else if ((r == XWIN && n.mover == X) || (r == OWIN && n.mover == O)) n.wins += 1;

        cur = n.parent;
    }
}

int Engine::mostVisitedChild() {
    int best = -1;
    float bestVisits = -1;

    for (int c : tree[0].children) {
        if (tree[c].visits > bestVisits) { bestVisits = tree[c].visits; best = c; }
    }

    return tree[best].move;
}

int Engine::bestMove(const Position& pos) {
    tree.clear();
    Node root;
    root.move = -1; root.parent = -1;
    root.mover = 1 - pos.getTurn();
    root.nUntried = pos.legalMoves(root.untried);
    tree.push_back(root);

    for (int iter{0}; iter < ITERATIONS; ++iter) {
        int cur = 0;
        Position p = pos;

        while (tree[cur].nUntried == 0 && !tree[cur].children.empty()) {
            cur = UCB(cur);
            p.makeMove(tree[cur].move);
        }

        if (tree[cur].nUntried > 0 && p.result() == NONE) cur = expand(cur, p);

        int r = playout(p);

        backprop(cur, r);
    }

    return mostVisitedChild();
}

int Engine::randomMove(const Position& pos) {
    int moves[81];
    int n = pos.legalMoves(moves);
    std::uniform_int_distribution<int> dist(0, n - 1);
    return moves[dist(rng)];
}


int Engine::playout(Position pos) {
    while (pos.result() == NONE) pos.makeMove(randomMove(pos));

    return pos.result();
}