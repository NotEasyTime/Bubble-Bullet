#include "raylib.h"
#include <list>
#include "objects.hpp"

int main() {
    // Initialize the window with the desired size
    InitWindow(1600, 900, "Raylib Test");

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
