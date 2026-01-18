#include "Snake.h"
#include "Constants.h"
#include <algorithm>

Snake::Snake() : direction({ 1, 0 }), color(YELLOW), addSegment(false) {
    body = { {6,9}, {5,9}, {4,9} };
}

void Snake::Update() {
    Vector2 newHead = { body[0].x + direction.x, body[0].y + direction.y };

    // Zawijanie przez tunele
    if (newHead.x < 0) newHead.x = CELL_COUNT - 1;
    else if (newHead.x >= CELL_COUNT) newHead.x = 0;
    if (newHead.y < 0) newHead.y = CELL_COUNT - 1;
    else if (newHead.y >= CELL_COUNT) newHead.y = 0;

    body.push_front(newHead);
    if (!addSegment) {
        body.pop_back();
    }
    else {
        addSegment = false;
    }
}

void Snake::Reset(Vector2 startPos) {
    body = { startPos, {startPos.x - 1, startPos.y}, {startPos.x - 2, startPos.y} };
    direction = { 1, 0 };
    addSegment = false;
}

void Snake::ChangeDirection(Vector2 newDirection) {
    // Zapobiegaj odwróceniu siê wê¿a do ty³u
    if ((newDirection.x != -direction.x) || (newDirection.y != -direction.y)) {
        direction = newDirection;
    }
}

void Snake::Grow() {
    addSegment = true;
}

void Snake::SetColor(Color newColor) {
    color = newColor;
}

bool Snake::IsGrowing() const {
    return addSegment;
}

Vector2 Snake::GetHeadPosition() const {
    return body[0];
}

const std::deque<Vector2>& Snake::GetBody() const {
    return body;
}

Color Snake::GetColor() const {
    return color;
}

Vector2 Snake::GetDirection() const {
    return direction;
}