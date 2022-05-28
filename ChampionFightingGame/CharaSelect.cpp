#include "CharaSelect.h"
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <cmath>
#include "Debugger.h"
#include "GameTexture.h"
#include "Loader.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

/// <summary>
/// The main function while on the character select screen.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void chara_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;

	GameLoader* game_loader = new GameLoader(3);
	std::thread loading_thread(LoadingScreen, (void*)game_loader);
	loading_thread.detach();

	CSS *css = new CSS;

	css->player_info[0] = player_info[0];
	css->player_info[1] = player_info[1];
	if (css->load_css()) {
		player_info[0]->crash_reason = "Could not open CSS file!";
		return game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}
	update_thread_progress(game_loader->loaded_items);
	if (css->num_rows == 0) {
		css->my_col[0] = 1;
		css->my_col[1] = 1;
	}
	for (int i = 0; i < 2; i++) {
		css->player_id = i;
		if (css->player_info[i]->chara_kind != CHARA_KIND_MAX) {
			css->find_prev_chara_kind(css->player_info[i]->chara_kind);
		}
		else {
			css->mobile_css_slots[i] = css->chara_slots[i].texture;
		}
	}

	css->cursors[0].init("resource/ui/menu/css/p1Cursor.png");
	css->cursors[0].texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	update_thread_progress(game_loader->loaded_items);

	css->cursors[1].init("resource/ui/menu/css/p2Cursor.png");
	css->cursors[1].texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	update_thread_progress(game_loader->loaded_items);

	game_manager->set_menu_info(css);
	
	game_loader->finished = true;

	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);

	while (game_manager->looping[game_manager->layer]) {
		wait_ms();
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					delete css;
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							int width;
							int height;
							SDL_GetWindowSize(g_window, &width, &height);
							glViewport(0, 0, width, height);
						} break;
					}
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		css->render();

		SDL_GL_SwapWindow(g_window);
	}

	delete game_loader;
	delete css;
}

/// <summary>
/// The constructor for the CSS class.
/// </summary>
CSS::CSS() {
	//initialize other textures
	background_texture.init("resource/ui/menu/css/CSSbackground.png");
	background_texture.set_width(WINDOW_WIDTH);
	background_texture.set_height(WINDOW_HEIGHT);
	big_bar_texture.init("resource/ui/menu/css/CSSbottombar.png");
	big_bar_texture.set_width(WINDOW_WIDTH);
	big_bar_texture.set_height(WINDOW_HEIGHT);
	top_bar_texture.init("resource/ui/menu/css/CSStopbar.png");
	top_bar_texture.set_width(WINDOW_WIDTH);
	top_bar_texture.set_height(WINDOW_HEIGHT);
}

CSS::~CSS() {
	for (int i = 0; i < num_slots; i++) {
		chara_slots[i].texture.destroy();
	}
	for (int i = 0; i < 2; i++) {
		big_chara_slots[i].texture.destroy();
		mobile_css_slots[i].destroy();
		cursors[i].texture.destroy();
	}
	background_texture.destroy();
	big_bar_texture.destroy();
	top_bar_texture.destroy();
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>0 if successful, -1 if the file fails to open.</returns>
int CSS::load_css() {
	std::ifstream css_file;
	css_file.open("resource/ui/menu/css/css_param.yml");
	int chara_kind;
	std::string resource_dir;
	bool selectable;

	if (css_file.fail()) {
		css_file.close();
		return -1;
	}

	std::string chara_name;
	for (int i = 0; getline(css_file, chara_name); i++) {
		css_file >> chara_kind >> resource_dir >> selectable;
		if (selectable) {
			add_slot(chara_kind, resource_dir, chara_name);
		}
		getline(css_file, chara_name);
	}
	css_file.close();

	int col = 0;
	int row = 0;
	for (int i = 0; i < num_slots; i++) {
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

	for (int i = 0; i < num_slots; i++) {
		if (chara_slots[i].my_row == num_rows && num_cols != NUM_COLS) {
			chara_slots[i].my_col += (cols_offset / 2);
		}
		chara_slots[i].texture.set_pos(glm::vec3(
			chara_slots[i].my_col * (WINDOW_WIDTH / 5) + (chara_slots[i].texture.get_width() / 2),
			chara_slots[i].my_row * chara_slots[i].texture.get_height() * 2.4 + 200,
			0
		));
	}

	for (int i = 0; i < num_slots; i++) {
		chara_slots_ordered[chara_slots[i].my_col][chara_slots[i].my_row] = &chara_slots[i];
	}

	for (int i = 0; i < 2; i++) {
		player_id = i;
		select_slot();
	}

	return 0;
}

/// <summary>
/// Adds an entry in the CSS slots array. 
/// </summary>
/// <param name="id">: The chara_kind the slot will correspond to.</param>
/// <param name="cardDir">: The directory for the slot's portrait.</param>
/// <param name="cardName">: The actual character name to display on the UI.</param>
void CSS::add_slot(int id, std::string cardDir, std::string cardName) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!chara_slots[i].is_initialized()) {
			chara_slots[i].init(id, cardDir, cardName);
			num_slots++;
			return;
		}
	}
}

