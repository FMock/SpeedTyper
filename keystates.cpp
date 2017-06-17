#include"keystates.h"

/*
	Track Keyboard Presses
*/

typedef Game_Data GD;
using std::ostringstream;

KeyStates::KeyStates(Game_Data& gd){
	std::fill_n(states, 128, 0);
	optionButtonPressed = false;
	optionButtonPressedCount = 0;
	gameData = &gd;
}

void KeyStates::zeroAllKeyStates(){
	std::fill_n(states, 128, 0);
	optionButtonPressed = false;
}

/*
	Sets the state for any key pressed to 1 in the states array
*/
void KeyStates::setKeyPressed(){

	SDL_Keymod modstates = SDL_GetModState(); // to check for modifier keys like shift or ctrl

	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT){

				// Determine where the player clicked
				int xPos = event.button.x;
				int yPos = event.button.y;
				printf("xPos = %d\n", xPos);
				printf("yPos = %d\n", yPos);

				if(xPos > GD::OPTION_BUTTON_MIN_X && xPos < GD::OPTION_BUTTON_MAX_X && 
					yPos > GD::OPTION_BUTTON_MIN_Y && yPos < GD::OPTION_BUTTON_MAX_Y){
					optionButtonPressed = true;
					optionButtonPressedCount += 1; // odd numbers correspond to open and even to close

					// Update options variable in GameData < TO DO >
				}
			}
			break;
			
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_BACKSPACE:
				states[SDLK_BACKSPACE] += 1;
				break;
			case SDLK_RETURN:
				states[SDLK_RETURN] += 1;
				break;
			case SDLK_SPACE:
				states[SDLK_SPACE] += 1;
				break;
			case SDLK_COMMA:
				states[SDLK_COMMA] += 1;
				break;
			case SDLK_PERIOD:
				states[SDLK_PERIOD] += 1;
				break;
			case SDLK_EQUALS:
				if(modstates & KMOD_SHIFT)
					states[SDLK_PLUS] += 1; // +
				else
					states[SDLK_EQUALS] += 1; // =
				break;
			case SDLK_MINUS:
				states[SDLK_MINUS] += 1; // -
				break;
			case SDLK_DOLLAR:
				states[SDLK_DOLLAR] += 1;
				break;
			case SDLK_SLASH:
				if(modstates & KMOD_SHIFT)
					states[SDLK_QUESTION] += 1; // ?
				else
					states[SDLK_SLASH] += 1; // /
				break;
			case SDLK_0:
				states[SDLK_0] += 1;
				break;
			case SDLK_1:
				if(modstates & KMOD_SHIFT)
					states[SDLK_EXCLAIM] += 1; // !
				else
					states[SDLK_1] += 1; // 1
				break;
			case SDLK_2:
				states[SDLK_2] += 1;
				break;
			case SDLK_3:
				if(modstates & KMOD_SHIFT)
					states[SDLK_HASH] += 1; // #
				else
					states[SDLK_3] += 1; // 3
				break;
			case SDLK_4:
				if(modstates & KMOD_SHIFT)
					states[SDLK_DOLLAR] += 1; // $
				else
					states[SDLK_4] += 1; // 4
				break;
			case SDLK_5:
				if(modstates & KMOD_SHIFT)
					states[SDLK_PERCENT] += 1; // %
				else
					states[SDLK_5] += 1; // 5
				break;
			case SDLK_6:
				states[SDLK_6] += 1;
				break;
			case SDLK_7:
				states[SDLK_7] += 1;
				break;
			case SDLK_8:
				states[SDLK_8] += 1;
				break;
			case SDLK_9:
				states[SDLK_9] += 1;
				break;
			case SDLK_a:
				states[SDLK_a] += 1;
				break;
			case SDLK_b:
				states[SDLK_b] += 1;
				break;
			case SDLK_c:
				states[SDLK_c] += 1;
				break;
			case SDLK_d:
				states[SDLK_d] += 1;
				break;
			case SDLK_e:
				states[SDLK_e] += 1;
				break;
			case SDLK_f:
				states[SDLK_f] += 1;
				break;
			case SDLK_g:
				states[SDLK_g] += 1;
				break;
			case SDLK_h:
				states[SDLK_h] += 1;
				break;
			case SDLK_i:
				states[SDLK_i] += 1;
				break;
			case SDLK_j:
				states[SDLK_j] += 1;
				break;
			case SDLK_k:
				states[SDLK_k] += 1;
				break;
			case SDLK_l:
				states[SDLK_l] += 1;
				break;
			case SDLK_m:
				states[SDLK_m] += 1;
				break;
			case SDLK_n:
				states[SDLK_n] += 1;
				break;
			case SDLK_o:
				states[SDLK_o] += 1;
				break;
			case SDLK_p:
				states[SDLK_p] += 1;
				break;
			case SDLK_q:
				states[SDLK_q] += 1;
				break;
			case SDLK_r:
				states[SDLK_r] += 1;
				break;
			case SDLK_s:
				states[SDLK_s] += 1;
				break;
			case SDLK_t:
				states[SDLK_t] += 1;
				break;
			case SDLK_u:
				states[SDLK_u] += 1;
				break;
			case SDLK_v:
				states[SDLK_v] += 1;
				break;
			case SDLK_w:
				states[SDLK_w] += 1;
				break;
			case SDLK_x:
				states[SDLK_x] += 1;
				break;
			case SDLK_y:
				states[SDLK_y] += 1;
				break;
			case SDLK_z:
				states[SDLK_z] += 1;
				break;
			}
			break;
		}
	}
}


bool KeyStates::getOptionButtonPressed(){
	return optionButtonPressed;
}

std::string KeyStates::to_string() const{
	ostringstream oss;

	for(int i = 0; i < NUM_KEYS; i++){
		if(states[i] == 1){
			oss <<  states[i] << "\n";
		}
	}

	oss << "optionButtonPressed = " << optionButtonPressed << "\n";
	return oss.str();
}


