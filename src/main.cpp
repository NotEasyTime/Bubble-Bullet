#include "raylib.h"
#include <list>
#include "objects.hpp"

int main() {
    // Get the monitor's resolution
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);

    // Initialize the window with borderless mode at native resolution
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);  // Remove borders and title bar
    InitWindow(screen_width, screen_height, "Raylib Test");

    // Position the window to cover the whole screen
    SetWindowPosition(0, 0);

    // List of Base objects
    std::list<Base> test;
    test.push_back(Base(Rectangle{50, 50, 30, 30}));
    test.push_back(Base(Rectangle{100, 100, 30, 30}));

    while (!WindowShouldClose()) {
        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Loop through the list and draw the rectangles
        for (std::list<Base>::iterator it = test.begin(); it != test.end(); ++it) {
            DrawRectangle(it->domain.x, it->domain.y, it->domain.width, it->domain.height,BLUE);
        }

        EndDrawing();
    }

    // Close the window when done
    CloseWindow();
    return 0;
}
