#pragma once
#include "Stage.h"
#include "GameController.h"

class PlayerInfo;

class Player {
public:
	Player();
	Player(int id);

	void set_default_button_mappings(int id);
	void load_player(PlayerInfo* player_info);

	GameController controller;
	int id;
	int chara_kind = 0;
	int alt_color = 0;
	int control_type = CONTROL_TYPE_ADVANCE;
	StageInfo stage_info;
	std::string name;
};