#include "GameManager.h"
#include "GameStates.h"

GameManager::GameManager() {
	player_info[0] = PlayerInfo(0);
	player_info[1] = PlayerInfo(1);
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		game_main[i] = nullptr;
	}
	set_game_state_functions();
}

void GameManager::set_game_state_functions() {
	game_main[GAME_STATE_BATTLE] = &battle_main;
	game_main[GAME_STATE_CHARA_SELECT] = &chara_select_main;
	game_main[GAME_STATE_CONTROLS] = &controls_main;
	game_main[GAME_STATE_DEBUG_MENU] = &debugMenu;
	game_main[GAME_STATE_MENU] = &menu_main;
	game_main[GAME_STATE_TITLE_SCREEN] = &title_screen_main;
}

void GameManager::update(PlayerInfo player_info[2], int game_state, int game_context) {
	this->player_info[0] = player_info[0];
	this->player_info[1] = player_info[1];
	prev_game_state = this->game_state;
	if (game_state != GAME_STATE_MAX) {
		this->game_state = game_state;
	}
	if (game_context != GAME_CONTEXT_MAX) {
		this->game_context = game_context;
	}
}