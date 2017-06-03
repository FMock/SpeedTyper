#ifndef KEYSTATES_H
#define KEYSTATES_H

#include<SDL.h>
#include <algorithm> //std::fill
#include<string>
#include<sstream>

/*
	Track Keyboard Presses
*/

class KeyStates{
public:
	KeyStates();
	int states[128]; // helps track key presses
	void zeroAllKeyStates();
	void setKeyPressed(); // sets a key to 1 if pressed
	std::string to_string() const;
};

#endif