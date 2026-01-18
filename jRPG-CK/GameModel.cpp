#include "GameModel.h"
#include "raylib.h"
#include "Constants.h"

GameModel::GameModel() : score(0), gameOver(false), scaredMode(false), scaredTimer(0) {
    currentHighScoreStr = scoreManager.GetHighScore();
}

void GameModel::HandleNameInput(int key, bool backspace) {
    if (backspace && !playerName.empty()) {
        playerName.pop_back();
    }
    else if (key >= 32 && key <= 125 && playerName.length() < 15) {
        playerName += (char)key;
    }
}

void GameModel::HandlePlayerInput(int key) {
    Vector2 currentPos = player.GetHeadPosition();

    if (key == KEY_UP && !board.IsWall((int)currentPos.x, (int)currentPos.y - 1))
        player.ChangeDirection({ 0, -1 });
    if (key == KEY_DOWN && !board.IsWall((int)currentPos.x, (int)currentPos.y + 1))
        player.ChangeDirection({ 0, 1 });
    if (key == KEY_LEFT && !board.IsWall((int)currentPos.x - 1, (int)currentPos.y))
        player.ChangeDirection({ -1, 0 });
    if (key == KEY_RIGHT && !board.IsWall((int)currentPos.x + 1, (int)currentPos.y))
        player.ChangeDirection({ 1, 0 });
}

void GameModel::Update() {
    // Aktualizacja timera przestraszenia
    if (scaredMode) {
        scaredTimer--;
        if (scaredTimer <= 0) {
            scaredMode = false;
        }
    }

    // Ruch gracza - sprawdzenie kolizji ze œcian¹
    Vector2 nextPos = player.GetHeadPosition();
    Vector2 dir = player.GetDirection();
    nextPos.x += dir.x;
    nextPos.y += dir.y;

    // Zawijanie przez tunele
    if (nextPos.x < 0) nextPos.x = CELL_COUNT - 1;
    else if (nextPos.x >= CELL_COUNT) nextPos.x = 0;

    if (!board.IsWall((int)nextPos.x, (int)nextPos.y)) {
        player.Update();
    }

    // Ruch ducha
    ghost.UpdateAI(board, player.GetHeadPosition(), scaredMode);
    Vector2 ghostNextPos = ghost.GetHeadPosition();
    Vector2 ghostDir = ghost.GetDirection();
    ghostNextPos.x += ghostDir.x;
    ghostNextPos.y += ghostDir.y;

    // Zawijanie przez tunele dla ducha
    if (ghostNextPos.x < 0) ghostNextPos.x = CELL_COUNT - 1;
    else if (ghostNextPos.x >= CELL_COUNT) ghostNextPos.x = 0;

    if (!board.IsWall((int)ghostNextPos.x, (int)ghostNextPos.y)) {
        ghost.Update();
    }

    // Kolizja z duchem
    Vector2 playerPos = player.GetHeadPosition();
    Vector2 ghostPos = ghost.GetHeadPosition();
    if ((int)playerPos.x == (int)ghostPos.x && (int)playerPos.y == (int)ghostPos.y) {
        if (scaredMode) {
            // Zjedzenie ducha w trybie przestraszenia
            ghost.Reset({ 20, 5 });
            score += 200;
            scaredMode = false; // Reset trybu po zjedzeniu ducha
        }
        else {
            GameOver();
        }
    }

    // Zbieranie jedzenia
    Vector2 head = player.GetHeadPosition();
    int headX = (int)head.x;
    int headY = (int)head.y;
    if (board.HasFood(headX, headY)) {
        board.EatFood(headX, headY);

        // Sprawdzenie czy to power pellet
        if (board.IsPowerPellet(headX, headY)) {
            score += 50;
            scaredMode = true;
            scaredTimer = 300;  // 5 sekund przy 60 FPS
        }
        else {
            score += 10;
        }

        // Sprawdzenie czy wszystkie jedzenie zebrane
        if (board.IsAllFoodEaten()) {
            ResetLevel();
            score += 1000;  // Bonus za ukoñczenie poziomu
        }
    }
}

void GameModel::GameOver() {
    scoreManager.SaveScore(playerName, score);
    currentHighScoreStr = scoreManager.GetHighScore();
    player.Reset({ 6, 9 });
    ghost.Reset({ 20, 5 });
    board.Reset();
    score = 0;
    gameOver = true;
    scaredMode = false;
    scaredTimer = 0;
}

void GameModel::ResetLevel() {
    player.Reset({ 6, 9 });
    ghost.Reset({ 20, 5 });
    board.Reset();
    scaredMode = false;
    scaredTimer = 0;
}

std::string GameModel::GetPlayerName() const {
    return playerName;
}

std::string GameModel::GetHighScoreString() const {
    return currentHighScoreStr;
}

int GameModel::GetScore() const {
    return score;
}

bool GameModel::IsGameOver() const {
    return gameOver;
}

bool GameModel::IsScaredMode() const {
    return scaredMode;
}

const Snake& GameModel::GetPlayer() const {
    return player;
}

const Ghost& GameModel::GetGhost() const {
    return ghost;
}

const Board& GameModel::GetBoard() const {
    return board;
}