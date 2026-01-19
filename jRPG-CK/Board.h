#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Enums.h"

class Board {
public:
    Board();

    void SetDifficulty(Difficulty difficulty);

    // Logika gry - NIE rysowanie!
    bool CheckWallPresence(int x, int y) const;
    bool CheckFoodPresence(int x, int y) const;
    bool CheckPowerPelletPresence(int x, int y) const;
    void ConsumeFoodAtPosition(int x, int y);
    void ResetAllFood();
    bool VerifyAllFoodConsumed() const;
    int CountRemainingFood() const;

    // Dostarczanie danych do renderowania
    void ProvideGridData(std::vector<std::vector<int>>& gridDataOutput) const;

private:
    static const int GRID_WIDTH = 25;
    static const int GRID_HEIGHT = 25;

    enum GridCellType {
        CELL_EMPTY = 0,
        CELL_WALL = 1,
        CELL_FOOD = 2,
        CELL_POWER_PELLET = 3
    };

    const int (*currentLayout)[GRID_WIDTH];
    
    // Layouts for different difficulties
    static const int layoutEasy[GRID_HEIGHT][GRID_WIDTH];
    static const int layoutMedium[GRID_HEIGHT][GRID_WIDTH];
    static const int layoutHard[GRID_HEIGHT][GRID_WIDTH];

    std::vector<std::vector<bool>> foodConsumptionState;

    GridCellType DetermineCellType(int x, int y) const;
    void InitializeFoodState();
};

#endif