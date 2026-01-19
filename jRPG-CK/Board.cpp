#include "Board.h"
#include <algorithm>

// Layout for Easy Difficulty - More open space
const int Board::layoutEasy[GRID_HEIGHT][GRID_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,3,1,0,0,1,2,1,0,0,0,1,2,1,0,0,0,1,2,1,0,0,1,3,1}, // Similar to original
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,0,0,0,0,0}, 
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,0,0,0,1,0,1,1,2,1,1,1,1,1,1},
    {0,2,2,2,2,2,2,2,2,0,1,0,0,0,1,0,2,2,2,2,2,2,2,2,0},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,3,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,3,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Layout for Medium Difficulty 
const int Board::layoutMedium[GRID_HEIGHT][GRID_WIDTH] = {
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

// Layout for Hard Difficulty
const int Board::layoutHard[GRID_HEIGHT][GRID_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2,1,1},
    {1,3,1,0,0,1,2,1,0,0,0,1,2,1,0,0,0,1,2,1,0,0,1,3,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,1,1,1,0,1,1,1,1,1,2,1,0,0,0,0,0}, 
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,0,0,0,1,0,1,1,2,1,1,1,1,1,1},
    {0,2,2,2,2,2,2,2,2,0,1,0,0,0,1,0,2,2,2,2,2,2,2,2,0},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,3,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,3,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

Board::Board() {
    currentLayout = layoutMedium; // Default
    InitializeFoodState();
}

void Board::SetDifficulty(Difficulty difficulty) {
    switch (difficulty) {
        case EASY: currentLayout = layoutEasy; break;
        case MEDIUM: currentLayout = layoutMedium; break;
        case HARD: currentLayout = layoutHard; break;
    }
    InitializeFoodState();
}

bool Board::CheckWallPresence(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return false; // Poza plansz¹ traktujemy jako puste
    }
    return DetermineCellType(x, y) == CELL_WALL;
}

bool Board::CheckFoodPresence(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return false;
    }

    GridCellType cell = DetermineCellType(x, y);
    return (cell == CELL_FOOD || cell == CELL_POWER_PELLET) &&
        !foodConsumptionState[y][x];
}

bool Board::CheckPowerPelletPresence(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return false;
    }
    return DetermineCellType(x, y) == CELL_POWER_PELLET &&
        !foodConsumptionState[y][x];
}

void Board::ConsumeFoodAtPosition(int x, int y) {
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
        foodConsumptionState[y][x] = true;
    }
}

void Board::ResetAllFood() {
    InitializeFoodState();
}

bool Board::VerifyAllFoodConsumed() const {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            GridCellType cell = DetermineCellType(x, y);
            if ((cell == CELL_FOOD || cell == CELL_POWER_PELLET) &&
                !foodConsumptionState[y][x]) {
                return false;
            }
        }
    }
    return true;
}

int Board::CountRemainingFood() const {
    int count = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (CheckFoodPresence(x, y)) {
                count++;
            }
        }
    }
    return count;
}

void Board::ProvideGridData(std::vector<std::vector<int>>& gridDataOutput) const {
    gridDataOutput.resize(GRID_HEIGHT, std::vector<int>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            GridCellType cell = DetermineCellType(x, y);

            if (cell == CELL_WALL) {
                gridDataOutput[y][x] = 1; // Œciana
            }
            else if ((cell == CELL_FOOD || cell == CELL_POWER_PELLET) &&
                !foodConsumptionState[y][x]) {
                gridDataOutput[y][x] = (cell == CELL_POWER_PELLET) ? 3 : 2; // Jedzenie/Power pellet
            }
            else {
                gridDataOutput[y][x] = 0; // Puste
            }
        }
    }
}

Board::GridCellType Board::DetermineCellType(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return CELL_EMPTY;
    }
    return static_cast<GridCellType>(currentLayout[y][x]);
}

void Board::InitializeFoodState() {
    foodConsumptionState.resize(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            GridCellType cell = DetermineCellType(x, y);
            foodConsumptionState[y][x] = !(cell == CELL_FOOD || cell == CELL_POWER_PELLET);
        }
    }
}