#include <cmath>
#include "StageSelect.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "MenuHandler.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

int stage_select_main(PlayerInfo player_info[2]) {
	bool stage_selecting = true;
	displayLoadingScreen();
	int* next_state = (int*)GAME_STATE_DEBUG_MENU;
	StageSelect stage_select(next_state);

	stage_select.player_info[0] = &player_info[0];
	stage_select.player_info[1] = &player_info[1];
	
	if (stage_select.load_stage_select()) {
		displayLoadingScreen();
		player_info[0].crash_reason = "Could not open Stage Select file!";
		stage_selecting = false;
		*next_state = GAME_STATE_DEBUG_MENU;
	}

	while (stage_selecting) {

	}

	return *next_state;
}

StageSelect::StageSelect() {

}

StageSelect::StageSelect(int* next_state) {
	this->next_state = next_state;
}

int StageSelect::load_stage_select() {
	ifstream stage_table;
	stage_table.open("resource/ui/menu/css/stage_select_param.yml");
	int stage_kind;
	string stage_name_dir;
	bool selectable;
	SSSSlot* tmpSlot;
	int iRowXdelta = 0;

	if (stage_table.fail()) {
		stage_table.close();
		return -1;
	}

	string stage_name_ui;
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

				tmpSlot->slot_texture.setScaleFactor(1.2);
				tmpSlot->slot_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					(WINDOW_WIDTH/10) is for the spacing between the cards
					(WINDOW_WIDTH/20) is for the offset to center the row, it should always be 1/2 the previous one
				*/

				tmpSlot->set_x(iColumn * (WINDOW_WIDTH / 10) + (WINDOW_WIDTH / 20));
				tmpSlot->set_y(iRow * (tmpSlot->slot_texture.getScaledHeight() + 20));
			}
		}
		else {
		 //row not filled code
			for (int iColumn = 0; iColumn < num_slots % 10; iColumn++) {
				tmpSlot = &stage_slots[((iRow - 1) * 10) + iColumn];

				tmpSlot->slot_texture.setScaleFactor(1.2);
				tmpSlot->slot_texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

				/*
					Basically.....
					Calculate the difference in x positions of the first and last card as iRowXdelta
					Divide iRowXdelta by 2 and subtract the WINDOW_WIDTH by iRowXdelta to get the starting point
					from which to render the row.
				*/
				iRowXdelta = (WINDOW_WIDTH / 10) * (num_slots % 10) - (WINDOW_WIDTH / 10); //yes this gets recalculated every frame, no im not moving it
				tmpSlot->set_x(((WINDOW_WIDTH - iRowXdelta) / 2) + iColumn * (WINDOW_WIDTH / 10));
				tmpSlot->set_y(iRow * (tmpSlot->slot_texture.getScaledHeight() + 20));
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

void StageSelect::add_stage_slot(int stage_kind, string stage_name_dir, string stage_name_ui, bool selectable) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!stage_slots[i].initialized) {
			SSSSlot temp_slot(stage_name_ui, stage_kind, stage_name_dir, selectable);
			stage_slots[i] = temp_slot;
			num_slots++;
			return;
		}
	}
}

void StageSelect::GAME_MENU_traverse_select() {

}
void StageSelect::GAME_MENU_traverse_back() {

}

void StageSelect::GAME_MENU_traverse_start() {

}

void StageSelect::GAME_MENU_traverse_up() {

}

void StageSelect::GAME_MENU_traverse_down() {

}

void StageSelect::GAME_MENU_traverse_left() {

}

void StageSelect::GAME_MENU_traverse_right() {

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

SSSSlot::SSSSlot(string ui_name, int stage_kind, string stage_name, bool selectable) {
	this->name = ui_name;
	this->selectable = selectable;
	stage = Stage(stage_kind, stage_name);
}

void SSSSlot::set_x(int x) {
	slot_texture.destRect.x = x;
}

void SSSSlot::set_y(int y) {
	slot_texture.destRect.y = y;
}

//Copied directly from CSSCursor

void SSSCursor::render() {
	partialX += (iXTarget - partialX) / 8;
	partialY += (iYTarget - partialY) / 8;
	cursorTexture.destRect.x = partialX;
	cursorTexture.destRect.y = partialY;
	cursorTexture.render();
};
void SSSCursor::init(string sTexturePath) {
	cursorTexture.init(sTexturePath);
};


void SSSCursor::setTarget(int x, int y) {
	iXTarget = x;
	iYTarget = y;
}