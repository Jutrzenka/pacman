#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Snake.h"
#include "Ghost.h"
#include "ScoreManager.h"
#include "Board.h"
#include <string>

class GameModel {
private:
    Snake player;
    Ghost ghost;
    ScoreManager scoreManager;
    Board board;
    std::string playerName;
    std::string currentHighScoreStr;
    int score;
    bool gameOver;
    bool scaredMode;
    int scaredTimer;

public:
    GameModel();

    // Input handling
    void HandleNameInput(int key, bool backspace);
    void HandlePlayerInput(int key);

    // Game logic
    void Update();
    void GameOver();
    void ResetLevel();

    // State access methods
    std::string GetPlayerName() const;
    std::string GetHighScoreString() const;
    int GetScore() const;
    bool IsGameOver() const;
    bool IsScaredMode() const;

    // Object access
    const Snake& GetPlayer() const;
    const Ghost& GetGhost() const;
    const Board& GetBoard() const;
};
#endif