#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SPEED 400
#define HORIZONTAL 1
#define VERTICAL 2
#define HIDE 1
#define SHOW 0
#define PARTICLE 2
#define HARD 1
#define SOFT 2

typedef struct Paddle
{
  Rectangle Rec;
  float speed;
  int score;
  int lives;
} Paddle;

typedef struct Ball
{
  Vector2 pos;
  float radius;
  Vector2 speed;
} Ball;

typedef struct Particle
{
  Rectangle Rec;
  Vector2 speed;
  float lifetime;
} Particle;

typedef struct Brick
{
  Rectangle Rec;
  int state;
  Particle **particles;
} Brick;

const int screenWidth = 800;
const int screenHeight = 450;

const float genWidth = screenWidth / 80.0;
const float paddleWidth = screenWidth / 5.0;
const float ballRadius = screenWidth / 80.0;

void Reset(Ball *ball, Paddle *paddle, Brick *bricks, int n, int type);
int CheckCollisionBallBrick(Ball ball, Brick bricks[], int n);
void ParticleInit(Brick bricks[], int i);

int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  int HIGHSCORE = 0;
  // Paddle
  Paddle paddle = {0};
  paddle.Rec.x = (screenWidth - paddleWidth) / 2;
  paddle.Rec.y = screenHeight - genWidth;
  paddle.Rec.width = paddleWidth;
  paddle.Rec.height = genWidth;
  paddle.speed = 0;
  paddle.score = 0;
  paddle.lives = 3;
  // Ball
  Ball ball = {0};
  ball.pos.x = (screenWidth - ballRadius) / 2;
  ball.pos.y = (screenHeight - ballRadius) / 2;
  ball.radius = ballRadius;
  srand(time(NULL));
  ball.speed.x = 200 + rand() % 20;
  ball.speed.y = 150 + rand() % 20;
  // Wall
  Rectangle walls[3] = {
      {0, 0, genWidth, screenHeight},
      {screenWidth - genWidth, 0, genWidth, screenHeight},
      {0, 0, screenWidth, genWidth},
  };

  // Bricks
  Brick bricks[7] = {0};
  for (int i = 0; i < 7; i++)
  {
    bricks[i].Rec.height = 30;
    bricks[i].Rec.width = 100;
    bricks[i].Rec.x = 20 + i * 110;
    bricks[i].Rec.y = 100;
    int h = bricks[i].Rec.height;
    int w = bricks[i].Rec.width;
    Particle **particles = (Particle **)malloc(bricks[i].Rec.height * sizeof(Particle *));
    // printf("Brick x: %2f y; %2f\n", bricks[i].Rec.x, bricks[i].Rec.y);
    for (int j = 0; j < bricks[i].Rec.height; j++)
    {
      particles[j] = (Particle *)malloc(bricks[i].Rec.width * sizeof(Particle));
      for (int k = 0; k < bricks[i].Rec.width; k++)
      {
        particles[j][k].Rec.x = bricks[i].Rec.x + k;
        particles[j][k].Rec.y = bricks[i].Rec.y + j;
        particles[j][k].Rec.height = 1;
        particles[j][k].Rec.width = 1;
        particles[j][k].lifetime = 0.4f;
        particles[j][k].speed.x = 0.0f;
        particles[j][k].speed.y = 0.0f;
        // printf("particle %d: %2f %2f \n", k, particles[j][k].pos.x, particles[j][k].pos.y);
      }
      // printf("--------------------------------------------------\n");
    }
    bricks[i].particles = particles;
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
      if (paddle.score > HIGHSCORE)
      {
        HIGHSCORE = paddle.score;
      }
      Reset(&ball, &paddle, bricks, 7, HARD);
    }
    if (ball.pos.y >= screenHeight)
    {
      if (paddle.lives < 2)
      {
        if (paddle.score > HIGHSCORE)
        {
          HIGHSCORE = paddle.score;
        }
        Reset(&ball, &paddle, bricks, 7, HARD);
      }
      else
      {
        Reset(&ball, &paddle, bricks, 7, SOFT);
      }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
      paddle.speed = -SPEED;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
      paddle.speed = SPEED;
    }

    // Particle
    //----------------------------------------------------------------------------------
    for (int i = 0; i < 7; i++)
    {
      if (bricks[i].state == PARTICLE)
      {

        for (int j = 0; j < bricks[i].Rec.height; j++)
        {
          for (int k = 0; k < bricks[i].Rec.width; k++)
          {
            if (bricks[i].particles[j][k].lifetime < 0)
            {
              bricks[i].state = HIDE;
            }
            bricks[i].particles[j][k].Rec.x += bricks[i].particles[j][k].speed.x * delta;
            bricks[i].particles[j][k].Rec.y += bricks[i].particles[j][k].speed.y * delta;
            bricks[i].particles[j][k].lifetime -= delta;
            // printf("Particle lifetime: %.2f\n", delta);
          }
        }
      }
    }

    // Collision
    //----------------------------------------------------------------------------------
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

    int bounce = CheckCollisionBallBrick(ball, bricks, 7);

    if (bounce == HORIZONTAL)
    {
      ball.speed.x *= -1;
      paddle.score++;
    }
    if (bounce == VERTICAL)
    {
      ball.speed.y *= -1;
      paddle.score++;
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
    for (int i = 0; i < 7; i++)
    {
      if (bricks[i].state == SHOW)
      {
        DrawRectangleRec(bricks[i].Rec, RED);
      }
      if (bricks[i].state == PARTICLE)
      {
        for (int j = 0; j < bricks[i].Rec.height; j++)
        {
          for (int k = 0; k < bricks[i].Rec.width; k++)
          {
            DrawRectangleRec(bricks[i].particles[j][k].Rec, RED);
          }
        }
      }
    }
    DrawText(TextFormat("SCORE: %i", paddle.score), 20, 20, 28, GREEN);
    DrawText(TextFormat("LIVES: %i", paddle.lives), 20, 50, 28, GREEN);
    DrawText(TextFormat("HIGH SCORE: %i", HIGHSCORE), 500, 20, 28, GREEN);
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
void Reset(Ball *ball, Paddle *paddle, Brick *bricks, int n, int type)
{
  if (type == HARD)
  {
    paddle->score = 0;
    paddle->lives = 3;
    for (int i = 0; i < n; i++)
    {
      bricks[i].state = SHOW;
      for (int j = 0; j < bricks[i].Rec.height; j++)
      {
        for (int k = 0; k < bricks[i].Rec.width; k++)
        {
          bricks[i].particles[j][k].Rec.x = bricks[i].Rec.x + k;
          bricks[i].particles[j][k].Rec.y = bricks[i].Rec.y + j;
          bricks[i].particles[j][k].lifetime = 0.4f;
          bricks[i].particles[j][k].speed.x = 0.0f;
          bricks[i].particles[j][k].speed.y = 0.0f;
        }
      }
    }
  }
  if (type == SOFT)
  {
    paddle->lives -= 1;
  }
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
int CheckCollisionBallBrick(Ball ball, Brick bricks[], int n)
{
  for (int i = 0; i < n; i++)
  {
    if (bricks[i].state == HIDE || bricks[i].state == PARTICLE)
    {
      continue;
    }

    Rectangle brick = bricks[i].Rec;

    float recCenterX = brick.x + brick.width / 2.0f;
    float recCenterY = brick.y + brick.height / 2.0f;

    float dx = fabsf(ball.pos.x - recCenterX);
    float dy = fabsf(ball.pos.y - recCenterY);

    if (dx > (brick.width / 2.0f + ball.radius))
    {
      continue;
    }
    if (dy > (brick.height / 2.0f + ball.radius))
    {
      continue;
    }

    if (dx < brick.width / 2.0f)
    {
      bricks[i].state = PARTICLE;
      ParticleInit(bricks, i);
      return VERTICAL;
    }
    else
    {
      bricks[i].state = PARTICLE;
      ParticleInit(bricks, i);
      return HORIZONTAL;
    }
  }
  return 0;
}

void ParticleInit(Brick bricks[], int i)
{

  float CentreX = bricks[i].Rec.x + bricks[i].Rec.width / 2.0f;
  float CentreY = bricks[i].Rec.y + bricks[i].Rec.height / 2.0f;

  for (int j = 0; j < bricks[i].Rec.height; j++)
  {
    for (int k = 0; k < bricks[i].Rec.width; k++)
    {
      bricks[i].particles[j][k].speed.x =
          (bricks[i].particles[j][k].Rec.x - CentreX) / bricks[i].particles[j][k].Rec.width;
      bricks[i].particles[j][k].speed.y =
          (bricks[i].particles[j][k].Rec.y - CentreY) / bricks[i].particles[j][k].Rec.height;
      // printf("%f %f\n", bricks[i].particles[j][k].speed.x, bricks[i].particles[j][k].speed.y);
    }
  }
}
// EXPERIMENTAL
//  float brickX = bricks[i].particles[j][k].Rec.x;
//  float brickY = bricks[i].particles[j][k].Rec.y;
//  float sagittaX = fabs(CentreX - brickX);
//  float sagittaY = fabs(CentreY - brickY);
//  float disX = sqrtf(8 * bricks[i].Rec.width - sagittaX * sagittaX);
//  float disY = sqrtf(8 * bricks[i].Rec.height - sagittaY * sagittaY);
//  bricks[i].particles[j][k].speed.x = disX / bricks[i].particles[j][k].lifetime;
//  bricks[i].particles[j][k].speed.y = disY / bricks[i].particles[j][k].lifetime;
//        if (bricks[i].particles[j][k].Rec.x < CentreX) {
//            bricks[i].particles[j][k].speed.x *= -1;
//        }
//        if (bricks[i].particles[j][k].Rec.y < CentreY) {
//            bricks[i].particles[j][k].speed.y *= -1;
//        }
