#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "Board.h"
#include "Enums.h"
#include <deque>
#include <vector>

class Ghost {
public:
    Ghost();
    virtual ~Ghost() = default;

    void ResetToPosition(Vector2 startPosition);
    void PerformMovement(const Board& board);
    void ApplyColor(Color color);
    void ProvideVisualData(std::deque<Vector2>& segmentsBuffer, Color& colorBuffer) const;

    void SetDead(bool dead);
    bool IsDead() const;

    virtual void UpdateAI(const Board& gameBoard, Vector2 playerPosition,
        Vector2 playerDirection, Vector2 greenGhostPosition,
        bool isFrightened) = 0;

protected:
    std::deque<Vector2> bodySegments;
    Vector2 currentDirection;
    Color visualColor;
    bool shouldGrowNextMove;
    bool isDead;
    
    // Movement throttling
    int movementCounter;
    int speedModifier;

    Vector2 CalculateNextPosition() const;
    Vector2 ApplyBoundaryWrapping(Vector2 position) const;
    bool CanMoveInDirection(Vector2 direction, const Board& board) const;
    Vector2 FindAlternativeDirection(const Board& board) const;
    
    // AI Helpers
    void DetermineRandomMovement(const Board& gameBoard);
    void MoveAwayFromPlayer(const Board& gameBoard, Vector2 playerPosition);
    void MoveTowardTarget(const Board& gameBoard, Vector2 target);
    bool ShouldMoveThisFrame();
};

class RedGhost : public Ghost {
public:
    RedGhost();
    void UpdateAI(const Board& gameBoard, Vector2 playerPosition,
        Vector2 playerDirection, Vector2 greenGhostPosition,
        bool isFrightened) override;
};

class GreenGhost : public Ghost {
public:
    GreenGhost();
    void UpdateAI(const Board& gameBoard, Vector2 playerPosition,
        Vector2 playerDirection, Vector2 greenGhostPosition,
        bool isFrightened) override;
};

class PurpleGhost : public Ghost {
public:
    PurpleGhost();
    void UpdateAI(const Board& gameBoard, Vector2 playerPosition,
        Vector2 playerDirection, Vector2 greenGhostPosition,
        bool isFrightened) override;
};

#endif