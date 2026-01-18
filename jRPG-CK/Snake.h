#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include <deque>

class Snake {
private:
    std::deque<Vector2> body;
    Vector2 direction;
    Color color;
    bool addSegment;

public:
    Snake();
    virtual ~Snake() = default;

    void Update();
    void Reset(Vector2 startPos);
    void ChangeDirection(Vector2 newDirection);
    void Grow();
    void SetColor(Color newColor);

    bool IsGrowing() const;
    Vector2 GetHeadPosition() const;
    const std::deque<Vector2>& GetBody() const;
    Color GetColor() const;
    Vector2 GetDirection() const;
};

#endif