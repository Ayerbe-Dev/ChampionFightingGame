#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "PlayerInfo.h"

void pause_battle_main();

class PauseBattle : public GameMenu {
public:
	PauseBattle();
	PauseBattle(int width, int height, std::string dir);

	void init(int width, int height, std::string dir);

	GameTexture panel;
	PlayerInfo* player_info[2];
};
