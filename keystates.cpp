#include"keystates.h"

/*
	Track Keyboard Presses
*/

using std::ostringstream;

KeyStates::KeyStates(){
	std::fill_n(states, 128, 0);
}

void KeyStates::zeroAllKeyStates(){
	std::fill_n(states, 128, 0);
}

/*
	Sets the state for any key pressed to 1 in the states array
*/
void KeyStates::setKeyPressed(){

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_RETURN:
				states[SDLK_RETURN] += 1;
				break;
			case SDLK_BACKSPACE:
				states[SDLK_BACKSPACE] += 1;
				break;
			case SDLK_SPACE:
				states[SDLK_SPACE] += 1;
				break;
			case SDLK_0:
				states[SDLK_0] += 1;
				break;
			case SDLK_1:
				states[SDLK_1] += 1;
				break;
			case SDLK_2:
				states[SDLK_2] += 1;
				break;
			case SDLK_3:
				states[SDLK_3] += 1;
				break;
			case SDLK_4:
				states[SDLK_4] += 1;
				break;
			case SDLK_5:
				states[SDLK_5] += 1;
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

std::string KeyStates::to_string() const{
	ostringstream oss;
	oss << "*************************\n"
		<< "P = " <<  states[19] << "\n";
	return oss.str();
}


