#include "Board.h"
#include <algorithm>

// Sta³y uk³ad planszy Pacman'a (25x25)
const int Board::layout[HEIGHT][WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
    {0,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,0},
    {0,3,1,0,0,1,2,1,0,0,0,1,2,1,0,0,0,1,2,1,0,0,1,3,0},
    {0,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,0},
    {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
    {0,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,0},
    {0,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,0},
    {0,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,0},
    {0,1,1,1,1,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,1,1,1,1,0},
    {0,0,0,0,0,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {0,1,1,1,1,1,2,1,1,0,1,0,0,0,1,0,1,1,2,1,1,1,1,1,0},
    {0,2,2,2,2,2,2,2,2,0,1,0,0,0,1,0,2,2,2,2,2,2,2,2,0},
    {0,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,0},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {0,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,0},
    {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
    {0,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,0},
    {0,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,0},
    {0,3,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,3,0},
    {0,1,1,2,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,2,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

Board::Board() {
    InitializeFood();
}

void Board::Draw() const {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            CellType cell = GetCellType(x, y);

            switch (cell) {
            case WALL:
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
                // Dodanie wewnêtrznego cienia dla œciany
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKBLUE);
                break;

            case FOOD:
                if (!foodEaten[y][x]) {
                    DrawCircle((int)(x * CELL_SIZE + CELL_SIZE / 2.0f),
                        (int)(y * CELL_SIZE + CELL_SIZE / 2.0f),
                        CELL_SIZE / 8, YELLOW);
                }
                break;

            case POWER_PELLET:
                if (!foodEaten[y][x]) {
                    DrawCircle((int)(x * CELL_SIZE + CELL_SIZE / 2.0f),
                        (int)(y * CELL_SIZE + CELL_SIZE / 2.0f),
                        CELL_SIZE / 4, YELLOW);
                }
                break;

            default:
                break;
            }
        }
    }
}

bool Board::IsWall(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return true; // Poza plansz¹ traktujemy jak œcianê
    }
    return GetCellType(x, y) == WALL;
}

bool Board::HasFood(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }

    CellType cell = GetCellType(x, y);
    if ((cell == FOOD || cell == POWER_PELLET) && !foodEaten[y][x]) {
        return true;
    }
    return false;
}

void Board::EatFood(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        foodEaten[y][x] = true;
    }
}

void Board::Reset() {
    InitializeFood();
}

bool Board::IsAllFoodEaten() const {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            CellType cell = GetCellType(x, y);
            if ((cell == FOOD || cell == POWER_PELLET) && !foodEaten[y][x]) {
                return false;
            }
        }
    }
    return true;
}

int Board::GetFoodCount() const {
    int count = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (HasFood(x, y)) {
                count++;
            }
        }
    }
    return count;
}

Board::CellType Board::GetCellType(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return WALL;
    }
    return static_cast<CellType>(layout[y][x]);
}

void Board::InitializeFood() {
    foodEaten.resize(HEIGHT, std::vector<bool>(WIDTH, false));

    // Ustaw jedzenie jako niezjedzone tylko tam gdzie powinno byæ
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            CellType cell = GetCellType(x, y);
            foodEaten[y][x] = !(cell == FOOD || cell == POWER_PELLET);
        }
    }
}

bool Board::IsPowerPellet(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }
    return GetCellType(x, y) == POWER_PELLET;
}