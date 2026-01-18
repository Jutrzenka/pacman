#include "GameController.h"
#include "raylib.h"

GameController::GameController() : state(MENU), lastUpdate(0) {}

bool GameController::ShouldUpdate(double interval) {
    double now = GetTime();
    if (now - lastUpdate >= interval) {
        lastUpdate = now;
        return true;
    }
    return false;
}

void GameController::ProcessInput() {
    if (state == MENU) {
        int key = GetCharPressed();
        while (key > 0) {
            model.ProcessCharacterInput(key, false);
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            model.ProcessCharacterInput(0, true);
        }

        std::string playerName, highScore;
        model.RetrievePlayerInformation(playerName, highScore);

        if (IsKeyPressed(KEY_ENTER) && !playerName.empty()) {
            model.ResetEntireGame();  // Reset przed rozpoczêciem
            state = GAMEPLAY;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            state = GAMEPLAY;  // Dodaj mo¿liwoœæ pominiêcia menu
        }
    }
    else if (state == GAMEPLAY) {
        if (IsKeyPressed(KEY_UP)) model.ProcessDirectionInput(KEY_UP);
        if (IsKeyPressed(KEY_DOWN)) model.ProcessDirectionInput(KEY_DOWN);
        if (IsKeyPressed(KEY_LEFT)) model.ProcessDirectionInput(KEY_LEFT);
        if (IsKeyPressed(KEY_RIGHT)) model.ProcessDirectionInput(KEY_RIGHT);
        if (IsKeyPressed(KEY_ESCAPE)) state = PAUSED;
    }
    else if (state == PAUSED) {
        if (IsKeyPressed(KEY_ESCAPE)) state = GAMEPLAY;
        if (IsKeyPressed(KEY_Q)) {
            model.CompleteGameWithSave();
            state = MENU;
        }
    }
}

void GameController::Update() {
    if (state == GAMEPLAY) {
        if (ShouldUpdate(0.12)) {
            model.AdvanceGameLogic();

            bool gameOverStatus;
            int scoreValue;
            bool ghostFrightenedStatus;
            int foodRemaining;
            model.RetrieveGameStatus(gameOverStatus, scoreValue, ghostFrightenedStatus, foodRemaining);

            if (gameOverStatus) {
                state = MENU;
            }
        }
    }
}

void GameController::Render() {
    if (state == MENU) {
        view.DrawMenu(model);
    }
    else if (state == GAMEPLAY) {
        view.DrawGameplay(model);
    }
    else if (state == PAUSED) {
        view.DrawGameplay(model);
        view.DrawPause();
    }
}

bool GameController::ShouldClose() const {
    return WindowShouldClose();
}