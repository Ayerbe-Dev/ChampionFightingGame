#pragma once
#include "PlayerInfo.h"

class GameManager {
public:
	GameManager();
	PlayerInfo player_info[2];
	int prev_game_state;
	int game_state = GAME_STATE_MENU;
	int game_context = GAME_CONTEXT_NORMAL;
	int overlay_layer = 0;

	void (*game_main[GAME_STATE_MAX])(GameManager* game_manager);
	void set_game_state_functions(); //Assigns a function to each index of game_main
	void update(PlayerInfo player_info[2], int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX); //Updates game_manager->player_info
		//to match the local one modified by the function
};
