#include <raylib.h>
#include <string>
#include <format>
#include <vector>
#include <cmath>
#include <algorithm>
#include <print>

int main() {
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;
    const std::string title = "SandboxGame";

    InitWindow(screenWidth, screenHeight, title.c_str());
    SetTargetFPS(60);


    int frame = 0;
    const char* message;
    float image_rot = 0.f;

    Texture2D image = LoadTexture("grass.png");
    Texture2D dirt = LoadTexture("dirt.png");
    Texture2D error = LoadTexture("error.png");

    Camera2D camera = {
        .offset = {
            .x = screenWidth / 2,
            .y = screenHeight / 2
        },
        .target = {
            .x = screenWidth / 2,
            .y = screenHeight / 2
        },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    int x = 0;
    int y = (screenHeight/2 - image.height/2) - 35;
    float rot_speed = 100.f;
    float move_speed = 2.f;
    float cube_speed = 500.f;
    float my = 0.0f;

    while (!WindowShouldClose()) {
        frame++;
        float delta = GetFrameTime();

        image_rot += 20.f * delta;

        Vector2 scroll_vector = GetMouseWheelMoveV();

        if (IsKeyDown(KEY_Q)) {
            camera.rotation -= rot_speed * delta;
        }

        if (IsKeyDown(KEY_E)) {
            camera.rotation += rot_speed * delta;
        } 

        if (IsKeyDown(KEY_RIGHT)) {
            x += cube_speed * delta;
        } else if (IsKeyDown(KEY_LEFT)) {
            x -= cube_speed * delta;
        }


        if (scroll_vector.y != 0) {
            float zoom_factor = 1.1f; // Adjust as needed for sensitivity
            if (scroll_vector.y > 0) {
                camera.zoom *= zoom_factor; // Zoom in
            } else {
                camera.zoom /= zoom_factor; // Zoom out
            }
            camera.zoom = std::clamp(camera.zoom, 0.01f, 10.f);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_delta = GetMouseDelta();
            float adjusted_move_speed = move_speed / camera.zoom;
            camera.target.x -= adjusted_move_speed * mouse_delta.x;
            camera.target.y -= adjusted_move_speed * mouse_delta.y;
            
        }
        Vector2 mouse_pos = GetMousePosition();
        Vector2 world_pos = GetScreenToWorld2D(mouse_pos, camera);

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            x = world_pos.x;
            my = world_pos.y;
        }

    
        //std::println("{}, {}", scroll_vector.x, scroll_vector.y);

        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode2D(camera);
            DrawRectangle(x, y, 35, 35, RED);
            for (int i = -64 ; i < 63 ; i++) {
                DrawTexture(image,
                    static_cast<float>(screenWidth/2 - image.width/2) + (i * image.width),
                    (screenHeight/2 - image.height/2),
                    WHITE
                );
            }

            for (int i = -64 ; i < 63 ; i++) {
                for (int j = 0 ; j < 16 ; j++) {
                    DrawTexture(dirt,   
                        static_cast<float>(screenWidth/2 - dirt.width/2) + (i * dirt.width),
                        (screenHeight/2 - dirt.height/2) + (dirt.height * (1 + j)),
                        WHITE
                    );
                }
            }
            
            DrawRectangle(0, -720 - ( std::sin(GetTime()) * 3000.f * delta), 400, 400, YELLOW);
            DrawText(std::format("({:.2f}, {:.2f})", static_cast<float>(x) / 32.f, static_cast<float>(y) / 32.f).c_str(), x, y - 40, 30, BLACK);
        EndMode2D(); 

        DrawText(std::format("Goto ({:.2f}, {:.2f})", static_cast<float>(world_pos.x) / 32.f, static_cast<float>(world_pos.y) / 32.f).c_str(), 5, 40, 16, BLACK);
        DrawText(std::format("Monitor: {}", GetMonitorName(GetCurrentMonitor())).c_str(), 0, screenHeight - 32, 32, BLACK);
        DrawFPS(0,0);    

        EndDrawing();
    }
    UnloadTexture(image);
    UnloadTexture(dirt);
    UnloadTexture(error);

    CloseWindow();
    return 0;
}


