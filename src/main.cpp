#include "raylib.h"
#include "raymath.h"        // Required for: Vector2Clamp()
#include <list>
#include "objects.hpp"
#include <iostream>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))


// GameState enum
//------------------------------------------------------------------------------------
enum GameState {
    MENU,
    PLAYING,
    BOSS,
    LEVELOC,
    PAUSED,
    GAME_OVER,
    WIN,
    LORE
};

GameState gameState = MENU; // Initialize gameState to MENU
int playerScore = 0;


void ResetGame(Player &player, std::list<Enemy> &enemies, std::list<Bullet> &bullets, std::list<Bullet> &enemyBullets) {
    // Reset player attributes
    player.domain.x = 320;  // Start in the middle of the screen
    player.domain.y = 240;
    player.health = 5;

    // Clear bullets
    bullets.clear();
    enemyBullets.clear();

    // Reset enemies
    enemies.clear();
    enemies.push_back(Enemy(Rectangle{120, 120, 100, 100}));
    enemies.push_back(Enemy(Rectangle{200, 200, 100, 100}));
    enemies.push_back(Enemy(Rectangle{300, 150, 100, 100}));

    // Reset score
    playerScore = 0;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 450;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "Squirrel vs Bess Game");
    SetWindowMinSize(320, 240);

    int gameScreenWidth = 1640;
    int gameScreenHeight = 1480;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    std::list<Wall> walls;
    std::list<Bullet> enemyBullets;
    std::list<Enemy> enemies;
    std::list<Bullet> bullets;
    enemies.push_back(Enemy(Rectangle{120, 120, 200, 200}));
    enemies.push_back(Enemy(Rectangle{120, 140, 200, 200}));
    enemies.push_back(Enemy(Rectangle{140, 120, 200, 200}));
    walls.push_back(Wall(Rectangle{50, 50,50,300}));


    Player C(Rectangle{gameScreenWidth / 2, gameScreenHeight / 2, 306 / 1.5,254 / 1.5});

    // Initialize camera
    Camera2D camera = {0};
    camera.target = (Vector2){ gameScreenWidth / 2.0f, gameScreenHeight / 2.0f };
    camera.offset = (Vector2){ gameScreenWidth / 2.0f, gameScreenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Textures
    Texture2D over_back = LoadTexture("../textures/back_ground_over.png");
    Texture2D chester_norm = LoadTexture("../textures/chester.png");
    Texture2D bee = LoadTexture("../textures/bee.png");
    Texture2D title = LoadTexture("../textures/title.png");
    Texture2D splash = LoadTexture("../textures/splash.png");
    Texture2D boss_back = LoadTexture("../textures/boss_back.png");
    Texture2D bubble = LoadTexture("../textures/bubble.png");
    Texture2D acorn = LoadTexture("../textures/acorn.png");
    Texture2D play = LoadTexture("../textures/play.png");
    Texture2D boss = LoadTexture("../textures/boss.png");
    Texture2D win = LoadTexture("../textures/win.png");
    Texture2D lose = LoadTexture("../textures/lose.png");
    Texture2D one= LoadTexture("../textures/one.png");
    Texture2D two= LoadTexture("../textures/two.png");
    Texture2D three= LoadTexture("../textures/three.png");
    Texture2D four= LoadTexture("../textures/four.png");
    Texture2D five= LoadTexture("../textures/five.png");
    Texture2D six= LoadTexture("../textures/six.png");
    Texture2D sev= LoadTexture("../textures/sev.png");
    Texture2D eight= LoadTexture("../textures/eight.png");
    Texture2D right = LoadTexture("../textures/right.png");
    Texture2D left = LoadTexture("../textures/left.png");
    Texture2D tree = LoadTexture("../textures/tree.png");
    Texture2D credit = LoadTexture("../textures/credit.png");

    one.width = gameScreenWidth;
    two.width = gameScreenWidth;
    three.width = gameScreenWidth;
    four.width = gameScreenWidth;
    five.width = gameScreenWidth;
    six.width = gameScreenWidth;
    sev.width = gameScreenWidth;
    eight.width = gameScreenWidth;

    right.width = 175;
    right.height = 175;
    left.width = 175;
    left.height = 175;

    int loreImg = 1;

    lose.width = gameScreenWidth;
    lose.height = gameScreenHeight;

    win.width = gameScreenWidth;
    win.height = gameScreenHeight;

    boss.width = 350;
    boss.height = 350;

    play.width = 200;
    play.height = 100;

    acorn.width = 100;
    acorn.height = 100;

    bubble.width = 100;
    bubble.height = 100;

    boss_back.width = gameScreenWidth;
    boss_back.height = gameScreenHeight;

    splash.width /= 2;
    splash.height /= 2;

    title.width = gameScreenWidth;
    title.height = gameScreenHeight;

    bee.width /= 2.3;
    bee.height /=2.3;

    chester_norm.width = 306 / 1.5;
    chester_norm.height = 254 / 1.5;

    Enemy BossBee(Rectangle{gameScreenWidth/2+50,gameScreenHeight/2+50, 350,350});
    BossBee.health = 50;


    // Calculate the new width and height
    over_back.width = gameScreenWidth;
    over_back.height = gameScreenHeight;



    // -------------------------------------
    // ORIGINAL CODE: Main Game Loop
    // -------------------------------------
    // Main Game Loop
    InitAudioDevice();

    Sound lore_music = LoadSound("../music/Chester the Squirrel Origin Song.mp3");
    Sound boss_music = LoadSound("../music/Queen Bee Theme.mp3");
    Sound fight_music = LoadSound("../music/Chester The Squiriel - World 1 Music _I'm actually going nuts.mp3");
    Sound start_music = LoadSound("../music/Thank you End screen _ Menu Screen _ Chester Ocean Theme.mp3");

    while (!WindowShouldClose()) {


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


        // Handle pause toggle
        if (IsKeyPressed(KEY_P) && gameState == PLAYING) {
            gameState = PAUSED;
        } else if (IsKeyPressed(KEY_P) && gameState == PAUSED) {
            gameState = PLAYING;
        }

        BeginTextureMode(target);
        BeginMode2D(camera);
        ClearBackground(BLACK);


        if (gameState == MENU) {
            // Main Menu

            if (!IsSoundPlaying(start_music)) {
                PlaySound(start_music);
            }
            if (IsSoundPlaying(boss_music)) {
                StopSound(boss_music);
            }
            if (IsSoundPlaying(fight_music)) {
                StopSound(fight_music);
            }
            if (IsSoundPlaying(lore_music)) {
                StopSound(lore_music);
            }

            DrawTexture(title, 0, 0, WHITE);
            DrawTexture(splash, 150, 20, WHITE);

            //DrawText("Welcome to Squirrel vs Bess!", gameScreenWidth / 2 - MeasureText("Welcome to Squirrel vs Bess!", 20) / 2, 100, 20, WHITE);
            //DrawRectangle(gameScreenWidth / 2 - 50, 600, 100, 50, DARKGRAY);
            DrawTexture(play,gameScreenWidth / 2 - 50, 600,WHITE);
            //DrawText("Start", gameScreenWidth / 2 - MeasureText("Start", 20) / 2, 615, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 50, 600, 200, 100})) {
                    ResetGame(C, enemies, bullets, enemyBullets);
                    gameState = LORE;
                }
            }
        } else if (gameState == PLAYING) {
            ClearBackground(RAYWHITE);

            if (IsSoundPlaying(start_music)) {
                StopSound(start_music);
            }
            if (IsSoundPlaying(boss_music)) {
                StopSound(boss_music);
            }
            if (!IsSoundPlaying(fight_music)) {
                PlaySound(fight_music);
            }
            if (IsSoundPlaying(lore_music)) {
                StopSound(lore_music);
            }


            DrawTexture(over_back, 0, 0, WHITE);

            // -------------------------------------
            // Player Movement
            // -------------------------------------
            if (IsKeyDown(KEY_W)) C.domain.y = MAX(C.domain.y - 5, 0);
            if (IsKeyDown(KEY_S)) C.domain.y = MIN(C.domain.y + 5, gameScreenHeight - C.domain.height);
            if (IsKeyDown(KEY_A)) C.domain.x = MAX(C.domain.x - 5, 0);
            if (IsKeyDown(KEY_D)) C.domain.x = MIN(C.domain.x + 5, gameScreenWidth - C.domain.width);

            // camera.target.x = C.domain.x;
            //camera.target.y = C.domain.y;

            // -------------------------------------
            // Player Shooting
            // -------------------------------------
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                Vector2 direction = Vector2Normalize(Vector2Subtract(mousePos, (Vector2){C.domain.x + C.domain.width / 2, C.domain.y + C.domain.height / 2}));
                bullets.push_back(Bullet(C.domain.x + C.domain.width / 2, C.domain.y + C.domain.height / 2, 35, direction, 5));
            }

            // -------------------------------------
            // Update Game Logic
            // -------------------------------------
            // Update Bullets
            for (auto it = bullets.begin(); it != bullets.end();) {
                it->x += it->speed * it->vec.x;
                it->y += it->speed * it->vec.y;

                if (it->x < 0 || it->x > gameScreenWidth || it->y < 0 || it->y > gameScreenHeight) {
                    it = bullets.erase(it);
                } else {
                    ++it;
                }
            }

            // Update Enemies
            for (auto &enemy : enemies) {
                if (!enemy.traveling) {
                    enemy.dest.x = GetRandomValue(0, gameScreenWidth - enemy.domain.width);
                    enemy.dest.y = GetRandomValue(0, gameScreenHeight - enemy.domain.height);
                    enemy.traveling = true;
                }

                float speed = 2.0f;
                if (enemy.domain.x < enemy.dest.x) enemy.domain.x += speed;
                else if (enemy.domain.x > enemy.dest.x) enemy.domain.x -= speed;

                if (enemy.domain.y < enemy.dest.y) enemy.domain.y += speed;
                else if (enemy.domain.y > enemy.dest.y) enemy.domain.y -= speed;

                if (fabs(enemy.domain.x - enemy.dest.x) < speed && fabs(enemy.domain.y - enemy.dest.y) < speed) {
                    enemy.traveling = false;
                }

                // Enemy Shooting
                if (GetRandomValue(0, 200) < 1) {
                    Vector2 direction = Vector2Subtract((Vector2){C.domain.x, C.domain.y}, (Vector2){enemy.domain.x, enemy.domain.y});
                    direction = Vector2Normalize(direction);
                    enemyBullets.push_back(Bullet(enemy.domain.x + enemy.domain.width / 2, enemy.domain.y + enemy.domain.height / 2, 35, direction, 3));
                }
            }

            // Update Enemy Bullets
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
                it->x += it->speed * it->vec.x;
                it->y += it->speed * it->vec.y;

                if (CheckCollisionCircleRec((Vector2){it->x, it->y}, it->r, C.domain)) {
                    C.health--;
                    it = enemyBullets.erase(it);
                } else if (it->x < 0 || it->x > gameScreenWidth || it->y < 0 || it->y > gameScreenHeight) {
                    it = enemyBullets.erase(it);
                } else {
                    ++it;
                }
            }

            // Collision Detection
            for (auto enemy = enemies.begin(); enemy != enemies.end();) {
                bool enemyHit = false;
                for (auto bullet = bullets.begin(); bullet != bullets.end();) {
                    if (CheckCollisionCircleRec((Vector2){bullet->x, bullet->y}, bullet->r, enemy->domain)) {
                        bullet = bullets.erase(bullet);
                        enemyHit = true;
                        break;
                    } else {
                        ++bullet;
                    }
                }

                if (enemyHit) {
                    enemy = enemies.erase(enemy);
                    playerScore += 10;
                } else {
                    ++enemy;
                }
            }

            if(playerScore < 100 && enemies.size() < 5){
                enemies.push_back(Enemy(Rectangle{GetRandomValue(0, gameScreenWidth),GetRandomValue(0, gameScreenHeight), 200,200}));
            }

            // Check Win Condition
            if (enemies.empty()) {
                gameState = LEVELOC;
            }

            // Draw Game Elements
            for (auto &enemy : enemies) {
                DrawTexture(bee, enemy.domain.x, enemy.domain.y, WHITE);
            }
            for (auto &bullet : bullets) {
                DrawTexture(acorn, bullet.x, bullet.y, WHITE);
            }
            for (auto &bullet : enemyBullets) {
                DrawCircle(bullet.x + 50,bullet.y + 50, 50, WHITE);
                DrawTexture(bubble,bullet.x, bullet.y, WHITE);

            }
            DrawTexture(chester_norm, C.domain.x, C.domain.y, WHITE);

            // Draw Health and Score
            for (int i = 0; i < C.health; ++i) {
                DrawCircle(15 + i * 25, 15, 10, RED);
            }
            DrawText(TextFormat("Score: %d", playerScore), 10, 40, 20, BLACK);

            if (C.health <= 0) {
                gameState = GAME_OVER;
            }
        } else if (gameState == PAUSED) {
            // Pause Screen
            ClearBackground(RAYWHITE);
            DrawText("PAUSED", gameScreenWidth / 2 - MeasureText("PAUSED", 40) / 2, 100, 40, YELLOW);

            DrawRectangle(gameScreenWidth / 2 - 60, 200, 120, 50, DARKGRAY);
            DrawText("Resume", gameScreenWidth / 2 - MeasureText("Resume", 20) / 2, 215, 20, WHITE);

            DrawRectangle(gameScreenWidth / 2 - 60, 300, 120, 50, DARKGRAY);
            DrawText("Main Menu", gameScreenWidth / 2 - MeasureText("Main Menu", 20) / 2, 315, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 200, 120, 50})) {
                    gameState = PLAYING;
                } else if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 300, 120, 50})) {
                    gameState = MENU;
                }
            }
        } else if (gameState == GAME_OVER) {
            ClearBackground(WHITE);
            DrawTexture(lose,0,0,WHITE);
            DrawText("GAME OVER", gameScreenWidth / 2 - MeasureText("GAME OVER", 40) / 2, 100, 40, RED);
            DrawText(TextFormat("Your Score: %d", playerScore), gameScreenWidth / 2 - MeasureText(TextFormat("Your Score: %d", playerScore), 20) / 2, 160, 20, WHITE);

            DrawRectangle(gameScreenWidth / 2 - 60, 240, 120, 50, DARKGRAY);
            DrawText("Restart", gameScreenWidth / 2 - MeasureText("Restart", 20) / 2, 255, 20, WHITE);

            DrawRectangle(gameScreenWidth / 2 - 60, 310, 120, 50, DARKGRAY);
            DrawText("Exit", gameScreenWidth / 2 - MeasureText("Exit", 20) / 2, 325, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 240, 120, 50})) {
                    ResetGame(C, enemies, bullets, enemyBullets);
                    gameState = PLAYING;
                } else if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 310, 120, 50})) {
                    break;
                }
            }
        } else if (gameState == WIN) {


            BossBee.health = 50;
            ClearBackground(WHITE);
            DrawTexture(win,0,0,WHITE);
            DrawText("CONGRATULATIONS!", gameScreenWidth / 2 - MeasureText("CONGRATULATIONS!", 40) / 2, 100, 40, GREEN);
            DrawText(TextFormat("Your Score: %d", playerScore), gameScreenWidth / 2 - MeasureText(TextFormat("Your Score: %d", playerScore), 20) / 2, 160, 20, WHITE);

            DrawRectangle(gameScreenWidth / 2 - 60, 240, 120, 50, DARKGRAY);
            DrawText("Restart", gameScreenWidth / 2 - MeasureText("Restart", 20) / 2, 255, 20, WHITE);

            DrawRectangle(gameScreenWidth / 2 - 60, 310, 120, 50, DARKGRAY);
            DrawText("Exit", gameScreenWidth / 2 - MeasureText("Exit", 20) / 2, 325, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 240, 120, 50})) {
                    ResetGame(C, enemies, bullets, enemyBullets);
                    gameState = PLAYING;
                } else if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 60, 310, 120, 50})) {
                    break;
                }
            }
        } else if(gameState == BOSS){

            if (IsSoundPlaying(start_music)) {
                StopSound(start_music);
            }
            if (!IsSoundPlaying(boss_music)) {
                PlaySound(boss_music);
            }
            if (IsSoundPlaying(fight_music)) {
                StopSound(fight_music);
            }
            if (IsSoundPlaying(lore_music)) {
                StopSound(lore_music);
            }

            ClearBackground(RAYWHITE);
            DrawTexture(boss_back, 0, 0, WHITE);

            if (!BossBee.traveling) {
                BossBee.dest.x = GetRandomValue(0, gameScreenWidth - BossBee.domain.width);
                BossBee.dest.y = GetRandomValue(0, gameScreenHeight - BossBee.domain.height);
                BossBee.traveling = true;
            }

            float speed = 2.0f;
            if (BossBee.domain.x < BossBee.dest.x){ BossBee.domain.x += speed;}
            else if (BossBee.domain.x > BossBee.dest.x) {BossBee.domain.x -= speed;}

            if (BossBee.domain.y < BossBee.dest.y){ BossBee.domain.y += speed;}
            else if (BossBee.domain.y > BossBee.dest.y){ BossBee.domain.y -= speed;}

            if (fabs(BossBee.domain.x - BossBee.dest.x) < speed && fabs(BossBee.domain.y - BossBee.dest.y) < speed) {
                BossBee.traveling = false;
            }

            // Boss Shooting
            if (GetRandomValue(0, 50) < 1) {
                Vector2 direction = Vector2Subtract((Vector2){C.domain.x, C.domain.y}, (Vector2){BossBee.domain.x, BossBee.domain.y});
                direction = Vector2Normalize(direction);

                // Normal direction bullet
                enemyBullets.push_back(Bullet(BossBee.domain.x + BossBee.domain.width / 2, BossBee.domain.y + BossBee.domain.height / 2, 35, direction, 3));

                // Opposite direction bullet
                enemyBullets.push_back(Bullet(BossBee.domain.x + BossBee.domain.width / 2, BossBee.domain.y + BossBee.domain.height / 2, 35, Vector2Scale(direction, -1), 3));

                // Perpendicular direction bullet
                Vector2 perpendicularDirection = (Vector2){-direction.y, direction.x}; // 90 degree clockwise rotation
                enemyBullets.push_back(Bullet(BossBee.domain.x + BossBee.domain.width / 2, BossBee.domain.y + BossBee.domain.height / 2, 35, perpendicularDirection, 3));

                // Negated perpendicular direction bullet
                Vector2 negatedPerpendicularDirection = (Vector2){direction.y, -direction.x}; // 90 degree counter-clockwise rotation
                enemyBullets.push_back(Bullet(BossBee.domain.x + BossBee.domain.width / 2, BossBee.domain.y + BossBee.domain.height / 2, 35, negatedPerpendicularDirection, 3));
            }


            DrawTexture(boss, BossBee.domain.x, BossBee.domain.y,WHITE);

            // -------------------------------------
            // Player Movement
            // -------------------------------------
            if (IsKeyDown(KEY_W)) C.domain.y = MAX(C.domain.y - 5, 0);
            if (IsKeyDown(KEY_S)) C.domain.y = MIN(C.domain.y + 5, gameScreenHeight - C.domain.height);
            if (IsKeyDown(KEY_A)) C.domain.x = MAX(C.domain.x - 5, 0);
            if (IsKeyDown(KEY_D)) C.domain.x = MIN(C.domain.x + 5, gameScreenWidth - C.domain.width);

            // camera.target.x = C.domain.x;
            //camera.target.y = C.domain.y;

            // -------------------------------------
            // Player Shooting
            // -------------------------------------
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                Vector2 direction = Vector2Normalize(Vector2Subtract(mousePos, (Vector2){C.domain.x + C.domain.width / 2, C.domain.y + C.domain.height / 2}));
                bullets.push_back(Bullet(C.domain.x + C.domain.width / 2, C.domain.y + C.domain.height / 2, 35, direction, 5));
            }

            DrawTexture(chester_norm, C.domain.x, C.domain.y, WHITE);

            // Draw Health and Score
            for (int i = 0; i < C.health; ++i) {
                DrawCircle(15 + i * 25, 15, 10, RED);
            }

            for (int i = 0; i < BossBee.health; ++i) {
                DrawCircle(gameScreenWidth -25 - i * 25, 25, 20, RED);
            }



            if (C.health <= 0) {
                gameState = GAME_OVER;
            }

            // Update Bullets
            for (auto it = bullets.begin(); it != bullets.end();) {
                it->x += it->speed * it->vec.x;
                it->y += it->speed * it->vec.y;

                if (it->x < 0 || it->x > gameScreenWidth || it->y < 0 || it->y > gameScreenHeight) {
                    it = bullets.erase(it);
                } else {
                    ++it;
                }
            }

            // Update Enemy Bullets
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
                it->x += it->speed * it->vec.x;
                it->y += it->speed * it->vec.y;

                if (CheckCollisionCircleRec((Vector2){it->x, it->y}, it->r, C.domain)) {
                    C.health--;
                    it = enemyBullets.erase(it);
                } else if (it->x < 0 || it->x > gameScreenWidth || it->y < 0 || it->y > gameScreenHeight) {
                    it = enemyBullets.erase(it);
                } else {
                    ++it;
                }
            }

            // Collision Detection

                bool enemyHit = false;
                for (auto bullet = bullets.begin(); bullet != bullets.end();) {
                    if (CheckCollisionCircleRec((Vector2){bullet->x, bullet->y}, bullet->r, BossBee.domain)) {
                        bullet = bullets.erase(bullet);
                        enemyHit = true;
                        break;
                    } else {
                        ++bullet;
                    }
                }

                if (enemyHit) {
                    playerScore += 10;
                    --BossBee.health;
                }


            for (auto &bullet : bullets) {
                DrawTexture(acorn, bullet.x, bullet.y, WHITE);
            }
            for (auto &bullet : enemyBullets) {
                DrawCircle(bullet.x + 50,bullet.y + 50, 50, WHITE);
                DrawTexture(bubble,bullet.x, bullet.y, WHITE);

            }

            if(BossBee.health == 0){
                gameState = WIN;
            }




        } else if(gameState == LEVELOC){
            ClearBackground(RAYWHITE);
            DrawText("You bee-t the first level!!",gameScreenWidth /2 - 50, gameScreenHeight /2, 30, BLACK);
            //DrawText("Welcome to Squirrel vs Bess!", gameScreenWidth / 2 - MeasureText("Welcome to Squirrel vs Bess!", 20) / 2, 100, 20, WHITE);
            DrawRectangle(gameScreenWidth / 2 - 50, 900, 100, 50, DARKGRAY);
            DrawText("Next", gameScreenWidth / 2 - MeasureText("Start", 20) / 2, 915, 20, WHITE);

            DrawTexture(tree,50,300,WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 50, 900, 100, 50})) {
                    ResetGame(C, enemies, bullets, enemyBullets);
                    gameState = BOSS;
                }
            }
        } else if(gameState == LORE){
            ClearBackground(RAYWHITE);

            if (IsSoundPlaying(start_music)) {
                StopSound(start_music);
            }
            if (IsSoundPlaying(boss_music)) {
                StopSound(boss_music);
            }
            if (IsSoundPlaying(fight_music)) {
                StopSound(fight_music);
            }
            if (!IsSoundPlaying(lore_music)) {
                PlaySound(lore_music);
            }


            switch(loreImg){
                case(1):
                    DrawTexture(one,0,0,WHITE);
                    DrawText("Once upon a time, the world was round,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("sealed in a bubble, safe and sound.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(2):
                    DrawTexture(two,0,0,WHITE);
                    DrawText("Inside this world lived Chester Squirrel,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("with fur as black as midnights swirl.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(3):
                    DrawTexture(three,0,0,WHITE);
                    DrawText("But peace was shattered, one fateful day,",gameScreenWidth/2 -500,1000,50,BLACK);
                    DrawText("when the Queen Bee rose to seize her way.",gameScreenWidth/2 - 500,1050,50,BLACK);
                    break;
                case(4):
                    DrawTexture(four,0,0,WHITE);
                    DrawText("Her honey bubbles filled the air,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("trapping creatures everywhere.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(5):
                    DrawTexture(five,0,0,WHITE);
                    DrawText("She sought to coat the woods in gold,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("a sticky prison, cruel and cold.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(6):
                    DrawTexture(six,0,0,WHITE);
                    DrawText("Chester saw his friends confined,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("and vowed to free them, heart and mind.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(7):
                    DrawTexture(sev,0,0,WHITE);
                    DrawText("With nuts in paw and sling held tight,",gameScreenWidth/2 -400,1000,50,BLACK);
                    DrawText("he set out popping each bubble in sight.",gameScreenWidth/2 - 400,1050,50,BLACK);
                    break;
                case(8):
                    DrawTexture(eight,0,0,WHITE);
                    DrawText("Through honeyed chaos, Chester fights,",gameScreenWidth/2 -450,1020,50,BLACK);
                    DrawText("to save the forest and make things right.",gameScreenWidth/2 - 450,1070,50,BLACK);
                    break;
                case(9):
                    DrawTexture(credit,50,50,WHITE);
                    break;

            }
            DrawTexture(play,gameScreenWidth / 2 - 50, 1350,WHITE);
            DrawTexture(left,100,1000,WHITE);
            DrawTexture(right,gameScreenWidth - 200,1000,WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = virtualMouse;
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth / 2 - 50, 1350, 200, 100})) {
                    gameState = PLAYING;
                }
                if (CheckCollisionPointRec(mousePos, (Rectangle){150,1000,175,175}) && loreImg > 1) {
                    --loreImg;
                }
                if (CheckCollisionPointRec(mousePos, (Rectangle){gameScreenWidth-150,1000,175,175}) && loreImg < 9) {
                    ++loreImg;
                }
            }

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
    CloseAudioDevice();



    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
