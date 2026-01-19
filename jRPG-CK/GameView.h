#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "raylib.h"
#include "GameModel.h"
#include <vector>
#include <deque>

class GameView {
public:
    void DrawMenu(const GameModel& model);
    void DrawGameplay(const GameModel& model);
    void DrawPause();

private:
    void DrawPlayerCharacter(const std::deque<Vector2>& segments, Color color);
    void DrawEnemyGhost(Vector2 position, Color color, bool isFrightened);
    void DrawBoardGrid(const std::vector<std::vector<int>>& gridData);
};

#endif