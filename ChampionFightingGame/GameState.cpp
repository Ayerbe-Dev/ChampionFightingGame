#include "GameState.h"
#include "GameTexture.h"
#include "FontManager.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "WindowManager.h"
#include "WindowConstants.h"

void GameState::pre_event_process_main() {}
void GameState::process_main() {}
void GameState::render_main() {}
void GameState::event_up_press(){}
void GameState::event_down_press(){}
void GameState::event_left_press(){}
void GameState::event_right_press(){}
void GameState::event_start_press(){}
void GameState::event_select_press(){}
void GameState::event_back_press(){}
void GameState::event_page_left_press(){}
void GameState::event_page_right_press(){}
void GameState::event_frame_pause_press(){}
void GameState::event_frame_advance_press(){}
void GameState::event_record_input_press(){}
void GameState::event_replay_input_press(){}
void GameState::event_switch_input_press(){}
void GameState::event_any_press() {}
void GameState::process_background() {}

GameState::GameState() {
	looping = true;
	last_pushed_texture = nullptr;
	game_context = GAME_CONTEXT_NORMAL;
	sub_state = GAME_STATE_NONE;
	internal_frame = 0;
	internal_state = 0;
	prev_internal_state = 0;
	prev_executed_frame = 0;
	player_id = 0;
	menu_objects.reserve(2);
	game_manager = GameManager::get_instance();
	for (int i = 0; i < 2; i++) {
		player[i] = game_manager->player[i];
	}
	game_manager->set_game_state(this);
}

GameState::~GameState() {
	menu_objects.clear();
	menu_object_map.clear();
	for (auto &f : font_map) {
		f.second.unload_font();
	}
	game_manager->delete_game_state();
}

void GameState::process_game_state() {
	if (prev_internal_state != internal_state) {
		internal_frame = 0;
		conditions.clear();
	}
	prev_internal_state = internal_state;
	mouse.poll_buttons();
	for (int i = 0; i < 2; i++) {
		player[i]->poll_controller_menu();
	}
	pre_event_process_main();
	game_manager->process_game_state_events();
	process_main();
	if (game_manager->get_game_state() == this) {
		for (std::list<UIMessage>::iterator it = messages_active.begin();
			it != messages_active.end(); it++) {
			if (!it->check_active()) {
				messages_fading.push_back(std::move(*it)); 
				messages_active.erase(it);
			
			}
		}
		for (std::list<UIMessage>::iterator it = messages_fading.begin();
			it != messages_fading.end(); it++) {
			if (!it->texture.alpha) {
				messages_fading.erase(it);				
			}
		}
	}
	ObjectManager::get_instance()->process();
	SoundManager::get_instance()->process_sounds();
	internal_frame++;
}

void GameState::render_game_state() {
	game_manager->render_game_states();
}

void GameState::update_state(int game_state, int game_context) {
	game_manager->update_state(game_state, game_context);
}

void GameState::push_menu_object(std::string name, int texture_hint, int child_hint, int activity_hint) {
	menu_object_map[name] = menu_objects.size();
	menu_objects.emplace_back(this, nullptr, name, texture_hint, child_hint, activity_hint);
	object_stack.push(&menu_objects.back());
	last_push_type_stack.push(true);
}

MenuObject& GameState::get_menu_object(std::string name) {
	if (!menu_object_map.contains(name)) {
		game_manager->add_crash_log("Failed to find Menu Object: " + name);
	}
	return menu_objects[menu_object_map[name]];
}

void GameState::render_menu_object(std::string name) {
	if (!menu_object_map.contains(name)) {
		game_manager->add_crash_log("Failed to find Menu Object: " + name);
		return;
	}
	menu_objects[menu_object_map[name]].render();
}

MenuObject& GameState::get_child(std::string name) {
	return main_object.get_child(name);
}

MenuObject& GameState::get_child(int idx) {
	return main_object.get_child(idx);
}

