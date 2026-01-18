#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include <deque>
#include <vector>

class Board;  // Forward declaration

class Snake {
protected:  // TUTAJ MUSI BYÆ protected
    std::deque<Vector2> bodySegments;
    Vector2 currentDirection;
    Color visualColor;
    bool shouldGrowNextMove;

    // Buforowanie zmian kierunku
    Vector2 queuedDirection;
    double queuedDirectionTime;
    const double QUEUE_TIMEOUT = 1.0;  // 1 sekunda

public:
    Snake();

    // Akcje
    void PerformMovement(const Board& board);
    void QueueDirection(Vector2 newDirection, double currentTime);
    void UpdateDirection(Vector2 newDirection);
    void InitializeAtPosition(Vector2 startPosition);
    void ScheduleGrowth();
    void ApplyColor(Color newColor);

    // Logika kolizji
    bool DetectSelfCollision() const;
    bool CheckCollisionWithPosition(Vector2 position) const;

    // Sprawdzanie ruchu
    bool CanMoveInDirection(Vector2 direction, const Board& board) const;

    // Metody dostarczaj¹ce dane do renderowania
    void ProvideVisualData(std::deque<Vector2>& segmentsBuffer, Color& colorBuffer) const;
    Vector2 CalculateNextPosition() const;

    // Nowa metoda - szukanie alternatywnego kierunku
    Vector2 FindAlternativeDirection(const Board& board) const;

private:
    Vector2 ApplyBoundaryWrapping(Vector2 position) const;
};

#endif