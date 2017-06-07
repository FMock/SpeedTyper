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
#include<iostream>
#include<fstream>

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

// Containers
std::vector<TextBlock *> blocks = std::vector<TextBlock *>();
std::vector<std::string> words = std::vector<std::string>();

// To regulate frame rate
int previousTime = 0;
int currentTime = 0;
float deltaTime = 0.0f;

int wordCount = 0;
std::ifstream inFile; // to read the words from file
std::string fileName = "words.txt";
std::string testing = "";

Timer timer;
int blockInterval = 4; // Interval in seconds at which a new block appears

// Function protoTypes
void readWords(std::ifstream &in);
void readLines(std::ifstream &in);
bool AABBIntersect(AABB box1, AABB box2);

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

	//********** Game initialization goes here. **********************

	// Read in the words from text file
	inFile.open(fileName);
	if(inFile.fail()){
		printf("Error opening word file %s ", fileName.c_str());
		SDL_Quit();
		return 1;
	}
	readLines(inFile);

	keyStates = KeyStates();
	gameData = Game_Data();

	timer = Timer(SDL_GetTicks());

	
	//********** GAME LOOP *************************************************************
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Create a TextBlock every 4 seconds. blockInterval = 4
		if(timer.count == blockInterval && blocks.size() < NUM_BLOCKS){
			srand (SDL_GetTicks());
			int xpos = rand() % 600 + 50;
			int ypos = 0;
			int i = rand() % wordCount; // used to get a random word for a block
			blocks.push_back(new TextBlock(xpos, ypos, 30, 30, words.at(i)));
		}

		if(timer.count == blockInterval)
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

/*
* Checks AABB/AABB collisions
*/
bool AABBIntersect(AABB box1, AABB box2){
	//box1 to the right
	if(box1.x > box2.x + box2.w){
		return false;
	}

	// box1 top the left
	if(box1.x + box1.w < box2.x){
		return false;
	}

	// box1 below
	if(box1.y > box2.y + box2.h){
		return false;
	}

	// box1 above
	if(box1.y + box1.h < box2.y){
		return false;
	}
	
	return true;
}

// Reads the words from text file and stores them in a String vector
void readWords(std::ifstream &in){
	std::string temp;
	while(in >> temp){
		words.push_back(temp);
		wordCount ++;
	}
}

// Reads lines from text file and stores them in a String vector
void readLines(std::ifstream &in){
	std::string line;
	while (std::getline(in, line)){
		words.push_back(line);
		wordCount ++;
	}
}