#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

static Color generateRandomColor(float saturation, float value) {
    return ColorFromHSV((float)(rand() % 360), saturation, value);
}

static void applyImpulse(Vector2 ballAPos, Vector2 *ballAVelocity, Vector2 ballBPos, Vector2 *ballBVelocity) {
    Vector2 normal = Vector2Normalize(Vector2Subtract(ballBPos, ballAPos));
    Vector2 relativeVelocity = Vector2Subtract(*ballBVelocity, *ballAVelocity);
    float velocityInNormal = Vector2DotProduct(relativeVelocity, normal);
    if (velocityInNormal > 0) {
        // Balls separating
        return;
    }

    float j = -velocityInNormal;
    Vector2 impulse = { normal.x * j, normal.y * j };

    *ballAVelocity = Vector2Subtract(*ballAVelocity, impulse);
    *ballBVelocity = Vector2Add(*ballBVelocity, impulse);
}

static void checkCollisions(int count, Vector2 ballsPos[], Vector2 ballsVelocity[], int radius) {
    if (count < 2) return;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (CheckCollisionCircles(ballsPos[i], radius, ballsPos[j], radius)) {
                applyImpulse(ballsPos[i], &ballsVelocity[i], ballsPos[j], &ballsVelocity[j]);
            }
        }
    }
}

static float randomVelocity(int minValue, int maxValue, int precision) {
    long carry = 1;
    for (int i = 0; i < precision; i++) {
        carry *= 10;
    }

    return ((float)(rand() % ((maxValue + minValue) * carry))) / carry - minValue;
}

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;

    const int ballCount = 10;
    const int ballRadius = 40;

    srand(time(NULL));

    Vector2 ballPositions[ballCount], ballVelocities[ballCount];
    Color ballColors[ballCount];

    for (int i = 0; i < ballCount; i++) {
        ballPositions[i].x = rand() % (screenWidth - ballRadius * 2) + ballRadius;
        ballPositions[i].y = rand() % (screenHeight - ballRadius * 2) + ballRadius;

        ballVelocities[i].x = randomVelocity(5, 10, 10);
        ballVelocities[i].y = randomVelocity(5, 10, 10);

        ballColors[i] = generateRandomColor(0.8f, 0.7f);
    }

    InitWindow(screenWidth, screenHeight, "raylib test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            for (int i = 0; i < ballCount; i++) {
                Vector2 *pos = &ballPositions[i], *velocity = &ballVelocities[i];
                pos->x += velocity->x;
                pos->y += velocity->y;

                // X axis border collision detection
                if (pos->x < ballRadius && velocity->x < 0 || pos->x > screenWidth - ballRadius && velocity->x > 0) {
                    velocity->x = -velocity->x;
                }
                // Y axis border collision detection
                if (pos->y < ballRadius && velocity->y < 0 || pos->y > screenHeight - ballRadius && velocity->y > 0) {
                    velocity->y = -velocity->y;
                }

                checkCollisions(ballCount, ballPositions, ballVelocities, ballRadius);

                DrawCircleV(*pos, ballRadius, ballColors[i]);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
