#include "raylib.h"

int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  float wallWidth = screenWidth / 100.0;

  InitWindow(screenWidth, screenHeight, "Breakout");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) 
  {
    // Update variables
    //----------------------------------------------------------------------------------


    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);
        // Draw walls
        DrawRectangle(0, 0, screenWidth, wallWidth, LIGHTGRAY);
        DrawRectangle(0, 0, wallWidth, screenHeight, LIGHTGRAY);
        DrawRectangle(screenWidth - wallWidth, 0, wallWidth, screenHeight, LIGHTGRAY);



    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
