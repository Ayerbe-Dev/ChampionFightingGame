#pragma once
#include "Stage.h"
#include "GameController.h"

class PlayerInfo;

class Player {
public:
	Player();
	Player(int id);

	void load_player(int index);
	void set_alt_for_chara();

	GameController controller;
	int id;
	int chara_kind;
	int alt_costume;
	int alt_color;
	int control_type;
	StageInfo stage_info;
	std::string name;
	PlayerInfo* player_info;
};