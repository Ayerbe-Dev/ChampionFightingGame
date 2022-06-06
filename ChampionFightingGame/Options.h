#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "Player.h"
#include "GameManager.h"

void controls_main();

class OptionsMenu : public GameMenu {
public:
	OptionsMenu();
	~OptionsMenu();

	void load_game_menu();
	void event_back_press();

	GameTexture panel;
	Player* player[2];
};