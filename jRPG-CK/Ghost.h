#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "Board.h"
#include <deque>
#include <vector>

class Ghost {
public:
    enum GhostType { RED_GHOST, GREEN_GHOST, PURPLE_GHOST };

    Ghost(GhostType color = RED_GHOST);
    void ResetToPosition(Vector2 startPosition);
    void UpdateAI(const Board& gameBoard, Vector2 playerPosition,
        Vector2 playerDirection, Vector2 greenGhostPosition,
        bool isFrightened);
    void PerformMovement(const Board& board);

    void ApplyColor(Color color);
    void ProvideVisualData(std::deque<Vector2>& segmentsBuffer, Color& colorBuffer) const;
    GhostType GetColorType() const { return ghostColorType; }

private:
    GhostType ghostColorType;
    std::deque<Vector2> bodySegments;
    Vector2 currentDirection;
    Color visualColor;
    bool shouldGrowNextMove;
    int movementCounter;
    int speedModifier;

    Vector2 CalculateNextPosition() const;
    Vector2 ApplyBoundaryWrapping(Vector2 position) const;
    bool CanMoveInDirection(Vector2 direction, const Board& board) const;
    Vector2 FindAlternativeDirection(const Board& board) const;
    void DetermineRandomMovement(const Board& gameBoard);
    void MoveAwayFromPlayer(const Board& gameBoard, Vector2 playerPosition);

    Vector2 CalculateRedTarget(Vector2 playerPosition);
    Vector2 CalculateGreenTarget(Vector2 playerPosition, Vector2 playerDirection);
    Vector2 CalculatePurpleTarget(Vector2 playerPosition, Vector2 playerDirection, Vector2 greenGhostPosition);
    void MoveTowardTarget(const Board& gameBoard, Vector2 target);
    bool ShouldMoveThisFrame();
};

#endif