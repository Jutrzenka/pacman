#include "Food.h"
#include "raylib.h"
#include "Constants.h"

Food::Food() {
    Respawn();
}

void Food::Respawn() {
    position.x = GetRandomValue(0, CELL_COUNT - 1);
    position.y = GetRandomValue(0, CELL_COUNT - 1);
}

Vector2 Food::GetPosition() const {
    return position;
}