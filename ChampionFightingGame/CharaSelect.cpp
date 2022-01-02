#include <cmath>
#include "CharaSelect.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "Loader.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

/// <summary>
/// The main function while on the character select screen.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void chara_select_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	Debugger debugger;
	debugger = Debugger();


	CharaSelectLoader* chara_select_loader = new CharaSelectLoader;
	chara_select_loader->player_info[0] = player_info[0];
	chara_select_loader->player_info[1] = player_info[1];


	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool loading = true;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	CSS css;
	CssCursor cursors[2];

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadCharaSelect, "Init.rar", (void*)chara_select_loader);
	SDL_DetachThread(loading_thread);

	game_manager->set_menu_info(nullptr);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (loading) {
		frameTimeDelay();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
					return;
				}
				break;
			}
		}

		load_icon.move();
		SDL_LockMutex(file_mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		int total_items = 3;
		loadingBar.setTargetPercent(((float)chara_select_loader->loaded_items / total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(file_mutex);

		if (chara_select_loader->finished) {
			if (!chara_select_loader->can_ret) {
				css = chara_select_loader->css;
				for (int i = 0; i < 2; i++) {
					cursors[i] = chara_select_loader->css_cursor[i];
					player_info[i] = chara_select_loader->player_info[i];
				}
				game_manager->set_menu_info(&css);
			}
			chara_select_loader->can_ret = true;	
			loading = false;
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (*game_manager->looping[game_manager->layer]) {
		frameTimeDelay();
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
		}
		SDL_RenderClear(g_renderer);
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}
		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		game_manager->handle_menus();

		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);

		css.render();

		for (int i = 0; i < 2; i++) {
			css.query_fixed_css_slot_pos(css.player_selected_index[i], &cursors[i].target_x, &cursors[i].target_y);
			cursors[i].render();
		}

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderClear(g_renderer);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}

	delete chara_select_loader;
}

/// <summary>
/// The constructor for the CSS class.
/// </summary>
CSS::CSS() {
	//initialize other textures
	background_texture.init("resource/ui/menu/css/CSSbackground.png");
	background_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	big_bar_texture.init("resource/ui/menu/css/CSSbottombar.png");
	big_bar_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	top_bar_texture.init("resource/ui/menu/css/CSStopbar.png");
	top_bar_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>0 if successful, -1 if the file fails to open.</returns>
int CSS::load_css() {
	ifstream fileCssTable;
	fileCssTable.open("resource/ui/menu/css/css_param.yml");
	int iCharacterKind;
	string sCharacterDir;
	bool bSelectable;
	CssSlot* tmpSlot;
	int iRowXdelta = 0;

	if (fileCssTable.fail()) {
		fileCssTable.close();
		return -1;
	}

	string sCharacterName;
	for (int i = 0; getline(fileCssTable, sCharacterName); i++) {
		fileCssTable >> iCharacterKind >> sCharacterDir >> bSelectable;
		if (bSelectable) {
			add_slot(iCharacterKind, sCharacterDir, sCharacterName);
		}
		getline(fileCssTable, sCharacterName); //100% authentic jank code
	}
	fileCssTable.close();

	for (int iRow = 1; iRow < 4; iRow++) {
		//Following line checks if the row is filled (the tenth item in the row is filled)
		if (chara_slots[iRow * 10 - 1].is_initialized()) {
			//row filled code
			for (int iColumn = 0; iColumn < 10; iColumn++) {
				tmpSlot = &chara_slots[((iRow - 1) * 10) + iColumn];

				tmpSlot->texture.setScaleFactor(1.2);
				tmpSlot->texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					(WINDOW_WIDTH/10) is for the spacing between the cards
					(WINDOW_WIDTH/20) is for the offset to center the row, it should always be 1/2 the previous one
				*/

				tmpSlot->set_x_pos(iColumn * (WINDOW_WIDTH / 10) + (WINDOW_WIDTH / 20));
				tmpSlot->set_y_pos(iRow * (tmpSlot->texture.getScaledHeight() + 20));
			}
		}
		else {
		 //row not filled code
			for (int iColumn = 0; iColumn < get_num_slots() % 10; iColumn++) {
				tmpSlot = &chara_slots[((iRow - 1) * 10) + iColumn];

				tmpSlot->texture.setScaleFactor(1.2);
				tmpSlot->texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					Basically.....
					Calculate the difference in x positions of the first and last card as iRowXdelta
					Divide iRowXdelta by 2 and subtract the WINDOW_WIDTH by iRowXdelta to get the starting point
					from which to render the row.
				*/
				iRowXdelta = (WINDOW_WIDTH / 10) * (get_num_slots() % 10) - (WINDOW_WIDTH / 10); //yes this gets recalculated every frame, no im not moving it
				tmpSlot->set_x_pos(((WINDOW_WIDTH - iRowXdelta) / 2) + iColumn * (WINDOW_WIDTH / 10));
				tmpSlot->set_y_pos(iRow * (tmpSlot->texture.getScaledHeight() + 20));
			}
		}
	}

	//Anyway so we're also using a 2D array now

	int col = 0;
	int row = 0;
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].is_initialized()) {
			chara_slots[i].my_col = col;
			chara_slots[i].my_row = row;
			chara_slots_ordered[col][row] = chara_slots[i];
			if (col == 9) {
				col = 0;
				row++;
			}
			else {
				col++;
			}
		}
	}
	num_cols = col;
	num_rows = row;
	center_slots();

	return 0;
}

