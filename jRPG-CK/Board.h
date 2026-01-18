#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include "Constants.h"
#include <vector>

class Board {
public:
    Board();

    // Rysowanie planszy
    void Draw() const;

    // Sprawdzenie czy pozycja jest œcian¹
    bool IsWall(int x, int y) const;

    // Sprawdzenie czy pozycja zawiera jedzenie
    bool HasFood(int x, int y) const;

    // Sprawdzenie czy to power pellet
    bool IsPowerPellet(int x, int y) const;

    // Zjedzenie jedzenia
    void EatFood(int x, int y);

    // Reset planszy
    void Reset();

    // Sprawdzenie czy wszystkie jedzenie zosta³o zebrane
    bool IsAllFoodEaten() const;

    // Pobierz iloœæ jedzenia
    int GetFoodCount() const;

private:
    // Wymiary planszy
    static const int WIDTH = 25;
    static const int HEIGHT = 25;

    // Typy pól
    enum CellType {
        EMPTY = 0,
        WALL = 1,
        FOOD = 2,
        POWER_PELLET = 3
    };

    // Plansza - sta³y uk³ad
    static const int layout[HEIGHT][WIDTH];

    // Dynamiczny stan jedzenia
    std::vector<std::vector<bool>> foodEaten;

    // Pobierz typ komórki
    CellType GetCellType(int x, int y) const;

    // Inicjalizacja stanu jedzenia
    void InitializeFood();
};

#endif