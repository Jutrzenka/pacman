#include "Ghost.h"
#include "raylib.h"
#include "Constants.h"
#include <cmath>

Ghost::Ghost() : shouldGrowNextMove(false), movementCounter(0), speedModifier(3), visualColor(WHITE), currentDirection({ -1, 0 }), isDead(false) {
}

void Ghost::ResetToPosition(Vector2 startPosition) {
    bodySegments = {
        startPosition,
        {startPosition.x - 1, startPosition.y},
        {startPosition.x - 2, startPosition.y}
    };
    currentDirection = { -1, 0 };
    shouldGrowNextMove = false;
    movementCounter = 0;
    isDead = false;
}

void Ghost::SetDead(bool dead) {
    isDead = dead;
}

bool Ghost::IsDead() const {
    return isDead;
}

void Ghost::SetSpeedModifier(int newSpeed) {
    if (newSpeed < 1) newSpeed = 1;
    if (newSpeed > 9) newSpeed = 9; // Avoid moving every frame or glitching
    speedModifier = newSpeed;
}

void Ghost::PerformMovement(const Board& board) {
    if (isDead) return;

    if (!ShouldMoveThisFrame()) {
        return;
    }

    if (bodySegments.empty()) return;

    Vector2 nextPos = CalculateNextPosition();

    if (board.CheckWallPresence((int)nextPos.x, (int)nextPos.y)) {
        Vector2 alternativeDirection = FindAlternativeDirection(board);

        if (alternativeDirection.x != currentDirection.x ||
            alternativeDirection.y != currentDirection.y) {
            currentDirection = alternativeDirection;
            nextPos = CalculateNextPosition();

            if (board.CheckWallPresence((int)nextPos.x, (int)nextPos.y)) {
                return;
            }
        }
        else {
            return;
        }
    }

    Vector2 newHead = nextPos;
    bodySegments.push_front(newHead);

    if (!shouldGrowNextMove) {
        bodySegments.pop_back();
    }
    else {
        shouldGrowNextMove = false;
    }
}

bool Ghost::ShouldMoveThisFrame() {
    movementCounter++;
    if (movementCounter >= 1000) {
        movementCounter = 0;
    }
    return (movementCounter % 10) < speedModifier;
}

void Ghost::ApplyColor(Color color) {
    visualColor = color;
}

void Ghost::ProvideVisualData(std::deque<Vector2>& segmentsBuffer, Color& colorBuffer) const {
    if (isDead) {
        segmentsBuffer.clear();
        return;
    }
    segmentsBuffer = bodySegments;
    colorBuffer = visualColor;
}

Vector2 Ghost::CalculateNextPosition() const {
    if (bodySegments.empty()) return { 0, 0 };

    Vector2 currentHead = bodySegments[0];
    Vector2 newHead = {
        currentHead.x + currentDirection.x,
        currentHead.y + currentDirection.y
    };

    return ApplyBoundaryWrapping(newHead);
}

Vector2 Ghost::ApplyBoundaryWrapping(Vector2 position) const {
    Vector2 wrapped = position;

    if (wrapped.x < 0) wrapped.x = CELL_COUNT - 1;
    else if (wrapped.x >= CELL_COUNT) wrapped.x = 0;

    if (wrapped.y < 0) wrapped.y = CELL_COUNT - 1;
    else if (wrapped.y >= CELL_COUNT) wrapped.y = 0;

    return wrapped;
}

bool Ghost::CanMoveInDirection(Vector2 direction, const Board& board) const {
    if (bodySegments.empty()) return false;

    Vector2 currentHead = bodySegments[0];
    Vector2 nextPosition = {
        currentHead.x + direction.x,
        currentHead.y + direction.y
    };

    nextPosition = ApplyBoundaryWrapping(nextPosition);

    return !board.CheckWallPresence((int)nextPosition.x, (int)nextPosition.y);
}

Vector2 Ghost::FindAlternativeDirection(const Board& board) const {
    std::vector<Vector2> possibleDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    Vector2 oppositeDirection = { -currentDirection.x, -currentDirection.y };

    for (const auto& direction : possibleDirections) {
        if ((direction.x != oppositeDirection.x || direction.y != oppositeDirection.y) &&
            CanMoveInDirection(direction, board)) {
            return direction;
        }
    }

    if (CanMoveInDirection(oppositeDirection, board)) {
        return oppositeDirection;
    }

    return currentDirection;
}

