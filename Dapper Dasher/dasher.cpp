#include <iostream>
#include <array>
#include "raylib.h"

struct Anim_Data {
    Rectangle m_rec;
    Vector2 m_pos;
    int m_frame;
    float m_update_time;
    float m_running_time;
};

int main() {

    std::array<int, 2> window_dimensions = { 800, 400 };

    InitWindow(window_dimensions.at(0), window_dimensions.at(1), "Dapper Dasher");

    int velocity{ 0 };
    constexpr int gravity{ 1'000 };

    Texture2D scarfy = LoadTexture("Textures/scarfy.png");
    Anim_Data scarfy_data { {0, 0, scarfy.width / 6, scarfy.height},
                            {window_dimensions.at(0)/2 - scarfy_data.m_rec.width/2, window_dimensions.at(1) - scarfy_data.m_rec.height},
                            0, 1.0/12.0, 0.0};

    Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");

    constexpr size_t amount_of_nebulae{ 6 };
    std::array<Anim_Data, amount_of_nebulae> nebulae{};
    
    for (size_t i = 0; i < amount_of_nebulae; ++i) {
        nebulae.at(i).m_rec = { 0, 0, static_cast<float> (nebula.width / 8), static_cast<float>(nebula.height / 8) };
        nebulae.at(i).m_pos.y = window_dimensions.at(1) - nebula.height / 8;
        nebulae.at(i).m_frame = 0;
        nebulae.at(i).m_running_time = 0.0;
        nebulae.at(i).m_update_time = 1.0/16.0;
        nebulae.at(i).m_pos.x = window_dimensions.at(0) + i * 300;
    }

    int neb_vel{ -200 };//Nebula X velocity

    constexpr int jump_velocity{ -600 }; //Pixels/second

    //Is rectangle in air?
    bool is_in_air{ false };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //Delta trime (time since last frame)
        const float d_t{ GetFrameTime() };

        BeginDrawing();
        ClearBackground(WHITE);

        if (scarfy_data.m_pos.y >= window_dimensions.at(1) - scarfy_data.m_rec.height) {
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

        //Update nebula position
        for (size_t i = 0; i < amount_of_nebulae; ++i) {
            nebulae.at(i).m_pos.x += neb_vel * d_t;
        }

        //Update scarfy position
        scarfy_data.m_pos.y += velocity * d_t;

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

        for (size_t i = 0; i < amount_of_nebulae; ++i) {
            //Update nebula animation frame
            nebulae.at(i).m_running_time += d_t;
            if (nebulae.at(i).m_running_time >= nebulae.at(0).m_update_time) {
                nebulae.at(i).m_running_time = 0.0;
                nebulae.at(i).m_rec.x = nebulae.at(0).m_frame * nebulae.at(0).m_rec.width;
                ++nebulae.at(i).m_frame;

                if (nebulae.at(i).m_frame > 7) {
                    nebulae.at(i).m_frame = 0;
                }
            }
        }

        for (size_t i = 0; i < amount_of_nebulae; ++i) {
            //Draw nebula
            DrawTextureRec(nebula, nebulae.at(i).m_rec, nebulae.at(i).m_pos, WHITE);
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfy_data.m_rec, scarfy_data.m_pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    return 0;
}