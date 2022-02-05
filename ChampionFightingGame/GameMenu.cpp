#include "GameMenu.h"

void GameMenu::event_up_press(){};
void GameMenu::event_down_press(){};
void GameMenu::event_left_press(){};
void GameMenu::event_right_press(){};
void GameMenu::event_start_press(){};
void GameMenu::event_select_press(){};
void GameMenu::event_back_press(){};
void GameMenu::event_any_press() {};

void GameMenu::process_background() {};

GameMenu::GameMenu() {

}

void GameMenu::update_state(int game_state, int game_context) {
	if (game_state != GAME_STATE_MAX) {
		*prev_game_state = *this->game_state;
		*this->game_state = game_state;
	}
	if (game_context != GAME_CONTEXT_MAX) {
		*prev_game_context = *this->game_context;
		*this->game_context = game_context;
	}
}