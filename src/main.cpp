#include "raylib.h"
#include "raymath.h"        // Required for: Vector2Clamp()
#include <list>
#include "objects.hpp"
#include <iostream>

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

    std::list<Wall> walls;
    std::list<Bullet> enemyBullets;
    std::list<Enemy> enemies;
    std::list<Bullet> bullets;
    enemies.push_back(Enemy(Rectangle{120, 120, 20, 20}));
    enemies.push_back(Enemy(Rectangle{120, 140, 20, 20}));
    enemies.push_back(Enemy(Rectangle{140, 120, 20, 20}));
    walls.push_back(Wall(Rectangle{50, 50,50,300}));

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

        if (IsMouseButtonPressed(0)) {
            // Convert screen mouse position to world position
            Vector2 worldMouse = GetScreenToWorld2D(virtualMouse, camera);

            // Calculate direction from player to mouse
            Vector2 direction = Vector2Subtract(worldMouse, (Vector2){ C.domain.x, C.domain.y });
            direction = Vector2Normalize(direction);

            // Add the bullet to the list
            bullets.push_back(Bullet(C.domain.x, C.domain.y, 10, direction, 5));
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

                int rand = GetRandomValue(0,100);
                if(rand < 5){
                    //Vector2 worldMouse = GetScreenToWorld2D(Vector2{it->domain.x,it->domain.y}, camera);

                    // Calculate direction from player to mouse
                    Vector2 direction = Vector2Subtract((Vector2){ C.domain.x, C.domain.y},Vector2{it->domain.x,it->domain.y}  );
                    direction = Vector2Normalize(direction);

                    // Add the bullet to the list
                    enemyBullets.push_back(Bullet(it->domain.x, it->domain.y, 10, direction, 5));
                }

            }

            bool enemyHit = false;
            for (std::list<Bullet>::iterator jit = bullets.begin(); jit != bullets.end(); ++jit) {
                if(CheckCollisionCircleRec(Vector2{jit->x,jit->y}, jit->r, it->domain)){
                    enemyHit = true;
                    break; // No need to check further bullets for this enemy
                }

            }
            if (enemyHit) {
                it = enemies.erase(it);
            }
        }

        for (std::list<Bullet>::iterator jit = bullets.begin(); jit != bullets.end(); ++jit) {
            for(std::list<Wall>::iterator it = walls.begin(); it != walls.end(); ++it){
                if(CheckCollisionCircleRec(Vector2{jit->x,jit->y}, jit->r, it->domain)){
                    jit = bullets.erase(jit);
                }
            }

        }


        for (std::list<Bullet>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); ++it) {
            if(CheckCollisionCircleRec(Vector2{it->x,it->y}, it->r, C.domain)){
                --C.health;
                break; // No need to check further bullets for this enemy
            }
            for (std::list<Wall>::iterator jit = walls.begin(); jit != walls.end(); ++jit) {
                if (CheckCollisionCircleRec(Vector2{it->x,it->y}, it->r, jit->domain)){
                    it = enemyBullets.erase(it);
                }

            }
        }

        for (std::list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
            it->x += it->speed * it->vec.x;
            it->y += it->speed * it->vec.y;
        }
        for (std::list<Bullet>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); ++it) {
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
        for (std::list<Bullet>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); ++it) {
            DrawCircle(it->x,it->y,it->r,YELLOW);
        }
        for (std::list<Wall>::iterator it = walls.begin(); it != walls.end(); ++it) {
            DrawRectangle(it->domain.x,it->domain.y,it->domain.width, it->domain.height, GREEN);
        }
        DrawRectangle(C.domain.x,C.domain.y,C.domain.width,C.domain.height,BLUE);
        DrawRectangle(50,50,10,10,BLACK);

        for(int i = 0; i < C.health; ++i) {
            DrawCircle(15 + i * 25, 15, 10, RED);
        }



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
