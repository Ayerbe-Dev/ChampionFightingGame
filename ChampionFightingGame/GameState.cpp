#include "GameState.h"
#include "Loader.h"
#include "GameManager.h"
#include "GameTexture.h"
#include "FontManager.h"
#include "utils.h"

void GameState::event_up_press(){}
void GameState::event_down_press(){}
void GameState::event_left_press(){}
void GameState::event_right_press(){}
void GameState::event_start_press(){}
void GameState::event_select_press(){}
void GameState::event_back_press(){}
void GameState::event_pause_press() {}
void GameState::event_any_press() {}
void GameState::process_background() {}

GameState::GameState() {
	average_ticks.reserve(10000);
	looping = nullptr;
	game_state = nullptr;
	prev_game_state = nullptr;
	game_context = nullptr;
	prev_game_context = nullptr;
	game_loader = nullptr;
	frame = 0;
	fps = 60;
	prev_fps = 0;
	FontManager* font_manager = font_manager->get_instance();

	fps_font = font_manager->load_font("FiraCode", 12);
	fps_counter.init(fps_font, std::to_string(60), glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	fps_counter.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	fps_counter.set_pos(glm::vec3(0.0, -10.0, 0.0));
	fps_texture.init(fps_font, "FPS", glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	fps_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	fps_texture.set_pos(glm::vec3(80.0, -10.0, 0.0));
}

GameState::~GameState() {
	menu_objects.clear();
	fps_font.unload_font();
	fps_counter.destroy();
	fps_texture.destroy();
}

void GameState::update_state(int game_state, int game_context) {
	if (game_state != GAME_STATE_MAX) {
		*prev_game_state = *this->game_state;
		*this->game_state = game_state;
	}
	if (game_context != GAME_CONTEXT_MAX) {
		*prev_game_context = *this->game_context;
		*this->game_context = game_context;
	}
}

void GameState::inc_thread() {
	update_thread_progress(game_loader->loaded_items);
}

void GameState::frame_delay() {
	wait_ms();
}

void GameState::frame_delay_check_fps() {
	if ((float)((std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_second).count()) / 1000.0) >= 1000.0) {
		fps = frame;
		frame = 0;
		wait_ms();
		last_second = std::chrono::high_resolution_clock::now();
	}
	else {
		wait_ms();
		frame++;
	}
	if (prev_fps != fps) {
		fps_counter.update_text(fps_font, std::to_string(fps), glm::vec4(0, 0, 0, 255), glm::vec4(0.0));
		prev_fps = fps;
	}
}

void GameState::frame_delay_check_performance() {
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

MenuObject::MenuObject(GameState* owner, MenuObject* parent, bool render_all_children) {
	this->owner = owner;
	this->parent = parent;
	this->render_all_children = render_all_children;
	active_child = 0;
}

MenuObject::MenuObject(MenuObject& other) {
	this->active_child = other.active_child;
	for (int i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
	}

	this->cursor = other.cursor;

	this->any_event = other.any_event;
	this->up_event = other.up_event;
	this->right_event = other.right_event;
	this->left_event = other.left_event;
	this->down_event = other.down_event;
	this->select_event = other.select_event;
	this->start_event = other.start_event;
	this->back_event = other.back_event;
	this->process_event = other.process_event;
}

MenuObject::MenuObject(MenuObject&& other) noexcept {
	this->active_child = other.active_child;
	for (int i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
	}

	this->cursor = other.cursor;

	this->any_event = other.any_event;
	this->up_event = other.up_event;
	this->right_event = other.right_event;
	this->left_event = other.left_event;
	this->down_event = other.down_event;
	this->select_event = other.select_event;
	this->start_event = other.start_event;
	this->back_event = other.back_event;
	this->process_event = other.process_event;
}

MenuObject::~MenuObject() {
	if (cursor.loaded) {
		cursor.destroy();
	}
	for (int i = 0, max = textures.size(); i < max; i++) {
		textures[i].destroy();
	}
	children.clear();
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

void MenuObject::add_child(bool render_all_children) {
	children.emplace_back(owner, this, render_all_children);
}

void MenuObject::add_texture(std::string path) {
	textures.emplace_back(path);
}

void MenuObject::add_texture(Font &font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs) {
	textures.emplace_back(font, text, rgba, border_rgbs);
}

void MenuObject::add_texture(const GameTexture& that) {
	textures.emplace_back(that);
}

void MenuObject::add_texture(unsigned int texture_id) {
	textures.emplace_back(GameTexture());
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