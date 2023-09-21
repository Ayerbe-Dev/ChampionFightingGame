#include "GameState.h"
#include "Loader.h"
#include "GameTexture.h"
#include "FontManager.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "utils.h"

void GameState::process_main() {}
void GameState::render_main() {}
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
	looping = true;
	game_loader = nullptr;
	game_context = GAME_CONTEXT_NORMAL;
	player_id = 0;
}

GameState::~GameState() {
	menu_objects.clear();
	GameManager::get_instance()->delete_game_state();
}

void GameState::process_game_state() {
	mouse.poll_buttons();
	process_main();
}

void GameState::render_game_state() {
	render_main();
}

void GameState::update_state(int game_state, int game_context) {
	GameManager::get_instance()->update_state(game_state, game_context);
}

void GameState::inc_thread() {
	update_thread_progress(game_loader->loaded_items);
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