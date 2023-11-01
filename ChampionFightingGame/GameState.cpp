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
	last_pushed_texture = nullptr;
	game_context = GAME_CONTEXT_NORMAL;
	player_id = 0;
	GameManager::get_instance()->set_game_state(this);
}

GameState::~GameState() {
	menu_objects.clear();
	menu_object_map.clear();
	GameManager::get_instance()->delete_game_state();
}

void GameState::process_game_state() {
	mouse.poll_buttons();
	process_main();
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
	}
}

void GameState::render_game_state() {
	GameManager::get_instance()->render_game_states();
}

void GameState::update_state(int game_state, int game_context) {
	GameManager::get_instance()->update_state(game_state, game_context);
}

void GameState::inc_thread() {
	update_thread_progress(game_loader->loaded_items);
}

void GameState::push_menu_object(std::string name, int hint) {
	menu_object_map[name] = menu_objects.size();
	menu_objects.emplace_back(MenuObject(this, nullptr, name, hint));
	object_stack.push(&menu_objects.back());
	last_push_type_stack.push(true);
}

MenuObject& GameState::get_menu_object(std::string name) {
	if (!menu_object_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Menu Object: " + name);
	}
	return menu_objects[menu_object_map[name]];
}

void GameState::render_menu_object(std::string name) {
	if (!menu_object_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Menu Object: " + name);
		return;
	}
	menu_objects[menu_object_map[name]].render();
}

void GameState::push_menu_child(std::string name, int hint) {
	if (last_push_type_stack.top()) {
		object_stack.top()->add_child(name, hint);
		object_stack.push(&(object_stack.top()->children.back()));
	}
	else {
		activity_group_stack.top()->add_child(name, hint);
		object_stack.push(&(activity_group_stack.top()->children.back()));
	}
	last_push_type_stack.push(true);
}

void GameState::push_menu_activity_group(std::string name, int* active_index, bool only_render_active, int hint) {
	object_stack.top()->add_activity_group(name, active_index, only_render_active, hint);
	activity_group_stack.push(&(object_stack.top()->activity_groups.back()));
	last_push_type_stack.push(false);
}

void GameState::push_menu_texture(std::string name, std::string path) {
	object_stack.top()->add_texture(name, path);
	last_pushed_texture = &object_stack.top()->textures.back();
}

void GameState::push_menu_texture(std::string name, Font& font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs) {
	object_stack.top()->add_texture(name, font, text, rgba, border_rgbs);
	last_pushed_texture = &object_stack.top()->textures.back();
}

void GameState::push_menu_texture(std::string name, const GameTexture& that) {
	object_stack.top()->add_texture(name, that);
	last_pushed_texture = &object_stack.top()->textures.back();
}

void GameState::push_menu_texture(std::string name, unsigned int texture_id) {
	object_stack.top()->add_texture(name, texture_id);
	last_pushed_texture = &object_stack.top()->textures.back();
}

void GameState::push_menu_int_var(std::string name, int val) {
	object_stack.top()->push_int_var(name, val);
}

void GameState::push_menu_float_var(std::string name, float val) {
	object_stack.top()->push_float_var(name, val);
}

void GameState::push_menu_bool_var(std::string name, bool val) {
	object_stack.top()->push_bool_var(name, val);
}

void GameState::push_menu_string_var(std::string name, std::string val) {
	object_stack.top()->push_string_var(name, val);
}

void GameState::push_menu_ptr_var(std::string name, void* val) {
	object_stack.top()->push_ptr_var(name, val);
}

void GameState::push_menu_up_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->up_event_function = function;
}

void GameState::push_menu_down_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->down_event_function = function;
}

void GameState::push_menu_left_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->left_event_function = function;
}

void GameState::push_menu_right_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->right_event_function = function;
}

void GameState::push_menu_select_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->select_event_function = function;
}

void GameState::push_menu_start_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->start_event_function = function;
}

void GameState::push_menu_back_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->back_event_function = function;
}

void GameState::push_menu_any_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->any_event_function = function;
}

void GameState::push_menu_process_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->process_function = function;
}

void GameState::push_menu_pre_render_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->pre_render_function = function;
}

void GameState::push_menu_post_render_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->post_render_function = function;
}

void GameState::push_menu_on_selected_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->on_selected_event_function = function;
}

void GameState::push_menu_on_deselected_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->on_deselected_event_function = function;
}