/// <summary>
/// Counts each initialized slot, then returns the final product. Fez didn't keep count of the slots length while actually adding to it, don't ask me 
/// why.
/// </summary>
/// <returns>The number of active CSS slots.</returns>
int CSS::get_num_slots() {
	return num_slots;
}

void CSS::event_select_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		player_info[player_id]->chara_kind = chara_slots[player_selected_index[player_id]].get_chara_kind();
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
	if (player_info[player_id]->chara_kind != CHARA_KIND_MAX) {
		mobile_slots_active[player_id] = false;
		mobile_css_slots[player_id].destroy(false);
		player_info[player_id]->chara_kind = CHARA_KIND_MAX;
	}
	else {
		*game_state = GAME_STATE_MENU;
		*looping = false;
	}
}

void CSS::event_start_press() {
	if (player_info[0]->chara_kind != CHARA_KIND_MAX && player_info[1]->chara_kind != CHARA_KIND_MAX) {
		*game_state = GAME_STATE_BATTLE;
		*looping = false;
	}
}

void CSS::event_right_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 9 && chara_slots_ordered[my_col[player_id] + 1][my_row[player_id]] != nullptr) {
			my_col[player_id]++;
		}
		is_last_input_right[player_id] = true;
		select_slot();
	}
}

void CSS::event_left_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 0 && chara_slots_ordered[my_col[player_id] - 1][my_row[player_id]] != nullptr) {
			my_col[player_id]--;
		}
		is_last_input_right[player_id] = false;
		select_slot();
	}
}

void CSS::event_down_press() {
	bool jump = false;
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
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
					player_info[player_id]->crash_reason = "Couldn't find a valid column!";
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
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
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
					player_info[player_id]->crash_reason = "Couldn't find a valid column!";
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
	for (int i = 0; i < num_slots; i++) {
		if (my_col[player_id] == chara_slots[i].my_col
		&& my_row[player_id] == chara_slots[i].my_row) {
			player_selected_index[player_id] = i;
			break;
		}
	}
	big_chara_slots[player_id].texture.destroy(false);
	big_chara_slots[player_id].texture = GameTexture(chara_slots[player_selected_index[player_id]].texture);

	big_chara_slots[player_id].name = chara_slots[player_selected_index[player_id]].name;
	big_chara_slots[player_id].texture_dir = chara_slots[player_selected_index[player_id]].texture_dir;


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
	background_texture.render();
	big_bar_texture.render();
	top_bar_texture.render();

	for (int i = 0; i < num_slots; i++) {
		chara_slots[i].render();
	}


	for (int i = 0; i < 2; i++) {
		big_chara_slots[i].texture.render();
		if (big_chara_slots[i].texture_dir == "default") { //Todo: OpenGL text rendering
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
		cursors[i].set_target(chara_slots[player_selected_index[i]].texture.pos.x, chara_slots[player_selected_index[i]].texture.pos.y);
		cursors[i].render();
	}
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
			player_info[player_id]->chara_kind = CHARA_KIND_MAX;
			my_col[player_id] = chara_slots[i].my_col;
			my_row[player_id] = chara_slots[i].my_row;
			select_slot();
			event_select_press();
			return;
		}
	}
}

/// <summary>
/// Get the character for a CSS slot. For the record, chara_id is public so this function is kinda irrelevant.
/// </summary>
/// <returns>: The character for a CSS slot.</returns>
int CssSlot::get_chara_kind() {
	return chara_kind;
}

/// <summary>
/// Check whether or not a CSS slot has been initialized. For the record, initialized is public so this function is kinda irrelevant.
/// </summary>
/// <returns>Whether or not a CSS slot has been initialized.</returns>
bool CssSlot::is_initialized() {
	return initialized;
}

/// <summary>
/// Initialize a CSS slot with the given values.
/// </summary>
/// <param name="chara_kind">: The chara_kind this slot will correspond to.</param>
/// <param name="textureDir">: The directory where the CSS slot's render is located.</param>
/// <param name="name">: The name of the character for UI purposes.</param>
void CssSlot::init(int chara_kind, std::string textureDir, std::string name) {
	texture.init("resource/ui/menu/css/chara/" + textureDir + "/render.png");
	texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	this->name = name;
	this->texture_dir = textureDir;
	this->chara_kind = chara_kind;
	initialized = true;
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

/// <summary>
/// Move the CSS Cursor based on its partial and target values, and render.
/// </summary>
void CssCursor::render() {
	partial_x += (target_x - partial_x) / 8;
	partial_y += (target_y - partial_y) / 8;
	texture.pos.x = partial_x;
	texture.pos.y = partial_y;
	texture.process();
	texture.render();
};

/// <summary>
/// Initialize the CSS Cursor
/// </summary>
/// <param name="texture_path">: The path to the CSS Cursor's texture.</param>
void CssCursor::init(std::string texture_path) {
	texture.init(texture_path);
};

/// <summary>
/// Set the CSS Cursor's target.
/// </summary>
/// <param name="x">: What to set the target X to.</param>
/// <param name="y">: What to set the target Y to.</param>
void CssCursor::set_target(int x, int y) {
	target_x = x;
	target_y = y;
}