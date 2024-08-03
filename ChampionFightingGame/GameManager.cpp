#include "GameManager.h"
#include "FontManager.h"
#include "GameStates.h"
#include "TimeFuncs.h"
#include "TargetVar.h"

GameManager::GameManager() {
	for (int i = 0; i < 2; i++) {
		player[i] = new Player(i);
		player[i]->load_player(-1);
		u_hold_frames[i] = 0;
		d_hold_frames[i] = 0;
		l_hold_frames[i] = 0;
		r_hold_frames[i] = 0;
	}
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		game_main[i] = nullptr;
	}
	game_main[GAME_STATE_BATTLE] = &battle_main;
	game_main[GAME_STATE_CHARA_SELECT] = &chara_select_main;
	game_main[GAME_STATE_STAGE_SELECT] = &stage_select_main;
	game_main[GAME_STATE_DEBUG_MENU] = &debug_main;
	game_main[GAME_STATE_MENU] = &menu_main;
	game_main[GAME_STATE_TITLE_SCREEN] = &title_screen_main;
	game_main[GAME_STATE_CONTROLS] = &controls_main;
	game_main[GAME_STATE_PAUSE_BATTLE] = &pause_battle_main;

	next_game_state = GAME_STATE_CHARA_SELECT;
	next_game_context = GAME_CONTEXT_TRAINING;

	FontManager* font_manager = font_manager->get_instance();
	average_ticks.reserve(10000);
	frame = 0;
	fps = 60;
	prev_fps = 0;
	fps_font = font_manager->load_font("FiraCode", 12);
	fps_counter.init(fps_font, std::to_string(60), glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	fps_counter.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	fps_counter.set_pos(glm::vec3(0.0, 0.0, 0.0));
	fps_texture.init(fps_font, "FPS", glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	fps_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	fps_texture.set_pos(glm::vec3(80.0, 0.0, 0.0));
}

void GameManager::update_state(int next_game_state, int next_game_context) {
	if (next_game_state != GAME_STATE_MAX) {
		if (next_game_state != this->next_game_state) {
			this->next_game_state = next_game_state;
		}
		if (next_game_state == GAME_STATE_CLOSE) {
			for (int i = 0; i < game_state.size(); i++) {
				game_state[i]->looping = false;
			}
		}
		else {
			game_state.back()->looping = false;
		}
	}
	if (next_game_context != GAME_CONTEXT_MAX) {
		if (next_game_context != this->next_game_context) {
			this->next_game_context = next_game_context;
		}
	}
}

void GameManager::set_game_state(GameState* game_state) {
	//Initialize GameManager values, assign the GameManager a target
	this->game_state.push_back(game_state);
	TargetVarManager::get_instance()->push_game_state_target_set();
	game_state->game_context = next_game_context;
}

void GameManager::delete_game_state() {
	game_state.pop_back();
	TargetVarManager::get_instance()->pop_game_state_target_set();
}

GameState* GameManager::get_game_state(int depth) {
	if (depth >= game_state.size()) {
		std::cout << "Tried to get GameState from invalid depth: " << depth << "\n";
		return game_state.back();
	}
	else {
		return game_state[game_state.size() - 1 - depth];
	}
}

void GameManager::process_game_state_events() {
	for (int i = 0; i < 2; i++) {
		game_state.back()->player_id = i;
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
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_PAGE_LEFT)) {
			event_page_left_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_PAGE_RIGHT)) {
			event_page_right_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_FRAME_PAUSE)) {
			event_frame_pause_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_FRAME_ADVANCE)) {
			event_frame_advance_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_RECORD_INPUT)) {
			event_record_input_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_REPLAY_INPUT)) {
			event_replay_input_press();
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_SWITCH_INPUT)) {
			event_switch_input_press();
		}
		if (is_any_menu_input(i)) {
			event_any_press();
		}
	}
	if (is_crash()) {
		update_state(GAME_STATE_DEBUG_MENU);
	}
}

