#include "GameModel.h"
#include "raylib.h"
#include "Constants.h"

GameModel::GameModel() : currentPoints(0), isGameCompleted(false),
isGhostFrightened(false), frightenedTimer(0) {
    std::string highScoreText;
    scoreSystem.GetHighScore(); // Inicjalizacja
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
    // SprawdŸ czy gracz mo¿e iœæ w wybranym kierunku
    Vector2 desiredDirection = { 0, 0 };

    if (directionKey == KEY_UP) desiredDirection = { 0, -1 };
    else if (directionKey == KEY_DOWN) desiredDirection = { 0, 1 };
    else if (directionKey == KEY_LEFT) desiredDirection = { -1, 0 };
    else if (directionKey == KEY_RIGHT) desiredDirection = { 1, 0 };

    // Zamiast bezpoœrednio zmieniaæ kierunek, zapisz go w buforze
    playerCharacter.QueueDirection(desiredDirection, GetTime());
}

void GameModel::AdvanceGameLogic() {
    // Aktualizacja timera przestraszenia
    if (isGhostFrightened) {
        frightenedTimer--;
        if (frightenedTimer <= 0) {
            isGhostFrightened = false;
        }
    }

    // Ruch gracza
    playerCharacter.PerformMovement(gameBoard);

    // Ruch ducha
    std::deque<Vector2> playerSegments;
    Color playerColor;
    playerCharacter.ProvideVisualData(playerSegments, playerColor);

    if (!playerSegments.empty()) {
        enemyGhost.UpdateArtificialIntelligence(gameBoard, playerSegments[0], isGhostFrightened);
        enemyGhost.PerformMovement(gameBoard);  // To wywo³uje ruch ducha!
    }

    // Kolizja z duchem
    std::deque<Vector2> ghostSegments;
    Color ghostColor;
    enemyGhost.ProvideVisualData(ghostSegments, ghostColor);

    if (!playerSegments.empty() && !ghostSegments.empty()) {
        Vector2 playerPos = playerSegments[0];
        Vector2 ghostPos = ghostSegments[0];

        if ((int)playerPos.x == (int)ghostPos.x && (int)playerPos.y == (int)ghostPos.y) {
            if (isGhostFrightened) {
                // Zjedzenie ducha w trybie przestraszenia
                enemyGhost.ResetToPosition({ 20, 5 });
                currentPoints += 200;
                isGhostFrightened = false;
            }
            else {
                CompleteGameWithSave();
            }
        }
    }

    // Zbieranie jedzenia
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
    enemyGhost.ResetToPosition({ 20, 5 });
    gameBoard.ResetAllFood();
    currentPoints = 0;
    isGameCompleted = true;
    isGhostFrightened = false;
    frightenedTimer = 0;
}

void GameModel::ResetEntireGame() {
    playerCharacter.InitializeAtPosition({ 6, 9 });
    enemyGhost.ResetToPosition({ 20, 5 });
    gameBoard.ResetAllFood();
    currentPoints = 0;
    isGameCompleted = false;
    isGhostFrightened = false;
    frightenedTimer = 0;
}

void GameModel::ResetCurrentLevel() {
    playerCharacter.InitializeAtPosition({ 6, 9 });
    enemyGhost.ResetToPosition({ 20, 5 });
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
    Vector2& ghostLocation,
    Color& ghostVisualColor,
    std::vector<std::vector<int>>& boardGrid) const {
    playerCharacter.ProvideVisualData(playerSegments, playerVisualColor);

    std::deque<Vector2> ghostSegments;
    enemyGhost.ProvideVisualData(ghostSegments, ghostVisualColor);

    if (!ghostSegments.empty()) {
        ghostLocation = ghostSegments[0];
    }
    else {
        ghostLocation = { 0, 0 };
    }

    gameBoard.ProvideGridData(boardGrid);
}