void GameState::pop_menu_stack() {
	if (last_push_type_stack.top()) {
		object_stack.pop();
	}
	else {
		activity_group_stack.pop();
	}
	last_push_type_stack.pop();
	last_pushed_texture = nullptr;
}

MenuActivityGroup::MenuActivityGroup() {
	this->owner = nullptr;
	this->parent = nullptr;
	this->active_index = nullptr;
	this->prev_active_index = 0;
	this->only_render_active = true;
}

MenuActivityGroup::MenuActivityGroup(GameState* owner, MenuObject* parent, std::string name, int* active_index, bool only_render_active, int hint) {
	this->owner = owner;
	this->parent = parent;
	this->name = name;
	this->active_index = active_index;
	this->prev_active_index = *active_index;
	this->only_render_active = only_render_active;
	if (hint != -1) {
		children.reserve(hint);
	}
}

MenuActivityGroup::MenuActivityGroup(MenuActivityGroup& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->active_index = other.active_index;
	this->prev_active_index = other.prev_active_index;
	this->only_render_active = other.only_render_active;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->child_map = other.child_map;
}

MenuActivityGroup::MenuActivityGroup(const MenuActivityGroup& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->active_index = other.active_index;
	this->prev_active_index = other.prev_active_index;
	this->only_render_active = other.only_render_active;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->child_map = other.child_map;
}

MenuActivityGroup::MenuActivityGroup(MenuActivityGroup&& other) noexcept {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->active_index = other.active_index;
	this->prev_active_index = other.prev_active_index;
	this->only_render_active = other.only_render_active;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->child_map = other.child_map;
}

MenuActivityGroup::MenuActivityGroup(const MenuActivityGroup&& other) noexcept {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->active_index = other.active_index;
	this->prev_active_index = other.prev_active_index;
	this->only_render_active = other.only_render_active;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->child_map = other.child_map;
}

MenuActivityGroup::~MenuActivityGroup() {
	children.clear();
}

void MenuActivityGroup::process() {
	if (*active_index != prev_active_index) {
		children[prev_active_index].event_on_deselected();
		children[prev_active_index].bool_var("Active") = false;
		children[*active_index].event_on_selected();
		children[*active_index].bool_var("Active") = true;
	}
	for (size_t i = 0, max = children.size(); i < max; i++) {
		children[i].event_process();
	}
	prev_active_index = *active_index;
}

void MenuActivityGroup::render() {
	if (!only_render_active) {
		for (size_t i = 0, max = children.size(); i < max; i++) {
			children[i].render();
		}
	}
	else {
		children[*active_index].render();
	}
}

void MenuActivityGroup::add_child(std::string name, int hint) {
	int idx = children.size();
	child_map[name] = idx;
	children.emplace_back(owner, this->parent, name, hint);
	children.back().name = name;
	if (idx == *active_index) {
		children.back().push_bool_var("Active", true);
	}
	else {
		children.back().push_bool_var("Active", false);
	}
}

int MenuActivityGroup::num_children() {
	return children.size();
}

MenuObject& MenuActivityGroup::get_child(std::string name) {
	if (!child_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Child: " + name + " in MenuActivityGroup " + this->name);
	}
	return children[child_map[name]];
}

MenuObject& MenuActivityGroup::get_child(int idx) {
	return children[idx];
}

MenuObject::MenuObject() {
	owner = nullptr;
	parent = nullptr;
	name = "";
	pos = glm::vec3(0.0);
	orientated_pos = glm::vec3(0.0);
	orientation = SCREEN_TEXTURE_ORIENTATION_MIDDLE;
	up_event_function = [](MenuObject* object) {};
	down_event_function = [](MenuObject* object) {};
	left_event_function = [](MenuObject* object) {};
	right_event_function = [](MenuObject* object) {};
	select_event_function = [](MenuObject* object) {};
	start_event_function = [](MenuObject* object) {};
	back_event_function = [](MenuObject* object) {};
	any_event_function = [](MenuObject* object) {};

	process_function = [](MenuObject* object) {};
	pre_render_function = [](MenuObject* object) {};
	post_render_function = [](MenuObject* object) {};
	on_selected_event_function = [](MenuObject* object) {};
	on_deselected_event_function = [](MenuObject* object) {};
}