void GameManager::render_game_states() {
	for (size_t i = 0, max = game_state.size(); i < max; i++) {
		game_state[i]->render_main();
		for (std::list<UIMessage>::iterator it = game_state[i]->messages_active.begin(),
			max = game_state[i]->messages_active.end();
			it != max; it++) {
			it->render();
		}
		for (std::list<UIMessage>::iterator it = game_state[i]->messages_fading.begin(),
			max = game_state[i]->messages_fading.end();
			it != max; it++) {
			it->render();
		}
	}
	fps_texture.render();
	fps_counter.render();
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
	for (int i = BUTTON_MENU_UP; i < BUTTON_MENU_MAX; i++) {
		if (player[id]->controller.check_button_trigger(i)) {
			return true;
		}
	}
	return false;
}

void GameManager::event_up_press() {
	(get_game_state()->*(&GameState::event_up_press))();
}

void GameManager::event_down_press() {
	(get_game_state()->*(&GameState::event_down_press))();
}

void GameManager::event_left_press() {
	(get_game_state()->*(&GameState::event_left_press))();
}

void GameManager::event_right_press() {
	(get_game_state()->*(&GameState::event_right_press))();
}

void GameManager::event_start_press() {
	(get_game_state()->*(&GameState::event_start_press))();
}

void GameManager::event_select_press() {
	(get_game_state()->*(&GameState::event_select_press))();
}

void GameManager::event_back_press() {
	(get_game_state()->*(&GameState::event_back_press))();
}

void GameManager::event_page_left_press() {
	(get_game_state()->*(&GameState::event_page_left_press))();
}

void GameManager::event_page_right_press() {
	(get_game_state()->*(&GameState::event_page_right_press))();
}

void GameManager::event_frame_pause_press() {
	(get_game_state()->*(&GameState::event_frame_pause_press))();
}

void GameManager::event_frame_advance_press() {
	(get_game_state()->*(&GameState::event_frame_advance_press))();
}

void GameManager::event_record_input_press() {
	(get_game_state()->*(&GameState::event_record_input_press))();
}

void GameManager::event_replay_input_press() {
	(get_game_state()->*(&GameState::event_replay_input_press))();
}

void GameManager::event_switch_input_press() {
	(get_game_state()->*(&GameState::event_switch_input_press))();
}

void GameManager::event_any_press() {
	(get_game_state()->*(&GameState::event_any_press))();
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

void GameManager::frame_delay() {
	TargetVarManager::get_instance()->process_targets();
//	wait_ms();
}

void GameManager::frame_delay_check_fps() {
	if ((float)((std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_second).count()) / 1000.0) >= 1000.0) {
		fps = frame;
		frame = 0;
		frame_delay();
		last_second = std::chrono::high_resolution_clock::now();
	}
	else {
		frame_delay();
		frame++;
	}
	if (prev_fps != fps) {
		fps_counter.update_text(fps_font, std::to_string(fps), glm::vec4(0, 0, 0, 255), glm::vec4(0.0));
		prev_fps = fps;
	}
}

void GameManager::frame_delay_check_performance() {
	int trials = 10000;

	if (average_ticks.size() < trials) {
		average_ticks.push_back((float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ms).count()) / 1000.0);
	}
	else {
		float highest = average_ticks[0];
		while (highest >= tick_frequency.size()) {
			tick_frequency.push_back(0);
		}
		int freq = 0;
		int frame_freq = 0;
		float total = 0;
		for (int i = 0; i < trials; i++) {
			total += average_ticks[i];
			if (average_ticks[i] >= 16.667) {
				frame_freq++;
			}
			if (average_ticks[i] > highest) {
				highest = average_ticks[i];
				while (highest >= tick_frequency.size()) {
					tick_frequency.push_back(0);
				}
				freq = 1;
			}
			else if (average_ticks[i] == highest) {
				freq++;
			}
			tick_frequency[(int)average_ticks[i]]++;
		}
		total /= (float)trials;
		std::cout << "Lengths of all iterations across " << trials << " tests: " << "\n";
		for (int i = 0; i < tick_frequency.size(); i++) {
			std::cout << "MS: " << i << ", Frequency: " << tick_frequency[i] << "\n";
		}
		std::cout << "On average, it took " << total << " ms to run the loop, and there were " << frame_freq << " instances of an iteration taking more than a frame." << "\n";
		average_ticks.clear();
		tick_frequency.clear();
	}
	frame_delay();
	ms = std::chrono::high_resolution_clock::now();
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
	fps_font.unload_font();
	fps_counter.destroy();
	fps_texture.destroy();
	if (instance != nullptr) {
		delete instance;
	}
}