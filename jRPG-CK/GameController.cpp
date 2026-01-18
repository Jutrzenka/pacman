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
            model.HandleNameInput(key, false);
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            model.HandleNameInput(0, true);
        }

        if (IsKeyPressed(KEY_ENTER) && !model.GetPlayerName().empty()) {
            state = GAMEPLAY;
        }
    }
    else if (state == GAMEPLAY) {
        if (IsKeyPressed(KEY_UP)) model.HandlePlayerInput(KEY_UP);
        if (IsKeyPressed(KEY_DOWN)) model.HandlePlayerInput(KEY_DOWN);
        if (IsKeyPressed(KEY_LEFT)) model.HandlePlayerInput(KEY_LEFT);
        if (IsKeyPressed(KEY_RIGHT)) model.HandlePlayerInput(KEY_RIGHT);
        if (IsKeyPressed(KEY_ESCAPE)) state = PAUSED;
    }
    else if (state == PAUSED) {
        if (IsKeyPressed(KEY_ESCAPE)) state = GAMEPLAY;
        if (IsKeyPressed(KEY_Q)) {
            model.GameOver();
            state = MENU;
        }
    }
}

void GameController::Update() {
    if (state == GAMEPLAY) {
        if (ShouldUpdate(0.12)) {
            model.Update();
            if (model.IsGameOver()) {
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