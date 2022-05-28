#include <cmath>
#include <iostream>
#include <fstream>
#include "StageSelect.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "Loader.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void stage_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	StageSelect stage_select;
	game_manager->set_menu_info(nullptr);

	if (stage_select.load_stage_select()) {
		player_info[0]->crash_reason = "Could not open Stage Select file!";
		return game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}

	game_manager->set_menu_info(&stage_select);

	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (game_manager->looping[game_manager->layer]) {
		wait_ms();
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
			player_info[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		SDL_SetRenderTarget(g_renderer, pScreenTexture);

/*
		 /$$$$$$$                            /$$                            /$$$$$$                  /$$
		| $$__  $$                          | $$                           /$$__  $$                | $$
		| $$  \ $$  /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$$  /$$$$$$
		| $$$$$$$/ /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$ /$$__  $$
		| $$__  $$| $$$$$$$$| $$  \ $$| $$  | $$| $$$$$$$$| $$  \__/      | $$      | $$  \ $$| $$  | $$| $$$$$$$$
		| $$  \ $$| $$_____/| $$  | $$| $$  | $$| $$_____/| $$            | $$    $$| $$  | $$| $$  | $$| $$_____/
		| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$| $$            |  $$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$
		|__/  |__/ \_______/|__/  |__/ \_______/ \_______/|__/             \______/  \______/  \_______/ \_______/



		  /$$$$$$                                      /$$   /$$
		 /$$__  $$                                    | $$  | $$
		| $$  \__/  /$$$$$$   /$$$$$$   /$$$$$$$      | $$  | $$  /$$$$$$   /$$$$$$   /$$$$$$
		| $$ /$$$$ /$$__  $$ /$$__  $$ /$$_____/      | $$$$$$$$ /$$__  $$ /$$__  $$ /$$__  $$
		| $$|_  $$| $$  \ $$| $$$$$$$$|  $$$$$$       | $$__  $$| $$$$$$$$| $$  \__/| $$$$$$$$
		| $$  \ $$| $$  | $$| $$_____/ \____  $$      | $$  | $$| $$_____/| $$      | $$_____/
		|  $$$$$$/|  $$$$$$/|  $$$$$$$ /$$$$$$$/      | $$  | $$|  $$$$$$$| $$      |  $$$$$$$
		 \______/  \______/  \_______/|_______/       |__/  |__/ \_______/|__/       \_______/
*/

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}
}

StageSelect::StageSelect() {

}

int StageSelect::load_stage_select() {
	std::ifstream stage_table;
	stage_table.open("resource/ui/menu/css/stage_select_param.yml");
	int stage_kind;
	std::string stage_name_dir;
	bool selectable;
	SSSSlot* tmpSlot;
	int iRowXdelta = 0;

	if (stage_table.fail()) {
		stage_table.close();
		return -1;
	}

	std::string stage_name_ui;
	for (int i = 0; getline(stage_table, stage_name_ui); i++) {
		stage_table >> stage_kind >> stage_name_dir >> selectable;
		add_stage_slot(stage_kind, stage_name_dir, stage_name_ui, selectable); //Unlike the CSS, all unlockable stages will be visible whether or not
		//you own them, so we want to pass whether or not it's selectable as an arg
		getline(stage_table, stage_name_ui);
	}
	stage_table.close();

	for (int iRow = 1; iRow < 4; iRow++) {
		//Following line checks if the row is filled (the tenth item in the row is filled)
		if (stage_slots[iRow * 10 - 1].initialized) {
			//row filled code
			for (int iColumn = 0; iColumn < 10; iColumn++) {
				tmpSlot = &stage_slots[((iRow - 1) * 10) + iColumn];

//				tmpSlot->slot_texture.setScaleFactor(1.2);
//				tmpSlot->slot_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					(WINDOW_WIDTH/10) is for the spacing between the cards
					(WINDOW_WIDTH/20) is for the offset to center the row, it should always be 1/2 the previous one
				*/

				tmpSlot->set_x(iColumn * (WINDOW_WIDTH / 10) + (WINDOW_WIDTH / 20));
//				tmpSlot->set_y(iRow * (tmpSlot->slot_texture.getScaledHeight() + 20));
			}
		}
		else {
		 //row not filled code
			for (int iColumn = 0; iColumn < num_slots % 10; iColumn++) {
				tmpSlot = &stage_slots[((iRow - 1) * 10) + iColumn];

//				tmpSlot->slot_texture.setScaleFactor(1.2);
//				tmpSlot->slot_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					Basically.....
					Calculate the difference in x positions of the first and last card as iRowXdelta
					Divide iRowXdelta by 2 and subtract the WINDOW_WIDTH by iRowXdelta to get the starting point
					from which to render the row.
				*/
				iRowXdelta = (WINDOW_WIDTH / 10) * (num_slots % 10) - (WINDOW_WIDTH / 10); //yes this gets recalculated every frame, no im not moving it
				tmpSlot->set_x(((WINDOW_WIDTH - iRowXdelta) / 2) + iColumn * (WINDOW_WIDTH / 10));
//				tmpSlot->set_y(iRow * (tmpSlot->slot_texture.getScaledHeight() + 20));
			}
		}
	}

	//Anyway so we're also using a 2D array now

	int col = 0;
	int row = 0;
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (stage_slots[i].initialized) {
			stage_slots[i].myCol = col;
			stage_slots[i].myRow = row;
			stage_slots_column[col][row] = stage_slots[i];
			if (col == 9) {
				col = 0;
				row++;
			}
			else {
				col++;
			}
		}
	}
	numCols = col;
	numRows = row;
	centerSlots();

	return 0;
}

