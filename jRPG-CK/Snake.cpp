#include "Snake.h"
#include "Board.h"
#include "Constants.h"
#include <algorithm>
#include <vector>
#include "raylib.h"  // Dla GetTime()

Snake::Snake() : currentDirection({ 1, 0 }), visualColor(YELLOW), shouldGrowNextMove(false),
queuedDirection({ 0, 0 }), queuedDirectionTime(0.0) {
    bodySegments = { {12, 12}, {11, 12}, {10, 12} };  // Pozycja bez œciany
}

void Snake::QueueDirection(Vector2 newDirection, double currentTime) {
    // Zapobiega natychmiastowemu zawracaniu (chyba ¿e to jedyna opcja)
    if ((newDirection.x != -currentDirection.x) || (newDirection.y != -currentDirection.y)) {
        queuedDirection = newDirection;
        queuedDirectionTime = currentTime;
    }
}

void Snake::UpdateDirection(Vector2 newDirection) {
    // Zachowaj oryginaln¹ logikê dla kompatybilnoœci
    if ((newDirection.x != -currentDirection.x) || (newDirection.y != -currentDirection.y)) {
        currentDirection = newDirection;
    }
}

Vector2 Snake::FindAlternativeDirection(const Board& board) const {
    std::vector<Vector2> possibleDirections = {
        {1, 0},   // prawo
        {-1, 0},  // lewo
        {0, 1},   // dó³
        {0, -1}   // góra
    };

    // Usuñ przeciwny kierunek (aby unikn¹æ natychmiastowego zawracania)
    Vector2 oppositeDirection = { -currentDirection.x, -currentDirection.y };

    // SprawdŸ wszystkie mo¿liwe kierunki
    for (const auto& direction : possibleDirections) {
        // Pomijaj przeciwny kierunek (chyba ¿e to jedyna opcja)
        if ((direction.x != oppositeDirection.x) || (direction.y != oppositeDirection.y)) {
            if (CanMoveInDirection(direction, board)) {
                return direction;
            }
        }
    }

    // Jeœli ¿aden kierunek nie dzia³a, spróbuj przeciwny (zawracanie)
    if (CanMoveInDirection(oppositeDirection, board)) {
        return oppositeDirection;
    }

    // Jeœli nic nie dzia³a, zwróæ bie¿¹cy kierunek (w¹¿ siê zatrzyma)
    return currentDirection;
}

void Snake::PerformMovement(const Board& board) {
    if (bodySegments.empty()) return;

    // 1. SprawdŸ buforowany kierunek (jeœli nie min¹³ czas)
    double currentTime = GetTime();
    if (queuedDirection.x != 0 || queuedDirection.y != 0) {
        if (currentTime - queuedDirectionTime <= QUEUE_TIMEOUT) {
            // SprawdŸ czy mo¿na iœæ w buforowanym kierunku
            if (CanMoveInDirection(queuedDirection, board)) {
                currentDirection = queuedDirection;
                queuedDirection = { 0, 0 };  // Wyczyœæ bufor TYLKO jeœli zosta³ u¿yty
            }
            // Jeœli nie mo¿na iœæ w buforowanym kierunku, NIE czyœæ bufora
            // Pozwól mu pozostaæ na kolejne klatki
        }
        else {
            queuedDirection = { 0, 0 };  // Czas min¹³ - wyczyœæ bufor
        }
    }

    // 2. SprawdŸ czy mo¿na iœæ w aktualnym kierunku
    Vector2 nextPos = CalculateNextPosition();

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

    // 3. Wykonaj ruch
    Vector2 newHead = nextPos;
    bodySegments.push_front(newHead);

    if (!shouldGrowNextMove) {
        bodySegments.pop_back();
    }
    else {
        shouldGrowNextMove = false;
    }
}

bool Snake::CanMoveInDirection(Vector2 direction, const Board& board) const {
    if (bodySegments.empty()) return false;

    Vector2 currentHead = bodySegments[0];
    Vector2 nextPosition = {
        currentHead.x + direction.x,
        currentHead.y + direction.y
    };

    // Zastosuj zawijanie granic
    nextPosition = ApplyBoundaryWrapping(nextPosition);

    // SprawdŸ czy na nastêpnej pozycji jest œciana
    return !board.CheckWallPresence((int)nextPosition.x, (int)nextPosition.y);
}

void Snake::InitializeAtPosition(Vector2 startPosition) {
    bodySegments = {
        startPosition,
        {startPosition.x - 1, startPosition.y},
        {startPosition.x - 2, startPosition.y}
    };
    currentDirection = { 1, 0 };
    shouldGrowNextMove = false;
    queuedDirection = { 0, 0 };
    queuedDirectionTime = 0.0;
}

void Snake::ScheduleGrowth() {
    shouldGrowNextMove = true;
}

void Snake::ApplyColor(Color newColor) {
    visualColor = newColor;
}

bool Snake::DetectSelfCollision() const {
    if (bodySegments.empty()) return false;

    Vector2 head = bodySegments[0];
    for (size_t i = 1; i < bodySegments.size(); i++) {
        if (head.x == bodySegments[i].x && head.y == bodySegments[i].y) {
            return true;
        }
    }
    return false;
}

bool Snake::CheckCollisionWithPosition(Vector2 position) const {
    for (const auto& segment : bodySegments) {
        if (position.x == segment.x && position.y == segment.y) {
            return true;
        }
    }
    return false;
}

void Snake::ProvideVisualData(std::deque<Vector2>& segmentsBuffer, Color& colorBuffer) const {
    segmentsBuffer = bodySegments;
    colorBuffer = visualColor;
}

Vector2 Snake::CalculateNextPosition() const {
    if (bodySegments.empty()) return { 0, 0 };

    Vector2 currentHead = bodySegments[0];
    Vector2 newHead = {
        currentHead.x + currentDirection.x,
        currentHead.y + currentDirection.y
    };

    return ApplyBoundaryWrapping(newHead);
}

Vector2 Snake::ApplyBoundaryWrapping(Vector2 position) const {
    Vector2 wrapped = position;

    if (wrapped.x < 0) wrapped.x = CELL_COUNT - 1;
    else if (wrapped.x >= CELL_COUNT) wrapped.x = 0;

    if (wrapped.y < 0) wrapped.y = CELL_COUNT - 1;
    else if (wrapped.y >= CELL_COUNT) wrapped.y = 0;

    return wrapped;
}