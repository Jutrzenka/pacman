#include "GameModel.h"
#include "raylib.h"
#include "Constants.h"

GameModel::GameModel() : score(0), gameOver(false) {
    food.Respawn();
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
    if (key == KEY_UP) player.ChangeDirection({ 0, -1 });
    if (key == KEY_DOWN) player.ChangeDirection({ 0, 1 });
    if (key == KEY_LEFT) player.ChangeDirection({ -1, 0 });
    if (key == KEY_RIGHT) player.ChangeDirection({ 1, 0 });
}

void GameModel::Update() {
    if (GetRandomValue(0, 100) < 10) {
        ghost.MoveRandom();
    }

    Vector2 nextP = player.GetHeadPosition();
    Vector2 dir = { 0,0 }; // Dummy direction for collision check

    // Collision with ghost
    if (nextP.x == ghost.GetHeadPosition().x && nextP.y == ghost.GetHeadPosition().y) {
        GameOver();
        gameOver = true;
        return;
    }

    // Food collection
    if (nextP.x == food.GetPosition().x && nextP.y == food.GetPosition().y) {
        player.Grow();
        score++;
        food.Respawn();
    }

    player.Update();
    ghost.Update();
}

void GameModel::GameOver() {
    scoreManager.SaveScore(playerName, score);
    currentHighScoreStr = scoreManager.GetHighScore();
    player.Reset({ 6, 9 });
    ghost.Reset({ 20, 5 });
    score = 0;
    gameOver = true;
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

const Snake& GameModel::GetPlayer() const {
    return player;
}

const Ghost& GameModel::GetGhost() const {
    return ghost;
}

const Food& GameModel::GetFood() const {
    return food;
}