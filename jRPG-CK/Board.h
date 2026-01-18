#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
public:
    Board();

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
    static const int GRID_WIDTH = 25;  // Zmienione z GRID_WIDTH
    static const int GRID_HEIGHT = 25; // Zmienione z GRID_HEIGHT

    enum GridCellType {
        CELL_EMPTY = 0,
        CELL_WALL = 1,
        CELL_FOOD = 2,
        CELL_POWER_PELLET = 3
    };

    static const int cellLayout[GRID_HEIGHT][GRID_WIDTH]; // U¿yj nowych nazw
    std::vector<std::vector<bool>> foodConsumptionState;

    GridCellType DetermineCellType(int x, int y) const;
    void InitializeFoodState();
};

#endif