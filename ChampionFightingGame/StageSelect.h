#pragma once
#include <SDL/SDL.h>
#include "PlayerInfo.h"
#include "GameTexture.h"
#include "GameMenu.h"
#include "Stage.h"

#include "GameManager.h"

#define CSS_SLOTS 32
#define NUM_COLS 10
#define NUM_ROWS 4
#define CSS_WIDTH WINDOW_WIDTH * 0.8
#define CSS_HEIGHT WINDOW_HEIGHT / 2.5

void stage_select_main(GameManager *game_manager);

class SSSSlot {
public:
	SSSSlot();
	SSSSlot(std::string ui_name, int stage_kind, std::string stage_name, bool selectable);

	GameTexture slot_texture;

	void set_x(int x);
	void set_y(int y);

	int myCol{ -1 };
	int myRow{ -1 };
	std::string name;
	bool selectable;
	Stage stage;

	bool initialized = false;
};

class StageSelect : public GameMenu {
public:
	StageSelect();
	
	int load_stage_select();

	void event_select_press();
	void event_back_press();
	void event_start_press();
	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();

	void add_stage_slot(int stage_kind, std::string stage_name_dir, std::string stage_name_ui, bool selectable);
	void centerSlots();

	void render();

	int cursor_index{ 0 };
	int myCol{ 0 };
	int myRow{ 0 };

	int numCols;
	int numRows;
	int colsOffset;

	PlayerInfo* player_info[2];

	int num_slots;
	SSSSlot stage_slots[CSS_SLOTS];
	SSSSlot stage_slots_column[10][4];
};

class SSSCursor {
public:
	void init(std::string sTexturePath);
	void setTarget(int x, int y);
	void render();
	GameTexture cursorTexture;
	int iXTarget = 0;
	int iYTarget = 0;
private:
	float partialX = 0;
	float partialY = 0;
};