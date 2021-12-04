#include "GameMenu.h"

void GameMenu::event_up_press(){};
void GameMenu::event_down_press(){};
void GameMenu::event_left_press(){};
void GameMenu::event_right_press(){};
void GameMenu::event_start_press(){};
void GameMenu::event_select_press(){};
void GameMenu::event_back_press(){};
void GameMenu::event_any_press() {};

void GameMenu::process_background(SDL_Texture *background) {};

GameMenu::GameMenu() {
	for (int i = 0; i < MAX_LAYERS; i++) {
		background_layer[i] = nullptr;
	}
}