#include "GameManager.h"
#include "GameStates.h"
#include "RenderManager.h"

GameManager::GameManager() {
	player[0] = new Player(0);
	player[1] = new Player(1);
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		game_main[i] = nullptr;
	}
	for (int i = 0; i < GAME_SUBSTATE_MAX; i++) {
		game_substate_main[i] = nullptr;
	}
	set_game_state_functions();
	game_state = new int;
	prev_game_state = new int;
	game_context = new int;
	prev_game_context = new int;

	*game_state = GAME_STATE_DEBUG_MENU;
	*prev_game_state = *game_state;
	*game_context = GAME_CONTEXT_NORMAL;
	*prev_game_context = *game_context;
	render_manager = RenderManager::get_instance();
}

void GameManager::set_game_state_functions() {
	game_main[GAME_STATE_BATTLE] = &battle_main;
	game_main[GAME_STATE_CHARA_SELECT] = &chara_select_main;
	game_main[GAME_STATE_DEBUG_MENU] = &debug_main;
	game_main[GAME_STATE_MENU] = &menu_main;
	game_main[GAME_STATE_TITLE_SCREEN] = &title_screen_main;

	game_substate_main[GAME_SUBSTATE_CONTROLS] = &controls_main;
	game_substate_main[GAME_SUBSTATE_PAUSE_BATTLE] = &pause_battle_main;

}

void GameManager::update_state(int game_state, int game_context) {
	if (game_state != GAME_STATE_MAX) {
		if (game_state != *this->game_state) {
			*prev_game_state = *this->game_state;
			*this->game_state = game_state;
		}
		if (game_state == GAME_STATE_CLOSE) {
			for (int i = 0; i < MAX_LAYERS; i++) {
				looping[i] = false;
			}
		}
		else {
			looping[layer] = false;
		}
	}
	if (game_context != GAME_CONTEXT_MAX) {
		if (game_context != *this->game_context) {
			*prev_game_context = *this->game_context;
			*this->game_context = game_context;
		}
	}
}

void GameManager::set_menu_info(GameMenu* menu_target, int init_hold_frames, int hold_rate) {
	//Initialize GameManager values, assign the GameManager a target
	this->init_hold_frames = init_hold_frames;
	this->hold_rate = hold_rate;
	this->menu_target[layer] = menu_target;

	looping[layer] = true;

	//Assign a few pointers from the current target to match the GameManager
	if (this->menu_target[layer] != nullptr) {
		this->menu_target[layer]->game_state = game_state;
		this->menu_target[layer]->prev_game_state = game_state;
		this->menu_target[layer]->game_context = game_context;
		this->menu_target[layer]->prev_game_context = prev_game_context;
		this->menu_target[layer]->looping = &looping[layer];
	}
}

GameMenu* GameManager::get_target(int layer) {
	if (layer == -1) {
		return menu_target[this->layer];
	}
	else if (layer >= MAX_LAYERS) {
		std::cout << "Tried to get invalid layer target: " << layer << "\n";
		return menu_target[this->layer];
	}
	else {
		return menu_target[layer];
	}
}

void GameManager::handle_window_events(std::function<void(SDL_Event*)> event_handler) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event_handler != nullptr) {
			event_handler(&event);
		}
		switch (event.type) {
		case SDL_QUIT: {
			update_state(GAME_STATE_CLOSE);
		} break;
		case SDL_WINDOWEVENT:
		{
			switch (event.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			{
				SDL_GetWindowSize(render_manager->window, &render_manager->s_window_width, &render_manager->s_window_height);
				glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);
			} break;
			}
		} break;
		}
	}
	SDL_PumpEvents();
}

void GameManager::handle_menus() {
	for (int i = 0; i < 2; i++) {
		menu_target[layer]->player_id = i;
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
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_START)) {
			event_start_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_SELECT)) {
			event_select_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_BACK)) {
			event_back_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_PAUSE)) {
			event_pause_press();
		}
		if (is_any_menu_input(i)) {
			event_any_press();
		}
	}
	if (is_crash()) {
		update_state(GAME_STATE_DEBUG_MENU);
	}
}

bool GameManager::is_up_press(int id) {
	bool ret = false;
	if (player[id]->controller.check_button_on(BUTTON_MENU_UP)) {
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
	if (player[id]->controller.check_button_on(BUTTON_MENU_DOWN)) {
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
	if (player[id]->controller.check_button_on(BUTTON_MENU_RIGHT)) {
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
	if (player[id]->controller.check_button_on(BUTTON_MENU_LEFT)) {
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
		if (player[id]->controller.check_button_trigger(i)) {
			return true;
		}
	}
	return false;
}

void GameManager::event_up_press() {
	(menu_target[layer]->*(&GameMenu::event_up_press))();
}

void GameManager::event_down_press() {
	(menu_target[layer]->*(&GameMenu::event_down_press))();
}

void GameManager::event_left_press() {
	(menu_target[layer]->*(&GameMenu::event_left_press))();
}

void GameManager::event_right_press() {
	(menu_target[layer]->*(&GameMenu::event_right_press))();
}

void GameManager::event_start_press() {
	(menu_target[layer]->*(&GameMenu::event_start_press))();
}

void GameManager::event_select_press() {
	(menu_target[layer]->*(&GameMenu::event_select_press))();
}

void GameManager::event_back_press() {
	(menu_target[layer]->*(&GameMenu::event_back_press))();
}

void GameManager::event_pause_press() {
	(menu_target[layer]->*(&GameMenu::event_pause_press))();
}

void GameManager::event_any_press() {
	(menu_target[layer]->*(&GameMenu::event_any_press))();
}

void GameManager::add_crash_log(std::string crash_reason) {
	crash_log.push(crash_reason);
	update_state(GAME_STATE_DEBUG_MENU);
}

bool GameManager::get_crash_log(std::string* ret) {
	if (crash_log.empty()) {
		return false;
	}

	*ret = crash_log.front();
	crash_log.pop();
	return true;
}

bool GameManager::is_crash() {
	return !crash_log.empty();
}

GameManager* GameManager::instance = nullptr;
GameManager* GameManager::get_instance() {
	if (instance == nullptr) {
		instance = new GameManager;
	}
	return instance;
}

void GameManager::destroy_instance() {
	delete player[0];
	delete player[1];
	delete game_state;
	delete prev_game_state;
	delete game_context;
	delete prev_game_context;
	if (instance != nullptr) {
		delete instance;
	}
}