/// <summary>
/// Adds an entry in the CSS slots array. 
/// </summary>
/// <param name="id">: The chara_kind the slot will correspond to.</param>
/// <param name="cardDir">: The directory for the slot's portrait.</param>
/// <param name="cardName">: The actual character name to display on the UI.</param>
void CSS::add_slot(int id, string cardDir, string cardName) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!chara_slots[i].is_initialized()) {
			chara_slots[i].init(id, cardDir, cardName);
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
	int size = 0;
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].is_initialized()) {
			size++;
		}
	}
	return size;
}

void CSS::event_select_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		player_info[player_id]->chara_kind = chara_slots[player_selected_index[player_id]].get_chara_kind();
		CssSlotMobile tmpSlot;
		tmpSlot.texture = chara_slots[player_selected_index[player_id]].texture;

		switch (player_id) {
			case 0:
				tmpSlot.set_target(117, 906, 1.5, 1.5);
				break;
			case 1:
				tmpSlot.set_target(1806, 906, 1.5, 1.5);
				break;
			default:
				printf("oof\n");
				break;
		}
		tmpSlot.texture.setScaleFactor(1.5);
		mobile_css_slots[player_id] = tmpSlot;
	}
}
void CSS::event_back_press() {
	if (player_info[player_id]->chara_kind != CHARA_KIND_MAX) {
		CssSlotMobile tmpSlot;
		mobile_css_slots[player_id] = tmpSlot;
		player_info[player_id]->chara_kind = CHARA_KIND_MAX;
	}
	else {
		displayLoadingScreen();
		*game_state = GAME_STATE_MENU;
		*looping = false;
	}
}

void CSS::event_start_press() {
	if (player_info[0]->chara_kind != CHARA_KIND_MAX && player_info[1]->chara_kind != CHARA_KIND_MAX) {
		displayLoadingScreen();
		*game_state = GAME_STATE_BATTLE;
		*looping = false;
	}
}

void CSS::event_right_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 9 && chara_slots_ordered[my_col[player_id] + 1][my_row[player_id]].is_initialized()) {
			my_col[player_id]++;
		}
		is_last_input_right[player_id] = true;
		select_slot();
	}
}

