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
#include"TextWriter.h"
#include"stats.h"
#include"hud.h"

/*
	main.cpp
	
	Program Name - SpeedTyper
	Author - Frank Mock
	Project Start Date - 06/2017
*/

typedef Game_Data GD;

Game_Data *gameData;

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// To get keyboard state via SDL
const Uint8 *keyState;

// tracks key states
KeyStates *keyStates;

// Sprite Amount Constants
const int NUM_BLOCKS = 24;

// Containers
//std::vector<TextBlock *> blocks = std::vector<TextBlock *>();
std::vector<TextBlock> blocks = std::vector<TextBlock>();
std::vector<std::string> words = std::vector<std::string>();
std::map<std::string, GLuint> stringToImageMap;

int font_width = 30;
int font_height = 30;

// Objects
GUI *gui;
Stats stats;
Hud hud;
TextWriter *textWriter;
int overlayWidth = 500;
int overlayHeight = 200;
GLuint gameOverOverlay;
GLuint playing;
GLuint pausedImg;
GLuint startImg;

// To regulate frame rate
int previousTime = 0;
int currentTime = 0;
float deltaTime = 0.0f;

int wordCount = 0;
std::ifstream inFile; // for reading files
std::ofstream outFile; // to write highscore
std::string wordsFile = "words.txt";
std::string highScoreFile = "high.txt";
std::string testing = "";

Timer timer;
int blockInterval = 4; // Interval in seconds at which a new block appears

// Function protoTypes
void readWords(std::ifstream &in);
void readLines(std::ifstream &in);
int readHighScore(std::ifstream &in);
void writeHighScore(std::ofstream &out);
int string_to_int(std::string);
bool AABBIntersect(AABB box1, AABB box2);

int high; // track highscore
bool start = true;
bool gameOver = false;
bool paused = true;
float previousPausePressTime = 0.0f;
float currentPausePressTime = 0.0f;
bool bombUsed = false;

static const int Y_POSITION_THRESHOLD = 90;
static int yPosReached = GD::WINDOW_HEIGHT - GD::BL_FLOOR_TO_BOTTOM;

// For sound functionality
FMOD::System *fmod_sys;
FMOD::Sound *scoreSound;

// category start numbers
int defaultNum = 0;
int animals;
int places;
int things;
int transportation;
int music;
int anime;


