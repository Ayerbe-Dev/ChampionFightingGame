#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "PlayerInfo.h"

void pause_battle_main();

class PauseBattle : public GameMenu {
public:
	PauseBattle();
	~PauseBattle();
	
	void load_game_menu();
	void event_back_press();

	GameTexture panel;
	PlayerInfo* player_info[2];
};
