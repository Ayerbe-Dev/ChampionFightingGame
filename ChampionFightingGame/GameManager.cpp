#include "GameManager.h"
#include "GameStates.h"

GameManager::GameManager() {
	player_info[0] = PlayerInfo(0);
	player_info[1] = PlayerInfo(1);
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		game_main[i] = nullptr;
	}
	for (int i = 0; i < GAME_SUBSTATE_MAX; i++) {
		game_substate_main[i] = nullptr;
	}
	set_game_state_functions();
	game_state = new int;
}

void GameManager::set_game_state_functions() {
	game_main[GAME_STATE_BATTLE] = &battle_main;
	game_main[GAME_STATE_CHARA_SELECT] = &chara_select_main;
	game_main[GAME_STATE_DEBUG_MENU] = &debugMenu;
	game_main[GAME_STATE_MENU] = &menu_main;
	game_main[GAME_STATE_TITLE_SCREEN] = &title_screen_main;

	game_substate_main[GAME_SUBSTATE_CONTROLS] = &controls_main;

}

void GameManager::update(PlayerInfo player_info[2], int game_state, int game_context) {
	this->player_info[0] = player_info[0];
	this->player_info[1] = player_info[1];
	prev_game_state = *this->game_state;
	if (game_state != GAME_STATE_MAX) {
		*this->game_state = game_state;
	}
	if (game_context != GAME_CONTEXT_MAX) {
		this->game_context = game_context;
	}
}

void GameManager::set_menu_info(GameMenu* menu_target, int init_hold_frames, int hold_rate) {
	this->init_hold_frames = init_hold_frames;
	this->hold_rate = hold_rate;
	this->menu_target = menu_target;
}

void GameManager::handle_menus() {
	for (int i = 0; i < 2; i++) {
		(&player_info[i])->update_controller();
		(&player_info[i])->update_buttons(SDL_GetKeyboardState(NULL));
		menu_target->player_id = i;
		if (is_up_press(i)) {
			event_up_press();
		}
		if (is_down_press(i)) {
			event_down_press();
		}
		if (is_left_press(i)) {
			event_left_press();
		}
		if (is_right_press(i)) {
			event_right_press();
		}
		if (player_info[i].check_button_trigger(BUTTON_MENU_START)) {
			event_start_press();
		}
		if (player_info[i].check_button_trigger(BUTTON_MENU_SELECT)) {
			event_select_press();
		}
		if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
			event_back_press();
		}
		if (is_any_menu_input(i)) {
			event_any_press();
		}
	}
}

bool GameManager::is_up_press(int id) {
	bool ret = false;
	if (player_info[id].check_button_on(BUTTON_MENU_UP)) {
		if (u_hold_frames[id] == init_hold_frames) {
			u_hold_frames[id] -= hold_rate;
			ret = true;
		}
		else {
			if (u_hold_frames[id] == 0) {
				ret = true;
			}
			u_hold_frames[id]++;
		}
	}
	else {
		u_hold_frames[id] = 0;
	}
	return ret;
}

bool GameManager::is_down_press(int id) {
	bool ret = false;
	if (player_info[id].check_button_on(BUTTON_MENU_DOWN)) {
		if (d_hold_frames[id] == init_hold_frames) {
			d_hold_frames[id] -= hold_rate;
			ret = true;
		}
		else {
			if (d_hold_frames[id] == 0) {
				ret = true;
			}
			d_hold_frames[id]++;
		}
	}
	else {
		d_hold_frames[id] = 0;
	}
	return ret;
}

bool GameManager::is_right_press(int id) {
	bool ret = false;
	if (player_info[id].check_button_on(BUTTON_MENU_RIGHT)) {
		if (r_hold_frames[id] == init_hold_frames) {
			r_hold_frames[id] -= hold_rate;
			ret = true;
		}
		else {
			if (r_hold_frames[id] == 0) {
				ret = true;
			}
			r_hold_frames[id]++;
		}
	}
	else {
		r_hold_frames[id] = 0;
	}
	return ret;
}

bool GameManager::is_left_press(int id) {
	bool ret = false;
	if (player_info[id].check_button_on(BUTTON_MENU_LEFT)) {
		if (l_hold_frames[id] == init_hold_frames) {
			l_hold_frames[id] -= hold_rate;
			ret = true;
		}
		else {
			if (l_hold_frames[id] == 0) {
				ret = true;
			}
			l_hold_frames[id]++;
		}
	}
	else {
		l_hold_frames[id] = 0;
	}
	return ret;
}

bool GameManager::is_any_menu_input(int id) {
	for (int i = 0; i < BUTTON_MAX; i++) {
		if (player_info[id].check_button_trigger(i)) {
			return true;
		}
	}
	return false;
}

void GameManager::event_up_press() {
	(menu_target->*(&GameMenu::event_up_press))();
}

void GameManager::event_down_press() {
	(menu_target->*(&GameMenu::event_down_press))();
}

void GameManager::event_left_press() {
	(menu_target->*(&GameMenu::event_left_press))();
}

void GameManager::event_right_press() {
	(menu_target->*(&GameMenu::event_right_press))();
}

void GameManager::event_start_press() {
	(menu_target->*(&GameMenu::event_start_press))();
}

void GameManager::event_select_press() {
	(menu_target->*(&GameMenu::event_select_press))();
}

void GameManager::event_back_press() {
	(menu_target->*(&GameMenu::event_back_press))();
}

void GameManager::event_any_press() {
	(menu_target->*(&GameMenu::event_any_press))();
}