void CSS::event_left_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (my_col[player_id] != 0 && chara_slots_ordered[my_col[player_id] - 1][my_row[player_id]].is_initialized()) {
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
			if (!chara_slots_ordered[my_col[player_id]][my_row[player_id] + 1].is_initialized()) {
				jump = true;
				bool valid_col = false;
				if (my_col[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (chara_slots_ordered[i][my_row[player_id] + 1].is_initialized()) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (chara_slots_ordered[i][my_row[player_id] + 1].is_initialized()) {
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
			if (!chara_slots_ordered[my_col[player_id]][my_row[player_id] - 1].is_initialized()) {
				jump = true;
				bool valid_col = false;
				if (my_col[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (chara_slots_ordered[i][my_row[player_id] - 1].is_initialized()) {
							my_col[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (chara_slots_ordered[i][my_row[player_id] - 1].is_initialized()) {
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
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (my_col[player_id] == chara_slots[i].my_col
		&& my_row[player_id] == chara_slots[i].my_row) {
			player_selected_index[player_id] = i;
			break;
		}
	}
}

/// <summary>
/// Render all CSS-related textures.
/// </summary>
void CSS::render() {
	background_texture.render();
	big_bar_texture.render();
	top_bar_texture.render();

	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].is_initialized()) {
			chara_slots[i].render();
		}
	}


	for (int i = 0; i < 2; i++) {
		CssSlot tmpSlot;
		tmpSlot.texture = chara_slots[player_selected_index[i]].texture;
		tmpSlot.name = chara_slots[player_selected_index[i]].name;
		tmpSlot.texture_dir = chara_slots[player_selected_index[i]].texture_dir;

		switch (i) {
			case 0:
				tmpSlot.texture.destRect.x = 117;
				tmpSlot.texture.destRect.y = 906;
				break;
			case 1:
				tmpSlot.texture.destRect.x = 1806;
				tmpSlot.texture.destRect.y = 906;
				break;
			default:
				cout << "How the fuck" << endl;
				break;
		}
		tmpSlot.texture.setScaleFactor(1.5);
		tmpSlot.texture.setAlpha((Uint8)127);
		tmpSlot.texture.render();
		tmpSlot.texture.setAlpha((Uint8)255);
		if (tmpSlot.texture_dir == "default") {
			draw_text_multi_lines("FiraCode-Regular.ttf", tmpSlot.name, tmpSlot.texture.destRect.x, tmpSlot.texture.destRect.y + 70, 24, 255, 255, 255, 255);
		}

		mobile_css_slots[i].play_anim();
		mobile_css_slots[i].texture.render();
	}
}

/// <summary>
/// Find the X and Y coordinates of a specified CSS slot.
/// </summary>
/// <param name="index">: The index of the CSS slot to query.</param>
/// <param name="x_ret">: Will be set to the X coord.</param>
/// <param name="y_ret">: Will be set to the Y coord.</param>
void CSS::query_fixed_css_slot_pos(int index, int* x_ret, int* y_ret) {
	if (index < get_num_slots()) {
		*x_ret = chara_slots[index].texture.destRect.x;
		*y_ret = chara_slots[index].texture.destRect.y;
	}
	else {
		printf("CSS::queryFixedCssSlotPosition --> Index out of range!\n");
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
/// Takes the regular CSS slots array and uses it to generate a 2D array which is organized by row and column. The logic behind this is kind of weird
/// but it makes navigating up and down on the CSS slightly easier.
/// </summary>
void CSS::center_slots() {
	int empty_cols = 1;
	int empty_row;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int i2 = 0; i2 < NUM_COLS; i2++) {
			if (!chara_slots_ordered[i2][i].is_initialized()) { //If this slot is empty
				if (chara_slots_ordered[0][i].is_initialized()) { //If the first slot on that row is full
					empty_cols = NUM_COLS - i2;
					empty_row = i;
					goto ENDL;
				}
			}
		}
	}
	ENDL: //im so funny
	
	cols_offset = empty_cols;

	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].my_row == empty_row) {
			chara_slots[i].my_col += (empty_cols / 2);
		}
	}
	for (int i = 0; i < NUM_COLS; i++) {
		for (int i2 = 0; i2 < NUM_ROWS; i2++) {
			chara_slots_ordered[i][i2].my_col = -1;
			chara_slots_ordered[i][i2].my_row = -1;
			chara_slots_ordered[i][i2].initialized = false;
			for (int i3 = 0; i3 < CSS_SLOTS; i3++) {
				if (chara_slots[i3].my_col == i
				&& chara_slots[i3].my_row == i2) {
					chara_slots_ordered[i][i2] = chara_slots[i3];
					break;
				}
			}
		}
	}
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
void CssSlot::init(int chara_kind, string textureDir, string name) {
	texture.init("resource/ui/menu/css/chara/" + textureDir + "/render.png");
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
	texture.destRect.y = y;
}

/// <summary>
/// Set the X position of the CSS Slot.
/// </summary>
/// <param name="X">: The intended X position.</param>
void CssSlot::set_x_pos(int x) {
	texture.destRect.x = x;
}

/// <summary>
/// Get the width of this CSS Slot's texture.
/// </summary>
/// <returns>The width of this CSS Slot's texture.</returns>
int CssSlot::get_texture_width() {
	return texture.destRect.w;
}

/// <summary>
/// Render the CSS Slot's texture.
/// </summary>
void CssSlot::render() {
	texture.render();
}

/// <summary>
/// Check if the CSS Slot is below a given coordinate.
/// </summary>
/// <param name="y">: The coord to compare against.</param>
/// <returns>Whether or not the CSS Slot is below a given coordinate.</returns>
bool CssSlot::is_below(int y) {
	if (texture.destRect.y > y) {
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
	if (texture.destRect.y < y) {
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
	texture.destRect.x = partial_x;
	texture.destRect.y = partial_y;
	texture.render();
};

/// <summary>
/// Initialize the CSS Cursor
/// </summary>
/// <param name="texture_path">: The path to the CSS Cursor's texture.</param>
void CssCursor::init(string texture_path) {
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

/// <summary>
/// Spaghetti code which makes it rotate once.
/// </summary>
void CssSlotMobile::play_anim() {
	//spaghetti code which makes it rotate once
	anim_time++;
	if (cos(theta) < 0) {
		theta = -3.14 / 2;
	}
	if (anim_time < anim_speed) {
		theta += (3.14) / anim_speed;
		texture.setHorizontalScaleFactor(cos(theta) + scale_x);
		texture.setVerticalScaleFactor(scale_y);
	}
	else {
		texture.setScaleFactor(1.5);
	}

	scale_y += (1.5 - scale_y) / (anim_speed / 3);
	scale_x += (0 - scale_x) / (anim_speed / 3);
	pos_x += (target_x - pos_x) / (anim_speed / 2);
	pos_y += (target_y - pos_y) / (anim_speed / 2);

	texture.destRect.x = pos_x;
	texture.destRect.y = pos_y;

}

/// <summary>
/// Set the Mobile CSS Slot's target.
/// </summary>
/// <param name="x">: The target X position.</param>
/// <param name="y">: The target Y position.</param>
/// <param name="w">: The intended width of the slot.</param>
/// <param name="h">: The intended height of the slot.</param>
void CssSlotMobile::set_target(int x, int y, float w, float h) {
	target_x = x;
	target_y = y;

	pos_x = texture.destRect.x;
	pos_y = texture.destRect.y;

	scale_x = w - 1.5;
	scale_y = h;
}