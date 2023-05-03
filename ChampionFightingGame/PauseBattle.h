#pragma once
#include "GameState.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "Player.h"

void pause_battle_main();

class PauseBattle : public GameState {
public:
	PauseBattle();
	~PauseBattle();

	void event_start_press();
	void event_select_press();
	void event_back_press();

	GameTexture panel;
	Player* player[2];
};