MenuObject::MenuObject(GameState* owner, MenuObject* parent, std::string name, int hint) {
	this->owner = owner;
	this->parent = parent;
	this->name = name;
	pos = glm::vec3(0.0);
	orientated_pos = glm::vec3(0.0);
	orientation = SCREEN_TEXTURE_ORIENTATION_MIDDLE;
	up_event_function = [](MenuObject* object) {};
	down_event_function = [](MenuObject* object) {};
	left_event_function = [](MenuObject* object) {};
	right_event_function = [](MenuObject* object) {};
	select_event_function = [](MenuObject* object) {};
	start_event_function = [](MenuObject* object) {};
	back_event_function = [](MenuObject* object) {};
	any_event_function = [](MenuObject* object) {};

	process_function = [](MenuObject* object) {};
	pre_render_function = [](MenuObject* object) {};
	post_render_function = [](MenuObject* object) {};
	on_selected_event_function = [](MenuObject* object) {};
	on_deselected_event_function = [](MenuObject* object) {};
	if (hint != -1) {
		children.reserve(hint);
		activity_groups.reserve(hint);
	}
}

MenuObject::MenuObject(MenuObject& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->pos = other.pos;
	this->orientated_pos = other.orientated_pos;
	this->orientation = other.orientation;
	for (size_t i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
		this->textures.back().attach_anchor_pos(&this->orientated_pos);
	}
	this->texture_map = other.texture_map;
	this->children.reserve(other.children.capacity());
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->child_map = other.child_map;
	for (size_t i = 0; i < other.activity_groups.size(); i++) {
		this->activity_groups.push_back(other.activity_groups[i]);
	}
	this->activity_groups.reserve(other.activity_groups.capacity());
	this->activity_group_map = other.activity_group_map;
	this->active_indices = std::move(other.active_indices);
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;

	this->any_event_function = other.any_event_function;
	this->up_event_function = other.up_event_function;
	this->right_event_function = other.right_event_function;
	this->left_event_function = other.left_event_function;
	this->down_event_function = other.down_event_function;
	this->select_event_function = other.select_event_function;
	this->start_event_function = other.start_event_function;
	this->back_event_function = other.back_event_function;
	this->process_function = other.process_function;
	this->pre_render_function = other.pre_render_function;
	this->post_render_function = other.post_render_function;
	this->on_selected_event_function = other.on_selected_event_function;
	this->on_deselected_event_function = other.on_deselected_event_function;
}

MenuObject::MenuObject(const MenuObject& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->pos = other.pos;
	this->orientated_pos = other.orientated_pos;
	this->orientation = other.orientation;
	for (size_t i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
		this->textures.back().attach_anchor_pos(&this->orientated_pos);
	}
	this->texture_map = other.texture_map;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->children.reserve(other.children.capacity());
	this->child_map = other.child_map;
	for (size_t i = 0; i < other.activity_groups.size(); i++) {
		this->activity_groups.push_back(other.activity_groups[i]);
	}
	this->activity_groups.reserve(other.activity_groups.capacity());
	this->activity_group_map = other.activity_group_map;
	this->active_indices = other.active_indices;
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;

	this->any_event_function = other.any_event_function;
	this->up_event_function = other.up_event_function;
	this->right_event_function = other.right_event_function;
	this->left_event_function = other.left_event_function;
	this->down_event_function = other.down_event_function;
	this->select_event_function = other.select_event_function;
	this->start_event_function = other.start_event_function;
	this->back_event_function = other.back_event_function;
	this->process_function = other.process_function;
	this->pre_render_function = other.pre_render_function;
	this->post_render_function = other.post_render_function;
	this->on_selected_event_function = other.on_selected_event_function;
	this->on_deselected_event_function = other.on_deselected_event_function;
}

MenuObject::MenuObject(MenuObject&& other) noexcept {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->pos = other.pos;
	this->orientated_pos = other.orientated_pos;
	this->orientation = other.orientation;
	for (size_t i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
		this->textures.back().attach_anchor_pos(&this->orientated_pos);
	}
	this->texture_map = other.texture_map;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->children.reserve(other.children.capacity());
	this->child_map = other.child_map;
	for (size_t i = 0; i < other.activity_groups.size(); i++) {
		this->activity_groups.push_back(other.activity_groups[i]);
	}
	this->activity_groups.reserve(other.activity_groups.capacity());
	this->activity_group_map = other.activity_group_map;
	this->active_indices = std::move(other.active_indices);
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;

	this->any_event_function = other.any_event_function;
	this->up_event_function = other.up_event_function;
	this->right_event_function = other.right_event_function;
	this->left_event_function = other.left_event_function;
	this->down_event_function = other.down_event_function;
	this->select_event_function = other.select_event_function;
	this->start_event_function = other.start_event_function;
	this->back_event_function = other.back_event_function;
	this->process_function = other.process_function;
	this->pre_render_function = other.pre_render_function;
	this->post_render_function = other.post_render_function;
	this->on_selected_event_function = other.on_selected_event_function;
	this->on_deselected_event_function = other.on_deselected_event_function;
}

