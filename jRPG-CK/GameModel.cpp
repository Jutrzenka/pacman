#include "GameModel.h"
#include "raylib.h"
#include "Constants.h"

GameModel::GameModel() :
    redGhost(Ghost::RED_GHOST),
    greenGhost(Ghost::GREEN_GHOST),
    purpleGhost(Ghost::PURPLE_GHOST),
    currentPoints(0),
    isGameCompleted(false),
    isGhostFrightened(false),
    frightenedTimer(0) {

    std::string highScoreText;
    scoreSystem.GetHighScore();
    topScoreDisplay = scoreSystem.GetHighScore();
}

void GameModel::ProcessCharacterInput(int characterCode, bool isBackspace) {
    if (isBackspace && !playerIdentifier.empty()) {
        playerIdentifier.pop_back();
    }
    else if (characterCode >= 32 && characterCode <= 125 && playerIdentifier.length() < 15) {
        playerIdentifier += (char)characterCode;
    }
}

void GameModel::ProcessDirectionInput(int directionKey) {
    Vector2 desiredDirection = { 0, 0 };

    if (directionKey == KEY_UP) desiredDirection = { 0, -1 };
    else if (directionKey == KEY_DOWN) desiredDirection = { 0, 1 };
    else if (directionKey == KEY_LEFT) desiredDirection = { -1, 0 };
    else if (directionKey == KEY_RIGHT) desiredDirection = { 1, 0 };

    playerCharacter.QueueDirection(desiredDirection, GetTime());
}

void GameModel::AdvanceGameLogic() {
    if (isGhostFrightened) {
        frightenedTimer--;
        if (frightenedTimer <= 0) {
            isGhostFrightened = false;
        }
    }

    playerCharacter.PerformMovement(gameBoard);

    std::deque<Vector2> playerSegments;
    Color playerColor;
    playerCharacter.ProvideVisualData(playerSegments, playerColor);

    if (!playerSegments.empty()) {
        Vector2 nextPos = playerCharacter.CalculateNextPosition();
        Vector2 playerDirection = { nextPos.x - playerSegments[0].x, nextPos.y - playerSegments[0].y };

        Vector2 greenGhostPosition;
        std::deque<Vector2> greenSegments;
        Color greenColor;
        greenGhost.ProvideVisualData(greenSegments, greenColor);
        if (!greenSegments.empty()) {
            greenGhostPosition = greenSegments[0];
        }

        redGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);
        greenGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);
        purpleGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);

        redGhost.PerformMovement(gameBoard);
        greenGhost.PerformMovement(gameBoard);
        purpleGhost.PerformMovement(gameBoard);
    }

    std::deque<Vector2> redSegments, greenSegments, purpleSegments;
    Color redColor, greenColor, purpleColor;
    redGhost.ProvideVisualData(redSegments, redColor);
    greenGhost.ProvideVisualData(greenSegments, greenColor);
    purpleGhost.ProvideVisualData(purpleSegments, purpleColor);

    if (!playerSegments.empty()) {
        Vector2 playerPos = playerSegments[0];

        if (!redSegments.empty()) {
            Vector2 ghostPos = redSegments[0];
            if ((int)playerPos.x == (int)ghostPos.x && (int)playerPos.y == (int)ghostPos.y) {
                if (isGhostFrightened) {
                    redGhost.ResetToPosition({ 20, 5 });
                    currentPoints += 200;
                    isGhostFrightened = false;
                }
                else {
                    CompleteGameWithSave();
                }
            }
        }

        if (!greenSegments.empty()) {
            Vector2 ghostPos = greenSegments[0];
            if ((int)playerPos.x == (int)ghostPos.x && (int)playerPos.y == (int)ghostPos.y) {
                if (isGhostFrightened) {
                    greenGhost.ResetToPosition({ 5, 5 });
                    currentPoints += 200;
                    isGhostFrightened = false;
                }
                else {
                    CompleteGameWithSave();
                }
            }
        }

        if (!purpleSegments.empty()) {
            Vector2 ghostPos = purpleSegments[0];
            if ((int)playerPos.x == (int)ghostPos.x && (int)playerPos.y == (int)ghostPos.y) {
                if (isGhostFrightened) {
                    purpleGhost.ResetToPosition({ 10, 5 });
                    currentPoints += 200;
                    isGhostFrightened = false;
                }
                else {
                    CompleteGameWithSave();
                }
            }
        }
    }

    if (!playerSegments.empty()) {
        Vector2 head = playerSegments[0];
        int headX = (int)head.x;
        int headY = (int)head.y;

        if (gameBoard.CheckFoodPresence(headX, headY)) {
            gameBoard.ConsumeFoodAtPosition(headX, headY);

            if (gameBoard.CheckPowerPelletPresence(headX, headY)) {
                currentPoints += 50;
                isGhostFrightened = true;
                frightenedTimer = 300;
            }
            else {
                currentPoints += 10;
            }

            if (gameBoard.VerifyAllFoodConsumed()) {
                ResetCurrentLevel();
                currentPoints += 1000;
            }
        }
    }
}