MenuActivityGroup& GameState::get_activity_group(std::string name) {
	return main_object.get_activity_group(name);
}

MenuActivityGroup& GameState::get_activity_group(int idx) {
	return main_object.get_activity_group(idx);
}

void GameState::set_hints(int texture_hint, int child_hint, int activity_hint) {
	main_object.set_hints(texture_hint, child_hint, activity_hint);
	while (!object_stack.empty()) {
		object_stack.pop();
		last_push_type_stack.pop();
	}
	object_stack.push(&main_object);
	last_push_type_stack.push(true);
}

void GameState::push_menu_pos(glm::vec3 pos) {
	object_stack.top()->set_pos(pos);
}

void GameState::push_menu_orientation(int orientation) {
	object_stack.top()->set_orientation(orientation);
}

void GameState::push_menu_dimensions(int width, int height) {
	object_stack.top()->set_dimensions(width, height);
}

void GameState::push_menu_dimensions(int texture_id) {
	object_stack.top()->set_dimensions(texture_id);
}

void GameState::push_menu_dimensions(std::string texture) {
	object_stack.top()->set_dimensions(texture);
}

void GameState::push_menu_child(std::string name, int texture_hint, int child_hint, int activity_hint) {
	if (last_push_type_stack.top()) {
		object_stack.top()->add_child(name, texture_hint, child_hint, activity_hint);
		object_stack.push(&(object_stack.top()->children.back()));
	}
	else {
		activity_group_stack.top()->add_child(name, texture_hint, child_hint, activity_hint);
		object_stack.push(&(activity_group_stack.top()->children.back()));
	}
	last_push_type_stack.push(true);
}

void GameState::push_menu_activity_group(std::string name, int* active_index, bool ignore_inactive, int hint) {
	object_stack.top()->add_activity_group(name, active_index, ignore_inactive, hint);
	activity_group_stack.push(&(object_stack.top()->activity_groups.back()));
	last_push_type_stack.push(false);
}

void GameState::push_menu_texture(std::string name) {
	object_stack.top()->add_texture(name);
	last_pushed_texture = &object_stack.top()->textures.back();
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

void GameState::push_menu_page_left_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->page_left_event_function = function;
}

