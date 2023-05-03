#include "CharaSelect.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Loader.h"
#include "GLM Helpers.h"
#include "utils.h"

/// <summary>
/// The main function while on the character select screen.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void chara_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];
	const Uint8* keyboard_state;

	CSS *css = new CSS;
	
	while (*css->looping) {
		css->frame_delay();
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < 2; i++) {
			player[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		css->render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	delete css;
}

CSS::CSS() {
	for (int i = 0; i < 2; i++) {
		player_selected_index[i] = 0;
		my_col[i] = 0;
		my_row[i] = 0;
	}
	num_cols = 0;
	num_rows = 0;
	cols_offset = 0;

	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	game_manager->set_menu_info(this);
	menu_objects.resize(CHARA_SELECT_GROUP_MAX);

	if (!load_css()) {
		game_manager->add_crash_log("Could not load CSS!");
		return;
	}

	menu_objects[CHARA_SELECT_GROUP_MISC].emplace_back(this, nullptr, false);
	menu_objects[CHARA_SELECT_GROUP_MISC].emplace_back(this, nullptr, true);

	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_1.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_2.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_3.png");

	for (int i = 0, max = menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures.size(); i < max; i++) {
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures[i].set_height_scale(1.5);
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures[i].set_width_scale(1.5);
	}

	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].add_texture("resource/game_state/chara_select/p1_cursor.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].add_texture("resource/game_state/chara_select/p2_cursor.png");
	for (int i = 0; i < 2; i++) {
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].textures[i].set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	}

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	if (num_rows == 0) {
		my_col[0] = 1;
		my_col[1] = 1;
	}
	for (int i = 0; i < 2; i++) {
		player_id = i;
		if (player[i]->chara_kind != CHARA_KIND_MAX) {
			find_prev_chara_kind(player[i]->chara_kind);
		}
		else {
			mobile_css_slots[i] = chara_slots[i].texture;
		}
	}
}

CSS::~CSS() {
	chara_slots.clear();
	for (int i = 0; i < 2; i++) {
		big_chara_slots[i].texture.destroy();
		mobile_css_slots[i].destroy();
	}
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>0 if successful, -1 if the file fails to open.</returns>
bool CSS::load_css() {
	ParamTable css_params("resource/game_state/chara_select/css_param.prmlst");
	if (css_params.load_failed()) {
		return false;
	}
	size_t list_start_offset = 1; 
	//The first spot on this param list is the number of params, so entry 0 isn't a list. We COULD bake this offset into 
	//get_param_list(int), but I think handling it manually is ok since otherwise we'd need to store a separate vector of pointers
	for (size_t i = list_start_offset, max = css_params.get_param_int("num_slots") + list_start_offset; i < max; i++) {
		if (css_params.get_param_bool("selectable", i)) {
			add_chara_slot(css_params.get_param_table(i));
		}
	}

	int col = 0;
	int row = 0;
	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots[i].texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		chara_slots[i].my_col = col;
		chara_slots[i].my_row = row;
		if (col == 9) {
			col = 0;
			row++;
		}
		else {
			col++;
		}
	}
	num_cols = col;
	num_rows = row;

	cols_offset = NUM_COLS - num_cols;

	for (size_t i = 0, max = chara_slots.size(); i < max; i++) {
		if (chara_slots[i].my_row == num_rows && num_cols != NUM_COLS) {
			chara_slots[i].my_col += (cols_offset / 2);
		}
		chara_slots[i].texture.set_pos(glm::vec3(
			chara_slots[i].my_col * (WINDOW_WIDTH / 5) + (chara_slots[i].texture.get_width() / 2),
			chara_slots[i].my_row * chara_slots[i].texture.get_height() * 2.4 + 200,
			0
		));
	}

	for (size_t i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots_ordered[chara_slots[i].my_col][chara_slots[i].my_row] = &chara_slots[i];
	}

	for (int i = 0; i < 2; i++) {
		player_id = i;
		select_slot();
	}

	return true;
}

/// <summary>
/// Adds an entry in the CSS slots array. 
/// </summary>
/// <param name="id">: The chara_kind the slot will correspond to.</param>
/// <param name="resource_dir">: The directory for the slot's portrait.</param>
/// <param name="chara_name">: The actual character name to display on the UI.</param>
void CSS::add_chara_slot(ParamTable param_table) {
	int id = param_table.get_param_int("chara_kind");
	std::string resource_dir = param_table.get_param_string("resource_name");
	std::string chara_name = param_table.get_param_string("chara_name");
	chara_slots.push_back(CssSlot(id, resource_dir, chara_name));
}

