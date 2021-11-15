#include "raylib.h"
#include "Game.h"

int main() {
    InitWindow(600, 600, "Pong!");
    InitAudioDevice();
    SetTargetFPS(60);

    Sound sfxBallHit = LoadSound(ASSETS_PATH"mixkit-quick-jump-arcade-game-239.wav");
    Sound sfxScore = LoadSound(ASSETS_PATH"mixkit-arcade-fast-game-over-233.wav");
    Music sfxMusic = LoadMusicStream(ASSETS_PATH"mixkit-game-level-music-689.wav");

    PlayMusicStream(sfxMusic);

    GameScreen screen = TITLE;

    Paddle left = CreatePaddle((float)SCREEN_OFFSET/2, KEY_W, KEY_S);
    Paddle right = CreatePaddle((float)GetScreenWidth()-SCREEN_OFFSET, KEY_UP, KEY_DOWN);
    Ball ball = CreateBall();

    while (!WindowShouldClose()) {
        UpdateMusicStream(sfxMusic);

        switch (screen) {
            case TITLE: {
               if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
            } break;
            case GAMEPLAY: {
                ball.position.x += ball.speed.x;
                ball.position.y += ball.speed.y;

                GetPaddleInput(&right);
                GetPaddleInput(&left);

                ClampPaddleWithinFrame(&right);
                ClampPaddleWithinFrame(&left);
                ClampBallVertical(&ball, sfxBallHit);

                if (CheckPaddleBallCollision(&right, &ball) || CheckPaddleBallCollision(&left, &ball)) {
                    PlaySound(sfxBallHit);
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
                    DrawText("PONG!", (float)(GetScreenWidth()-MeasureText("PONG!", 40))/2, (float)GetScreenHeight()/3, 40, WHITE);
                    DrawText("Press Enter To Play!", (float)(GetScreenWidth()-MeasureText("Press Enter To Play!", 20))/2, (float)GetScreenHeight()/2, 20, WHITE);
                    DrawText("Left Paddle: W/S", (float)(GetScreenWidth()-MeasureText("Left Paddle: W/S", 15))/18, (float)GetScreenHeight()/1.5, 15, LIGHTGRAY);
                    DrawText("Right Paddle: Up/Down", (float)(GetScreenWidth()-MeasureText("Right Paddle: Up/Down", 15))/1.05, (float)GetScreenHeight()/1.5, 15, LIGHTGRAY);
                } break;
                case GAMEPLAY: {
                    DrawLineEx((Vector2){(float)GetScreenWidth()/2, 0}, (Vector2){(float)GetScreenWidth()/2, (float)GetScreenHeight()}, 2.5f, WHITE);
                    DrawRectangle(left.position.x, left.position.y, left.width, left.height, WHITE);
                    DrawRectangle(right.position.x, right.position.y, right.width, right.height, WHITE);
                    DrawCircleV(ball.position, ball.radius, WHITE);

                    DrawText(TextFormat("Score: %i", left.score), 10, 20, 30, WHITE);
                    DrawText(TextFormat("Score: %i", right.score), (float)GetScreenWidth()/2+10.0f, 20, 30, WHITE);
                } break;
                default: break;
            }
        EndDrawing();
    }

    UnloadMusicStream(sfxMusic);
    UnloadSound(sfxBallHit);
    UnloadSound(sfxScore);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