void GameModel::CompleteGameWithSave() {
    scoreSystem.SaveScore(playerIdentifier, currentPoints);
    topScoreDisplay = scoreSystem.GetHighScore();
    playerCharacter.InitializeAtPosition({ 6, 9 });
    redGhost.ResetToPosition({ 20, 5 });
    greenGhost.ResetToPosition({ 5, 5 });
    purpleGhost.ResetToPosition({ 10, 5 });
    gameBoard.ResetAllFood();
    currentPoints = 0;
    isGameCompleted = true;
    isGhostFrightened = false;
    frightenedTimer = 0;
}

void GameModel::ResetEntireGame() {
    playerCharacter.InitializeAtPosition({ 6, 9 });
    redGhost.ResetToPosition({ 20, 5 });
    greenGhost.ResetToPosition({ 5, 5 });
    purpleGhost.ResetToPosition({ 10, 5 });
    gameBoard.ResetAllFood();
    currentPoints = 0;
    isGameCompleted = false;
    isGhostFrightened = false;
    frightenedTimer = 0;
}

void GameModel::ResetCurrentLevel() {
    playerCharacter.InitializeAtPosition({ 6, 9 });
    redGhost.ResetToPosition({ 20, 5 });
    greenGhost.ResetToPosition({ 5, 5 });
    purpleGhost.ResetToPosition({ 10, 5 });
    gameBoard.ResetAllFood();
    isGhostFrightened = false;
    frightenedTimer = 0;
}

void GameModel::RetrieveGameStatus(bool& gameOverStatus, int& scoreValue,
    bool& ghostFrightenedStatus, int& foodRemaining) const {
    gameOverStatus = isGameCompleted;
    scoreValue = currentPoints;
    ghostFrightenedStatus = isGhostFrightened;
    foodRemaining = gameBoard.CountRemainingFood();
}

void GameModel::RetrievePlayerInformation(std::string& playerName, std::string& highScoreText) const {
    playerName = playerIdentifier;
    highScoreText = topScoreDisplay;
}

void GameModel::ProvideRenderingData(std::deque<Vector2>& playerSegments,
    Color& playerVisualColor,
    Vector2& redGhostLocation,
    Vector2& greenGhostLocation,
    Vector2& purpleGhostLocation,
    std::vector<std::vector<int>>& boardGrid) const {

    playerCharacter.ProvideVisualData(playerSegments, playerVisualColor);

    std::deque<Vector2> redSegments, greenSegments, purpleSegments;
    Color redColor, greenColor, purpleColor;

    redGhost.ProvideVisualData(redSegments, redColor);
    greenGhost.ProvideVisualData(greenSegments, greenColor);
    purpleGhost.ProvideVisualData(purpleSegments, purpleColor);

    if (!redSegments.empty()) {
        redGhostLocation = redSegments[0];
    }
    else {
        redGhostLocation = { 0, 0 };
    }

    if (!greenSegments.empty()) {
        greenGhostLocation = greenSegments[0];
    }
    else {
        greenGhostLocation = { 0, 0 };
    }

    if (!purpleSegments.empty()) {
        purpleGhostLocation = purpleSegments[0];
    }
    else {
        purpleGhostLocation = { 0, 0 };
    }

    gameBoard.ProvideGridData(boardGrid);
}