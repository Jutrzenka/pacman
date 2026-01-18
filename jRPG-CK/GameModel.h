#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Snake.h"
#include "Ghost.h"
#include "ScoreManager.h"
#include "Board.h"
#include <string>

class GameModel {
private:
    Snake playerCharacter;
    Ghost enemyGhost;
    ScoreManager scoreSystem;
    Board gameBoard;
    std::string playerIdentifier;
    std::string topScoreDisplay;
    int currentPoints;
    bool isGameCompleted;
    bool isGhostFrightened;
    int frightenedTimer;

public:
    GameModel();

    // Przetwarzanie wejœcia
    void ProcessCharacterInput(int characterCode, bool isBackspace);
    void ProcessDirectionInput(int directionKey);

    // Logika gry
    void AdvanceGameLogic();
    void CompleteGameWithSave();
    void ResetEntireGame();
    void ResetCurrentLevel();

    // Status gry (zwracany przez parametry)
    void RetrieveGameStatus(bool& gameOverStatus, int& scoreValue,
        bool& ghostFrightenedStatus, int& foodRemaining) const;
    void RetrievePlayerInformation(std::string& playerName, std::string& highScoreText) const;

    // Dane do renderowania
    void ProvideRenderingData(std::deque<Vector2>& playerSegments,
        Color& playerVisualColor,
        Vector2& ghostLocation,
        Color& ghostVisualColor,
        std::vector<std::vector<int>>& boardGrid) const;
};

#endif