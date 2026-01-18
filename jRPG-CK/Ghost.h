#ifndef GHOST_H
#define GHOST_H

#include "Snake.h"
#include "Board.h"

class Ghost : public Snake {
public:
    Ghost();
    void Reset(Vector2 startPos);
    void Update();
    void MoveRandom(const Board& board);
    void UpdateAI(const Board& board, Vector2 playerPos, bool isScared);

private:
    void MoveTowardsPlayer(const Board& board, Vector2 playerPos);
    void MoveAwayFromPlayer(const Board& board, Vector2 playerPos);
};
#endif