void GameState::push_menu_page_right_event_function(std::function<void(MenuObject* menu_object)> function) {
	object_stack.top()->page_right_event_function = function;
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

void GameState::load_font(std::string name, std::string font_name, int font_size) {
	if (font_map.contains(name)) {
		game_manager->add_crash_log("Font " + name + " already loaded");
		return;
	}
	font_map[name] = FontManager::get_instance()->load_font(font_name, font_size);
}

Font& GameState::get_font(std::string name) {
	if (!font_map.contains(name)) {
		game_manager->add_crash_log("Font " + name + " not loaded");
	}
	return font_map[name];
}

void GameState::add_message(std::string font_name, std::string text, int active_duration, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	messages_active.push_back(UIMessage());
	messages_active.back().init(&get_font(font_name), text, active_duration, fade_frames, pos, rgba, border_rgbs);
}

void GameState::add_message(std::string font_name, std::string text, bool* active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	messages_active.push_back(UIMessage());
	messages_active.back().init(&get_font(font_name), text, active_condition, fade_frames, pos, rgba, border_rgbs);
}

void GameState::add_message(std::string font_name, std::string text, VBP active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	messages_active.push_back(UIMessage());
	messages_active.back().init(&get_font(font_name), text, active_condition, fade_frames, pos, rgba, border_rgbs);
}

bool GameState::execute_if(std::string name, int num_allowed_executions, bool condition) {
	bool ret = false;
	if (!conditions.contains(name)) {
		conditions[name].num_allowed_executions = num_allowed_executions;
		conditions[name].executed_frame = -1;
	}
	if (conditions[name].num_allowed_executions && condition) {
		if (conditions[name].num_allowed_executions != -1) conditions[name].num_allowed_executions--;
		conditions[name].executed_frame = internal_frame;
		ret = true;
	}
	if (conditions[name].executed_frame != -1) {
		prev_executed_frame = conditions[name].executed_frame;
	}
	else {
		prev_executed_frame = internal_frame;
	}
	return ret;
}

bool GameState::execute_wait(std::string name, int num_allowed_executions, unsigned int frames) {
	bool ret = false;
	if (!conditions.contains(name)) {
		conditions[name].num_allowed_executions = num_allowed_executions;
		conditions[name].executed_frame = -1;
	}
	if (conditions[name].num_allowed_executions && (prev_executed_frame + frames <= internal_frame)) {
		if (conditions[name].num_allowed_executions != -1) conditions[name].num_allowed_executions--;
		conditions[name].executed_frame = internal_frame;
		ret = true;
	}
	if (conditions[name].executed_frame != -1) {
		prev_executed_frame = conditions[name].executed_frame;
	}
	else {
		prev_executed_frame = internal_frame;
	}
	return ret;
}

MenuActivityGroup::MenuActivityGroup() {
	this->owner = nullptr;
	this->parent = nullptr;
	this->active_index = nullptr;
	this->prev_active_index = 0;
	this->ignore_inactive = true;
}

MenuActivityGroup::MenuActivityGroup(GameState* owner, MenuObject* parent, std::string name, int* active_index, bool ignore_inactive, int hint) {
	this->owner = owner;
	this->parent = parent;
	this->name = name;
	this->active_index = active_index;
	this->prev_active_index = *active_index;
	this->ignore_inactive = ignore_inactive;
	children.reserve(hint);
}

MenuActivityGroup::MenuActivityGroup(MenuActivityGroup& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->active_index = other.active_index;
	this->prev_active_index = other.prev_active_index;
	this->ignore_inactive = other.ignore_inactive;
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
	this->ignore_inactive = other.ignore_inactive;
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
	this->ignore_inactive = other.ignore_inactive;
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
	this->ignore_inactive = other.ignore_inactive;
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
		if (prev_active_index >= 0 && prev_active_index < children.size()) {
			children[prev_active_index].event_on_deselected();
			children[prev_active_index].bool_var("Active") = false;
		}
		if (*active_index >= 0 && *active_index < children.size()) {
			children[*active_index].event_on_selected();
			children[*active_index].bool_var("Active") = true;
		}
	}
	prev_active_index = *active_index;
	if (!ignore_inactive) {
		for (size_t i = 0, max = children.size(); i < max; i++) {
			children[i].event_process();
		}
	}
	else if (*active_index >= 0 && *active_index < children.size()) {
		children[*active_index].event_process();
	}
}

void MenuActivityGroup::render() {
	if (!ignore_inactive) {
		for (size_t i = 0, max = children.size(); i < max; i++) {
			children[i].render();
		}
	}
	else if (*active_index >= 0 && *active_index < children.size()){
		children[*active_index].render();
	}
}

