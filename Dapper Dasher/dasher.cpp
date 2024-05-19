#include <iostream>
#include "raylib.h"

struct Anim_Data {
    Rectangle m_rec;
    Vector2 m_pos;
    int m_frame;
    float m_update_time;
    float m_running_time;
};

int main() {

    constexpr size_t window_height{ 400 };
    constexpr size_t window_width{ 800 };

    InitWindow(window_width, window_height, "Dapper Dasher");

    int velocity{ 0 };
    constexpr int gravity{ 1'000 };

    Texture2D scarfy = LoadTexture("Textures/scarfy.png");
    Anim_Data scarfy_data { {0, 0, scarfy.width / 6, scarfy.height},
                            {window_width/2 - scarfy_data.m_rec.width/2, window_height - scarfy_data.m_rec.height},
                            0, 1.0/12.0, 0.0
                          };

    Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");
    Anim_Data neb_data { {0, 0, nebula.width / 8, nebula.height / 8},
                         {window_width, window_height - nebula.height / 8}, 
                         0, 1.0 / 12.0, 0};

    int neb_vel{ -400 };//Nebula X velocity

    constexpr int jump_velocity{ -600 }; //Pixels/second

    //Is rectangle in air?
    bool is_in_air{ false };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //Delta trime (time since last frame)
        const float d_t{ GetFrameTime() };

        BeginDrawing();
        ClearBackground(WHITE);

        if (scarfy_data.m_pos.y >= window_height - scarfy_data.m_rec.height) {
            //Scarfy is on the ground
            velocity = 0;
            is_in_air = false;
        }
        //Apply gravity
        else {
            //Apply gravity
            velocity += gravity * d_t;
            is_in_air = true;
        }
        if (IsKeyPressed(KEY_SPACE) && !is_in_air) {
            velocity += jump_velocity;
        }
        
        //Update scarfy animation time
        if (!is_in_air) {
            //Update running time
            scarfy_data.m_running_time += d_t;
            if (scarfy_data.m_running_time >= scarfy_data.m_update_time) {
                scarfy_data.m_running_time = 0.0;
                scarfy_data.m_rec.x = scarfy_data.m_frame * scarfy_data.m_rec.width;
                ++scarfy_data.m_frame;
                if (scarfy_data.m_frame > 5) {
                    scarfy_data.m_frame = 0;
                }
            }
        }

        //Update nebula animation frame
        neb_data.m_running_time += d_t;
        if (neb_data.m_running_time >= neb_data.m_update_time) {
            neb_data.m_running_time = 0.0;
            neb_data.m_rec.x = neb_data.m_frame * neb_data.m_rec.width;
            ++neb_data.m_frame;

            if (neb_data.m_frame > 7) {
                neb_data.m_frame = 0;
            }
        }

        //Update nebula position
        neb_data.m_pos.x += neb_vel * d_t;

        //Update scarfy position
        scarfy_data.m_pos.y += velocity * d_t;

        //Draw nebula
        DrawTextureRec(nebula, neb_data.m_rec, neb_data.m_pos, WHITE);

        //Draw scarfy
        DrawTextureRec(scarfy, scarfy_data.m_rec, scarfy_data.m_pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    return 0;
}