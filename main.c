#include "raylib.h"
#include <stdio.h>
#define SPEED 400

typedef struct Paddle
{
  Rectangle Rec;
  float speed;
} Paddle;

int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  const float genWidth = screenWidth / 80.0;
  const float paddleWidth = screenWidth / 5.0;
  Paddle paddle = {0};
  paddle.Rec.x = (screenWidth - paddleWidth) / 2;
  paddle.Rec.y = screenHeight - genWidth;
  paddle.Rec.width = paddleWidth;
  paddle.Rec.height = genWidth;
  paddle.speed = 0;

  InitWindow(screenWidth, screenHeight, "Breakout");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())
  {
    float delta = GetFrameTime();
    paddle.speed = 0;
    // Update variables
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
      paddle.speed = -SPEED;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
      paddle.speed = SPEED;
    }

    paddle.Rec.x += paddle.speed * delta;

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);
    // Draw walls
    DrawRectangle(0, 0, screenWidth, genWidth, LIGHTGRAY);
    DrawRectangle(0, 0, genWidth, screenHeight, LIGHTGRAY);
    DrawRectangle(screenWidth - genWidth, 0, genWidth, screenHeight, LIGHTGRAY);
    printf("Rectangle x : %f, y: %f\n", paddle.Rec.x, paddle.Rec.y);
    DrawRectangleRec(paddle.Rec, DARKPURPLE);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
