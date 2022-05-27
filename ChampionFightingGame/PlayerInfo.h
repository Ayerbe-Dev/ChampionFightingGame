#pragma once
#include "utils.h"
#include "Stage.h"
#include "GameController.h"

class PlayerInfo {
public:
	PlayerInfo();
	PlayerInfo(int id);

	void set_default_button_mappings(int id);

	GameController controller;
	int id;
	int chara_kind{ 0 };
	StageInfo stage_info;
	std::string name;

	std::string crash_reason{ "Crash Message Goes Here" };
};