MenuObject::MenuObject(const MenuObject&& other) noexcept {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->pos = other.pos;
	this->orientated_pos = other.orientated_pos;
	this->orientation = other.orientation;
	for (size_t i = 0; i < other.textures.size(); i++) {
		this->textures.push_back(other.textures[i]);
		this->textures.back().attach_anchor_pos(&this->orientated_pos);
	}
	this->texture_map = other.texture_map;
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
	}
	this->children.reserve(other.children.capacity());
	this->child_map = other.child_map;
	for (size_t i = 0; i < other.activity_groups.size(); i++) {
		this->activity_groups.push_back(other.activity_groups[i]);
	}
	this->activity_groups.reserve(other.activity_groups.capacity());
	this->activity_group_map = other.activity_group_map;
	this->active_indices = other.active_indices;
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;

	this->any_event_function = other.any_event_function;
	this->up_event_function = other.up_event_function;
	this->right_event_function = other.right_event_function;
	this->left_event_function = other.left_event_function;
	this->down_event_function = other.down_event_function;
	this->select_event_function = other.select_event_function;
	this->start_event_function = other.start_event_function;
	this->back_event_function = other.back_event_function;
	this->process_function = other.process_function;
	this->pre_render_function = other.pre_render_function;
	this->post_render_function = other.post_render_function;
	this->on_selected_event_function = other.on_selected_event_function;
	this->on_deselected_event_function = other.on_deselected_event_function;
}

MenuObject::~MenuObject() {
	for (int i = 0, max = textures.size(); i < max; i++) {
		textures[i].destroy();
	}
	int_vars.clear();
	float_vars.clear();
	bool_vars.clear();
	string_vars.clear();
	ptr_vars.clear();
	children.clear();
}

void MenuObject::render() {
	orientated_pos = pos.get_val();
	switch (orientation) {
		default:
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE): {

		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT): {
			orientated_pos.x -= WINDOW_WIDTH;
			orientated_pos.y -= WINDOW_HEIGHT;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_MIDDLE): {
			orientated_pos.y -= WINDOW_HEIGHT;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.x += WINDOW_WIDTH;
			orientated_pos.y -= WINDOW_HEIGHT;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			orientated_pos.x -= WINDOW_WIDTH;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.x += WINDOW_WIDTH;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_LEFT): {
			orientated_pos.y *= -1.0;
			orientated_pos.x -= WINDOW_WIDTH;
			orientated_pos.y += WINDOW_HEIGHT;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			orientated_pos.y *= -1.0;
			orientated_pos.y += WINDOW_HEIGHT;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.y *= -1.0;
			orientated_pos.x += WINDOW_WIDTH;
			orientated_pos.y += WINDOW_HEIGHT;
		} break;
	}
	if (parent != nullptr) {
		orientated_pos += parent->orientated_pos;
	}
	pre_render_function(this);
	for (int i = 0, max = textures.size(); i < max; i++) {
		textures[i].process(); //Once we transition from GameTexture to ScreenTexture, this line is gone
		textures[i].render();
	}
	
	for (size_t i = 0, i2 = 0, max = children.size(), max2 = activity_group_orders.size(); i < max || i2 < max2; i++) {
		if (i2 < max2 && i == activity_group_orders[i2]) {
			activity_groups[i2].render();
			i2++;
		}
		if (i < max) {
			children[i].render();
		}
	}
	post_render_function(this);
}

void MenuObject::add_child(std::string name, int hint) {
	child_map[name] = children.size();
	children.emplace_back(owner, this, name, hint);
	children.back().name = name;
}

void MenuObject::add_activity_group(std::string name, int* active_index, bool only_render_active, int hint) {
	activity_group_map[name] = activity_groups.size();
	activity_group_orders.emplace_back(children.size());
	if (active_index == nullptr) {
		active_indices.push_back(0);
		active_index = &active_indices.back();
	}
	activity_groups.emplace_back(owner, this, name, active_index, only_render_active, hint);
}

void MenuObject::add_texture(std::string name, std::string path) {
	texture_map[name] = textures.size();
	textures.emplace_back(path);
	textures.back().attach_anchor_pos(&orientated_pos);
}

