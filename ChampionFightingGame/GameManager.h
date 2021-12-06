#pragma once
#include "PlayerInfo.h"
#include "GameMenu.h"

class GameManager {
public:
	GameManager();
	~GameManager();

	PlayerInfo *player_info[2];
	int overlay_layer = 0;
	int* game_state;
	int* prev_game_state;
	int* game_context;
	int* prev_game_context;
	bool* looping;

	void (*game_main[GAME_STATE_MAX])(GameManager* game_manager);
	void (*game_substate_main[GAME_SUBSTATE_MAX])(GameManager* game_manager, SDL_Texture* background, GameMenu *game_menu);

	void set_game_state_functions(); //Assigns a function to each index of game_main

	void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
	void set_menu_info(GameMenu *menu_target, int init_hold_frames = 20, int hold_rate = 4);

	void handle_menus();

	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
	void event_start_press();
	void event_select_press();
	void event_back_press();
	void event_any_press();
private:
	GameMenu* menu_target;

	bool is_up_press(int id);
	bool is_down_press(int id);
	bool is_left_press(int id);
	bool is_right_press(int id);

	bool is_any_menu_input(int id);

	int init_hold_frames = 20;
	int hold_rate = 4;

	int u_hold_frames[2];
	int d_hold_frames[2];
	int l_hold_frames[2];
	int r_hold_frames[2];
};
