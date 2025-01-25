#include "raylib.h"
#include "raymath.h"        // Required for: Vector2Clamp()
#include <list>
#include "objects.hpp"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 450;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "raylib [core] example - window scale letterbox");
    SetWindowMinSize(320, 240);

    int gameScreenWidth = 640;
    int gameScreenHeight = 480;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    std::list<Enemy> enemies;
    std::list<Bullet> bullets;
    enemies.push_back(Enemy(Rectangle{120, 120, 20, 20}));

    Player C(Rectangle{gameScreenWidth / 2, gameScreenHeight / 2, 20,20});

    // Initialize camera
    Camera2D camera = {0};
    camera.target = (Vector2){ gameScreenWidth / 2.0f, gameScreenHeight / 2.0f };
    camera.offset = (Vector2){ gameScreenWidth / 2.0f, gameScreenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update Camera
        const float cameraSpeed = 200.0f;  // Camera movement speed

        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });


        // Update domain and camera target positions
        if (IsKeyDown(KEY_W)) {
            C.domain.y -= 5;
        }
        if (IsKeyDown(KEY_S)) {
            C.domain.y += 5;
        }
        if (IsKeyDown(KEY_A)) {
            C.domain.x -= 5;
        }
        if (IsKeyDown(KEY_D)) {
            C.domain.x += 5;
        }


        // Sync camera target with domain position
        camera.target.x = C.domain.x;
        camera.target.y = C.domain.y;

        if(IsMouseButtonPressed(0)){
            float xDir = GetMouseX() - C.domain.x;
            float yDir = GetMouseY() - C.domain.y;
            float magnitude = sqrt(xDir * xDir + yDir * yDir);
            Vector2 direction = Vector2Subtract(virtualMouse, Vector2{C.domain.x,C.domain.y});
            direction = Vector2Normalize(direction);
            bullets.push_back(Bullet(C.domain.x, C.domain.y, 10, direction, 1));
        }



        //UPDATE ENEMY
        for (std::list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
            // Assign a random destination if not traveling
            if (!it->traveling) {
                it->dest.x = it->domain.x + (GetRandomValue(0, 100) - 50);
                it->dest.y = it->domain.y + (GetRandomValue(0, 100) - 50);
                it->traveling = true;
            }

            // Move toward destination
            if (it->traveling) {
                if (it->domain.x < it->dest.x) {
                    it->move(1, 0);
                    if (it->domain.x > it->dest.x) it->domain.x = it->dest.x; // Prevent overshoot
                } else if (it->domain.x > it->dest.x) {
                    it->move(-1, 0);
                    if (it->domain.x < it->dest.x) it->domain.x = it->dest.x; // Prevent overshoot
                }

                if (it->domain.y < it->dest.y) {
                    it->move(0, 1);
                    if (it->domain.y > it->dest.y) it->domain.y = it->dest.y; // Prevent overshoot
                } else if (it->domain.y > it->dest.y) {
                    it->move(0, -1);
                    if (it->domain.y < it->dest.y) it->domain.y = it->dest.y; // Prevent overshoot
                }

                // Check if destination is reached
                if (it->domain.x == it->dest.x && it->domain.y == it->dest.y) {
                    it->traveling = false;
                }

            }
            // for (std::list<Bullet>::iterator jit = bullets.begin(); jit != bullets.end(); ++jit) {
            //     if(CheckCollisionCircleRec(Vector{jit->x,jit->y}, jit->r, it->domain)){
            //
            //     }
            // }
        }

        for (std::list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
            it->x += it->speed * it->vec.x;
            it->y += it->speed * it->vec.y;
        }




        // Draw
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Clear render texture background color

        BeginMode2D(camera);
        for (std::list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
            DrawRectangle(it->domain.x, it->domain.y, it->domain.width, it->domain.height, RED);
        }
        for (std::list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
            DrawCircle(it->x,it->y,it->r,BROWN);
        }
        DrawRectangle(C.domain.x,C.domain.y,C.domain.width,C.domain.height,BLUE);
        DrawRectangle(50,50,10,10,BLACK);
        EndMode2D();

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);  // Clear screen background

        DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                       (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f,
                           (float)gameScreenWidth * scale, (float)gameScreenHeight * scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
