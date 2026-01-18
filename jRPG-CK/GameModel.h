#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Snake.h"
#include "Ghost.h"
#include "Food.h"
#include "ScoreManager.h"
#include <string>

class GameModel {
private:
    Snake player;
    Ghost ghost;
    Food food;
    ScoreManager scoreManager;
    std::string playerName;
    std::string currentHighScoreStr;
    int score;
    bool gameOver;

public:
    GameModel();

    // Input handling
    void HandleNameInput(int key, bool backspace);
    void HandlePlayerInput(int key);

    // Game logic
    void Update();
    void GameOver();

    // State access methods
    std::string GetPlayerName() const;
    std::string GetHighScoreString() const;
    int GetScore() const;
    bool IsGameOver() const;

    // Object access (const references for rendering)
    const Snake& GetPlayer() const;
    const Ghost& GetGhost() const;
    const Food& GetFood() const;
};
#endif