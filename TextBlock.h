#pragma once
#ifndef HIT_H
#define HIT_H
#include"sprite.h"
#include<map>
#include"game_data.h"
#include <sstream>

// Author - Frank Mock
// TextBlock.h
// TextBlock is a composite sprite consisting of a rectangular 
// block with text imposed over it

class TextBlock : public Sprite{
public:
	TextBlock():Sprite(){}
	TextBlock(int, int, int, int, std::string);

	std::map<std::string, GLuint> strToImageMap;
	
	std::string text; // the text to be displayed on the block

	int fontWidth;

	// The direction the character is facing
	int direction;

	int prev_change_x;

	int prev_change_y;

	// If x and y velocity is 0
	bool stopped;

	bool collieded;

	// True when hit by any enemy
	bool isHit;

	// If True remove skeleton from game
	bool isDead;

	// load images of blocks and text
	void loadImages();

	void update(float deltaTime);

	void draw();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stop();
	void collision(Sprite &sprite);
	std::string to_string() const;
};
#endif