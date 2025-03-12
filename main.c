#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SPEED 400

typedef struct Paddle
{
  Rectangle Rec;
  float speed;
} Paddle;

typedef struct Ball
{
  Vector2 pos;
  float radius;
  Vector2 speed;
} Ball;

typedef struct Brick
{
  Rectangle Rec;
} Brick;

const int screenWidth = 800;
const int screenHeight = 450;

const float genWidth = screenWidth / 80.0;
const float paddleWidth = screenWidth / 5.0;
const float ballRadius = screenWidth / 80.0;

void Reset(Ball *ball, Paddle *paddle)
{
  paddle->Rec.x = (screenWidth - paddleWidth) / 2;
  paddle->Rec.y = screenHeight - genWidth;
  ball->pos.x = (screenWidth - ballRadius) / 2;
  ball->pos.y = (screenHeight - ballRadius) / 2;
  ball->speed.x = (200 + rand() % 20) * (1 - 2 * (rand() % 2));
  ball->speed.y = 150 + rand() % 20;
  for (int i = 3; i > 0; i--)
  {
    time_t timer = time(NULL);
    while (time(NULL) - timer < 1) // Wait for 1 second
    {
      BeginDrawing();
      ClearBackground(LIGHTGRAY);
      char buf[3]; // Buffer size should be enough for 2 digits and null terminator
      sprintf(buf, "%d", i);
      DrawText(buf, screenWidth / 2.0 - 10, screenHeight / 2.0 - 10, 40, GRAY);
      EndDrawing();
    }
  }
}

int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  // Paddle
  Paddle paddle = {0};
  paddle.Rec.x = (screenWidth - paddleWidth) / 2;
  paddle.Rec.y = screenHeight - genWidth;
  paddle.Rec.width = paddleWidth;
  paddle.Rec.height = genWidth;
  paddle.speed = 0;
  // Ball
  Ball ball = {0};
  ball.pos.x = (screenWidth - ballRadius) / 2;
  ball.pos.y = (screenHeight - ballRadius) / 2;
  ball.radius = ballRadius;
  ball.speed.x = 200 + rand() % 20;
  ball.speed.y = 150 + rand() % 20;
  // Wall
  Rectangle walls[3] = {
      {0, 0, genWidth, screenHeight},
      {screenWidth - genWidth, 0, genWidth, screenHeight},
      {0, 0, screenWidth, genWidth},
  };

  // Bricks
  Brick bricks[5];
  for (int i = 0; i < 5; i++)
  {
    bricks[i].Rec.height = 30;
    bricks[i].Rec.width = 100;
    bricks[i].Rec.x = 20 + i * 110;
    bricks[i].Rec.y = 100;
  }

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
    if (IsKeyDown(KEY_R))
    {
      Reset(&ball, &paddle);
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
      paddle.speed = -SPEED;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
      paddle.speed = SPEED;
    }
    if (CheckCollisionCircleRec(ball.pos, ball.radius, walls[0]) ||
        CheckCollisionCircleRec(ball.pos, ball.radius, walls[1]))
    {
      ball.speed.x *= -1;
    }
    if (CheckCollisionCircleRec(ball.pos, ball.radius, walls[2]) ||
        CheckCollisionCircleRec(ball.pos, ball.radius, paddle.Rec))
    {
      ball.speed.y *= -1;
    }

    paddle.Rec.x += paddle.speed * delta;
    if (CheckCollisionRecs(paddle.Rec, walls[0]) || CheckCollisionRecs(paddle.Rec, walls[1]))
    {
      paddle.Rec.x -= paddle.speed * delta;
    }
    ball.pos.x += ball.speed.x * delta;
    ball.pos.y += ball.speed.y * delta;

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);
    // Draw walls
    DrawRectangleRec(walls[0], LIGHTGRAY);
    DrawRectangleRec(walls[1], LIGHTGRAY);
    DrawRectangleRec(walls[2], LIGHTGRAY);
    for (int i = 0; i < 5; i++)
    {
      DrawRectangleRec(bricks[i].Rec, RED);
    }
    DrawCircleV(ball.pos, ball.radius, GRAY);
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
