#include"gui.h"

typedef Game_Data GD;

GUI::GUI(KeyStates& k, Game_Data& g){
	keyStates = &k;
	gameData = &g;
	loadImages();
	width = 20;
	height = 20;
	options = false;
	menuItemSelected = "default";
	gameData->menuItemSelected = menuItemSelected;
	selectedIndex = 0;
	buildSelections();
}

// Fills vector with Selection objects for category menu itmes
void GUI::buildSelections(){
	selectionItems.push_back(Selection("default", 745, 767, 286, 303, true));
	selectionItems.push_back(Selection("animals", 745, 767, 312, 329, false));
	selectionItems.push_back(Selection("places", 745, 767, 339, 356, false));
	selectionItems.push_back(Selection("things", 745, 767, 364, 381, false));
	selectionItems.push_back(Selection("transportation", 745, 767, 390, 407, false));
	selectionItems.push_back(Selection("music", 745, 767, 416, 433, false));
	selectionItems.push_back(Selection("anime", 745, 767, 440, 457, false));
	selectionItems.push_back(Selection("music_and_sound", 745, 767, 490, 507, false));
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

	for(int i = 0; i < selectionItems.size(); i++){
		if(selectionItems.at(i).selected)
			menuItemSelected = selectionItems.at(i).categoryName;
	}

	// If options are being shown, check/change menu selection
	if(keyStates->optionsDisplayed){
		if(keyStates->mouseClicked){
			for(int i = 0; i < selectionItems.size(); i++){
				if(keyStates->mouseX >= selectionItems.at(i).xMin &&
					keyStates->mouseX <= selectionItems.at(i).xMax &&
					keyStates->mouseY >= selectionItems.at(i).yMin &&
					keyStates->mouseY <= selectionItems.at(i).yMax){
					
					    // A menus item was clicked
						if(i != selectedIndex){
							selectionItems.at(i).selected = true;
							selectionItems.at(selectedIndex).selected = false;
							selectedIndex = i;
							menuItemSelected = selectionItems.at(i).categoryName;
							gameData->menuItemSelected = menuItemSelected;
						}
				}
			}
		}
	} // End check/change menu selection
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

	// Only draw the menu selections if the options are being displayed
	if(keyStates->optionsDisplayed){
		for(int i = 0; i < selectionItems.size(); i++){
			selectionItems.at(i).draw();
		}
	}
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
		<< "menuItemSelected = " << menuItemSelected << "\n"
		<< "options = " << options << "\n";
	return oss.str();
}