#include"gui.h"

typedef Game_Data GD;

GUI::GUI(KeyStates& k){
	keyStates = &k;
	loadImages();
	width = 20;
	height = 20;
	options = false;
}

void GUI::loadImages(){
	logo = glTexImageTGAFile("images/logo.tga", &width, &height);
	vertBorder = glTexImageTGAFile("images/vertBorder.tga", &width, &height);
	horzBorder = glTexImageTGAFile("images/horzBorder.tga", &width, &height);
	cloudBackground = glTexImageTGAFile("images/cloud_background.tga", &width, &height);
	textTypeArea = glTexImageTGAFile("images/textTypeArea.tga", &width, &height);
	optionsClosed = glTexImageTGAFile("images/options.tga", &width, &height);
	optionsOpen = glTexImageTGAFile("images/options_open.tga", &width, &height);
	helpClosed = glTexImageTGAFile("images/help.tga", &width, &height);
	aboutClosed = glTexImageTGAFile("images/about.tga", &width, &height);
}

void GUI::update(float deltaTime){
	options = keyStates->getOptionButtonPressed();
}
	
void GUI::draw(){
	glDrawSprite(logo, 520, 20, 260, 100);

	// Options Menu
	if(keyStates->optionButtonPressedCount % 2 == 0){
		glDrawSprite(optionsClosed, 520, 220, 260, 40);
			// Help menu
		glDrawSprite(helpClosed, 520, 260, 260, 40);
		// About menu
		glDrawSprite(aboutClosed, 520, 300, 260, 40);
	}else{
		glDrawSprite(optionsOpen, 520, 220, 260, 300);
	}


	glDrawSprite(cloudBackground,  0, 0, 500, 500);
	glDrawSprite(vertBorder,  0, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far left
	glDrawSprite(vertBorder,  500, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Left edge menu
	glDrawSprite(vertBorder,  780, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far right
	glDrawSprite(horzBorder,  0, 0, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Upper
	glDrawSprite(horzBorder,  0, 500, GD::BLOCK_AREA_WIDTH, GD::BORDER_WIDTH); // Block Floor
	glDrawSprite(horzBorder,  0, 580, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Lower
	glDrawSprite(textTypeArea,  20, 520, GD::WINDOW_WIDTH - 40, 60); // text typing area background
}

void GUI::setOptions(bool b){
	options = b;
}

bool GUI::getOptions() const{
	return options;
}

std::string GUI::to_string() const{
	ostringstream oss;
	oss << "GUI ************\n"
		<< "options = " << options << "\n";
	return oss.str();
}