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

	void process_main();
	void render_main();

	void event_start_press();
	void event_select_press();
	void event_back_press();
	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
};
