#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "raylib.h"
#include "GameModel.h"

class GameView {
public:
    void DrawMenu(const GameModel& model);
    void DrawGameplay(const GameModel& model);
    void DrawPause();

private:
    void DrawSnake(const Snake& snake);
    void DrawGhost(const Ghost& ghost, bool isScared);
    void DrawBoard(const Board& board);
};
#endif