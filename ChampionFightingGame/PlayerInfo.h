#pragma once
#include <iostream>
#include <vector>
#include "Button.h"
#include "CharaKind.h"

class PlayerInfo {
public:
	PlayerInfo();
	PlayerInfo(std::string name);

	std::string name;
	int control_type;
	std::vector<Button> custom_mappings;
	int preferred_chara;
	int preferred_color[CHARA_KIND_MAX];
};