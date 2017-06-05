#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string>
#include<assert.h>
#include"DrawUtils.h"
#include"game_data.h"
#include"sprite.h"
#include"keystates.h"
#include"TextBlock.h"
#include<fmod.hpp> // For Sound Functionality
#include<time.h>
#include"timer.h"
#include<vector>

/*
	main.cpp
	
	Program Name - SpeedTyper
	Author - Frank Mock
	Project Start Date - 06/2017
*/

typedef Game_Data GD;

Game_Data gameData;

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// To get keyboard state via SDL
const Uint8 *keyState;

// tracks key states
KeyStates keyStates;

// Sprite Amount Constants
const int NUM_BLOCKS = 10;

// Containers for sprites
std::vector<TextBlock *> blocks = std::vector<TextBlock *>();

//Sprite reference
//TextBlock *textBlock;

// To regulate frame rate
int previousTime = 0;
int currentTime = 0;
float deltaTime = 0.0f;

std::string testing = "";

Timer timer;

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
	glViewport(0, 0, GD::WINDOW_WIDTH, GD::WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, GD::WINDOW_WIDTH, GD::WINDOW_HEIGHT, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game initialization goes here.
	keyStates = KeyStates();
	gameData = Game_Data();

	timer = Timer(SDL_GetTicks());

	
	//********** GAME LOOP *************************************************************
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Create a TextBlock every 4 seconds
		if(timer.count == 4 && blocks.size() < NUM_BLOCKS){
			srand (SDL_GetTicks());
			int xpos = rand() % 600 + 50;
			int ypos = 0;
			blocks.push_back(new TextBlock(xpos, ypos, 10, 10, "220"));
		}

		if(timer.count == 4)
			timer.reSet();

		// Find out how many seconds have past since last loop iteration
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - previousTime) / 1000.0f;

		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Register any keys pressed in tracking array
		keyStates.setKeyPressed();


		// Loop through key states array and get key that was pressed
		for(int i = 0; i < 128; i++){
			if(keyStates.states[i] == 1){
				std::string temp = SDL_GetKeyName(i);
				//printf(temp.c_str());
				if(temp.compare("Return") == 0){
					testing = "";
				}else if(temp.compare("Space") == 0){
					testing.append(" ");
				}else if(temp.compare("Backspace") == 0){
					testing = testing.substr(0, testing.size()-1);
				}else{
					testing.append(SDL_GetKeyName(i));
				}

				printf(testing.c_str());
				printf("\n");
				
			}
		}


		// Take action if any of arrowkeys are pushed
		if (kbState[SDL_SCANCODE_RIGHT]) {
			//textBlock->moveRight();
		}
		else if (kbState[SDL_SCANCODE_LEFT]) {
			//textBlock->moveLeft();
		}
		else if (kbState[SDL_SCANCODE_UP]) {
			//textBlock->moveUp();
		}
		else if (kbState[SDL_SCANCODE_DOWN]) {
			//textBlock->moveDown();
		}else{
			//textBlock->stop();
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		//************* Updates  **************************************************

		timer.update();

		// update TextBlocks
		for(int i = 0; i < blocks.size(); i++){
			blocks.at(i)->update(deltaTime);
		}
		

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//*********** Game drawing goes here ****************************************
		for(int i = 0; i < blocks.size(); i++){
			blocks.at(i)->draw();
		}
		//textBlock->draw();
		//printf(gem->to_string().c_str());
		//printf(gameData.to_string().c_str());
		//printf(keyStates.to_string().c_str());
		//printf(timer.to_string().c_str());
		// Clear all key states in key state tracking array
		keyStates.zeroAllKeyStates();
		
		// Present the most recent frame.
		SDL_GL_SwapWindow(window);

	} //***** END GAME LOOP ********************************************************

	SDL_Quit();

	return 0;
}
