#include "Roy.h"

Roy::Roy() {

}

Roy::Roy(SDL_Renderer *renderer, int id) {
	resource_dir = "resource/chara/roy";
	superInit(id, renderer);
}

void Roy::chara_id() {

}

void Roy::loadRoyACMD() {
	script("stand_lp", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(1, 30, 5, 1.2, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_ALL, 12, 9, 9, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, false, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 2)) {
			new_hitbox(0, 30, 5, 1.2, GameCoordinate{ 5,70 }, GameCoordinate{ 50, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_ALL, 12, 9, 9, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, false, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_wait(3, 2)) {
			clear_hitbox_all();
		}
	});
}

void Roy::loadRoyStatusFunctions() {

}

RoyScript::RoyScript() {};

RoyScript::RoyScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->id = id;
//	this->move_script = &move_script;
}


void Roy::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (roy_scripts[i].name == anim_name) {
//			moveScript = &RoyScript::move_script;
		}
	}
}

void Roy::script(string name, function<void()> move_script) {
	for (int i = 0; i < 256; i++) {
		if (roy_scripts[i].id == -1) {
			roy_scripts[i] = RoyScript::RoyScript(name, move_script, id);
		}
	}
}