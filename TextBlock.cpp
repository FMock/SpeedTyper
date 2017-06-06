#include "TextBlock.h"

/**
* File Name - TextBlock.cpp
* Author - Frank Mock
* 
* TextBlock class represents a colored block with text on it
*/

using std::ostringstream;
typedef Game_Data GD;

enum Direction{LEFT, RIGHT, UP, DOWN };

TextBlock::TextBlock(int x, int y, int w, int h, std::string s):Sprite(x, y, w, h){
	type = "TextBlock";
	colors.push_back("red"), colors.push_back("blue"), colors.push_back("green");
	colors.push_back("yellow"), colors.push_back("purple");
	color = colors.at(rand() % 5);
	fontWidth = 20;
	text = s;
	collieded = false;
	isDead = false;
	isHit = false;
	box.setW(characterSize[0]);
	box.setH(characterSize[1]);

	direction = DOWN;
	stopped = true;
	loadImages();
	setWidth(100);
	setHeight(30);
	setXSpeed(100);
	setYSpeed(100);
}

void TextBlock::loadImages(){
	GLuint red = glTexImageTGAFile("images/red_block.tga", &characterSize[0], &characterSize[1]);  // red
	strToImageMap["red"] = red;
	GLuint blue = glTexImageTGAFile("images/blue_block.tga", &characterSize[0], &characterSize[1]);  // blue
	strToImageMap["blue"] = blue;
	GLuint green = glTexImageTGAFile("images/green_block.tga", &characterSize[0], &characterSize[1]);  // green
	strToImageMap["green"] = green;
	GLuint yellow = glTexImageTGAFile("images/yellow_block.tga", &characterSize[0], &characterSize[1]);  // yellow
	strToImageMap["yellow"] = yellow;
	GLuint purple = glTexImageTGAFile("images/purple_block.tga", &characterSize[0], &characterSize[1]);  // purple
	strToImageMap["purple"] = purple;
	GLuint zero = glTexImageTGAFile("images/num0.tga", &characterSize[0], &characterSize[1]); // 0
	strToImageMap["0"] = zero;
	GLuint one = glTexImageTGAFile("images/num1.tga", &characterSize[0], &characterSize[1]);  // 1
	strToImageMap["1"] = one;
	GLuint two = glTexImageTGAFile("images/num2.tga", &characterSize[0], &characterSize[1]);  // 2
	strToImageMap["2"] = two;
}

// Draw the textBlock to the screen
void TextBlock::draw(){
	glDrawSprite(strToImageMap[color],  characterPos[0], characterPos[1], characterSize[0], characterSize[1]);

	// For each character of text, get it's corresponding image from the map and draw it
	int offset = 0;
	for(std::string::size_type i = 0; i < text.size(); ++i) {
		// convert char to string
		std::stringstream ss;
		ss << text[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s],  characterPos[0] + offset, characterPos[1] + 4, 19, 22);
		offset += fontWidth;
	}
}

void TextBlock::update(float deltaTime){
	moveDown();

	if(characterPos[0] > GD::WORLD_WIDTH - characterSize[0]){
		change_x = 0; //stop block from going out of bounds right
		characterPos[0] -= 1;
	}

	if(characterPos[0] < 0){
		change_x = 0; //stop block from going out of bounds left
		characterPos[0] += 1;
	}

	if(characterPos[1] < 0){
		change_y = 0; //stop block from going out of bounds up
		characterPos[1] += 1;
	}

	if(characterPos[1] > GD::WORLD_HEIGHT - characterSize[1]){
		change_y = 0; //stop block from going out of bounds up
		characterPos[1] -= 1;
	}

	characterPos[0] += change_x * deltaTime;
	box.setX(abs(characterPos[0]));
	characterPos[1] += change_y * deltaTime;
	box.setY(abs(characterPos[1]));
}

void TextBlock::moveLeft(){
	stopped = false;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = -speedX;
	direction = LEFT;
}

void TextBlock::moveRight(){
	stopped = false;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = speedX;
	direction = RIGHT;
}

void TextBlock::moveUp(){
	stopped = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = -speedY;
	direction = UP;
}

void TextBlock::moveDown(){
	stopped = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = speedY;
	direction = DOWN;
}

void TextBlock::stop(){
	stopped = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = 0;
}

void TextBlock::collision(Sprite &sprite){
	collieded = true;
}


std::string TextBlock::to_string() const{
	ostringstream oss;
	oss << "TextBlock ************\n"
		<< "isDead = " << isDead << "\n"
		<< "Direction = " << direction << "\n"
		<< "isHit = "  << isHit << "\n"
		<< "AABB x = " << box.x << "\n"
		<< "AABB y = " << box.y << "\n"
		<< "AABB w = " << box.w << "\n"
		<< "AABB h = " << box.h << "\n"
		<< "xPos = " << getXPos() << "\n"
		<< "YPos = " << getYPos() << "\n"
		<< "END TextBlock *********\n";
	return oss.str();
}
