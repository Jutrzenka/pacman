#include "GameView.h"
#include "Constants.h"

void GameView::DrawMenu(const GameModel& model) {
    ClearBackground({ 0, 0, 50, 255 });
    DrawText("PACMAN SNAKE", 375 - MeasureText("PACMAN SNAKE", 50) / 2, 80, 50, YELLOW);

    // Score panel
    DrawRectangle(175, 250, 400, 110, ColorAlpha(BLACK, 0.3f));
    DrawText("NAJLEPSZY WYNIK:", 375 - MeasureText("NAJLEPSZY WYNIK:", 20) / 2, 265, 20, WHITE);
    DrawText(model.GetHighScoreString().c_str(), 375 - MeasureText(model.GetHighScoreString().c_str(), 20) / 2, 300, 20, YELLOW);

    DrawText(TextFormat("TWOJE IMIE: %s", model.GetPlayerName().c_str()), 375 - MeasureText(TextFormat("TWOJE IMIE: %s", model.GetPlayerName().c_str()), 20) / 2, 400, 20, WHITE);

    if (!model.GetPlayerName().empty()) {
        DrawText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 375 - MeasureText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 20) / 2, 480, 20, GREEN);
    }
}

void GameView::DrawGameplay(const GameModel& model) {
    ClearBackground({ 0, 0, 50, 255 });

    DrawFood(model.GetFood());
    DrawSnake(model.GetPlayer());
    DrawGhost(model.GetGhost());

    DrawText(TextFormat("WYNIK: %i", model.GetScore()), 20, 20, 20, YELLOW);
}

void GameView::DrawPause() {
    DrawRectangle(0, 0, 750, 750, ColorAlpha(BLACK, 0.5f));
    DrawText("PAUZA", 310, 350, 40, WHITE);
}

void GameView::DrawSnake(const Snake& snake) {
    for (const auto& segment : snake.GetBody()) {
        DrawRectangle(segment.x * CELL_SIZE, segment.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, snake.GetColor());
    }
}

void GameView::DrawGhost(const Ghost& ghost) {
    const auto& body = ghost.GetBody();
    if (!body.empty()) {
        DrawRectangle(body[0].x * CELL_SIZE, body[0].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ghost.GetColor());
    }
}

void GameView::DrawFood(const Food& food) {
    Vector2 pos = food.GetPosition();
    DrawCircle(pos.x * CELL_SIZE + CELL_SIZE / 2,
        pos.y * CELL_SIZE + CELL_SIZE / 2,
        CELL_SIZE / 4, WHITE);
}