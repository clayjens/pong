#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_OFFSET 20.0f
#define TARGET_FPS 60

typedef enum GameScreen { TITLE = 0, GAMEPLAY } GameScreen;
typedef enum ScoringPosition { LEFT = 0, RIGHT } ScoringPosition;

typedef struct Ball {
    Vector2 origin;
    Vector2 position;
    Vector2 defaultSpeed;
    Vector2 speed;
    float radius;
} Ball;

typedef struct Paddle {
    Vector2 position;
    float speed;
    float width;
    float height;
    int score;
    KeyboardKey Up;
    KeyboardKey Down;
} Paddle;

void CreatePaddle(Paddle *paddle, float xpos, KeyboardKey Up, KeyboardKey Down) {
    paddle->position = (Vector2){ xpos, (float)SCREEN_HEIGHT/2 };
    paddle->speed = 8.0f;
    paddle->width = 10.0f;
    paddle->height = 80.0f;
    paddle->score = 0;
    paddle->Up = Up;
    paddle->Down = Down;
}

void CreateBall(Ball *ball) {
    ball->origin = (Vector2){ (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    ball->position = ball->origin;
    ball->defaultSpeed = (Vector2){ 5.0f, 4.0f };
    ball->speed = ball->defaultSpeed;
    ball->radius = 8.0f;
}

void GetPaddleInput(Paddle *paddle) {
    if (IsKeyDown(paddle->Up)) paddle->position.y -= paddle->speed;
    else if (IsKeyDown(paddle->Down)) paddle->position.y += paddle->speed;
}

void ClampPaddleWithinFrame(Paddle *paddle) {
    if (paddle->position.y > SCREEN_HEIGHT-SCREEN_OFFSET-paddle->height) paddle->position.y = SCREEN_HEIGHT-SCREEN_OFFSET-paddle->height;
    else if (paddle->position.y < SCREEN_OFFSET) paddle->position.y = SCREEN_OFFSET;
}

void ClampBallVertical(Ball *ball, Sound sfxBallHit) {
    if ((ball->position.y >= (SCREEN_WIDTH - ball->radius)) || (ball->position.y <= ball->radius)) {
        PlaySound(sfxBallHit);
        ball->speed.y *= -1.0f;
    }
}

bool CheckPaddleBallCollision(Paddle *paddle, Ball *ball) {
    return CheckCollisionCircleRec(ball->position, ball->radius, (Rectangle){ paddle->position.x, paddle->position.y, paddle->width, paddle->height}) ;
}

void IncreasePaddleScoreAndResetBall(Paddle *paddle, Ball *ball, Sound sfxScore) {
    paddle->score++;
    ball->position = ball->origin;
    ball->speed = ball->defaultSpeed;
    ball->speed.x *= 1.0f;
    ball->speed.y *= 1.0f;
    PlaySound(sfxScore);
}

bool CheckScore(Ball *ball, ScoringPosition scoringPos) {
    if (scoringPos == LEFT && ball->position.x < 0) return true;
    else if (scoringPos == RIGHT && ball->position.x > SCREEN_WIDTH) return true;
    else return false;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong!");
    InitAudioDevice();
    SetTargetFPS(TARGET_FPS);

    Sound sfxPlink = LoadSound(ASSETS_PATH"mixkit-quick-jump-arcade-game-239.wav");
    Sound sfxScore = LoadSound(ASSETS_PATH"mixkit-arcade-fast-game-over-233.wav");
    Music sfxMusic = LoadMusicStream(ASSETS_PATH"mixkit-game-level-music-689.wav");

    PlayMusicStream(sfxMusic);

    GameScreen screen = TITLE;

    Paddle left = { 0 };
    CreatePaddle(&left, (float)SCREEN_OFFSET-10.0f, KEY_W, KEY_S);
    Paddle right = { 0 };
    CreatePaddle(&right, (float)SCREEN_WIDTH-SCREEN_OFFSET, KEY_UP, KEY_DOWN);
    Ball ball = { 0 };
    CreateBall(&ball);

    while (!WindowShouldClose()) {
        UpdateMusicStream(sfxMusic);

        switch (screen) {
            case TITLE: {
               if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
            } break;
            case GAMEPLAY: {
//          UPDATE
                ball.position.x += ball.speed.x;
                ball.position.y += ball.speed.y;

                GetPaddleInput(&right);
                GetPaddleInput(&left);

                ClampPaddleWithinFrame(&right);
                ClampPaddleWithinFrame(&left);
                ClampBallVertical(&ball, sfxPlink);

                if (CheckPaddleBallCollision(&right, &ball) || CheckPaddleBallCollision(&left, &ball)) {
                    PlaySound(sfxPlink);
                    ball.speed.x *= -1.0f;
                }

                if (CheckScore(&ball, LEFT)) IncreasePaddleScoreAndResetBall(&left, &ball, sfxScore);
                if (CheckScore(&ball, RIGHT)) IncreasePaddleScoreAndResetBall(&right, &ball, sfxScore);
            } break;
            default: break;
        }
//  DRAW
        BeginDrawing();
            ClearBackground(BLACK);

            switch (screen) {
                case TITLE: {
                   DrawText("PONG!", 225, 120, 50, WHITE);
                   DrawText("Press Enter To Play!",170, 220, 20, WHITE);
                } break;
                case GAMEPLAY: {
                    DrawLineEx((Vector2){(float)SCREEN_WIDTH/2, 0}, (Vector2){(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT}, 2.5f, WHITE);
                    DrawRectangle(left.position.x, left.position.y, left.width, left.height, WHITE);
                    DrawRectangle(right.position.x, right.position.y, right.width, right.height, WHITE);
                    DrawCircleV(ball.position, ball.radius, WHITE);

                    DrawText(TextFormat("Score: %i", left.score), 10, 20, 30, WHITE);
                    DrawText(TextFormat("Score: %i", right.score), ((float)SCREEN_WIDTH / 2)+10.0f, 20, 30, WHITE);
                } break;
                default: break;
            }
        EndDrawing();
    }

    UnloadMusicStream(sfxMusic);
    UnloadSound(sfxPlink);
    UnloadSound(sfxScore);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
