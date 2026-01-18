#include "Ghost.h"
#include "raylib.h"
#include "Constants.h"
#include <cmath>

Ghost::Ghost() {
    ApplyColor(RED);
    ResetToPosition({ 20,5 });
}

void Ghost::ResetToPosition(Vector2 startPosition) {
    Snake::InitializeAtPosition(startPosition);
    UpdateDirection({ -1, 0 });
}

void Ghost::PerformMovement(const Board& board) {
    // Duch ma dostêp do protected pól Snake
    if (bodySegments.empty()) return;

    Vector2 nextPos = CalculateNextPosition();

    // SprawdŸ czy mo¿na iœæ w aktualnym kierunku
    if (board.CheckWallPresence((int)nextPos.x, (int)nextPos.y)) {
        // Œciana przed nami - znajdŸ alternatywny kierunek
        Vector2 alternativeDirection = FindAlternativeDirection(board);

        // Jeœli alternatywny kierunek ró¿ni siê od aktualnego, zmieñ go
        if (alternativeDirection.x != currentDirection.x ||
            alternativeDirection.y != currentDirection.y) {
            currentDirection = alternativeDirection;

            // Przelicz nastêpn¹ pozycjê z nowym kierunkiem
            nextPos = CalculateNextPosition();

            // Upewnij siê, ¿e nowy kierunek te¿ nie prowadzi do œciany
            if (board.CheckWallPresence((int)nextPos.x, (int)nextPos.y)) {
                return;  // Nadal œciana - zatrzymaj siê
            }
        }
        else {
            return;  // Nie znaleziono alternatywnego kierunku - zatrzymaj siê
        }
    }

    // Wykonaj ruch
    Vector2 newHead = nextPos;
    bodySegments.push_front(newHead);

    if (!shouldGrowNextMove) {
        bodySegments.pop_back();
    }
    else {
        shouldGrowNextMove = false;
    }
}

bool Ghost::CanGhostMoveInDirection(Vector2 direction, const Board& board) const {
    return CanMoveInDirection(direction, board);
}

void Ghost::DetermineRandomMovement(const Board& gameBoard) {
    int dir = GetRandomValue(0, 100);

    if (dir < 25) {
        std::vector<Vector2> possibleDirections = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        // ZnajdŸ kierunki bez œcian
        std::vector<Vector2> validDirections;
        for (const auto& direction : possibleDirections) {
            if (CanGhostMoveInDirection(direction, gameBoard)) {
                validDirections.push_back(direction);
            }
        }

        // Jeœli s¹ dostêpne kierunki, wybierz losowy
        if (!validDirections.empty()) {
            int index = GetRandomValue(0, (int)validDirections.size() - 1);
            UpdateDirection(validDirections[index]);
        }
    }
}

void Ghost::UpdateArtificialIntelligence(const Board& gameBoard, Vector2 playerPosition, bool isFrightened) {
    if (isFrightened) {
        MoveAwayFromPlayer(gameBoard, playerPosition);
    }
    else {
        MoveTowardPlayer(gameBoard, playerPosition);
    }

    // Duch nie wykonuje ruchu tutaj - ruch wykonuje siê w PerformMovement()
}

void Ghost::MoveTowardPlayer(const Board& gameBoard, Vector2 playerPosition) {
    // Pobierz dostêpne kierunki
    std::vector<Vector2> possibleDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    std::vector<Vector2> validDirections;
    for (const auto& direction : possibleDirections) {
        if (CanGhostMoveInDirection(direction, gameBoard)) {
            validDirections.push_back(direction);
        }
    }

    if (validDirections.empty()) {
        DetermineRandomMovement(gameBoard);
        return;
    }

    // Wybierz kierunek który najbli¿ej gracza
    Vector2 bestDirection = validDirections[0];
    float minDistance = 9999.0f;

    std::deque<Vector2> ghostSegments;
    Color ghostColor;
    ProvideVisualData(ghostSegments, ghostColor);

    if (ghostSegments.empty()) return;

    Vector2 currentPos = ghostSegments[0];

    for (const auto& direction : validDirections) {
        Vector2 testPosition = {
            currentPos.x + direction.x,
            currentPos.y + direction.y
        };

        // Zastosuj zawijanie granic
        if (testPosition.x < 0) testPosition.x = CELL_COUNT - 1;
        else if (testPosition.x >= CELL_COUNT) testPosition.x = 0;
        if (testPosition.y < 0) testPosition.y = CELL_COUNT - 1;
        else if (testPosition.y >= CELL_COUNT) testPosition.y = 0;

        float distX = fabsf(playerPosition.x - testPosition.x);
        float distY = fabsf(playerPosition.y - testPosition.y);

        // Uwzglêdnij zawijanie w odleg³oœci
        if (distX > CELL_COUNT / 2) distX = CELL_COUNT - distX;
        if (distY > CELL_COUNT / 2) distY = CELL_COUNT - distY;

        float distance = sqrtf(distX * distX + distY * distY);

        if (distance < minDistance) {
            minDistance = distance;
            bestDirection = direction;
        }
    }

    UpdateDirection(bestDirection);
}

void Ghost::MoveAwayFromPlayer(const Board& gameBoard, Vector2 playerPosition) {
    // Pobierz dostêpne kierunki
    std::vector<Vector2> possibleDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    std::vector<Vector2> validDirections;
    for (const auto& direction : possibleDirections) {
        if (CanGhostMoveInDirection(direction, gameBoard)) {
            validDirections.push_back(direction);
        }
    }

    if (validDirections.empty()) {
        DetermineRandomMovement(gameBoard);
        return;
    }

    // Wybierz kierunek który najdalej od gracza
    Vector2 bestDirection = validDirections[0];
    float maxDistance = 0.0f;

    std::deque<Vector2> ghostSegments;
    Color ghostColor;
    ProvideVisualData(ghostSegments, ghostColor);

    if (ghostSegments.empty()) return;

    Vector2 currentPos = ghostSegments[0];

    for (const auto& direction : validDirections) {
        Vector2 testPosition = {
            currentPos.x + direction.x,
            currentPos.y + direction.y
        };

        // Zastosuj zawijanie granic
        if (testPosition.x < 0) testPosition.x = CELL_COUNT - 1;
        else if (testPosition.x >= CELL_COUNT) testPosition.x = 0;
        if (testPosition.y < 0) testPosition.y = CELL_COUNT - 1;
        else if (testPosition.y >= CELL_COUNT) testPosition.y = 0;

        float distX = fabsf(playerPosition.x - testPosition.x);
        float distY = fabsf(playerPosition.y - testPosition.y);

        // Uwzglêdnij zawijanie w odleg³oœci
        if (distX > CELL_COUNT / 2) distX = CELL_COUNT - distX;
        if (distY > CELL_COUNT / 2) distY = CELL_COUNT - distY;

        float distance = sqrtf(distX * distX + distY * distY);

        if (distance > maxDistance) {
            maxDistance = distance;
            bestDirection = direction;
        }
    }

    UpdateDirection(bestDirection);
}