void MenuActivityGroup::add_child(std::string name, int texture_hint, int child_hint, int activity_hint) {
	int idx = children.size();
	child_map[name] = idx;
	children.emplace_back(owner, this->parent, name, texture_hint, child_hint, activity_hint);
	children.back().name = name;
	if (idx == *active_index) {
		children.back().push_bool_var("Active", true);
	}
	else {
		children.back().push_bool_var("Active", false);
	}
	children.back().push_ptr_var("Activity Group", this);
	children.back().push_ptr_var("Active Index", active_index);
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

MenuObject& MenuActivityGroup::get_active_child() {
	return children[*active_index];
}

int MenuActivityGroup::get_active_child_index() {
	return *active_index;
}

void MenuActivityGroup::set_active_child(std::string name) {
	if (!child_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Failed to find Child: " + name + " in MenuActivityGroup " + this->name);
	}
	*active_index = child_map[name];
}

void MenuActivityGroup::set_active_child(int idx) {
	*active_index = idx;
}

void MenuActivityGroup::inc_active_child(int idx) {
	if (*active_index + idx < 0) {
		idx += children.size();
	}
	*active_index = (*active_index + idx) % children.size();
}

MenuObject::MenuObject() {
	owner = nullptr;
	parent = nullptr;
	name = "";
	pos = glm::vec3(0.0);
	orientated_pos = glm::vec3(0.0);
	orientation = SCREEN_TEXTURE_ORIENTATION_MIDDLE;
	width = 0;
	height = 0;
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

MenuObject::MenuObject(GameState* owner, MenuObject* parent, std::string name, int texture_hint, int child_hint, int activity_hint) {
	this->owner = owner;
	this->parent = parent;
	this->name = name;
	pos = glm::vec3(0.0);
	orientated_pos = glm::vec3(0.0);
	orientation = SCREEN_TEXTURE_ORIENTATION_MIDDLE;
	width = 0;
	height = 0;
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
	textures.reserve(texture_hint);
	children.reserve(child_hint);
	activity_groups.reserve(activity_hint);
}

MenuObject::MenuObject(MenuObject& other) {
	this->owner = other.owner;
	this->parent = other.parent;
	this->name = other.name;
	this->pos = other.pos;
	this->orientated_pos = other.orientated_pos;
	this->orientation = other.orientation;
	this->width = other.width;
	this->height = other.height;
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
	this->width = other.width;
	this->height = other.height;
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
	this->width = other.width;
	this->height = other.height;
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
	this->width = other.width;
	this->height = other.height;
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
	sound_player.stop_sound_all();
	sound_player.stop_reserved_sound();
	sound_player.unload_all_sounds();
	int_vars.clear();
	float_vars.clear();
	bool_vars.clear();
	string_vars.clear();
	ptr_vars.clear();
	children.clear();
}

void MenuObject::set_hints(int texture_hint, int child_hint, int activity_hint) {
	textures.reserve(texture_hint);
	children.reserve(child_hint);
	activity_groups.reserve(activity_hint);
}

void MenuObject::render() {
	orientated_pos = pos.get_val();
	switch (orientation) {
		default:
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE): {

		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT): {
			orientated_pos.x -= WINDOW_WIDTH - width;
			orientated_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_MIDDLE): {
			orientated_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.x += WINDOW_WIDTH - width;
			orientated_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			orientated_pos.x -= WINDOW_WIDTH - width;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.x += WINDOW_WIDTH - width;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_LEFT): {
			orientated_pos.y *= -1.0;
			orientated_pos.x -= WINDOW_WIDTH - width;
			orientated_pos.y += WINDOW_HEIGHT - height;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			orientated_pos.y *= -1.0;
			orientated_pos.y += WINDOW_HEIGHT - height;
		} break;
		case (SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT): {
			orientated_pos.x *= -1.0;
			orientated_pos.y *= -1.0;
			orientated_pos.x += WINDOW_WIDTH - width;
			orientated_pos.y += WINDOW_HEIGHT - height;
		} break;
	}
	if (parent != nullptr) {
		orientated_pos += parent->orientated_pos;
	}
	pre_render_function(this);
	for (size_t i = 0, max = render_vec.size(); i < max; i++) {
		switch (render_vec[i].second) {
			case (RENDER_TYPE_TEXTURE): {
				GameTexture* texture = (GameTexture*)render_vec[i].first;
				texture->process();
				texture->render();
			} break;
			case (RENDER_TYPE_CHILD): {
				MenuObject* child = (MenuObject*)render_vec[i].first;
				child->render();
			} break;
			case (RENDER_TYPE_ACTIVITY): {
				MenuActivityGroup* activity_group = (MenuActivityGroup*)render_vec[i].first;
				activity_group->render();
			} break;
		}
	}
	post_render_function(this);
}

void MenuObject::add_child(std::string name, int texture_hint, int child_hint, int activity_hint) {
	child_map[name] = children.size();
	children.emplace_back(owner, this, name, texture_hint, child_hint, activity_hint);
	children.back().name = name;
	render_vec.push_back(std::pair(&children.back(), RENDER_TYPE_CHILD));
}

void MenuObject::add_activity_group(std::string name, int* active_index, bool ignore_inactive, int hint) {
	activity_group_map[name] = activity_groups.size();
	if (active_index == nullptr) {
		active_indices.push_back(0);
		active_index = &active_indices.back();
	}
	activity_groups.emplace_back(owner, this, name, active_index, ignore_inactive, hint);
	render_vec.push_back(std::pair(&activity_groups.back(), RENDER_TYPE_ACTIVITY));
}

void MenuObject::add_texture(std::string name) {
	texture_map[name] = textures.size();
	textures.emplace_back();
	textures.back().attach_anchor_pos(&orientated_pos);
	render_vec.push_back(std::pair(&textures.back(), RENDER_TYPE_TEXTURE));
}

void MenuObject::add_texture(std::string name, std::string path) {
	texture_map[name] = textures.size();
	textures.emplace_back(path);
	textures.back().attach_anchor_pos(&orientated_pos);
	render_vec.push_back(std::pair(&textures.back(), RENDER_TYPE_TEXTURE));
}

void MenuObject::add_texture(std::string name, Font& font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs) {
	texture_map[name] = textures.size();
	textures.emplace_back(font, text, rgba, border_rgbs);
	textures.back().attach_anchor_pos(&orientated_pos);
	render_vec.push_back(std::pair(&textures.back(), RENDER_TYPE_TEXTURE));
}

void MenuObject::add_texture(std::string name, const GameTexture& that) {
	texture_map[name] = textures.size();
	textures.emplace_back(that);
	textures.back().attach_anchor_pos(&orientated_pos);
	render_vec.push_back(std::pair(&textures.back(), RENDER_TYPE_TEXTURE));
}

void MenuObject::add_texture(std::string name, unsigned int texture_id) {
	texture_map[name] = textures.size();
	textures.emplace_back(GameTexture());
	textures.back().init(texture_id);
	textures.back().attach_anchor_pos(&orientated_pos);
	render_vec.push_back(std::pair(&textures.back(), RENDER_TYPE_TEXTURE));
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

MenuObject& MenuObject::get_active_child(std::string activity_group) {
	return get_activity_group(activity_group).get_active_child();
}

int MenuObject::get_active_child_index(std::string activity_group) {
	return get_activity_group(activity_group).get_active_child_index();
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

std::string MenuObject::get_name() {
	return name;
}

MenuObject& MenuObject::get_parent() {
	return *parent;
}

bool MenuObject::has_child(std::string name) {
	return child_map.contains(name);
}

bool MenuObject::has_texture(std::string name) {
	return texture_map.contains(name);
}

void MenuObject::set_active_sibling(std::string name) {
	if (ptr_var("Activity Group") == nullptr) return;
	((MenuActivityGroup*)ptr_var("Activity Group"))->set_active_child(name);
}

void MenuObject::set_active_sibling(int idx) {
	if (ptr_var("Activity Group") == nullptr) return;
	((MenuActivityGroup*)ptr_var("Activity Group"))->set_active_child(idx);
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

void MenuObject::set_dimensions(int width, int height) {
	this->width = width;
	this->height = height;
}

void MenuObject::set_dimensions(int texture_id) {
	this->width = textures[texture_id].get_width();
	this->height = textures[texture_id].get_height();
}

void MenuObject::set_dimensions(std::string texture) {
	this->width = get_texture(texture).get_width();
	this->height = get_texture(texture).get_height();
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

void MenuObject::event_page_left_press() {
	page_left_event_function(this);
}

void MenuObject::event_page_right_press() {
	page_right_event_function(this);
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