#include "Roy.h"

Roy::Roy() {
	resource_dir = "resource/chara/roy";
}

Roy::Roy(PlayerInfo* player_info, int id) {

}

void Roy::chara_id() {

}

void Roy::loadRoyStatusFunctions() {

}

RoyScript::RoyScript() {};

RoyScript::RoyScript(string name, function<void()> move_script) {
	this->name = name;
//	this->move_script = &move_script;
}

/*
void Roy::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (roy_scripts[i].name == anim_name) {
			moveScript = &RoyScript::move_script;
		}
	}
}*/