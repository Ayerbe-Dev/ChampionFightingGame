#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "PlayerInfo.h"
#include "GameManager.h"

void controls_main(GameManager *game_manager);

class OptionsOverlay : public GameMenu {
public:
	OptionsOverlay();
	OptionsOverlay(int width, int height, string dir);

	GameTexture panel;
	PlayerInfo* player_info[2];
};