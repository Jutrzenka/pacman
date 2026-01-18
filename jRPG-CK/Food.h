#ifndef FOOD_H
#define FOOD_H

#include "raylib.h"

class Food {
private:
    Vector2 position;

public:
    Food();
    void Respawn();
    Vector2 GetPosition() const;
};
#endif