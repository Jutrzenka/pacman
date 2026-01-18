#include "Ghost.h"
#include "raylib.h"
#include "Constants.h"
#include <cmath>

Ghost::Ghost() {
    SetColor(RED);
    Reset({ 20,5 });
}

void Ghost::Reset(Vector2 startPos) {
    Snake::Reset(startPos);
    ChangeDirection({ -1, 0 });
}

void Ghost::Update() {
    Snake::Update();
}

void Ghost::MoveRandom(const Board& board) {
    int dir = GetRandomValue(0, 100);

    if (dir < 25) {  // 25% szans na zmianê kierunku
        std::vector<Vector2> possibleDirections = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        Vector2 currentPos = GetHeadPosition();
        int currentX = (int)currentPos.x;
        int currentY = (int)currentPos.y;

        for (const auto& direction : possibleDirections) {
            int nextX = currentX + (int)direction.x;
            int nextY = currentY + (int)direction.y;

            // Zawijanie przez tunele
            if (nextX < 0) nextX = CELL_COUNT - 1;
            else if (nextX >= CELL_COUNT) nextX = 0;

            if (!board.IsWall(nextX, nextY)) {
                ChangeDirection(direction);
                break;
            }
        }
    }
}

void Ghost::UpdateAI(const Board& board, Vector2 playerPos, bool isScared) {
    if (isScared) {
        // W trybie przestraszenia ucieka od gracza
        MoveAwayFromPlayer(board, playerPos);
    }
    else {
        // Normalnie goni gracza
        MoveTowardsPlayer(board, playerPos);
    }
}

void Ghost::MoveTowardsPlayer(const Board& board, Vector2 playerPos) {
    Vector2 currentPos = GetHeadPosition();
    int currentX = (int)currentPos.x;
    int currentY = (int)currentPos.y;
    int playerX = (int)playerPos.x;
    int playerY = (int)playerPos.y;

    // Proste œciganie: wybierz kierunek który zbli¿a do gracza
    std::vector<Vector2> possibleDirections;

    if (playerX > currentX && !board.IsWall(currentX + 1, currentY))
        possibleDirections.push_back({ 1, 0 });
    if (playerX < currentX && !board.IsWall(currentX - 1, currentY))
        possibleDirections.push_back({ -1, 0 });
    if (playerY > currentY && !board.IsWall(currentX, currentY + 1))
        possibleDirections.push_back({ 0, 1 });
    if (playerY < currentY && !board.IsWall(currentX, currentY - 1))
        possibleDirections.push_back({ 0, -1 });

    if (!possibleDirections.empty()) {
        // Wybierz losowy kierunek z mo¿liwych
        int index = GetRandomValue(0, (int)possibleDirections.size() - 1);
        ChangeDirection(possibleDirections[index]);
    }
    else {
        // Jeœli nie ma dobrego kierunku, porusz siê losowo
        MoveRandom(board);
    }
}

void Ghost::MoveAwayFromPlayer(const Board& board, Vector2 playerPos) {
    Vector2 currentPos = GetHeadPosition();
    int currentX = (int)currentPos.x;
    int currentY = (int)currentPos.y;
    int playerX = (int)playerPos.x;
    int playerY = (int)playerPos.y;

    // Uciekaj od gracza
    std::vector<Vector2> possibleDirections;

    // Dodaj wszystkie mo¿liwe kierunki bez œcian
    if (!board.IsWall(currentX + 1, currentY))
        possibleDirections.push_back({ 1, 0 });
    if (!board.IsWall(currentX - 1, currentY))
        possibleDirections.push_back({ -1, 0 });
    if (!board.IsWall(currentX, currentY + 1))
        possibleDirections.push_back({ 0, 1 });
    if (!board.IsWall(currentX, currentY - 1))
        possibleDirections.push_back({ 0, -1 });

    if (!possibleDirections.empty()) {
        // Wybierz kierunek który najdalej od gracza
        Vector2 bestDir = possibleDirections[0];
        float maxDistance = 0;

        for (const auto& dir : possibleDirections) {
            float dist = sqrtf(powf((float)(playerX - (currentX + (int)dir.x)), 2) +
                powf((float)(playerY - (currentY + (int)dir.y)), 2));
            if (dist > maxDistance) {
                maxDistance = dist;
                bestDir = dir;
            }
        }
        ChangeDirection(bestDir);
    }
}