void Ghost::DetermineRandomMovement(const Board& gameBoard) {
    int dir = GetRandomValue(0, 100);

    if (dir < 25) {
        std::vector<Vector2> possibleDirections = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        std::vector<Vector2> validDirections;
        for (const auto& direction : possibleDirections) {
            if (CanMoveInDirection(direction, gameBoard)) {
                validDirections.push_back(direction);
            }
        }

        if (!validDirections.empty()) {
            int index = GetRandomValue(0, (int)validDirections.size() - 1);
            currentDirection = validDirections[index];
        }
    }
}

void Ghost::MoveAwayFromPlayer(const Board& gameBoard, Vector2 playerPosition) {
    std::vector<Vector2> possibleDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    Vector2 oppositeDir = { -currentDirection.x, -currentDirection.y };
    bool hasDirection = (currentDirection.x != 0 || currentDirection.y != 0);

    std::vector<Vector2> validDirections;
    for (const auto& direction : possibleDirections) {
        // Prevent moving backwards (180 turn)
        if (hasDirection && direction.x == oppositeDir.x && direction.y == oppositeDir.y) {
            continue;
        }

        if (CanMoveInDirection(direction, gameBoard)) {
            validDirections.push_back(direction);
        }
    }

    if (validDirections.empty()) {
        if (hasDirection && CanMoveInDirection(oppositeDir, gameBoard)) {
            validDirections.push_back(oppositeDir);
        } else {
            DetermineRandomMovement(gameBoard);
            return;
        }
    }

    Vector2 bestDirection = validDirections[0];
    float maxDistance = 0.0f;

    if (bodySegments.empty()) return;
    Vector2 currentPos = bodySegments[0];

    for (const auto& direction : validDirections) {
        Vector2 testPosition = {
            currentPos.x + direction.x,
            currentPos.y + direction.y
        };

        if (testPosition.x < 0) testPosition.x = CELL_COUNT - 1;
        else if (testPosition.x >= CELL_COUNT) testPosition.x = 0;
        if (testPosition.y < 0) testPosition.y = CELL_COUNT - 1;
        else if (testPosition.y >= CELL_COUNT) testPosition.y = 0;

        float distX = fabsf(playerPosition.x - testPosition.x);
        float distY = fabsf(playerPosition.y - testPosition.y);

        if (distX > CELL_COUNT / 2) distX = CELL_COUNT - distX;
        if (distY > CELL_COUNT / 2) distY = CELL_COUNT - distY;

        float distance = sqrtf(distX * distX + distY * distY);

        if (distance > maxDistance) {
            maxDistance = distance;
            bestDirection = direction;
        }
    }

    currentDirection = bestDirection;
}

void Ghost::MoveTowardTarget(const Board& gameBoard, Vector2 target) {
    std::vector<Vector2> possibleDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    Vector2 oppositeDir = { -currentDirection.x, -currentDirection.y };
    bool hasDirection = (currentDirection.x != 0 || currentDirection.y != 0);

    std::vector<Vector2> validDirections;
    for (const auto& direction : possibleDirections) {
        // Prevent moving backwards (180 turn)
        if (hasDirection && direction.x == oppositeDir.x && direction.y == oppositeDir.y) {
            continue;
        }

        if (CanMoveInDirection(direction, gameBoard)) {
            validDirections.push_back(direction);
        }
    }

    if (validDirections.empty()) {
        if (hasDirection && CanMoveInDirection(oppositeDir, gameBoard)) {
            validDirections.push_back(oppositeDir);
        } else {
            DetermineRandomMovement(gameBoard);
            return;
        }
    }

    Vector2 bestDirection = validDirections[0];
    float minDistance = 99999.0f; // Increased max val

    if (bodySegments.empty()) return;
    Vector2 currentPos = bodySegments[0];

    for (const auto& direction : validDirections) {
        Vector2 testPosition = {
            currentPos.x + direction.x,
            currentPos.y + direction.y
        };

        if (testPosition.x < 0) testPosition.x = CELL_COUNT - 1;
        else if (testPosition.x >= CELL_COUNT) testPosition.x = 0;
        if (testPosition.y < 0) testPosition.y = CELL_COUNT - 1;
        else if (testPosition.y >= CELL_COUNT) testPosition.y = 0;

        float distX = fabsf(target.x - testPosition.x);
        float distY = fabsf(target.y - testPosition.y);

        if (distX > CELL_COUNT / 2) distX = CELL_COUNT - distX;
        if (distY > CELL_COUNT / 2) distY = CELL_COUNT - distY;

        float distance = sqrtf(distX * distX + distY * distY);

        if (distance < minDistance) {
            minDistance = distance;
            bestDirection = direction;
        }
    }

    currentDirection = bestDirection;
}

