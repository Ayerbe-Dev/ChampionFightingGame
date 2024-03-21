#pragma once
#include <iostream>
#include <vector>
#include "Button.h"
#include "CharaKind.h"

const int CONTROL_TYPE_ADVANCE = 0;
const int CONTROL_TYPE_BASIC = 1;

class PlayerInfo {
public:
	PlayerInfo();
	PlayerInfo(std::string name);

	std::string name;
	int control_type;
	std::vector<Button> custom_mappings;
	int preferred_chara;
	int preferred_costume[CHARA_KIND_MAX];
	int preferred_color[CHARA_KIND_MAX];
};