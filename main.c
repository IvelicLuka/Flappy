#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "raylib.h"
#define WIDTH 800
#define HEIGHT 600
#define GRAV 0.5
#define VEL 10
#define MAX_VEL 20

#define BIRD_WIDTH 25
#define BIRD_HEIGHT BIRD_WIDTH
#define BIRD_OFFSET 100

#define PIPE_WIDTH 50
#define PIPE_HOLE 200
#define PIPE_COUNT 8
#define PIPE_SPACING 250
#define PIPE_SPEED 3
#define PIPE_BORDER 20
#define PIPE_CAP 5
typedef struct {
	float x;
	float y;

} pipe;

float randPipeY() {
	return ((float) rand()/RAND_MAX) * (HEIGHT-PIPE_HOLE-PIPE_BORDER) + PIPE_HOLE/2 + PIPE_BORDER/2;
}
int main(void) {
	srand(time(NULL));
	InitWindow(WIDTH, HEIGHT, "Hello from C");
	float birdY = HEIGHT / 2 - BIRD_HEIGHT / 2;
	float vel = 0;
	float rot;
	SetTargetFPS(60);
	bool paused = false;
	pipe pipes[PIPE_COUNT] = { 0 };
	for (int i = 0; i < PIPE_COUNT; i++) {
		pipes[i].x = WIDTH + i*PIPE_SPACING;
		pipes[i].y = randPipeY();
	}
	Image img = LoadImage("images/flappy8.png");
	Texture2D texture = LoadTextureFromImage(img);
	UnloadImage(img);
//	assert(texture != NULL);
	while(!WindowShouldClose()) {
		if (IsKeyPressed(KEY_P)) paused = !paused;
		if (!paused) {
		vel -= GRAV;
		if (vel > MAX_VEL) vel = MAX_VEL;
		if (vel < -MAX_VEL) vel = -MAX_VEL;
		birdY -= vel;
		rot -= (rot - (vel / MAX_VEL)*-90)*0.2;
		if (birdY < 0) {birdY = 0; vel = 0;}
		if (birdY > HEIGHT-BIRD_HEIGHT) {birdY = HEIGHT-BIRD_HEIGHT; vel = 0;}
		for (int i = 0; i < PIPE_COUNT; i++) {
			pipes[i].x -= PIPE_SPEED;
			if (CheckCollisionRecs(CLITERAL(Rectangle){pipes[i].x, 0, PIPE_WIDTH, pipes[i].y - PIPE_HOLE/2},
								   CLITERAL(Rectangle){BIRD_OFFSET, birdY, BIRD_WIDTH, BIRD_HEIGHT}) ||
				CheckCollisionRecs(CLITERAL(Rectangle){pipes[i].x, pipes[i].y + PIPE_HOLE/2, PIPE_WIDTH, HEIGHT - pipes[i].y - PIPE_HOLE/2 + 10},
 								   CLITERAL(Rectangle){BIRD_OFFSET, birdY, BIRD_WIDTH, BIRD_HEIGHT}) ||
				CheckCollisionRecs(CLITERAL(Rectangle){pipes[i].x - PIPE_CAP, pipes[i].y - PIPE_HOLE/2 - PIPE_BORDER, 2 * PIPE_CAP + PIPE_WIDTH, PIPE_BORDER},
 								   CLITERAL(Rectangle){BIRD_OFFSET, birdY, BIRD_WIDTH, BIRD_HEIGHT}) ||
				CheckCollisionRecs(CLITERAL(Rectangle){pipes[i].x - PIPE_CAP, pipes[i].y + PIPE_HOLE/2, 2 * PIPE_CAP + PIPE_WIDTH, PIPE_BORDER},
 								   CLITERAL(Rectangle){BIRD_OFFSET, birdY, BIRD_WIDTH, BIRD_HEIGHT})
			) CloseWindow();
		}
		if (pipes[0].x < -PIPE_WIDTH) {
			for (int i = 0; i < PIPE_COUNT-1; i++) {
				pipes[i] = pipes[i+1];
			}
			pipes[PIPE_COUNT-1].x = pipes[PIPE_COUNT-2].x + PIPE_SPACING;
			pipes[PIPE_COUNT-1].y = randPipeY();
		}
		if (IsKeyPressed(KEY_SPACE)) vel = VEL + vel / 4;
		}
		BeginDrawing();
			ClearBackground(BLUE);
//			DrawRectangle(BIRD_OFFSET, birdY, BIRD_WIDTH, BIRD_HEIGHT, YELLOW);
			DrawTextureEx(texture, CLITERAL(Vector2){BIRD_OFFSET + BIRD_WIDTH/2, birdY + BIRD_HEIGHT/2}, rot, (float) BIRD_HEIGHT/img.height, CLITERAL(Color){ 255, 255, 255, 255 });

			for (int i = 0; i < PIPE_COUNT; i++) {
				DrawRectangle(pipes[i].x, 0, PIPE_WIDTH, pipes[i].y - PIPE_HOLE/2, GREEN);
				DrawRectangle(pipes[i].x - PIPE_CAP, pipes[i].y - PIPE_HOLE/2 - PIPE_BORDER, 2 * PIPE_CAP + PIPE_WIDTH, PIPE_BORDER, GREEN);

				//DrawTexture(texture, 200, 200, WHITE); 
				

				DrawRectangle(pipes[i].x, pipes[i].y + PIPE_HOLE/2, PIPE_WIDTH, HEIGHT - pipes[i].y - PIPE_HOLE/2 + 10, GREEN);
				DrawRectangle(pipes[i].x - PIPE_CAP, pipes[i].y + PIPE_HOLE/2, 2 * PIPE_CAP + PIPE_WIDTH, PIPE_BORDER, GREEN);
			}
			
		EndDrawing();
	}
	UnloadTexture(texture);
	CloseWindow();
}
