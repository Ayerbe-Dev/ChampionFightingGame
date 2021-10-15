#include "Eric.h"

Eric::Eric() {
	
}

Eric::Eric(SDL_Renderer *renderer, int id) {
	resource_dir = "resource/chara/eric";
	superInit(id, renderer);
	loadEricACMD();
	loadEricStatusFunctions();
	set_current_move_script("default");
}

void Eric::chara_id() {

}

void Eric::loadEricACMD() {
	script("default", [this]() {
		return;
	});
}

void Eric::loadEricStatusFunctions() {

}

EricScript::EricScript() {};

EricScript::EricScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->eric_script = move_script;
	this->id = id;
}

void Eric::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (eric_scripts[i].name == anim_name) {
			move_script = eric_scripts[i].eric_script;
			break;
		}
		else {
			move_script = eric_scripts[0].eric_script;
		}
	}
}

void Eric::script(string name, function<void()> move_script) {
	for (int i = 0; i < 256; i++) {
		if (eric_scripts[i].id == -1) {
			eric_scripts[i] = EricScript::EricScript(name, move_script, i);
			break;
		}
	}
}