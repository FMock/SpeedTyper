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
	colors.push_back("white"), colors.push_back("blue"), colors.push_back("green");
	colors.push_back("yellow"), colors.push_back("purple"), colors.push_back("red");
	colors.push_back("orange");
	srand(time(0));
	color = colors.at(rand() % 7);
	fontWidth = 23;
	fontHeight = 25;
	text = s;
	collided = false;
	remove = false;
	isHit = false;
	moving = true;
	direction = DOWN;
	box.setW(characterSize[0]);
	box.setH(characterSize[1]);
	loadImages();
	setWidth(100);
	setHeight(25);
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
	GLuint white = glTexImageTGAFile("images/white_block.tga", &characterSize[0], &characterSize[1]);  // white
	strToImageMap["white"] = white;
	GLuint orange = glTexImageTGAFile("images/orange_block.tga", &characterSize[0], &characterSize[1]);  // orange
	strToImageMap["orange"] = orange;
	GLuint zero = glTexImageTGAFile("images/num0.tga", &fontWidth, &fontHeight); // 0
	strToImageMap["0"] = zero;
	GLuint one = glTexImageTGAFile("images/num1.tga", &fontWidth, &fontHeight);  // 1
	strToImageMap["1"] = one;
	GLuint two = glTexImageTGAFile("images/num2.tga", &fontWidth, &fontHeight);  // 2
	strToImageMap["2"] = two;
	GLuint three = glTexImageTGAFile("images/num3.tga", &fontWidth, &fontHeight);  // 3
	strToImageMap["3"] = three;
	GLuint four = glTexImageTGAFile("images/num4.tga", &fontWidth, &fontHeight);  // 4
	strToImageMap["4"] = four;
	GLuint five = glTexImageTGAFile("images/num5.tga", &fontWidth, &fontHeight);  // 5
	strToImageMap["5"] = five;
	GLuint six = glTexImageTGAFile("images/num6.tga", &fontWidth, &fontHeight);  // 6
	strToImageMap["6"] = six;
	GLuint seven = glTexImageTGAFile("images/num7.tga", &fontWidth, &fontHeight);  // 7
	strToImageMap["7"] = seven;
	GLuint eight = glTexImageTGAFile("images/num8.tga", &fontWidth, &fontHeight);  // 8
	strToImageMap["8"] = eight;
	GLuint nine = glTexImageTGAFile("images/num9.tga", &fontWidth, &fontHeight);  // 9
	strToImageMap["9"] = nine;
	GLuint A = glTexImageTGAFile("images/a.tga", &fontWidth, &fontHeight);  // A
	strToImageMap["A"] = A;
	GLuint B = glTexImageTGAFile("images/b.tga", &fontWidth, &fontHeight);  // B
	strToImageMap["B"] = B;
	GLuint C = glTexImageTGAFile("images/c.tga", &fontWidth, &fontHeight);  // C
	strToImageMap["C"] = C;
	GLuint D = glTexImageTGAFile("images/d.tga", &fontWidth, &fontHeight);  // D
	strToImageMap["D"] = D;
	GLuint E = glTexImageTGAFile("images/e.tga", &fontWidth, &fontHeight);  // E
	strToImageMap["E"] = E;
	GLuint F = glTexImageTGAFile("images/f.tga", &fontWidth, &fontHeight);  // F
	strToImageMap["F"] = F;
	GLuint G = glTexImageTGAFile("images/g.tga", &fontWidth, &fontHeight);  // G
	strToImageMap["G"] = G;
	GLuint H = glTexImageTGAFile("images/h.tga", &fontWidth, &fontHeight);  // H
	strToImageMap["H"] = H;
	GLuint I = glTexImageTGAFile("images/i.tga", &fontWidth, &fontHeight);  // I
	strToImageMap["I"] = I;
	GLuint J = glTexImageTGAFile("images/j.tga", &fontWidth, &fontHeight);  // J
	strToImageMap["J"] = J;
	GLuint K = glTexImageTGAFile("images/k.tga", &fontWidth, &fontHeight);  // K
	strToImageMap["K"] = K;
	GLuint L = glTexImageTGAFile("images/l.tga", &fontWidth, &fontHeight);  // L
	strToImageMap["L"] = L;
	GLuint M = glTexImageTGAFile("images/m.tga", &fontWidth, &fontHeight);  // M
	strToImageMap["M"] = M;
	GLuint N = glTexImageTGAFile("images/n.tga", &fontWidth, &fontHeight);  // N
	strToImageMap["N"] = N;
	GLuint O = glTexImageTGAFile("images/o.tga", &fontWidth, &fontHeight);  // O
	strToImageMap["O"] = O;
	GLuint P = glTexImageTGAFile("images/p.tga", &fontWidth, &fontHeight);  // P
	strToImageMap["P"] = P;
	GLuint Q = glTexImageTGAFile("images/q.tga", &fontWidth, &fontHeight);  // Q
	strToImageMap["Q"] = Q;
	GLuint R = glTexImageTGAFile("images/r.tga", &fontWidth, &fontHeight);  // R
	strToImageMap["R"] = R;
	GLuint S = glTexImageTGAFile("images/s.tga", &fontWidth, &fontHeight);  // S
	strToImageMap["S"] = S;
	GLuint T = glTexImageTGAFile("images/t.tga", &fontWidth, &fontHeight);  // T
	strToImageMap["T"] = T;
	GLuint U = glTexImageTGAFile("images/u.tga", &fontWidth, &fontHeight);  // U
	strToImageMap["U"] = U;
	GLuint V = glTexImageTGAFile("images/v.tga", &fontWidth, &fontHeight);  // V
	strToImageMap["V"] = V;
	GLuint W = glTexImageTGAFile("images/w.tga", &fontWidth, &fontHeight);  // W
	strToImageMap["W"] = W;
	GLuint X = glTexImageTGAFile("images/x.tga", &fontWidth, &fontHeight);  // X
	strToImageMap["X"] = X;
	GLuint Y = glTexImageTGAFile("images/y.tga", &fontWidth, &fontHeight);  // Y
	strToImageMap["Y"] = Y;
	GLuint Z = glTexImageTGAFile("images/z.tga", &fontWidth, &fontHeight);  // Z
	strToImageMap["Z"] = Z;
	GLuint dollar = glTexImageTGAFile("images/dollarsign.tga", &fontWidth, &fontHeight);  // $
	strToImageMap["$"] = dollar;
	GLuint exclamation = glTexImageTGAFile("images/exclamation.tga", &fontWidth, &fontHeight);  // !
	strToImageMap["!"] = exclamation;
	GLuint hashtag = glTexImageTGAFile("images/hashtag.tga", &fontWidth, &fontHeight);  // #
	strToImageMap["#"] = hashtag;
	GLuint minus = glTexImageTGAFile("images/minussign.tga", &fontWidth, &fontHeight);  // -
	strToImageMap["-"] = minus;
	GLuint plus = glTexImageTGAFile("images/plussign.tga", &fontWidth, &fontHeight);  // +
	strToImageMap["+"] = plus;
	GLuint percent = glTexImageTGAFile("images/percentsign.tga", &fontWidth, &fontHeight);  // %
	strToImageMap["%"] = percent;
	GLuint equal = glTexImageTGAFile("images/equal.tga", &fontWidth, &fontHeight);  // =
	strToImageMap["="] = equal;
	GLuint question = glTexImageTGAFile("images/question.tga", &fontWidth, &fontHeight);  // ?
	strToImageMap["?"] = question;
	GLuint space = glTexImageTGAFile("images/space.tga", &fontWidth, &fontHeight);  // <blank space>
	strToImageMap[""] = space;
}

