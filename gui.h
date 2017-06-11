#pragma once
#ifndef GUI_H
#define GUI_H
#include<GL/glew.h>
#include"DrawUtils.h"
#include"game_data.h"
#include <sstream>
#include<ctime>
#include"cursor.h"
#include"keystates.h"

// Author - Frank Mock
// gui.h
// The graphic display of the game 

class GUI{
public:
	GUI(KeyStates&);

	void loadImages();
	KeyStates *keyStates;
	GLuint logo;
	GLuint cloudBackground;
	GLuint textTypeArea;
	GLuint vertBorder;
	GLuint horzBorder;
	GLuint optionsClosed;
	GLuint optionsOpen;
	GLuint helpClosed;
	GLuint aboutClosed;

	int width;
	int height;

	bool options;
	void setOptions(bool);
	bool getOptions() const;

	void update(float deltaTime);

	void draw();

	std::string to_string() const;
};
#endif