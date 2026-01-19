#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Snake.h"
#include "Ghost.h"
#include "ScoreManager.h"
#include "Board.h"
#include "Enums.h"
#include <string>

class GameModel {
private:
    Snake playerCharacter;
    RedGhost redGhost;
    GreenGhost greenGhost;
    PurpleGhost purpleGhost;
    ScoreManager scoreSystem;
    Board gameBoard;
    std::string playerIdentifier;
    std::string topScoreDisplay;
    int currentPoints;
    bool isGameCompleted;
    bool isGhostFrightened;
    double frightenedTimer; // Changed to double for better time tracking
    Difficulty currentDifficulty;

    // Ghost dead timers
    double redDeadTimer;
    double greenDeadTimer;
    double purpleDeadTimer;

    Vector2 FindSafeSpawnPosition() const;

public:
    GameModel();

    void ProcessCharacterInput(int characterCode, bool isBackspace);
    void ProcessDirectionInput(int directionKey);
    void SetDifficulty(Difficulty difficulty);
    Difficulty GetDifficulty() const;
    void SetNextDifficulty();
    void SetPreviousDifficulty();

    void AdvanceGameLogic();
    void CompleteGameWithSave();
    void ResetEntireGame();
    void ResetCurrentLevel();

    void RetrieveGameStatus(bool& gameOverStatus, int& scoreValue,
        bool& ghostFrightenedStatus, int& foodRemaining) const;
    void RetrievePlayerInformation(std::string& playerName, std::string& highScoreText) const;

    void ProvideRenderingData(std::deque<Vector2>& playerSegments,
        Color& playerVisualColor,
        Vector2& redGhostLocation,
        Vector2& greenGhostLocation,
        Vector2& purpleGhostLocation,
        std::vector<std::vector<int>>& boardGrid) const;
};

#endif