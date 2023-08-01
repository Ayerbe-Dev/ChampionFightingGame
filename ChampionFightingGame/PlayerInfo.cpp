#include "PlayerInfo.h"

PlayerInfo::PlayerInfo() {
	name = "";
	control_type = CONTROL_TYPE_ADVANCE;
	preferred_chara = CHARA_KIND_ROWAN;
	for (int i = 0; i < CHARA_KIND_MAX; i++) {
		preferred_costume[i] = 0;
		preferred_color[i] = 0;
	}
}

PlayerInfo::PlayerInfo(std::string name) {
	this->name = name;
	control_type = CONTROL_TYPE_ADVANCE;
	preferred_chara = CHARA_KIND_ROWAN;
	for (int i = 0; i < CHARA_KIND_MAX; i++) {
		preferred_costume[i] = 0;
		preferred_color[i] = 0;
	}
}