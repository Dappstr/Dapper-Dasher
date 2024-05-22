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

bool is_on_ground(const Anim_Data& data, int height) {
    return data.m_pos.y >= height - data.m_rec.height;
}

bool check_collision_recs(const Rectangle& rec1, const Rectangle& rec2) {
    return (rec1.x < rec2.x + rec2.width &&
        rec1.x + rec1.width > rec2.x &&
        rec1.y < rec2.y + rec2.height &&
        rec1.y + rec2.height > rec2.y);
}

void update_anim_data(Anim_Data& data, const double delta_time, const int max_frame) {
    data.m_running_time += delta_time;
    if (data.m_running_time >= data.m_update_time) {
        data.m_running_time = 0;

        //Update animation frame
        data.m_rec.x = data.m_frame * data.m_rec.width;
        ++data.m_frame;
        if (data.m_frame > max_frame) {
            data.m_frame = 0;
        }
    }
}

int main() {

    std::array<int, 2> window_dimensions = { 512, 380 };

    InitWindow(window_dimensions.at(0), window_dimensions.at(1), "Dapper Dasher");

    int velocity{ 0 };
    constexpr int gravity{ 1'000 };

    Texture2D background = LoadTexture("Textures/far-buildings.png");
    double bg_x{};

    Texture2D midground = LoadTexture("Textures/back-buildings.png");
    double mg_x{};

    Texture2D foreground = LoadTexture("Textures/foreground.png");
    double fg_x{};

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

    double finish_line{ nebulae.at(amount_of_nebulae - 1).m_pos.x };

    int neb_vel{ -200 };//Nebula X velocity

    constexpr int jump_velocity{ -600 }; //Pixels/second

    //Is rectangle in air?
    bool is_in_air{};
    bool collision{};


    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //Delta trime (time since last frame)
        const float d_t{ GetFrameTime() };

        BeginDrawing();
        ClearBackground(WHITE);

        //Scroll the background
        bg_x -= 20 * d_t;
        if (bg_x <= -background.width * 2) {
            bg_x = 0;
        }

        //Scroll the midground
        mg_x -= 40 * d_t;
        if (mg_x <= -midground.width * 2) {
            mg_x = 0;
        }

        //Scroll the foreground;
        fg_x -= 80 * d_t;
        if (fg_x <= -foreground.width * 2) {
            fg_x = 0;
        }

        //Draw background
        Vector2 bg1_pos{ bg_x, 0 };
        DrawTextureEx(background, bg1_pos, 0.0, 2.0, WHITE);
        Vector2 bg2_pos{ bg_x + background.width * 2, 0.0 };
        DrawTextureEx(background, bg2_pos, 0.0, 2.0, WHITE);

        //Draw midground
        Vector2 mg1_pos{ mg_x, 0 };
        DrawTextureEx(midground, mg1_pos, 0.0, 2.0, WHITE);
        Vector2 mg2_pos{ mg_x + midground.width * 2, 0 };
        DrawTextureEx(midground, mg2_pos, 0.0, 2.0, WHITE);

        //Draw foreground
        Vector2 fg1_pos{ fg_x, 0 };
        DrawTextureEx(foreground, fg1_pos, 0.0, 2.0, WHITE);
        Vector2 fg2_pos{ fg_x + foreground.width * 2, 0 };
        DrawTextureEx(foreground, fg2_pos, 0.0, 2.0, WHITE);

        if (is_on_ground(scarfy_data, window_dimensions.at(1))) {
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

        //Update finish line
        finish_line += neb_vel * d_t;

        //Update scarfy position
        scarfy_data.m_pos.y += velocity * d_t;

        //Update scarfy animation time
        if (!is_in_air) {
            //Update running time
            update_anim_data(scarfy_data, d_t, 5);
        }

        for (size_t i = 0; i < amount_of_nebulae; ++i) {
            //Update nebula animation frame
            update_anim_data(nebulae.at(i), d_t, 7);
        }
        for (auto& nebula : nebulae) {
            double pad{ 20 };
            Rectangle neb_rec{
                nebula.m_pos.x + pad,
                nebula.m_pos.y + pad,
                nebula.m_rec.width - 2 * pad,
                nebula.m_rec.height - 2 * pad
            };
            Rectangle scarfy_rec{
                scarfy_data.m_pos.x,
                scarfy_data.m_pos.y,
                scarfy_data.m_rec.width,
                scarfy_data.m_rec.height
            };
            if (check_collision_recs(neb_rec, scarfy_rec)) {
                collision = true;
            }
        }

        if (collision) {
            //Lose game
            DrawText("Game over!", window_dimensions.at(0) / 4, window_dimensions.at(1) / 2, 40, RED);
        }
        else if (scarfy_data.m_pos.x >= finish_line) {
            //Win game
            DrawText("You win!", window_dimensions.at(0) / 4, window_dimensions.at(1) / 2, 40, RED);
        }
        else {
            for (size_t i = 0; i < amount_of_nebulae; ++i) {
                //Draw nebula
                DrawTextureRec(nebula, nebulae.at(i).m_rec, nebulae.at(i).m_pos, WHITE);
            }
            //Draw scarfy
            DrawTextureRec(scarfy, scarfy_data.m_rec, scarfy_data.m_pos, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    return 0;
}