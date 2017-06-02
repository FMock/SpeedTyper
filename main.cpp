#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"DrawUtils.h"

/*
	main.cpp
	
	Program Name - SpeedTyper
	Author - Frank Mock
	Project Start Date - 06/2017
*/

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// Position of the sprite.
float spritePos[2] = { 10.0f, 10.0f };

// Texture for the sprite.
GLuint spriteTex;

// Size of the sprite.
int spriteSize[2];

// Game Window Size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// To regulate frame rate
int previousTime = 0;
int currentTime = 0;
float deltaTime = 0.0f;

// To control the speed of the sprite Pixels/Sec
float spriteSpeedX = 200;
float spriteSpeedY = 200;

// To get keyboard state
const Uint8 *keyState;

int main(void)
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return 1;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"SpeedTyper",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
			glGetString(GL_VERSION));
	}
	else {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL state.
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 600, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game initialization goes here.
	spriteTex = glTexImageTGAFile("images/gem01.tga", &spriteSize[0], &spriteSize[1]);

	// The game loop.
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Find out how many seconds have past since last loop iteration
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - previousTime) / 1000.0f;

		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Handle OS message pump.
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				shouldExit = 1;
			}
		}

		keyState = SDL_GetKeyboardState(NULL); // We want status of all the keys

		// Take action if any of arrowkeys are pushed
		if (keyState[SDL_SCANCODE_RIGHT]) {
			if(spritePos[0] < WINDOW_WIDTH - spriteSize[0])
				spritePos[0] += spriteSpeedX * deltaTime;
		}
		else if (keyState[SDL_SCANCODE_LEFT]) {
			if(spritePos[0] > 0)
				spritePos[0] -= spriteSpeedX * deltaTime;
		}
		else if (keyState[SDL_SCANCODE_UP]) {
			if(spritePos[1] > 0)
				spritePos[1] -= spriteSpeedX * deltaTime;
		}
		else if (keyState[SDL_SCANCODE_DOWN]) {
			if(spritePos[1] < WINDOW_HEIGHT - spriteSize[1])
				spritePos[1] += spriteSpeedX * deltaTime;
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Game drawing goes here.
		glDrawSprite(spriteTex, spritePos[0], spritePos[1], spriteSize[0], spriteSize[1]);

		// Present the most recent frame.
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();

	return 0;
}
