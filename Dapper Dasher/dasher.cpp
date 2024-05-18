#include <iostream>
#include "raylib.h"

int main()
{
    constexpr size_t window_height{ 400 };
    constexpr size_t window_width{ 800 };

    InitWindow(window_width, window_height, "Dapper Dasher");
    SetTargetFPS(60);

    int velocity{ 0 };
    constexpr int gravity{ 1 };

    Texture2D scarfy = LoadTexture("Textures/scarfy.png");
    Rectangle scarfy_rect = {0, 0, scarfy.width/6, scarfy.height};
    Vector2 scarfy_pos = {window_width/2 - scarfy_rect.width/2, window_height - scarfy_rect.height}; //Scarfy position

    constexpr int jump_velocity{ -22 };

    //Is rectangle in air?
    bool is_in_air{ false };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTextureRec(scarfy, scarfy_rect, scarfy_pos, WHITE);

        if (scarfy_pos.y >= window_height - scarfy_rect.height) {
            //Scarfy is on the ground
            velocity = 0;
            is_in_air = false;
        }
        //Apply gravity
        else {
            //Apply gravity
            velocity += gravity;
            is_in_air = true;
        }
        if (IsKeyPressed(KEY_SPACE) && !is_in_air) {
            velocity += jump_velocity;
        }
        //Update position
        scarfy_pos.y += velocity;

        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
    return 0;
}