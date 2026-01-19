#include "GameModel.h"
#include "raylib.h"
#include "Constants.h"

GameModel::GameModel() :
    // Derived ghosts are initialized by their default constructors now
    currentPoints(0),
    isGameCompleted(false),
    isGhostFrightened(false),
    frightenedTimer(0),
    currentDifficulty(MEDIUM), 
    redDeadTimer(0), greenDeadTimer(0), purpleDeadTimer(0) { // Default difficulty

    std::string highScoreText;
    scoreSystem.GetHighScore(); // This load call might be redundant but harmless
    topScoreDisplay = scoreSystem.GetHighScore();
    
    // Ensure all ghosts are alive
    redGhost.SetDead(false);
    greenGhost.SetDead(false);
    purpleGhost.SetDead(false);
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

void GameModel::SetDifficulty(Difficulty difficulty) {
    currentDifficulty = difficulty;
    gameBoard.SetDifficulty(difficulty);
    ResetEntireGame();
}

void GameModel::AdvanceGameLogic() {
    // 1. Update Frightened Timer
    if (isGhostFrightened) {
        frightenedTimer--;
        if (frightenedTimer <= 0) {
            isGhostFrightened = false;
        }
    }

    // 2. Update Ghost Dead Timers
    if (redGhost.IsDead()) {
        redDeadTimer -= 0.12; // Approximation of delta time
        if (redDeadTimer <= 0) {
            Vector2 spawn = FindSafeSpawnPosition();
            redGhost.ResetToPosition(spawn);
        }
    }

    if (greenGhost.IsDead()) {
        greenDeadTimer -= 0.12;
        if (greenDeadTimer <= 0) {
            Vector2 spawn = FindSafeSpawnPosition();
            greenGhost.ResetToPosition(spawn);
        }
    }

    if (purpleGhost.IsDead()) {
        purpleDeadTimer -= 0.12;
        if (purpleDeadTimer <= 0) {
            Vector2 spawn = FindSafeSpawnPosition();
            purpleGhost.ResetToPosition(spawn);
        }
    }

    // Update ghost colors based on frightened state (only if alive)
    if (!redGhost.IsDead()) redGhost.ApplyColor(isGhostFrightened ? BLUE : RED);
    if (!greenGhost.IsDead()) greenGhost.ApplyColor(isGhostFrightened ? BLUE : GREEN);
    if (!purpleGhost.IsDead()) purpleGhost.ApplyColor(isGhostFrightened ? BLUE : PURPLE);

    playerCharacter.PerformMovement(gameBoard);

    std::deque<Vector2> playerSegments;
    Color playerColor;
    playerCharacter.ProvideVisualData(playerSegments, playerColor);

    if (!playerSegments.empty()) {
        Vector2 nextPos = playerCharacter.CalculateNextPosition();
        Vector2 playerDirection = { nextPos.x - playerSegments[0].x, nextPos.y - playerSegments[0].y };

        Vector2 greenGhostPosition = { 0, 0 }; // Initialize default
        std::deque<Vector2> greenSegments;
        Color greenColor;
        greenGhost.ProvideVisualData(greenSegments, greenColor);
        if (!greenSegments.empty()) {
            greenGhostPosition = greenSegments[0];
        }

        // Only update AI if ghost is ALIVE
        if (!redGhost.IsDead()) redGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);
        if (!greenGhost.IsDead()) greenGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);
        if (!purpleGhost.IsDead()) purpleGhost.UpdateAI(gameBoard, playerSegments[0], playerDirection, greenGhostPosition, isGhostFrightened);

        redGhost.PerformMovement(gameBoard);
        greenGhost.PerformMovement(gameBoard);
        purpleGhost.PerformMovement(gameBoard);
    }

    // Pobierz segmenty duchów
    std::deque<Vector2> redSegments, greenSegments, purpleSegments;
    Color redColor, greenColor, purpleColor;
    redGhost.ProvideVisualData(redSegments, redColor);
    greenGhost.ProvideVisualData(greenSegments, greenColor);
    purpleGhost.ProvideVisualData(purpleSegments, purpleColor);

    // SprawdŸ kolizjê dla CA£EGO cia³a wê¿a z duchami
    for (const auto& playerSegment : playerSegments) {
        // Czerwony duch
        if (!redSegments.empty()) {
            for (const auto& ghostSegment : redSegments) {
                if ((int)playerSegment.x == (int)ghostSegment.x && (int)playerSegment.y == (int)ghostSegment.y) {
                    if (isGhostFrightened) {
                        redGhost.SetDead(true);
                        redDeadTimer = 5.0; // 5 seconds
                        currentPoints += 200;
                        playerCharacter.ScheduleGrowth();
                    }
                    else {
                        CompleteGameWithSave();
                        return; // Koñczymy sprawdzanie, gra ju¿ siê zakoñczy³a
                    }
                }
            }
        }

        // Zielony duch
        if (!greenSegments.empty()) {
            for (const auto& ghostSegment : greenSegments) {
                if ((int)playerSegment.x == (int)ghostSegment.x && (int)playerSegment.y == (int)ghostSegment.y) {
                    if (isGhostFrightened) {
                        greenGhost.SetDead(true);
                        greenDeadTimer = 5.0; // 5 seconds
                        currentPoints += 200;
                        playerCharacter.ScheduleGrowth();
                    }
                    else {
                        CompleteGameWithSave();
                        return;
                    }
                }
            }
        }

        // Fioletowy duch
        if (!purpleSegments.empty()) {
            for (const auto& ghostSegment : purpleSegments) {
                if ((int)playerSegment.x == (int)ghostSegment.x && (int)playerSegment.y == (int)ghostSegment.y) {
                    if (isGhostFrightened) {
                        purpleGhost.SetDead(true);
                        purpleDeadTimer = 5.0; // 5 seconds
                        currentPoints += 200;
                        playerCharacter.ScheduleGrowth();
                    }
                    else {
                        CompleteGameWithSave();
                        return;
                    }
                }
            }
        }
    }

    if (!playerSegments.empty()) {
        Vector2 head = playerSegments[0];
        int headX = (int)head.x;
        int headY = (int)head.y;

        // Check types BEFORE consumption
        bool isPowerPellet = gameBoard.CheckPowerPelletPresence(headX, headY);
        bool isFood = gameBoard.CheckFoodPresence(headX, headY);

        if (isFood) {
            gameBoard.ConsumeFoodAtPosition(headX, headY);

            if (isPowerPellet) {
                currentPoints += 50;
                isGhostFrightened = true;
                frightenedTimer = 42; // ~5 seconds at 0.12s update interval
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

Vector2 GameModel::FindSafeSpawnPosition() const {
    int maxAttempts = 100;
    Vector2 bestPos = { 10, 10 }; // Default fallback

    Vector2 playerHead = { -100, -100 };
    std::deque<Vector2> playerBody;
    Color tempColor;
    // We cannot change const, so we rely on what we have.
    // playerCharacter is member, we can access it.
    // However, the method is marked const, so we use const method of Snake.
    // Snake::ProvideVisualData works but copies.
    playerCharacter.ProvideVisualData(playerBody, tempColor);
    
    if(!playerBody.empty()) {
        playerHead = playerBody[0];
    }

    for (int i = 0; i < maxAttempts; i++) {
        int x = GetRandomValue(1, CELL_COUNT - 2);
        int y = GetRandomValue(1, CELL_COUNT - 2);

        // 1. Is Wall?
        if (gameBoard.CheckWallPresence(x, y)) continue;

        // 2. Is on Snake?
        bool onSnake = false;
        for (const auto& segment : playerBody) {
            if ((int)segment.x == x && (int)segment.y == y) {
                onSnake = true;
                break;
            }
        }
        if (onSnake) continue;

        // 3. Distance from Head (if exists)
        if (!playerBody.empty()) {
            float dist = std::abs(x - playerHead.x) + std::abs(y - playerHead.y);
            if (dist < 5) continue; // Too close
        }

        return { (float)x, (float)y };
    }

    return bestPos;
}

void GameModel::CompleteGameWithSave() {
    scoreSystem.SaveScore(playerIdentifier, currentPoints);
    topScoreDisplay = scoreSystem.GetHighScore();
    ResetEntireGame(); 
    // ^ Reset logic consolidated.
    // Previously reset was manual here.
    // ResetEntireGame resets internal state + ghosts.
    
    isGameCompleted = true; // This will trigger MENU state in Controller
    
    // Note: ResetEntireGame sets isGameCompleted = false. 
    // We want it true so controller sees it.
    // So we reset first to clear board, then set flag.
    isGameCompleted = true;
}

void GameModel::ResetEntireGame() {
    playerCharacter.InitializeAtPosition({ 6, 9 });
    redGhost.ResetToPosition({ 20, 5 });
    greenGhost.ResetToPosition({ 5, 5 });
    purpleGhost.ResetToPosition({ 10, 5 });
    
    // Reset Dead State
    redGhost.SetDead(false);
    greenGhost.SetDead(false);
    purpleGhost.SetDead(false);
    redDeadTimer = 0;
    greenDeadTimer = 0;
    purpleDeadTimer = 0;

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
    
    // Reset Dead State
    redGhost.SetDead(false);
    greenGhost.SetDead(false);
    purpleGhost.SetDead(false);
    redDeadTimer = 0;
    greenDeadTimer = 0;
    purpleDeadTimer = 0;

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
        redGhostLocation = { -100, -100 };
    }

    if (!greenSegments.empty()) {
        greenGhostLocation = greenSegments[0];
    }
    else {
        greenGhostLocation = { -100, -100 };
    }

    if (!purpleSegments.empty()) {
        purpleGhostLocation = purpleSegments[0];
    }
    else {
        purpleGhostLocation = { -100, -100 };
    }

    gameBoard.ProvideGridData(boardGrid);
}