// Draw the textBlock to the screen
void TextBlock::draw(){
	int textSize = text.size();
	setWidth(textSize * fontWidth);
	glDrawSprite(strToImageMap[color],  characterPos[0], characterPos[1], characterSize[0], characterSize[1]);

	// For each character of text, get it's corresponding image from the map and draw it
	int offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << text[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s],  characterPos[0] + offset, characterPos[1], fontWidth, fontHeight);
		offset += fontWidth;
	}
}

void TextBlock::update(float deltaTime){
	if(moving){
		moveDown();

		if(characterPos[0] > GD::WORLD_WIDTH - characterSize[0] - GD::BLOCK_AREA_TO_RH_EDGE){
			change_x = 0; //stop block from going out of bounds (right boundry)
			characterPos[0] -= 1;
		}

		if(characterPos[0] < 0){
			change_x = 0; //stop block from going out of bounds (left boundry)
			characterPos[0] += 1;
		}

		if(characterPos[1] < 0){
			change_y = 0; //stop block from going out of bounds up (upper boundry)
			characterPos[1] += 1;
		}

		if(characterPos[1] > GD::WORLD_HEIGHT - characterSize[1] - GD::BL_FLOOR_TO_BOTTOM){
			change_y = 0; //stop block from going out of bounds up (lower boundry)
			characterPos[1] -= 1;
			moving = false;
		}

		characterPos[0] += change_x * deltaTime;
		box.setX(abs(characterPos[0]));
		characterPos[1] += change_y * deltaTime;
		box.setY(abs(characterPos[1]));
	}else{
		stop();
	}
}

void TextBlock::moveLeft(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = -speedX;
	direction = LEFT;
}

void TextBlock::moveRight(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = speedX;
	direction = RIGHT;
}

void TextBlock::moveUp(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = -speedY;
	direction = UP;
}

void TextBlock::moveDown(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = speedY;
	direction = DOWN;
}

void TextBlock::stop(){
	moving = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = 0;
}

void TextBlock::collision(Sprite &sprite){
	collided = true;
}


std::string TextBlock::to_string() const{
	ostringstream oss;
	oss << "TextBlock ************\n"
		<< "isDead = " << remove << "\n"
		<< "moving = " << moving << "\n"
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
