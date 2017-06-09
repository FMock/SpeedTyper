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
#include"gui.h"

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
const int NUM_BLOCKS = 24;

// Containers
std::vector<TextBlock *> blocks = std::vector<TextBlock *>();
std::vector<std::string> words = std::vector<std::string>();
std::map<std::string, GLuint> stringToImageMap;
int font_width = 30;
int font_height = 30;

// Objects
GUI *gui;

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


	/*Initialize the stringToImageMap
	* TextBlock and TextWriter objects will share this map
	*/
	GLuint red = glTexImageTGAFile("images/red_block.tga", &font_width, &font_height);  // red
	stringToImageMap["red"] = red;
	GLuint blue = glTexImageTGAFile("images/blue_block.tga", &font_width, &font_height);  // blue
	stringToImageMap["blue"] = blue;
	GLuint green = glTexImageTGAFile("images/green_block.tga", &font_width, &font_height);  // green
	stringToImageMap["green"] = green;
	GLuint yellow = glTexImageTGAFile("images/yellow_block.tga", &font_width, &font_height);  // yellow
	stringToImageMap["yellow"] = yellow;
	GLuint purple = glTexImageTGAFile("images/purple_block.tga", &font_width, &font_height);  // purple
	stringToImageMap["purple"] = purple;
	GLuint white = glTexImageTGAFile("images/white_block.tga", &font_width, &font_height);  // white
	stringToImageMap["white"] = white;
	GLuint orange = glTexImageTGAFile("images/orange_block.tga", &font_width, &font_height);  // orange
	stringToImageMap["orange"] = orange;
	GLuint zero = glTexImageTGAFile("images/num0.tga", &font_width,&font_height); // 0
	stringToImageMap["0"] = zero;
	GLuint one = glTexImageTGAFile("images/num1.tga", &font_width,&font_height);  // 1
	stringToImageMap["1"] = one;
	GLuint two = glTexImageTGAFile("images/num2.tga", &font_width,&font_height);  // 2
	stringToImageMap["2"] = two;
	GLuint three = glTexImageTGAFile("images/num3.tga", &font_width,&font_height);  // 3
	stringToImageMap["3"] = three;
	GLuint four = glTexImageTGAFile("images/num4.tga", &font_width,&font_height);  // 4
	stringToImageMap["4"] = four;
	GLuint five = glTexImageTGAFile("images/num5.tga", &font_width,&font_height);  // 5
	stringToImageMap["5"] = five;
	GLuint six = glTexImageTGAFile("images/num6.tga", &font_width,&font_height);  // 6
	stringToImageMap["6"] = six;
	GLuint seven = glTexImageTGAFile("images/num7.tga", &font_width,&font_height);  // 7
	stringToImageMap["7"] = seven;
	GLuint eight = glTexImageTGAFile("images/num8.tga", &font_width,&font_height);  // 8
	stringToImageMap["8"] = eight;
	GLuint nine = glTexImageTGAFile("images/num9.tga", &font_width,&font_height);  // 9
	stringToImageMap["9"] = nine;
	GLuint A = glTexImageTGAFile("images/a.tga", &font_width,&font_height);  // A
	stringToImageMap["A"] = A;
	GLuint B = glTexImageTGAFile("images/b.tga", &font_width,&font_height);  // B
	stringToImageMap["B"] = B;
	GLuint C = glTexImageTGAFile("images/c.tga", &font_width,&font_height);  // C
	stringToImageMap["C"] = C;
	GLuint D = glTexImageTGAFile("images/d.tga", &font_width,&font_height);  // D
	stringToImageMap["D"] = D;
	GLuint E = glTexImageTGAFile("images/e.tga", &font_width,&font_height);  // E
	stringToImageMap["E"] = E;
	GLuint F = glTexImageTGAFile("images/f.tga", &font_width,&font_height);  // F
	stringToImageMap["F"] = F;
	GLuint G = glTexImageTGAFile("images/g.tga", &font_width,&font_height);  // G
	stringToImageMap["G"] = G;
	GLuint H = glTexImageTGAFile("images/h.tga", &font_width,&font_height);  // H
	stringToImageMap["H"] = H;
	GLuint I = glTexImageTGAFile("images/i.tga", &font_width,&font_height);  // I
	stringToImageMap["I"] = I;
	GLuint J = glTexImageTGAFile("images/j.tga", &font_width,&font_height);  // J
	stringToImageMap["J"] = J;
	GLuint K = glTexImageTGAFile("images/k.tga", &font_width,&font_height);  // K
	stringToImageMap["K"] = K;
	GLuint L = glTexImageTGAFile("images/l.tga", &font_width,&font_height);  // L
	stringToImageMap["L"] = L;
	GLuint M = glTexImageTGAFile("images/m.tga", &font_width,&font_height);  // M
	stringToImageMap["M"] = M;
	GLuint N = glTexImageTGAFile("images/n.tga", &font_width,&font_height);  // N
	stringToImageMap["N"] = N;
	GLuint O = glTexImageTGAFile("images/o.tga", &font_width,&font_height);  // O
	stringToImageMap["O"] = O;
	GLuint P = glTexImageTGAFile("images/p.tga", &font_width,&font_height);  // P
	stringToImageMap["P"] = P;
	GLuint Q = glTexImageTGAFile("images/q.tga", &font_width,&font_height);  // Q
	stringToImageMap["Q"] = Q;
	GLuint R = glTexImageTGAFile("images/r.tga", &font_width,&font_height);  // R
	stringToImageMap["R"] = R;
	GLuint S = glTexImageTGAFile("images/s.tga", &font_width,&font_height);  // S
	stringToImageMap["S"] = S;
	GLuint T = glTexImageTGAFile("images/t.tga", &font_width,&font_height);  // T
	stringToImageMap["T"] = T;
	GLuint U = glTexImageTGAFile("images/u.tga", &font_width,&font_height);  // U
	stringToImageMap["U"] = U;
	GLuint V = glTexImageTGAFile("images/v.tga", &font_width,&font_height);  // V
	stringToImageMap["V"] = V;
	GLuint W = glTexImageTGAFile("images/w.tga", &font_width,&font_height);  // W
	stringToImageMap["W"] = W;
	GLuint X = glTexImageTGAFile("images/x.tga", &font_width,&font_height);  // X
	stringToImageMap["X"] = X;
	GLuint Y = glTexImageTGAFile("images/y.tga", &font_width,&font_height);  // Y
	stringToImageMap["Y"] = Y;
	GLuint Z = glTexImageTGAFile("images/z.tga", &font_width,&font_height);  // Z
	stringToImageMap["Z"] = Z;
	GLuint dollar = glTexImageTGAFile("images/dollarsign.tga", &font_width,&font_height);  // $
	stringToImageMap["$"] = dollar;
	GLuint exclamation = glTexImageTGAFile("images/exclamation.tga", &font_width,&font_height);  // !
	stringToImageMap["!"] = exclamation;
	GLuint hashtag = glTexImageTGAFile("images/hashtag.tga", &font_width,&font_height);  // #
	stringToImageMap["#"] = hashtag;
	GLuint minus = glTexImageTGAFile("images/minussign.tga", &font_width,&font_height);  // -
	stringToImageMap["-"] = minus;
	GLuint plus = glTexImageTGAFile("images/plussign.tga", &font_width,&font_height);  // +
	stringToImageMap["+"] = plus;
	GLuint percent = glTexImageTGAFile("images/percentsign.tga", &font_width,&font_height);  // %
	stringToImageMap["%"] = percent;
	GLuint equal = glTexImageTGAFile("images/equal.tga", &font_width,&font_height);  // =
	stringToImageMap["="] = equal;
	GLuint question = glTexImageTGAFile("images/question.tga", &font_width,&font_height);  // ?
	stringToImageMap["?"] = question;
	GLuint space = glTexImageTGAFile("images/space.tga", &font_width,&font_height);  // <blank space>
	stringToImageMap[""] = space;


	gui = new GUI();
	keyStates = KeyStates();
	gameData = Game_Data();

	timer = Timer(SDL_GetTicks());

	
	//********** GAME LOOP *************************************************************
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Find out how much time has past since last loop iteration
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - previousTime) / 1000.0f;

		//*********** Get Player Input ***************************************

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
			//printf("Right arrow pressed\n");
		}
		else if (kbState[SDL_SCANCODE_LEFT]) {
			//printf("Left arrow pressed\n");
		}
		else if (kbState[SDL_SCANCODE_UP]) {
			//printf("Up arrow pressed\n");
		}
		else if (kbState[SDL_SCANCODE_DOWN]) {
			//printf("Down arrow pressed\n");
		}else{
			//textBlock->stop();
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		// ************* Do Updates  **************************************************
		gui->update(deltaTime);

		// Create a TextBlock every 4 seconds. blockInterval = 4
		if(timer.count == blockInterval && blocks.size() < NUM_BLOCKS){
			srand (SDL_GetTicks());
			int xpos = rand() % GD::BLOCK_AREA_WIDTH + GD::BORDER_WIDTH;
			int ypos = 0;
			int i = rand() % wordCount; // used to get a random word for a block
			blocks.push_back(new TextBlock(xpos, ypos, 30, 30, words.at(i), stringToImageMap));
		}

		if(timer.count == blockInterval)
			timer.reSet();

		timer.update();

		// update TextBlocks
		for(int i = 0; i < blocks.size(); i++){
			// if not flaged for removal, update
			if(!blocks.at(i)->remove){
				blocks.at(i)->update(deltaTime);
			}

			// If a block is moving, check for collisons
			if(blocks.at(i)->moving && i > 0){
				// Check for collisons with blocks in lower index
				for(int j =  i - 1; j >= 0; j--){
					if(AABBIntersect(blocks.at(i)->getBox(), blocks.at(j)->getBox())){
						blocks.at(i)->moving = false;
					}
				}

				if(!blocks.at(i)->remove && blocks.at(i)->text.compare(testing) == 0){
					blocks.at(i)->remove = true;
				}
			}
		}
		
		//*********** Drawing **********************************************************
		glClearColor(0, 0, 0, 1);  
		glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

		gui->draw();

		for(int i = 0; i < blocks.size(); i++){
			blocks.at(i)->draw();
		}

		// Remove TextBlocks that have been flagged for removal
		for(int i = 0; i < blocks.size(); i++){
			if(blocks.at(i)->moving && blocks.at(i)->remove){
				blocks.erase(blocks.begin() + i);
			}
		}

		//*********** Troubleshooting *************************************************

		//printf(gem->to_string().c_str());
		//printf(gameData.to_string().c_str());
		//printf(keyStates.to_string().c_str());
		//printf(timer.to_string().c_str());
		//if(blocks.size() > 0)
			//printf(blocks.at(0)->to_string().c_str());

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