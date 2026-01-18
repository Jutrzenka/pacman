#ifndef MAIN_H
#define MAIN_H

#include "raylib.h"
#include <deque>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

const int CELL_SIZE = 30;
const int CELL_COUNT = 25;

class ScoreManager {
public:
    void SaveScore(string name, int score) {
        ofstream file("scores.txt", ios::app);
        if (file.is_open()) {
            file << "Gracz: " << (name.empty() ? "Anonim" : name) << " | Wynik: " << score << endl;
            file.close();
        }
    }

    string GetHighScore() {
        ifstream file("scores.txt");
        string line, bestName = "Brak";
        int highScore = 0;
        if (!file.is_open()) return "Rekord: 0 (Brak)";
        while (getline(file, line)) {
            size_t namePos = line.find("Gracz: ");
            size_t scorePos = line.find("Wynik: ");
            if (namePos != string::npos && scorePos != string::npos) {
                string namePart = line.substr(namePos + 7, line.find(" |") - (namePos + 7));
                try {
                    int currentScore = stoi(line.substr(scorePos + 7));
                    if (currentScore > highScore) {
                        highScore = currentScore;
                        bestName = namePart;
                    }
                }
                catch (...) {}
            }
        }
        file.close();
        return "Rekord: " + to_string(highScore) + " (" + bestName + ")";
    }
};

class Snake {
public:
    deque<Vector2> body = { {6,9}, {5,9}, {4,9} };
    Vector2 direction = { 1, 0 };
    Color color = { 43, 51, 24, 255 };
    bool addSegment = false;

    virtual void Update() {
        Vector2 newHead = { body[0].x + direction.x, body[0].y + direction.y };
        if (newHead.x < 0) newHead.x = CELL_COUNT - 1;
        else if (newHead.x >= CELL_COUNT) newHead.x = 0;
        if (newHead.y < 0) newHead.y = CELL_COUNT - 1;
        else if (newHead.y >= CELL_COUNT) newHead.y = 0;
        body.push_front(newHead);
        if (addSegment) addSegment = false;
        else body.pop_back();
    }

    void Reset(Vector2 startPos) {
        body = { startPos, {startPos.x - 1, startPos.y}, {startPos.x - 2, startPos.y} };
        direction = { 1, 0 };
    }
};

class AISnake : public Snake {
public:
    AISnake() {
        color = DARKBLUE;
        body = { {20,5}, {20,6}, {20,7} };
    }
    void DecideDirection(Vector2 foodPos) {
        Vector2 head = body[0];
        if (foodPos.x > head.x && direction.x != -1) direction = { 1, 0 };
        else if (foodPos.x < head.x && direction.x != 1) direction = { -1, 0 };
        else if (foodPos.y > head.y && direction.y != -1) direction = { 0, 1 };
        else if (foodPos.y < head.y && direction.y != 1) direction = { 0, -1 };
    }
};

class Food {
public:
    Vector2 position;
    void Respawn() {
        position.x = GetRandomValue(0, CELL_COUNT - 1);
        position.y = GetRandomValue(0, CELL_COUNT - 1);
    }
};

class MenuAnimation {
public:
    void Draw(int centerX, int centerY) {
        float time = GetTime() * 3.0f;
        for (int i = 0; i < 10; i++) {
            float angle = time - (i * 0.3f);
            float x = centerX + cos(angle) * 60;
            float y = centerY + sin(angle) * 40;
            float size = 20 - i;
            DrawRectangle(x, y, size, size, { 43, 51, 24, 200 });
        }
    }
};

class GameLogic {
public:
    Snake player;
    AISnake ai;
    Food food;
    ScoreManager scoreManager;
    MenuAnimation menuAnim;
    string playerName = "";
    string currentHighScoreStr = "";
    int score = 0;

    GameLogic() {
        food.Respawn();
        currentHighScoreStr = scoreManager.GetHighScore();
    }

    void HandleNameInput() {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (playerName.length() < 15)) playerName += (char)key;
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) playerName.pop_back();
    }

    void HandlePlayerInput() {
        if (IsKeyPressed(KEY_UP) && player.direction.y != 1)    player.direction = { 0, -1 };
        if (IsKeyPressed(KEY_DOWN) && player.direction.y != -1) player.direction = { 0, 1 };
        if (IsKeyPressed(KEY_LEFT) && player.direction.x != 1)  player.direction = { -1, 0 };
        if (IsKeyPressed(KEY_RIGHT) && player.direction.x != -1) player.direction = { 1, 0 };
    }

    bool Update() {
        ai.DecideDirection(food.position);
        Vector2 nextP = GetWrappedPos(player.body[0], player.direction);
        Vector2 nextA = GetWrappedPos(ai.body[0], ai.direction);

        if (CheckBodyCollision(nextP, player.body) || CheckBodyCollision(nextP, ai.body)) {
            GameOver();
            return true;
        }
        if (CheckBodyCollision(nextA, ai.body) || CheckBodyCollision(nextA, player.body)) ai.Reset({ 20, 5 });

        if (nextP.x == food.position.x && nextP.y == food.position.y) {
            player.addSegment = true;
            score++;
            food.Respawn();
        }
        else if (nextA.x == food.position.x && nextA.y == food.position.y) {
            ai.addSegment = true;
            food.Respawn();
        }

        player.Update();
        ai.Update();
        return false;
    }

    Vector2 GetWrappedPos(Vector2 head, Vector2 dir) {
        Vector2 p = { head.x + dir.x, head.y + dir.y };
        if (p.x < 0) p.x = CELL_COUNT - 1;
        else if (p.x >= CELL_COUNT) p.x = 0;
        if (p.y < 0) p.y = CELL_COUNT - 1;
        else if (p.y >= CELL_COUNT) p.y = 0;
        return p;
    }

    bool CheckBodyCollision(Vector2 head, deque<Vector2>& body) {
        for (auto const& segment : body) if (head.x == segment.x && head.y == segment.y) return true;
        return false;
    }

    void GameOver() {
        scoreManager.SaveScore(playerName, score);
        currentHighScoreStr = scoreManager.GetHighScore();
        player.Reset({ 6, 9 });
        ai.Reset({ 20, 5 });
        score = 0;
    }
};

#endif