void CSS::event_select_press() {
	if (player[player_id]->chara_kind == CHARA_KIND_MAX) {
		player[player_id]->chara_kind = chara_slots[player_selected_index[player_id]].get_chara_kind();
		mobile_css_slots[player_id] = GameTexture(chara_slots[player_selected_index[player_id]].texture);

		if (player_id) {
			mobile_css_slots[player_id].set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE);
		}
		else {
			mobile_css_slots[player_id].set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		}
		mobile_css_slots[player_id].set_pos(chara_slots[player_selected_index[player_id]].texture.get_pos_vacuum(&mobile_css_slots[player_id]));
		mobile_css_slots[player_id].set_target_pos(big_chara_slots[player_id].texture.pos, 16.0);
		mobile_slots_active[player_id] = true;
	}
}
void CSS::event_back_press() {
	if (player[player_id]->chara_kind != CHARA_KIND_MAX) {
		mobile_slots_active[player_id] = false;
		player[player_id]->chara_kind = CHARA_KIND_MAX;
	}
	else {
		*game_state = GAME_STATE_STAGE_SELECT;
		*looping = false;
	}
}

void CSS::event_start_press() {
	if (player[0]->chara_kind != CHARA_KIND_MAX && player[1]->chara_kind != CHARA_KIND_MAX) {
		*game_state = GAME_STATE_BATTLE;
		*looping = false;
	}
}

void CSS::event_right_press() {
	if (player[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 9 && chara_slots_ordered[my_col[player_id] + 1][my_row[player_id]] != nullptr) {
			my_col[player_id]++;
		}
		is_last_input_right[player_id] = true;
		select_slot();
	}
}

void CSS::event_left_press() {
	if (player[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 0 && chara_slots_ordered[my_col[player_id] - 1][my_row[player_id]] != nullptr) {
			my_col[player_id]--;
		}
		is_last_input_right[player_id] = false;
		select_slot();
	}
}

void CSS::event_down_press() {
	bool jump = false;
	if (player[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_row[player_id] < num_rows) {
			if (chara_slots_ordered[my_col[player_id]][my_row[player_id] + 1] == nullptr) {
				jump = true;
				bool valid_col = false;
				if (my_col[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (chara_slots_ordered[i][my_row[player_id] + 1] != nullptr) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (chara_slots_ordered[i][my_row[player_id] + 1] != nullptr) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				if (!valid_col) {
					GameManager::get_instance()->add_crash_log("Couldn't find a valid column!");
					*looping = false;
					*game_state = GAME_STATE_DEBUG_MENU;
					return;
				}
			}
			my_row[player_id]++;
			bool min = my_col[player_id] <= cols_offset / 2;

			if (num_cols % 2) {
				if (!is_last_input_right[player_id] && my_row[player_id] == num_rows && !min) {
					my_col[player_id]--;
				}
				if (!jump) {
					is_last_input_right[player_id] = !is_last_input_right[player_id];
				}
			}
		}

		select_slot();
	}
}

void CSS::event_up_press() {
	bool jump = false;
	if (player[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_row[player_id] != 0) {
			if (chara_slots_ordered[my_col[player_id]][my_row[player_id] - 1] == nullptr) {
				jump = true;
				bool valid_col = false;
				if (my_col[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (chara_slots_ordered[i][my_row[player_id] - 1] != nullptr) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (chara_slots_ordered[i][my_row[player_id] - 1] != nullptr) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				if (!valid_col) {
					GameManager::get_instance()->add_crash_log("Couldn't find a valid column!");
					*looping = false;
					*game_state = GAME_STATE_DEBUG_MENU;
					return;
				}
			}
			my_row[player_id]--;
			bool max = my_col[player_id] >= NUM_COLS - 1;

			if (num_cols % 2) {
				if (is_last_input_right[player_id] && my_row[player_id] == num_rows - 1 && !max) {
					my_col[player_id]++;
				}
				if (!jump) {
					is_last_input_right[player_id] = !is_last_input_right[player_id];
				}
			}
		}

		select_slot();
	}
}

/// <summary>
/// Select the CSS slot that the cursor is hovering over. Works by comparing the column and row of each slot to the column and row of the player, so 
/// make sure to set that up properly if this function is copied into another menu.
/// </summary>
void CSS::select_slot() {
	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		if (my_col[player_id] == chara_slots[i].my_col
		&& my_row[player_id] == chara_slots[i].my_row) {
			player_selected_index[player_id] = i;
			break;
		}
	}
	big_chara_slots[player_id].texture = GameTexture(chara_slots[player_selected_index[player_id]].texture);

	big_chara_slots[player_id].chara_name = chara_slots[player_selected_index[player_id]].chara_name;
	big_chara_slots[player_id].resource_dir = chara_slots[player_selected_index[player_id]].resource_dir;


	if (player_id) {
		big_chara_slots[player_id].texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE);
	}
	else {
		big_chara_slots[player_id].texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	big_chara_slots[player_id].texture.set_pos(glm::vec3(
		40 + (player_id * (WINDOW_WIDTH - 270)),
		70, 
		0));
	big_chara_slots[player_id].texture.set_width(190);
	big_chara_slots[player_id].texture.set_height(280);
	big_chara_slots[player_id].texture.set_alpha((Uint8)127);
}

/// <summary>
/// Render all CSS-related textures.
/// </summary>
void CSS::render() {
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].render();
	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots[i].render();
	}

	for (int i = 0; i < 2; i++) {
		big_chara_slots[i].texture.render();
		if (big_chara_slots[i].resource_dir == "default") { //Todo: OpenGL text rendering
//			draw_text_multi_lines("FiraCode-Regular.ttf", tmpSlot.name, tmpSlot.texture.destRect.x, tmpSlot.texture.destRect.y + 70, 24, 255, 255, 255, 255);
		}

		if (mobile_slots_active[i]) {
			if (mobile_css_slots[i].pos != big_chara_slots[i].texture.pos) {
				mobile_css_slots[i].add_rot(glm::vec3(0.0, 360.0 / 16.0, 0.0));
			}
			else {
				mobile_css_slots[i].set_rot(glm::vec3(0.0, 0.0, 0.0));
				mobile_css_slots[i].set_width(190);
				mobile_css_slots[i].set_height(280);
			}
			mobile_css_slots[i].process();
			mobile_css_slots[i].render();
		}
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].textures[i].set_target_pos(
			glm::vec3(chara_slots[player_selected_index[i]].texture.pos.x, chara_slots[player_selected_index[i]].texture.pos.y, 0.0f),
			8.0f
		);
	}
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].render();
}

