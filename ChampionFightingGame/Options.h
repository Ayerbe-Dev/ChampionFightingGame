#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "PlayerInfo.h"

int controls_main(PlayerInfo player_info[2]);

class OptionsOverlay : public GameMenu {
public:
	OptionsOverlay();
	OptionsOverlay(int width, int height, string dir);

	GameTexture panel;
	PlayerInfo* player_info[2];
};