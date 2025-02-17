#include "GameManager.h"
#include "FontManager.h"
#include "InputManager.h"
#include "GameStates.h"
#include "TimeFuncs.h"
#include "TargetVar.h"

GameManager::GameManager() {
	for (int i = 0; i < 2; i++) {
		player[i] = new Player(i);
		player[i]->load_player_info(-1);
		u_hold_frames[i] = 0;
		d_hold_frames[i] = 0;
		l_hold_frames[i] = 0;
		r_hold_frames[i] = 0;
	}
	for (int i = 0; i < SCENE_MAX; i++) {
		scene_main[i] = nullptr;
	}
	scene_main[SCENE_BATTLE] = &battle_main;
	scene_main[SCENE_CHARA_SELECT] = &chara_select_main;
	scene_main[SCENE_STAGE_SELECT] = &stage_select_main;
	scene_main[SCENE_DEBUG_MENU] = &debug_main;
	scene_main[SCENE_MAIN_MENU] = &main_menu_main;
	scene_main[SCENE_TITLE_SCREEN] = &title_screen_main;
	scene_main[SCENE_CONTROLS] = &controls_main;
	scene_main[SCENE_PAUSE_BATTLE] = &pause_battle_main;

	next_scene = SCENE_DEBUG_MENU;
	next_scene_context = SCENE_CONTEXT_NONE;

#ifdef DEBUG
	frame_capped = true;
#endif

	FontManager* font_manager = font_manager->get_instance();
	frame = 0;
	fps = 60;
	prev_fps = 0;
	fps_font = font_manager->load_font("FiraCode", 36);
	fps_counter.init(&fps_font, "60 FPS", TextSpecifier().color(glm::vec3(255.0f))).set_orientation(TEXTURE_TOP_LEFT);
}

void GameManager::update_scene(int next_scene, int next_scene_context) {
	if (next_scene != SCENE_MAX) {
		if (next_scene != this->next_scene) {
			this->next_scene = next_scene;
		}
		if (next_scene == SCENE_CLOSE) {
			for (int i = 0; i < scenes.size(); i++) {
				scenes[i]->looping = false;
			}
		}
		else {
			scenes.back()->looping = false;
		}
	}
	if (next_scene_context != SCENE_CONTEXT_MAX) {
		if (next_scene_context != this->next_scene_context) {
			this->next_scene_context = next_scene_context;
		}
	}
}

void GameManager::set_scene(Scene* scene) {
	this->scenes.push_back(scene);
	scene->context = next_scene_context;
}

void GameManager::delete_scene() {
	scenes.pop_back();
}

Scene* GameManager::get_scene(int depth) {
	if (depth >= scenes.size()) {
		std::cout << "Tried to get GameState from invalid depth: " << depth << "\n";
		return scenes.back();
	}
	else {
		return scenes[scenes.size() - 1 - depth];
	}
}

void GameManager::process_events() {
	for (int i = 0; i < 2; i++) {
		scenes.back()->player_id = i;
		if (is_up_press(i)) {
			get_scene()->execute_event("up_press");
		}
		if (is_down_press(i)) {
			get_scene()->execute_event("down_press");
		}
		if (is_left_press(i)) {
			get_scene()->execute_event("left_press");
		}
		if (is_right_press(i)) {
			get_scene()->execute_event("right_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_START)) {
			get_scene()->execute_event("start_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_SELECT)) {
			get_scene()->execute_event("select_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_BACK)) {
			get_scene()->execute_event("back_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_PAGE_LEFT)) {
			get_scene()->execute_event("page_left_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_PAGE_RIGHT)) {
			get_scene()->execute_event("page_right_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_FRAME_PAUSE)) {
			get_scene()->execute_event("frame_pause_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_FRAME_ADVANCE)) {
			get_scene()->execute_event("frame_advance_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_RECORD_INPUT)) {
			get_scene()->execute_event("record_input_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_REPLAY_INPUT)) {
			get_scene()->execute_event("replay_input_press");
		}
		if (player[i]->controller.check_button_trigger(BUTTON_MENU_SWITCH_INPUT)) {
			get_scene()->execute_event("switch_input_press");
		}
		if (is_any_menu_input(i)) {
			get_scene()->execute_event("button_press");
		}
	}
	if (is_crash()) {
		update_scene(SCENE_DEBUG_MENU);
	}
}

void GameManager::render() {
	for (size_t i = 0, max = scenes.size(); i < max; i++) {
		scenes[i]->render_main();
	}
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

void GameManager::add_crash_log(std::string crash_reason) {
	crash_log.push(crash_reason);
	update_scene(SCENE_DEBUG_MENU);
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
	InputManager::get_instance()->check_controllers();
#ifdef DEBUG
	if (frame_capped) {
#endif
	wait_ms();
#ifdef DEBUG
	}
#endif
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
		fps_counter.update_text(std::to_string(fps) + " FPS");
		prev_fps = fps;
	}
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
	if (instance != nullptr) {
		delete instance;
	}
}