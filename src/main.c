#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define BEGIN 0
#define RUNNING 1
#define WIN 2

#define RIGHT 0
#define LEFT 1
#define NONE -1

#define PLANK_WIDTH 10
#define PLANK_HEIGHT 60

#define PLANK_SPEED_K 200.0f

#define TEXT_FONT_SIZE 30
#define BIG_TEXT_FONT_SIZE 45

#define BALL_RADIUS 13

typedef struct Ball {
    Vector2 center;
    double angle;
    int direction;
    double radius;
    int speed;
} Ball;

float centeredTextX(const char* text, const int fontSize) {
    return SCREEN_WIDTH / 2.0f - MeasureText(text, fontSize) / 2.0f;
}

void restoreBall(Ball* ball) {
    ball->center.x = SCREEN_WIDTH / 2;
    ball->center.y = SCREEN_HEIGHT / 2;
    ball->angle = rand() % 181;
    ball->direction = rand() % 2;
    ball->radius = BALL_RADIUS;
    ball->speed = 200;
}

int main(void) {
    srand(time(0));

    int gameState = BEGIN;

    int winner = 0, leftScore = 0, rightScore = 0;

    Ball ball;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong!");

    Rectangle leftPlank = {10, 10, PLANK_WIDTH, PLANK_HEIGHT};
    Rectangle rightPlank = {SCREEN_WIDTH - 20, 10, PLANK_WIDTH, PLANK_HEIGHT};

    const char pongText[] = "PONG!";
    const char ruleText[] = "The first who score 3 points will win";
    const char startText[] = "Press ENTER to start!";
    const char restartText[] = "Press ENTER to play again!";

    const int pongTextX = centeredTextX(pongText, TEXT_FONT_SIZE);
    const int ruleTextX = centeredTextX(ruleText, TEXT_FONT_SIZE);
    const int startTextX = centeredTextX(startText, TEXT_FONT_SIZE);
    const int restartTextX = centeredTextX(restartText, TEXT_FONT_SIZE);

    char* winnerText = (char*)malloc(11 * sizeof(char));

    bool roundEnd = true;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();

        const float plankStepLength = PLANK_SPEED_K * deltaTime;
        const float ballStepLength = ball.speed * deltaTime;

        ball.speed += 1;

        // right plank movement
        if (rightPlank.y < SCREEN_HEIGHT - PLANK_HEIGHT)
            if (IsKeyDown(KEY_DOWN)) rightPlank.y += plankStepLength;
        if (rightPlank.y > 0)
            if (IsKeyDown(KEY_UP)) rightPlank.y -= plankStepLength;

        // left plank movement
        if (leftPlank.y < SCREEN_HEIGHT - PLANK_HEIGHT)
            if (IsKeyDown(KEY_S)) leftPlank.y += plankStepLength;
        if (leftPlank.y > 0)
            if (IsKeyDown(KEY_W)) leftPlank.y -= plankStepLength;

        // Game starting
        if ((gameState == BEGIN || gameState == WIN) && IsKeyReleased(KEY_ENTER)) {
            winner = NONE;
            leftScore = 0;
            rightScore = 0;
            gameState = RUNNING;
        }

        if (gameState == RUNNING) {
            if (roundEnd) {
                restoreBall(&ball);
                roundEnd = false;
            }

            if (ball.direction == LEFT)
                ball.center.x -= ballStepLength;
            else if (ball.direction == RIGHT)
                ball.center.x += ballStepLength;

            ball.center.y += cos(ball.angle);

            if (ball.center.y - ball.radius <= 0)
                ball.angle = ball.angle + 90;
            else if (ball.center.y + ball.radius >= SCREEN_HEIGHT)
                ball.angle = ball.angle + 90;

            if (CheckCollisionCircleRec(ball.center, ball.radius, leftPlank)) {
                ball.direction = RIGHT;
                ball.angle = -ball.angle + rand() % 90;
            } else if (CheckCollisionCircleRec(ball.center, ball.radius, rightPlank)) {
                ball.direction = LEFT;
                ball.angle = -ball.angle + rand() % 90;
            }

            if (ball.center.x + ball.radius < 0) {
                rightScore += 1;
                roundEnd = true;
            } else if (ball.center.x - ball.radius > SCREEN_WIDTH) {
                leftScore += 1;
                roundEnd = true;
            }

            if (leftScore >= 3) winner = LEFT;
            if (rightScore >= 3) winner = RIGHT;
            if (winner != NONE) gameState = WIN;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(ball.center.x, ball.center.y, ball.radius, WHITE);

        DrawRectangleRec(leftPlank, WHITE);
        DrawRectangleRec(rightPlank, WHITE);

        if (gameState == BEGIN) {
            DrawText(pongText, pongTextX, 10, TEXT_FONT_SIZE, WHITE);
            DrawText(ruleText, ruleTextX, 400, TEXT_FONT_SIZE, WHITE);
            DrawText(startText, startTextX, 150, TEXT_FONT_SIZE, WHITE);
        }

        if (gameState == RUNNING) {
            char score[4];
            sprintf(score, "%d:%d", leftScore, rightScore);
            const int scoreX = centeredTextX(score, BIG_TEXT_FONT_SIZE);
            DrawText(score, scoreX, 10, BIG_TEXT_FONT_SIZE, WHITE);
        }

        if (gameState == WIN) {
            if (winner == LEFT)
                winnerText = "Left WON!";
            else if (winner == RIGHT)
                winnerText = "Right WON!";

            const int winnerTextX = centeredTextX(winnerText, BIG_TEXT_FONT_SIZE);
            DrawText(winnerText, winnerTextX, 50, BIG_TEXT_FONT_SIZE, WHITE);
            DrawText(restartText, restartTextX, 150, TEXT_FONT_SIZE, WHITE);
        }

        EndDrawing();
    }

    free(winnerText);

    CloseWindow();

    return 0;
}
