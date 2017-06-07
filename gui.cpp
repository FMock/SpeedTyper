#include"gui.h"

typedef Game_Data GD;

GUI::GUI(){
	loadImages();
	width = 20;
	height = 20;
}

void GUI::loadImages(){
	logo = glTexImageTGAFile("images/logo.tga", &width, &height);
	vertBorder = glTexImageTGAFile("images/vertBorder.tga", &width, &height);
	horzBorder = glTexImageTGAFile("images/horzBorder.tga", &width, &height);
	cloudBackground = glTexImageTGAFile("images/cloud_background.tga", &width, &height);
}

void GUI::update(float deltaTime){

}
	
void GUI::draw(){
	glDrawSprite(logo, 520, 20, 260, 100);
	glDrawSprite(cloudBackground,  0, 0, 500, 500);
	glDrawSprite(vertBorder,  0, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far left
	glDrawSprite(vertBorder,  500, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Left edge menu
	glDrawSprite(vertBorder,  780, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far right
	glDrawSprite(horzBorder,  0, 0, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Upper
	glDrawSprite(horzBorder,  0, 500, GD::BLOCK_AREA_WIDTH, GD::BORDER_WIDTH); // Block Floor
	glDrawSprite(horzBorder,  0, 580, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Lower
}

std::string GUI::to_string() const{
	ostringstream oss;
	oss << "GUI ************\n"
		<< "width = " << width << "\n";
	return oss.str();
}