#pragma once
#include "GameMenu.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "Player.h"

void pause_battle_main();

class PauseBattle : public GameMenu {
public:
	PauseBattle();
	~PauseBattle();

	void event_start_press();
	void event_select_press();

	GameTexture panel;
	Player* player[2];
};
