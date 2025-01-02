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

    Texture2D image = LoadTexture("opengl.png");

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

    while (!WindowShouldClose()) {
        frame++;
        float delta = GetFrameTime();
        float rot_speed = 100.f;
        float move_speed = 2.f;

        image_rot += 2.f * delta;

        Vector2 scroll_vector = GetMouseWheelMoveV();

        if (IsKeyDown(KEY_Q)) {
            camera.rotation -= rot_speed * delta;
        }

        if (IsKeyDown(KEY_E)) {
            camera.rotation += rot_speed * delta;
        } 

        if (scroll_vector.y != 0) {
            camera.zoom += delta * scroll_vector.y;
            camera.zoom = std::clamp(camera.zoom, 0.01f, 10.f);

        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_delta = GetMouseDelta();
            camera.target.x -= move_speed * mouse_delta.x;
            camera.target.y -= move_speed * mouse_delta.y;
        }
        //std::println("{}, {}", scroll_vector.x, scroll_vector.y);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);
            DrawRectangle(20, 20, 35, 35, RED);
            DrawText(std::format("{}", GetFrameTime()).c_str(), 300, 0, 64, GREEN);
            DrawTextureEx(image, {
                static_cast<float>(screenWidth/2 - image.width/2),
                static_cast<float>(screenHeight/2 - image.height/2)},
                image_rot,
                1.0f,
                WHITE
            );
            DrawRectangleGradientV(1280, 720, 400, 400, BLACK, WHITE);
        EndMode2D();       

        EndDrawing();
    }
    UnloadTexture(image);
    CloseWindow();
    return 0;
}


