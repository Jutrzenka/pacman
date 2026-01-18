#include "GameView.h"
#include "Constants.h"

void GameView::DrawMenu(const GameModel& model) {
    ClearBackground({ 0, 0, 50, 255 });
    DrawText("PACMAN SNAKE", 375 - MeasureText("PACMAN SNAKE", 50) / 2, 80, 50, YELLOW);

    DrawRectangle(175, 250, 400, 150, ColorAlpha(BLACK, 0.3f));

    std::string playerName, highScoreText;
    model.RetrievePlayerInformation(playerName, highScoreText);

    // Podziel tekst na linie
    int yPos = 265;
    size_t pos = 0;
    std::string delimiter = "\n";
    std::string token;
    while ((pos = highScoreText.find(delimiter)) != std::string::npos) {
        token = highScoreText.substr(0, pos);
        DrawText(token.c_str(), 375 - MeasureText(token.c_str(), 20) / 2, yPos, 20, YELLOW);
        yPos += 25;
        highScoreText.erase(0, pos + delimiter.length());
    }

    DrawText(TextFormat("TWOJE IMIE: %s", playerName.c_str()),
        375 - MeasureText(TextFormat("TWOJE IMIE: %s", playerName.c_str()), 20) / 2,
        400, 20, WHITE);

    if (!playerName.empty()) {
        DrawText("NACIŒNIJ ENTER, ABY ZACZ¥Æ",
            375 - MeasureText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 20) / 2,
            480, 20, GREEN);
    }
}

void GameView::DrawGameplay(const GameModel& model) {
    ClearBackground(BLACK);

    // Pobierz dane z modelu
    std::deque<Vector2> playerSegments;
    Color playerColor;
    Vector2 ghostPosition;
    Color ghostColor;
    std::vector<std::vector<int>> boardGrid;

    model.ProvideRenderingData(playerSegments, playerColor, ghostPosition, ghostColor, boardGrid);

    bool gameOverStatus;
    int scoreValue;
    bool ghostFrightenedStatus;
    int foodRemaining;
    model.RetrieveGameStatus(gameOverStatus, scoreValue, ghostFrightenedStatus, foodRemaining);

    // Rysuj planszê
    DrawBoardGrid(boardGrid);

    // Rysuj gracza
    DrawPlayerCharacter(playerSegments, playerColor);

    // Rysuj ducha
    DrawEnemyGhost(ghostPosition, ghostColor, ghostFrightenedStatus);

    DrawText(TextFormat("WYNIK: %i", scoreValue), 20, 20, 20, YELLOW);
    DrawText(TextFormat("POZOSTALO JEDZENIA: %i", foodRemaining), 20, 50, 20, YELLOW);
}

void GameView::DrawPause() {
    DrawRectangle(0, 0, 750, 750, ColorAlpha(BLACK, 0.5f));
    DrawText("PAUZA", 310, 350, 40, WHITE);
}

void GameView::DrawPlayerCharacter(const std::deque<Vector2>& segments, Color color) {
    for (const auto& segment : segments) {
        int x = (int)(segment.x * CELL_SIZE);
        int y = (int)(segment.y * CELL_SIZE);

        DrawCircle((float)(x + CELL_SIZE / 2),
            (float)(y + CELL_SIZE / 2),
            (float)(CELL_SIZE / 2 - 2), color);
    }
}

void GameView::DrawEnemyGhost(Vector2 position, Color color, bool isFrightened) {
    Color ghostColor = isFrightened ? SKYBLUE : color;

    int x = (int)(position.x * CELL_SIZE);
    int y = (int)(position.y * CELL_SIZE);

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

    // Jeœli przestraszony
    if (isFrightened) {
        DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + 2 * CELL_SIZE / 3),
            (float)(eyeSize / 2), WHITE);
        DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + 2 * CELL_SIZE / 3),
            (float)(eyeSize / 2), WHITE);
    }
}

void GameView::DrawBoardGrid(const std::vector<std::vector<int>>& gridData) {
    for (size_t y = 0; y < gridData.size(); y++) {
        for (size_t x = 0; x < gridData[y].size(); x++) {
            float xPos = x * CELL_SIZE;
            float yPos = y * CELL_SIZE;

            switch (gridData[y][x]) {
            case 1: // Œciana
                DrawRectangle((int)xPos, (int)yPos,
                    CELL_SIZE, CELL_SIZE, BLUE);
                DrawRectangleLines((int)xPos, (int)yPos,
                    CELL_SIZE, CELL_SIZE, DARKBLUE);
                break;

            case 2: // Jedzenie
                DrawCircle((int)(xPos + CELL_SIZE / 2.0f),
                    (int)(yPos + CELL_SIZE / 2.0f),
                    CELL_SIZE / 8, YELLOW);
                break;

            case 3: // Power pellet
                DrawCircle((int)(xPos + CELL_SIZE / 2.0f),
                    (int)(yPos + CELL_SIZE / 2.0f),
                    CELL_SIZE / 4, YELLOW);
                break;

            default:
                break;
            }
        }
    }
}