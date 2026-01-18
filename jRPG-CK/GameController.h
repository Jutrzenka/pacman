#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameModel.h"
#include "GameView.h"

enum GameState { MENU, GAMEPLAY, PAUSED };

class GameController {
private:
    GameModel model;
    GameView view;
    GameState state;
    double lastUpdate;

    bool ShouldUpdate(double interval);

public:
    GameController();
    void ProcessInput();
    void Update();
    void Render();
    bool ShouldClose() const;
};
#endif