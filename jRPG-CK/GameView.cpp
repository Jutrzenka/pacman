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
    ClearBackground(BLACK);

    DrawBoard(model.GetBoard());
    DrawSnake(model.GetPlayer());
    DrawGhost(model.GetGhost(), model.IsScaredMode());

    DrawText(TextFormat("WYNIK: %i", model.GetScore()), 20, 20, 20, YELLOW);
    DrawText(TextFormat("POZOSTALO JEDZENIA: %i", model.GetBoard().GetFoodCount()), 20, 50, 20, YELLOW);
}

void GameView::DrawPause() {
    DrawRectangle(0, 0, 750, 750, ColorAlpha(BLACK, 0.5f));
    DrawText("PAUZA", 310, 350, 40, WHITE);
}

void GameView::DrawSnake(const Snake& snake) {
    for (const auto& segment : snake.GetBody()) {
        // Rysowanie Pacmana jako ¿ó³tego kó³ka z "ustami"
        int x = (int)(segment.x * CELL_SIZE);
        int y = (int)(segment.y * CELL_SIZE);

        DrawCircle((float)(x + CELL_SIZE / 2),
            (float)(y + CELL_SIZE / 2),
            (float)(CELL_SIZE / 2 - 2), YELLOW);

        // Rysowanie "ust" Pacmana
        Vector2 direction = snake.GetDirection();
        if (direction.x != 0 || direction.y != 0) {
            float startAngle = 0.0f;
            if (direction.x == 1) startAngle = 0.0f;      // Prawo
            else if (direction.x == -1) startAngle = 180.0f; // Lewo
            else if (direction.y == 1) startAngle = 90.0f;   // Dó³
            else if (direction.y == -1) startAngle = 270.0f; // Góra

            DrawCircleSector({ (float)(x + CELL_SIZE / 2.0f), (float)(y + CELL_SIZE / 2.0f) },
                (float)(CELL_SIZE / 2 - 2), startAngle + 45, startAngle + 315, 0, BLACK);
        }
    }
}

void GameView::DrawGhost(const Ghost& ghost, bool isScared) {
    const auto& body = ghost.GetBody();
    if (!body.empty()) {
        Color ghostColor = isScared ? SKYBLUE : ghost.GetColor();

        // Rysowanie ducha jako zaokr¹glonego kwadratu
        int x = (int)(body[0].x * CELL_SIZE);
        int y = (int)(body[0].y * CELL_SIZE);

        // G³ówny korpus
        DrawRectangleRounded(Rectangle{ (float)x + 2, (float)y + 2,
                                       (float)(CELL_SIZE - 4), (float)(CELL_SIZE - 4) },
            0.5f, 8, ghostColor);

        // Oczy
        int eyeSize = CELL_SIZE / 8;
        DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
            (float)eyeSize, WHITE);
        DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
            (float)eyeSize, WHITE);
        DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
            (float)(eyeSize / 2), BLACK);
        DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
            (float)(eyeSize / 2), BLACK);

        // Jeœli przestraszony - narysuj przestraszon¹ twarz
        if (isScared) {
            DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + 2 * CELL_SIZE / 3),
                (float)(eyeSize / 2), WHITE);
            DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + 2 * CELL_SIZE / 3),
                (float)(eyeSize / 2), WHITE);
        }
    }
}

void GameView::DrawBoard(const Board& board) {
    board.Draw();
}