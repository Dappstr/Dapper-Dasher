#include <iostream>
#include "raylib.h"

int main()
{
    constexpr size_t window_height{ 400 };
    constexpr size_t window_width{ 800 };

    InitWindow(window_width, window_height, "Dapper Dasher");
    SetTargetFPS(60);

    //Rectangle dimensions
    constexpr int rect_width{ 50 };
    constexpr int rect_height{ 80 };

    int pos_y{ window_height - rect_height };
    int velocity{ 0 };
    const int gravity{ 1 };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawRectangle(window_width / 2, pos_y, rect_width, rect_height, RED);

        if (pos_y >= window_height - rect_height) {
            //Rectangle is on the ground
            velocity = 0;
        }

        //Apply gravity
        else {
            //Apply gravity
            velocity += gravity;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            velocity -= 10;
        }
        //Update position
        pos_y += velocity;

        EndDrawing();
    }
    CloseWindow();
    return 0;
}