void StageSelect::add_stage_slot(int stage_kind, std::string stage_name_dir, std::string stage_name_ui, bool selectable) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!stage_slots[i].initialized) {
			SSSSlot temp_slot(stage_name_ui, stage_kind, stage_name_dir, selectable);
			stage_slots[i] = temp_slot;
			num_slots++;
			return;
		}
	}
}

void StageSelect::event_select_press() {

}
void StageSelect::event_back_press() {

}

void StageSelect::event_start_press() {

}

void StageSelect::event_up_press() {

}

void StageSelect::event_down_press() {

}

void StageSelect::event_left_press() {

}

void StageSelect::event_right_press() {

}

void StageSelect::render() {

}

void StageSelect::centerSlots() {
	int empty_cols = 1;
	int empty_row;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int i2 = 0; i2 < NUM_COLS; i2++) {
			if (!stage_slots_column[i2][i].initialized) { //If this slot is empty
				if (stage_slots_column[0][i].initialized) { //If the first slot on that row is full
					empty_cols = NUM_COLS - i2;
					empty_row = i;
					goto ENDL;
				}
			}
		}
	}
	ENDL: //im so funny

	colsOffset = empty_cols;

	for (int i = 0; i < CSS_SLOTS; i++) {
		if (stage_slots[i].myRow == empty_row) {
			stage_slots[i].myCol += (empty_cols / 2);
		}
	}
	for (int i = 0; i < NUM_COLS; i++) {
		for (int i2 = 0; i2 < NUM_ROWS; i2++) {
			stage_slots_column[i][i2].myCol = -1;
			stage_slots_column[i][i2].myRow = -1;
			stage_slots_column[i][i2].initialized = false;
			for (int i3 = 0; i3 < CSS_SLOTS; i3++) {
				if (stage_slots[i3].myCol == i
				&& stage_slots[i3].myRow == i2) {
					stage_slots_column[i][i2] = stage_slots[i3];
					break;
				}
			}
		}
	}
}

SSSSlot::SSSSlot() {}

SSSSlot::SSSSlot(std::string ui_name, int stage_kind, std::string stage_name, bool selectable) {
	this->name = ui_name;
	this->selectable = selectable;
//	stage = Stage(stage_kind, stage_name);
}

void SSSSlot::set_x(int x) {
//	slot_texture.destRect.x = x;
}

void SSSSlot::set_y(int y) {
//	slot_texture.destRect.y = y;
}

//Copied directly from CSSCursor

void SSSCursor::render() {
	partialX += (iXTarget - partialX) / 8;
	partialY += (iYTarget - partialY) / 8;
//	cursorTexture.destRect.x = partialX;
//	cursorTexture.destRect.y = partialY;
	cursorTexture.render();
};
void SSSCursor::init(std::string sTexturePath) {
	cursorTexture.init(sTexturePath);
};


void SSSCursor::setTarget(int x, int y) {
	iXTarget = x;
	iYTarget = y;
}