int main(void)
{
	// Sound Setup
	FMOD_RESULT fmodResult = System_Create(&fmod_sys);
	fmod_sys->init(100, FMOD_INIT_NORMAL, 0);
	fmod_sys->createSound("sounds/score_sound.wav", FMOD_DEFAULT, 0, &scoreSound);

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
	inFile.open(wordsFile);
	if(inFile.fail()){
		printf("Error opening word file %s ", wordsFile.c_str());
		SDL_Quit();
		return 1;
	}
	readLines(inFile);
	inFile.close();

	// Read high score from text file
	inFile.open(highScoreFile);
	if(inFile.fail()){
		printf("Error opening high score file %s ", highScoreFile.c_str());
		SDL_Quit();
		return 1;
	}
	high = readHighScore(inFile);
	inFile.close();


	// **********  LOAD IMAGE RESOURCES ***************************************************
	gameOverOverlay = glTexImageTGAFile("images/endGame.tga", &overlayWidth, &overlayHeight);
	playing = glTexImageTGAFile("images/playing.tga", &overlayWidth, &overlayHeight);
	pausedImg = glTexImageTGAFile("images/paused.tga", &overlayWidth, &overlayHeight);
	startImg = glTexImageTGAFile("images/startImg.tga", &overlayWidth, &overlayHeight);


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
	GLuint period = glTexImageTGAFile("images/period.tga", &font_width,&font_height);  // .
	stringToImageMap["."] = period;

	gameData = new Game_Data();
	gameData->highScore = high;
	hud = Hud(stringToImageMap, *gameData);
	stats = Stats(*gameData);
	keyStates = new KeyStates(*gameData);
	textWriter = new TextWriter(testing, stringToImageMap);
	gui = new GUI(*keyStates, *gameData);
	timer = Timer(SDL_GetTicks());

	blocks.reserve(24); // Try to prevent blocks vector resize overhead

	//********** GAME LOOP *************************************************************
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Find out how much time has past since last loop iteration
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - previousTime) / 1000.0f;
		gameData->deltaTime = deltaTime;

		//*********** Get Player Input ***************************************

		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Register any keys pressed in tracking array
		keyStates->setKeyPressed();
		

		// Loop through key states array and get key that was pressed
		for(int i = 0; i < 128; i++){
			if(keyStates->states[i] == 1){
				std::string temp = SDL_GetKeyName(i);
				//printf(temp.c_str());
				if(temp.compare("Return") == 0){
					testing = "";
				}else if(temp.compare("Space") == 0){
					if(testing.size() < GD::MAX_STRING_SIZE)
						testing.append(" ");
				}else if(temp.compare("Backspace") == 0){
					testing = testing.substr(0, testing.size()-1);
				}else{
					if(testing.size() < GD::MAX_STRING_SIZE)
						testing.append(SDL_GetKeyName(i));
				}

				//printf(testing.c_str());
				//printf("\n");
				
			}
		}


		// Take action if any of arrowkeys are pushed
		if (kbState[SDL_SCANCODE_RIGHT]) {
			//printf("Right arrow pressed\n");

			//******* RESET THE GAME ****************************
			// Flag blocks for removal
			for(int i = 0; i < blocks.size(); i++){
				blocks.at(i).remove = true;
			}

			// Remove TextBlocks that have been flagged for removal
			for(int i = 0; i < blocks.size(); i++){
				if(blocks.at(i).remove){
					blocks.erase(blocks.begin() + i);
				}
			}

			stats.reset();
			gameOver = false;
			paused = false;
			start = false;
		}
		else if (kbState[SDL_SCANCODE_LEFT]) {
			//printf("Left arrow pressed\n");

			start = false;

			//******** PAUSE THE GAME ***************************

			// Determine how long it's been since left arrow was last pressed
			previousPausePressTime = currentPausePressTime;
			currentPausePressTime = SDL_GetTicks();
			float sinceLastPressed = (currentPausePressTime - previousPausePressTime) / 1000.0f;

			if(sinceLastPressed > 0.01){
				if(paused)
					paused = false;
				else
					paused = true;
			}
		}
		else if (kbState[SDL_SCANCODE_UP]) {
			//printf("Up arrow pressed\n");
		}
		else if (kbState[SDL_SCANCODE_DOWN]) {
			//printf("Down arrow pressed\n");
		}else if(kbState[SDL_SCANCODE_F1]){
			//printf("F1 pressed\n");
		}
		else{
			//textBlock->stop();
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		// ************* Do Updates  **************************************************
		fmod_sys->update(); // If you don't update the sound will play once

		gui->update(deltaTime);

		if(!paused){
			textWriter->update(testing);

			int var = rand() % 1001 + 1; // just to add further variation in word choices

			// Create a TextBlock every 4 seconds. blockInterval = 4
			if(timer.count == blockInterval && blocks.size() < NUM_BLOCKS && !gameOver){
				srand (SDL_GetTicks() + var);
				int xpos = rand() % GD::BLOCK_FALL_AREA_MAX_X + GD::BORDER_WIDTH;
				int ypos = 0;
				int i;
				int numWords; // number words in category
				std::string category = gameData->menuItemSelected;

				if(category.compare("animals") == 0){
					numWords = places - animals;
					i = rand() % numWords + animals; // animals category
				}else if(category.compare("places") == 0){
					numWords = things - places;
					i = rand() % numWords + places; // places category
				}else if(category.compare("things") == 0){
					numWords = transportation - things;
					i = rand() % numWords + things; // things category
				}else if(category.compare("transportation") == 0){
					numWords = music - transportation;
					i = rand() % numWords + transportation; // transportation category
				}else if(category.compare("music") == 0){
					numWords = anime - music;
					i = rand() % numWords + music; // music category
				}else if(category.compare("anime") == 0){
					numWords = wordCount - anime;
					i = rand() % numWords + anime; // anime category
				}
				else{
					i = rand() % wordCount; // default category == all the words
				}

				blocks.push_back(TextBlock(xpos, ypos, 30, 30, words.at(i), stringToImageMap));
				gameData->currentWord = words.at(i);
				stats.increaseTotalCount(1); // Keep track of the amount of words presented to player
			}

			if(timer.count == blockInterval)
				timer.reSet();

			timer.update();

			// update TextBlocks
			for(int i = 0; i < blocks.size(); i++){

				// if not flaged for removal, update
				if(!blocks.at(i).remove){
					blocks.at(i).update(deltaTime);
				}

				// If a block is moving, check for collisons
				if(blocks.at(i).moving){

					// Check for collisons with blocks in lower index
					if(i > 0){
						for(int j =  i - 1; j >= 0; j--){
							if(AABBIntersect(blocks.at(i).getBox(), blocks.at(j).getBox())){
								blocks.at(i).moving = false;
							}
						}
					}

					// check if player typed the correct string
					if(!blocks.at(i).remove && blocks.at(i).text.compare(testing) == 0){
						blocks.at(i).remove = true;
						fmod_sys->playSound(scoreSound, 0, false, NULL);
					}

					// Check if player typed 'BOMB' to blowup a word (remove a word)
					if(testing.compare("BOMB") == 0){
						bombUsed = true;
						blocks.at(i).remove = true;
						fmod_sys->playSound(scoreSound, 0, false, NULL);
					}
				}
			}


			// Get the Y position of the highest stacked block
			int lowestYPosition = GD::WINDOW_HEIGHT;
			for(int i = 0; i < blocks.size(); i++){
				if(!blocks.at(i).moving && blocks.at(i).getYPos() < lowestYPosition)
					lowestYPosition = blocks.at(i).getYPos();
			}

			yPosReached = lowestYPosition;
			// GAME OVER CONDITION - Heighest stacked block at or above threshold
			if(!gameOver &&  yPosReached <= Y_POSITION_THRESHOLD){
				gameOver = true;
			}

			// GAME OVER CONDITION - Amount of TextBlocks displayed == NUM_BLOCKS
			if(blocks.size() == NUM_BLOCKS && !gameOver && !blocks.at(NUM_BLOCKS - 1).moving){
				gameOver = true;
			}
		}// END !PAUSED


		//****** UPDATE HIGHSCORE FILE ***********************/
		if(gameOver){
			if(gameData->score > high){
				gameData->highScore = gameData->score;

				outFile.open(highScoreFile);
				if(outFile.fail()){
					printf("Error opening high score file %s ", highScoreFile.c_str());
					SDL_Quit();
					return 1;
				}

				writeHighScore(outFile);
				outFile.close();
			}
		}
		
		//*********** Drawing **********************************************************
		glClearColor(0, 0, 0, 1);  
		glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

		if(paused){
			if(start){
				glDrawSprite(startImg, 520, 340, overlayWidth, overlayHeight);
			}else{
				glDrawSprite(pausedImg, 520, 340, overlayWidth, overlayHeight);
			}
		}
		else if(gameOver){
			glDrawSprite(gameOverOverlay, 520, 340, overlayWidth, overlayHeight);
		}else{
			glDrawSprite(playing, 520, 340, overlayWidth, overlayHeight);
		}

		gui->draw();
		hud.draw();

		textWriter->draw();

		for(int i = 0; i < blocks.size(); i++){
			blocks.at(i).draw();
		}


		// Remove TextBlocks that have been flagged for removal
		for(int i = 0; i < blocks.size(); i++){
			if(blocks.at(i).moving && blocks.at(i).remove){
				std::string text = blocks.at(i).text;
				int charCount = text.length();
				if(!bombUsed){
					stats.increaseScore(charCount);
					stats.increaseCorrectCount(1);
				}else{
					stats.decreaseScore(charCount);
				}
				blocks.erase(blocks.begin() + i);
			}
		}
		bombUsed = false; // reset bomb

		//*********** Troubleshooting *************************************************
		//printf(stats.to_string().c_str());
		//printf("animals = %i, places = %i, things = %i, transportation = %i, music = %i, anime = %i\n", 
			//animals, places, things, transportation, music, anime);
		//printf("resetGame = %i\n", resetGame);
		//printf("yPosReached = %i\n", yPosReached);
		//printf(gem->to_string().c_str());
		//printf(gui->to_string().c_str());
		//printf(gameData->to_string().c_str());
		printf(keyStates->to_string().c_str());
		//printf(timer.to_string().c_str());
		//if(blocks.size() > 0)
			//printf(blocks.at(0)->to_string().c_str());

		// Clear all key states in key state tracking array
		keyStates->zeroAllKeyStates();
		
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
		if(line.compare("#ANIMALS") == 0){
			animals = wordCount;
		}else if(line.compare("#PLACES") == 0){
			places = wordCount;
		}else if(line.compare("#THINGS") == 0){
			things = wordCount;
		}else if(line.compare("#TRANSPORTATION") == 0){
			transportation = wordCount;
		}else if(line.compare("#MUSIC") == 0){
			music = wordCount;
		}else if(line.compare("#ANIME") == 0){
			anime = wordCount;
		}else{}

		words.push_back(line);
		wordCount ++;
	}
}

// Reads the high score file and updates gameData
int readHighScore(std::ifstream &in){
	std::string line;
	while(std::getline(in, line)){
		return string_to_int(line);
	}
}

// Takes a numeric string and returns its integer representation
int string_to_int(std::string s){
	std::istringstream instr(s);
	int n;
	instr >> n;
	return n;
}

// convert an integer to a string. return the string
std::string int_to_string(int n){
	ostringstream oss;
	oss << n;
	return oss.str();
}

// Writes a highscore to file
void writeHighScore(std::ofstream &out){
	std::string highStr = int_to_string(gameData->highScore);
	out << highStr;
}