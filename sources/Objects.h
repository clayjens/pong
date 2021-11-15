#ifndef PONG_OBJECTS_H
#define PONG_OBJECTS_H

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

#endif //PONG_OBJECTS_H
