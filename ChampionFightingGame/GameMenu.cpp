#include "GameMenu.h"
#include "Loader.h"
#include "GameManager.h"
#include "GameTexture.h"

void GameMenu::event_up_press(){}
void GameMenu::event_down_press(){}
void GameMenu::event_left_press(){}
void GameMenu::event_right_press(){}
void GameMenu::event_start_press(){}
void GameMenu::event_select_press(){}
void GameMenu::event_back_press(){}
void GameMenu::event_pause_press() {}
void GameMenu::event_any_press() {}
void GameMenu::process_background() {}

void GameMenu::load_game_menu() {
	GameManager::get_instance()->set_menu_info(this);
}

GameMenu::GameMenu() {
	average_ticks.reserve(10000);
	looping = nullptr;
	game_state = nullptr;
	prev_game_state = nullptr;
	game_context = nullptr;
	prev_game_context = nullptr;
	game_loader = nullptr;
	frame = 0;
	fps = 60;
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

void GameMenu::inc_thread() {
	update_thread_progress(game_loader->loaded_items);
}

void GameMenu::frame_delay() {
	wait_ms();
}

void GameMenu::frame_delay_check_fps() {
	if ((float)((std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_second).count()) / 1000.0) >= 1000.0) {
		fps = frame;
		frame = 0;
//		wait_ms();
		last_second = std::chrono::high_resolution_clock::now();
	}
	else {
//		wait_ms();
		frame++;
	}
}

void GameMenu::frame_delay_check_performance() {
	int trials = 10000;

	if (average_ticks.size() < trials) {
		average_ticks.push_back((float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ms).count())/1000.0);
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
	wait_ms();
	ms = std::chrono::high_resolution_clock::now();
}

MenuObject::MenuObject() {
	owner = nullptr;
	parent = nullptr;
	render_all_children = false;
	active_child = 0;
}

MenuObject::MenuObject(GameMenu* owner, MenuObject* parent, bool render_all_children) {
	this->owner = owner;
	this->parent = parent;
	this->render_all_children = render_all_children;
	active_child = 0;
}

void MenuObject::render() {
	for (int i = 0, max = textures.size(); i < max; i++) {
		textures[i].process();
		textures[i].render();
	}
	if (render_all_children) {
		for (int i = 0, max = children.size(); i < max; i++) {
			children[i].render();
		}
	}
	else if (active_child != -1 && !children.empty()) {
		children[active_child].render();
	}
}

void MenuObject::destroy() {
	for (int i = 0, max = textures.size(); i < max; i++) {
		textures[i].destroy();
	}
	for (int i = 0, max = children.size(); i < max; i++) {
		children[i].destroy();
	}
}

void MenuObject::add_child(bool render_all_children) {
	children.push_back(MenuObject(owner, this, render_all_children));
}

void MenuObject::add_texture(std::string path) {
	textures.push_back(GameTexture(path));
}

void MenuObject::add_texture(Font font, std::string text, glm::vec4 rgba, float border_x, float border_y) {
	textures.push_back(GameTexture(font, text, rgba, border_x, border_y));
}

void MenuObject::add_texture(const GameTexture& that) {
	textures.push_back(that);
}

void MenuObject::add_texture(unsigned int texture_id) {
	textures.push_back(GameTexture());
	textures.back().init(texture_id);
}

void MenuObject::event_up_press() {
	up_event(this);
}

void MenuObject::event_down_press() {
	down_event(this);
}

void MenuObject::event_left_press() {
	left_event(this);
}

void MenuObject::event_right_press() {
	right_event(this);
}

void MenuObject::event_select_press() {
	select_event(this);
}

void MenuObject::event_start_press() {
	start_event(this);
}

void MenuObject::event_back_press() {
	back_event(this);
}

void MenuObject::event_any_press() {
	any_event(this);
}

void MenuObject::process() {
	process_event(this);
}