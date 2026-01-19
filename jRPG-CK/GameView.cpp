#include "GameView.h"
#include "Constants.h"

void GameView::DrawMenu(const GameModel& model) {
    ClearBackground({ 0, 0, 50, 255 });
    DrawText("PACMAN SNAKE", 375 - MeasureText("PACMAN SNAKE", 50) / 2, 80, 50, YELLOW);

    DrawRectangle(175, 250, 400, 150, ColorAlpha(BLACK, 0.3f));

    std::string playerName, highScoreText;
    model.RetrievePlayerInformation(playerName, highScoreText);

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

    // Difficulty Selection
    Difficulty diff = model.GetDifficulty();
    
    // Updated text for arrow key navigation
    DrawText("< POZIOM TRUDNOSCI >", 250, 430, 20, WHITE);
    
    const char* diffText = "UNKNOWN";
    Color diffColor = WHITE;
    
    if (diff == EASY) {
        diffText = "EASY";
        diffColor = GREEN;
    } else if (diff == MEDIUM) {
        diffText = "MEDIUM";
        diffColor = YELLOW;
    } else if (diff == HARD) {
        diffText = "HARD";
        diffColor = RED;
    }
    
    // Draw centered difficulty text
    // Center point is approx 375 (750/2)
    DrawText(diffText, 375 - MeasureText(diffText, 30) / 2, 460, 30, diffColor);


    if (!playerName.empty()) {
        DrawText("NACIŒNIJ ENTER, ABY ZACZ¥Æ",
            375 - MeasureText("NACIŒNIJ ENTER, ABY ZACZ¥Æ", 20) / 2,
            500, 20, GREEN); // Moved down slightly
    }
}

void GameView::DrawGameplay(const GameModel& model) {
    ClearBackground(BLACK);

    std::deque<Vector2> playerSegments;
    Color playerColor;
    Vector2 redGhostPosition, greenGhostPosition, purpleGhostPosition;
    std::vector<std::vector<int>> boardGrid;

    model.ProvideRenderingData(playerSegments, playerColor,
        redGhostPosition, greenGhostPosition, purpleGhostPosition,
        boardGrid);

    bool gameOverStatus;
    int scoreValue;
    bool ghostFrightenedStatus;
    int foodRemaining;
    model.RetrieveGameStatus(gameOverStatus, scoreValue, ghostFrightenedStatus, foodRemaining);

    DrawBoardGrid(boardGrid);
    DrawPlayerCharacter(playerSegments, playerColor);

    DrawEnemyGhost(redGhostPosition, RED, ghostFrightenedStatus);
    DrawEnemyGhost(greenGhostPosition, GREEN, ghostFrightenedStatus);
    DrawEnemyGhost(purpleGhostPosition, PURPLE, ghostFrightenedStatus);

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

    DrawRectangleRounded(Rectangle{ (float)x + 2, (float)y + 2,
                                   (float)(CELL_SIZE - 4), (float)(CELL_SIZE - 4) },
        0.5f, 8, ghostColor);

    int eyeSize = CELL_SIZE / 8;
    DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
        (float)eyeSize, WHITE);
    DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
        (float)eyeSize, WHITE);
    DrawCircle((float)(x + CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
        (float)(eyeSize / 2), BLACK);
    DrawCircle((float)(x + 2 * CELL_SIZE / 3), (float)(y + CELL_SIZE / 3),
        (float)(eyeSize / 2), BLACK);

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
            case 1:
                DrawRectangle((int)xPos, (int)yPos,
                    CELL_SIZE, CELL_SIZE, BLUE);
                DrawRectangleLines((int)xPos, (int)yPos,
                    CELL_SIZE, CELL_SIZE, DARKBLUE);
                break;

            case 2:
                DrawCircle((int)(xPos + CELL_SIZE / 2.0f),
                    (int)(yPos + CELL_SIZE / 2.0f),
                    CELL_SIZE / 8, YELLOW);
                break;

            case 3:
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