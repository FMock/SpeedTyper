#pragma once
#ifndef GUI_H
#define GUI_H
#include<GL/glew.h>
#include"DrawUtils.h"
#include"game_data.h"
#include <sstream>
#include<ctime>
#include"cursor.h"

// Author - Frank Mock
// gui.h
// The graphic display of the game 

class GUI{
public:
	GUI();

	void loadImages();
	GLuint logo;
	GLuint cloudBackground;
	GLuint textTypeArea;
	GLuint vertBorder;
	GLuint horzBorder;

	int width;
	int height;

	void update(float deltaTime);

	void draw();

	std::string to_string() const;
};
#endif