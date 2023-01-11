#pragma once
#include "GameState.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "Player.h"

void controls_main();

class OptionsMenu : public GameState {
public:
	OptionsMenu();
	~OptionsMenu();

	void event_back_press();

	GameTexture panel;
	Player* player[2];
};