#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"

class GameLoader {
public:
	GameLoader();
	GameLoader(PlayerInfo player_info[2]);

	Fighter* fighter[2];
	FighterAccessor* fighter_accessor;
	IObject* p1;
	IObject* p2;
	GameTimer timer;
	HealthBar health_bar[2];
	PlayerIndicator player_indicator[2];
	int loaded_items = 0;
	Stage stage;

	PlayerInfo player_info[2];
};