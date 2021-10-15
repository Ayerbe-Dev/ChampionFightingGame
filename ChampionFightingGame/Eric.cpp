#include "Eric.h"

Eric::Eric() {
	
}

Eric::Eric(SDL_Renderer *renderer, int id) {
	resource_dir = "resource/chara/eric";
	superInit(id, renderer);
}

void Eric::chara_id() {

}

void Eric::loadEricACMD() {

}

void Eric::loadEricStatusFunctions() {

}

EricScript::EricScript() {};

EricScript::EricScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->id = id;
	this->move_script = &move_script;
}

void Eric::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (eric_scripts[i].name == anim_name) {
			moveScript = &EricScript::move_script;
		}
	}
}

void Eric::script(string name, function<void()> move_script) {
	for (int i = 0; i < 256; i++) {
		if (eric_scripts[i].id == -1) {
			eric_scripts[i] = EricScript::EricScript(name, move_script, id);
		}
	}
}