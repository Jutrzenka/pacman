#include "raylib.h"
#include "Main.h"

enum GameState { MENU, GAMEPLAY, PAUSED };

double lastUpdate = 0;
bool shouldUpdate(double interval) {
    double now = GetTime();
    if (now - lastUpdate >= interval) {
        lastUpdate = now;
        return true;
    }
    return false;
}

class GameRenderer {
public:
    void Draw(GameLogic& logic, GameState state) {
        ClearBackground({ 0, 0, 50, 255 });  // Niebieskie t³o jak w Pacman
        if (state == MENU) {
            DrawText("PACMAN SNAKE", 375 - MeasureText("PACMAN SNAKE", 50) / 2, 80, 50, YELLOW);

            // Animacja wê¿a
            logic.menuAnim.Draw(375, 180);

            // Panel wyników
            DrawRectangle(175, 250, 400, 110, ColorAlpha(BLACK, 0.3f));
            DrawText("NAJLEPSZY WYNIK:", 375 - MeasureText("NAJLEPSZY WYNIK:", 20) / 2, 265, 20, WHITE);
            DrawText(logic.currentHighScoreStr.c_str(), 375 - MeasureText(logic.currentHighScoreStr.c_str(), 20) / 2, 300, 20, YELLOW);

            DrawText(TextFormat("TWOJE IMIE: %s", logic.playerName.c_str()), 375 - MeasureText(TextFormat("TWOJE IMIE: %s", logic.playerName.c_str()), 20) / 2, 400, 20, WHITE);

            if (!logic.playerName.empty()) {
                DrawText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 375 - MeasureText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 20) / 2, 480, 20, GREEN);
            }
        }
        else {
            // Rysuj jedzenie jako ma³e kó³ko (jak kropka w Pacman)
            DrawCircle(food.position.x * CELL_SIZE + CELL_SIZE / 2,
                food.position.y * CELL_SIZE + CELL_SIZE / 2,
                CELL_SIZE / 4, WHITE);

            // Rysuj gracza (¿ó³ty w¹¿ jak Pacman)
            for (auto const& s : logic.player.body)
                DrawRectangle(s.x * CELL_SIZE, s.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, logic.player.color);

            // Rysuj ducha (czerwony kwadrat)
            DrawRectangle(logic.ghost.body[0].x * CELL_SIZE,
                logic.ghost.body[0].y * CELL_SIZE,
                CELL_SIZE, CELL_SIZE, logic.ghost.color);

            DrawText(TextFormat("WYNIK: %i", logic.score), 20, 20, 20, YELLOW);

            if (state == PAUSED) {
                DrawRectangle(0, 0, 750, 750, ColorAlpha(BLACK, 0.5f));
                DrawText("PAUZA", 310, 350, 40, WHITE);
            }
        }
    }
private:
    Food food;  // Dodano dla rysowania jedzenia
};

int main() {
    InitWindow(750, 750, "Pacman Snake");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameLogic logic;
    GameRenderer renderer;
    GameState state = MENU;

    while (!WindowShouldClose()) {
        if (state == MENU) {
            logic.HandleNameInput();
            // Start gry klawiszem ENTER
            if (IsKeyPressed(KEY_ENTER) && !logic.playerName.empty()) state = GAMEPLAY;
        }
        else if (state == GAMEPLAY) {
            logic.HandlePlayerInput();
            if (shouldUpdate(0.12))
                if (logic.Update())
                    state = MENU;
            if (IsKeyPressed(KEY_ESCAPE)) state = PAUSED;
        }
        else if (state == PAUSED) {
            if (IsKeyPressed(KEY_ESCAPE)) state = GAMEPLAY;
            if (IsKeyPressed(KEY_Q)) { logic.GameOver(); state = MENU; }
        }

        BeginDrawing();
        renderer.Draw(logic, state);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}