/// <summary>
/// Returns the character bound to the CSS slot which a given player is selecting.
/// </summary>
/// <param name="player">: The player to get the character for.</param>
/// <returns></returns>
int CSS::get_chara_kind(int player) {
	return chara_slots[player_selected_index[player]].get_chara_kind();
}

/// <summary>
/// Takes the character that the current player is already selecting, and automatically puts their cursor on the first CSS slot corresponding to that
/// character. Note: This function is used for when you re-enter the CSS after having already selected a character, such as after a match.
/// </summary>
/// <param name="chara_kind">: The character whose CSS slot to select.</param>
void CSS::find_prev_chara_kind(int chara_kind) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].chara_kind == chara_kind) {
			player[player_id]->chara_kind = CHARA_KIND_MAX;
			my_col[player_id] = chara_slots[i].my_col;
			my_row[player_id] = chara_slots[i].my_row;
			select_slot();
			event_select_press();
			return;
		}
	}
}

CssSlot::CssSlot() {
	chara_kind = CHARA_KIND_MAX;
	my_col = -1;
	my_row = -1;
	resource_dir = "";
	chara_name = "";
}

CssSlot::CssSlot(int id, std::string resource_name, std::string chara_name) {
	init(id, resource_name, chara_name);
}

/// <summary>
/// Get the character for a CSS slot. For the record, chara_id is public so this function is kinda irrelevant.
/// </summary>
/// <returns>: The character for a CSS slot.</returns>
int CssSlot::get_chara_kind() {
	return chara_kind;
}

/// <summary>
/// Initialize a CSS slot with the given values.
/// </summary>
/// <param name="chara_kind">: The chara_kind this slot will correspond to.</param>
/// <param name="resource_dir">: The directory where the CSS slot's render is located.</param>
/// <param name="name">: The name of the character for UI purposes.</param>
void CssSlot::init(int chara_kind, std::string resource_name, std::string name) {
	texture.init("resource/game_state/chara_select/chara/" + resource_name + "/render.png");
	texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	this->chara_name = name;
	this->resource_dir = resource_name;
	this->chara_kind = chara_kind;
}

/// <summary>
/// Set the Y position of the CSS Slot.
/// </summary>
/// <param name="y">: The intended Y position.</param>
void CssSlot::set_y_pos(int y) {
	texture.set_pos(glm::vec3(texture.pos.x, y, 0));
}

/// <summary>
/// Set the X position of the CSS Slot.
/// </summary>
/// <param name="X">: The intended X position.</param>
void CssSlot::set_x_pos(int x) {
	texture.set_pos(glm::vec3(x, texture.pos.y, 0));
}

/// <summary>
/// Get the width of this CSS Slot's texture.
/// </summary>
/// <returns>The width of this CSS Slot's texture.</returns>
int CssSlot::get_texture_width() {
	return texture.get_width();
}

/// <summary>
/// Render the CSS Slot's texture.
/// </summary>
void CssSlot::render() {
	texture.process();
	texture.render();
}

/// <summary>
/// Check if the CSS Slot is below a given coordinate.
/// </summary>
/// <param name="y">: The coord to compare against.</param>
/// <returns>Whether or not the CSS Slot is below a given coordinate.</returns>
bool CssSlot::is_below(int y) {
	if (texture.pos.y > y) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// Check if the CSS Slot is above a given coordinate.
/// </summary>
/// <param name="y">: The coord to compare against.</param>
/// <returns>Whether or not the CSS Slot is above a given coordinate.</returns>
bool CssSlot::is_above(int y) {
	if (texture.pos.y < y) {
		return true;
	}
	else {
		return false;
	}
}