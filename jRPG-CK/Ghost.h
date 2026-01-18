#ifndef GHOST_H
#define GHOST_H

#include "Snake.h"

class Ghost : public Snake {
public:
    Ghost();
    void Reset(Vector2 startPos);
    void Update();
    void MoveRandom();
};
#endif