void MenuObject::add_texture(std::string name, Font& font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs) {
	texture_map[name] = textures.size();
	textures.emplace_back(font, text, rgba, border_rgbs);
	textures.back().attach_anchor_pos(&orientated_pos);
}

void MenuObject::add_texture(std::string name, const GameTexture& that) {
	texture_map[name] = textures.size();
	textures.emplace_back(that);
	textures.back().attach_anchor_pos(&orientated_pos);
}

void MenuObject::add_texture(std::string name, unsigned int texture_id) {
	texture_map[name] = textures.size();
	textures.emplace_back(GameTexture());
	textures.back().init(texture_id);
	textures.back().attach_anchor_pos(&orientated_pos);
}

int MenuObject::num_children() {
	return children.size();
}

int MenuObject::num_textures() {
	return textures.size();
}

MenuObject& MenuObject::get_child(std::string name) {
	if (!child_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Child: " + name + " in MenuObject " + this->name);
	}
	return children[child_map[name]];
}

MenuObject& MenuObject::get_child(int idx) {
	return children[idx];
}

MenuActivityGroup& MenuObject::get_activity_group(std::string name) {
	if (!activity_group_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Activity Group: " + name + " in MenuObject " + this->name);
	}
	return activity_groups[activity_group_map[name]];
}

MenuActivityGroup& MenuObject::get_activity_group(int idx) {
	return activity_groups[idx];
}

GameTexture& MenuObject::get_texture(std::string name) {
	if (!texture_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Texture: " + name + " in MenuObject " + this->name);
	}
	return textures[texture_map[name]];
}

GameTexture& MenuObject::get_texture(int idx) {
	return textures[idx];
}

void MenuObject::set_orientation(int orientation) {
	this->orientation = orientation;
}

glm::vec3 MenuObject::get_pos() {
	return pos.get_val();
}

void MenuObject::add_pos(glm::vec3 pos) {
	this->pos += pos;
}

void MenuObject::set_pos(glm::vec3 pos) {
	this->pos.set_val(pos);
}

void MenuObject::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
}

void MenuObject::push_int_var(std::string name, int val) {
	int_vars[name] = val;
}

void MenuObject::push_float_var(std::string name, float val) {
	float_vars[name] = val;
}

void MenuObject::push_bool_var(std::string name, bool val) {
	bool_vars[name] = val;
}

void MenuObject::push_string_var(std::string name, std::string val) {
	string_vars[name] = val;
}

void MenuObject::push_ptr_var(std::string name, void* val) {
	ptr_vars[name] = val;
}

int& MenuObject::int_var(std::string name) {
	if (!int_vars.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find variable " + name + " in MenuObject: " + this->name);
	}
	return int_vars[name];
}

float& MenuObject::float_var(std::string name) {
	if (!float_vars.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find variable " + name + " in MenuObject: " + this->name);
	}
	return float_vars[name];
}

bool& MenuObject::bool_var(std::string name) {
	if (!bool_vars.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find variable " + name + " in MenuObject: " + this->name);
	}
	return bool_vars[name];
}

std::string& MenuObject::string_var(std::string name) {
	if (!string_vars.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find variable " + name + " in MenuObject: " + this->name);
	}
	return string_vars[name];
}

void* MenuObject::ptr_var(std::string name) {
	if (!ptr_vars.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find variable " + name + " in MenuObject: " + this->name);
	}
	return ptr_vars[name];
}

void MenuObject::event_up_press() {
	up_event_function(this);
}

void MenuObject::event_down_press() {
	down_event_function(this);
}

void MenuObject::event_left_press() {
	left_event_function(this);
}

void MenuObject::event_right_press() {
	right_event_function(this);
}

void MenuObject::event_select_press() {
	select_event_function(this);
}

void MenuObject::event_start_press() {
	start_event_function(this);
}

void MenuObject::event_back_press() {
	back_event_function(this);
}

void MenuObject::event_any_press() {
	any_event_function(this);
}

void MenuObject::event_process() {
	process_function(this);
	for (size_t i = 0, max = children.size(); i < max; i++) {
		children[i].event_process();
	}
	for (size_t i = 0, max = activity_groups.size(); i < max; i++) {
		activity_groups[i].process();
	}
}

void MenuObject::event_on_selected() {
	on_selected_event_function(this);
}

void MenuObject::event_on_deselected() {
	on_deselected_event_function(this);
}