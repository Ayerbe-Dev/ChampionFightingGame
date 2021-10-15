#include "Eric.h"

Eric::Eric() {
	resource_dir = "resource/chara/eric";
}

Eric::Eric(PlayerInfo *player_info, int id) {

}

void Eric::chara_id() {

}

void Eric::loadEricStatusFunctions() {

}

EricScript::EricScript() {};

EricScript::EricScript(string name, function<void()> move_script) {
	this->name = name;
//	this->move_script = &move_script;
}
/*
void Eric::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (eric_scripts[i].name == anim_name) {
			moveScript = &EricScript::move_script;
		}
	}
}*/