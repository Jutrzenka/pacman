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
    Color color = YELLOW;  // Zmieniono na ¿ó³ty jak Pacman
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

class Ghost : public Snake {  // Zmieniono nazwê z AISnake na Ghost
public:
    Ghost() {
        color = RED;  // Duszki maj¹ ró¿ne kolory
        body = { {20,5} };  // Tylko jeden segment - kwadrat
        direction = { -1, 0 };
    }

    void Reset(Vector2 startPos) {
        body = { startPos };
        direction = { -1, 0 };
    }

    void Update() override {
        Vector2 newHead = { body[0].x + direction.x, body[0].y + direction.y };
        if (newHead.x < 0) newHead.x = CELL_COUNT - 1;
        else if (newHead.x >= CELL_COUNT) newHead.x = 0;
        if (newHead.y < 0) newHead.y = CELL_COUNT - 1;
        else if (newHead.y >= CELL_COUNT) newHead.y = 0;
        body[0] = newHead;  // Ghost siê nie wyd³u¿a, tylko przesuwa
    }

    void MoveRandom() {
        // Proste losowe poruszanie siê duszka
        int dir = GetRandomValue(0, 3);
        switch (dir) {
        case 0: direction = { 1, 0 }; break;  // prawo
        case 1: direction = { -1, 0 }; break; // lewo
        case 2: direction = { 0, 1 }; break;  // dó³
        case 3: direction = { 0, -1 }; break; // góra
        }
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
    Ghost ghost;  // Zmieniono na pojedynczego ducha
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
        // Duch porusza siê losowo
        if (GetRandomValue(0, 100) < 10) {  // 10% szans na zmianê kierunku w ka¿dej klatce
            ghost.MoveRandom();
        }

        ghost.Update();

        Vector2 nextP = GetWrappedPos(player.body[0], player.direction);

        // Kolizja gracza z duchem
        if (CheckCollision(nextP, ghost.body[0])) {
            GameOver();
            return true;
        }

        if (nextP.x == food.position.x && nextP.y == food.position.y) {
            player.addSegment = true;
            score++;
            food.Respawn();
        }

        player.Update();
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

    bool CheckCollision(Vector2 a, Vector2 b) {
        return a.x == b.x && a.y == b.y;
    }

    void GameOver() {
        scoreManager.SaveScore(playerName, score);
        currentHighScoreStr = scoreManager.GetHighScore();
        player.Reset({ 6, 9 });
        ghost.Reset({ 20, 5 });
        score = 0;
    }
};

#endif