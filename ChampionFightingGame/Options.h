#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "PlayerInfo.h"
#include "GameManager.h"

void controls_main(GameManager *game_manager, SDL_Texture *background, GameMenu *background_menu);

class OptionsMenu : public GameMenu {
public:
	OptionsMenu();

	void event_select_press();
	void event_back_press();
	void event_start_press();
	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
};

class OptionsOverlay : public GameMenu {
public:
	OptionsOverlay();
	OptionsOverlay(int width, int height, string dir);

	GameTexture panel;
	PlayerInfo* player_info[2];
};