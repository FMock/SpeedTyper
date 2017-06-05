#pragma once
#ifndef GAME_DATA_H
#define GAME_DATA_H
#include<string>
#include<sstream>

/* All objects with a reference to Game_Data can track important game
* information to use with game control or response */

using std::ostringstream;

struct Game_Data{
public:
	inline Game_Data::Game_Data():currentGameState(0),
								  player_x(0), 
								  player_y(0),
								  score(0),
								  level(0),
								  player_direction(0),
								  lives_remaining(0){}

	enum state {START, PLAY, PAUSE, END};

	// General Game Info
	static const int WINDOW_WIDTH = 800;   // Camera Width
	static const int WINDOW_HEIGHT = 600;  // Camera Height
	static const int WORLD_WIDTH = 800;
	static const int WORLD_HEIGHT = 600;

	int currentGameState;
	int score;
	int lives_remaining;
	int level;

	// Player Info
	int player_x;
	int player_y;
	int player_direction; // LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
	
	inline void recordPlayersXPos(int x){ player_x = x * 2; }
	inline void recordPlayersYPos(int y){ player_y = y * 2; }
	inline void recordPlayerDirection(int d) {player_direction = d; }

	inline int getPlayerXPos(){ return player_x; }
	inline int getPlayerYPos(){ return player_y; }

	inline std::string Game_Data::to_string() const{
	ostringstream oss;
	oss << "*************************\n"
		<< "score = " << score << "\n"
		<< "level = " << level << "\n"
		<< "lives remaining = " << lives_remaining << "\n"
		<< "Player's X pos. = " <<  player_x << "\n"
		<< "Player's Y pos. = " <<  player_y << "\n"
		<< "currentGameState = " << currentGameState << "\n"
		<< "Player's Direction = " <<  player_direction << "\n";
		return oss.str();
	}
};
#endif