// RedGhost Implementation
RedGhost::RedGhost() {
    visualColor = RED;
    speedModifier = 3;
    ResetToPosition({ 20, 5 });
}

void RedGhost::UpdateAI(const Board& gameBoard, Vector2 playerPosition,
    Vector2 playerDirection, Vector2 greenGhostPosition,
    bool isFrightened) {
    if (isFrightened) {
        MoveAwayFromPlayer(gameBoard, playerPosition);
    }
    else {
        MoveTowardTarget(gameBoard, playerPosition);
    }
}

// GreenGhost Implementation
GreenGhost::GreenGhost() {
    visualColor = GREEN;
    speedModifier = 4;
    ResetToPosition({ 5, 5 });
}

void GreenGhost::UpdateAI(const Board& gameBoard, Vector2 playerPosition,
    Vector2 playerDirection, Vector2 greenGhostPosition,
    bool isFrightened) {
    if (isFrightened) {
        MoveAwayFromPlayer(gameBoard, playerPosition);
    }
    else {
        Vector2 target = playerPosition;

        if (playerDirection.x == 0 && playerDirection.y == 0) {
            playerDirection = { 1, 0 };
        }

        target.x += playerDirection.x * 4;
        target.y += playerDirection.y * 4;

        if (target.x < 0) target.x += CELL_COUNT;
        else if (target.x >= CELL_COUNT) target.x -= CELL_COUNT;
        if (target.y < 0) target.y += CELL_COUNT;
        else if (target.y >= CELL_COUNT) target.y -= CELL_COUNT;

        MoveTowardTarget(gameBoard, target);
    }
}

// PurpleGhost Implementation
PurpleGhost::PurpleGhost() {
    visualColor = PURPLE;
    speedModifier = 5;
    ResetToPosition({ 10, 5 });
}

void PurpleGhost::UpdateAI(const Board& gameBoard, Vector2 playerPosition,
    Vector2 playerDirection, Vector2 greenGhostPosition,
    bool isFrightened) {
    if (isFrightened) {
        MoveAwayFromPlayer(gameBoard, playerPosition);
    }
    else {
        Vector2 target;
        // Logic specific to Purple Ghost
        if (playerDirection.x == 0 && playerDirection.y == 0) {
            playerDirection = { 1, 0 };
        }

        Vector2 pointInFrontOfPlayer = {
            playerPosition.x + playerDirection.x * 2,
            playerPosition.y + playerDirection.y * 2
        };

        if (pointInFrontOfPlayer.x < 0) pointInFrontOfPlayer.x += CELL_COUNT;
        else if (pointInFrontOfPlayer.x >= CELL_COUNT) pointInFrontOfPlayer.x -= CELL_COUNT;
        if (pointInFrontOfPlayer.y < 0) pointInFrontOfPlayer.y += CELL_COUNT;
        else if (pointInFrontOfPlayer.y >= CELL_COUNT) pointInFrontOfPlayer.y -= CELL_COUNT;

        Vector2 vectorFromGreenToPoint = {
            pointInFrontOfPlayer.x - greenGhostPosition.x,
            pointInFrontOfPlayer.y - greenGhostPosition.y
        };

        vectorFromGreenToPoint.x *= 2;
        vectorFromGreenToPoint.y *= 2;

        target = {
            greenGhostPosition.x + vectorFromGreenToPoint.x,
            greenGhostPosition.y + vectorFromGreenToPoint.y
        };

        if (target.x < 0) target.x += CELL_COUNT;
        else if (target.x >= CELL_COUNT) target.x -= CELL_COUNT;
        if (target.y < 0) target.y += CELL_COUNT;
        else if (target.y >= CELL_COUNT) target.y -= CELL_COUNT;

        MoveTowardTarget(gameBoard, target);
    }
}