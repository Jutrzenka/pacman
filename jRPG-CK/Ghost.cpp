#include "Ghost.h"
#include "raylib.h"
#include "Constants.h"

Ghost::Ghost() {
    SetColor(RED);
    Reset({ 20,5 });
}

void Ghost::Reset(Vector2 startPos) {
    Snake::Reset(startPos);
    ChangeDirection({ -1, 0 });
}

void Ghost::Update() {
    Vector2 newHead = { GetBody()[0].x + GetBody()[0].x, GetBody()[0].y + GetBody()[0].y };
    if (newHead.x < 0) newHead.x = CELL_COUNT - 1;
    else if (newHead.x >= CELL_COUNT) newHead.x = 0;
    if (newHead.y < 0) newHead.y = CELL_COUNT - 1;
    else if (newHead.y >= CELL_COUNT) newHead.y = 0;

    Snake::Update();
}

void Ghost::MoveRandom() {
    int dir = GetRandomValue(0, 3);
    switch (dir) {
    case 0: ChangeDirection({ 1, 0 }); break;
    case 1: ChangeDirection({ -1, 0 }); break;
    case 2: ChangeDirection({ 0, 1 }); break;
    case 3: ChangeDirection({ 0, -1 }); break;
    }
}