#pragma once
#include <iostream>
#include <vector>
#include "Button.h"
#include "CharaKind.h"

class PlayerInfo {
public:
	PlayerInfo();

	std::string name;
	int control_type;
	std::vector<Button> custom_mappings;
	int preferred_chara = CHARA_KIND_ROWAN;
	int preferred_color[CHARA_KIND_MAX] = { 0 };
};