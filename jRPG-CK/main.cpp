#include "raylib.h"
#include "GameController.h"

int main() {
    InitWindow(750, 750, "Pacman Snake");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameController controller;

    while (!controller.ShouldClose()) {
        controller.ProcessInput();
        controller.Update();

        BeginDrawing();
        controller.Render();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}