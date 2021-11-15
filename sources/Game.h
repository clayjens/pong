#include "raylib.h"
#include "Objects.h"
#include "Types.h"

#define SCREEN_OFFSET 20.0f

#ifndef PONG_LOGIC_H
#define PONG_LOGIC_H
Paddle CreatePaddle(float xpos, KeyboardKey Up, KeyboardKey Down) {
    Paddle temp = {
        .position=(Vector2){ xpos, (float)GetScreenHeight()/2 },
        .speed=8.0f,
        .width=10.0f,
        .height=80.0f,
        .score=0,
        .Up=Up,
        .Down=Down
    };
    return temp;
}

Ball CreateBall() {
    Ball temp = {
        .origin=(Vector2){ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 },
        .position=temp.origin,
        .defaultSpeed=(Vector2){ 5.0f, 4.0f },
        .speed=temp.defaultSpeed,
        .radius=8.0f,
    };
    return temp;
}

void GetPaddleInput(Paddle *paddle) {
    if (IsKeyDown(paddle->Up)) paddle->position.y -= paddle->speed;
    else if (IsKeyDown(paddle->Down)) paddle->position.y += paddle->speed;
}

void ClampPaddleWithinFrame(Paddle *paddle) {
    if (paddle->position.y > GetScreenHeight()-SCREEN_OFFSET-paddle->height) paddle->position.y = GetScreenHeight()-SCREEN_OFFSET-paddle->height;
    else if (paddle->position.y < SCREEN_OFFSET) paddle->position.y = SCREEN_OFFSET;
}

void ClampBallVertical(Ball *ball, Sound sfxBallHit) {
    if ((ball->position.y >= (GetScreenWidth() - ball->radius)) || (ball->position.y <= ball->radius)) {
        PlaySound(sfxBallHit);
        ball->speed.y *= -1.0f;
    }
}

bool CheckPaddleBallCollision(Paddle *paddle, Ball *ball) {
    // FIXME: There's a bug here that causes the ball to bounce multiple times while caught IN the paddle rect.
    // NOTE: It might be possible to fix this by creating a plane in front of the paddle that the ball checks collision for rather than the paddle's entire rect itself.

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
    else if (scoringPos == RIGHT && ball->position.x > GetScreenWidth()) return true;
    else return false;
}
#endif //PONG_LOGIC_H
