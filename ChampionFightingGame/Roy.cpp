#include "Roy.h"

Roy::Roy() {

}

Roy::Roy(SDL_Renderer *renderer, int id) {
	resource_dir = "resource/chara/roy";
	superInit(id, renderer);
	loadRoyACMD();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ROY;
	this->base_texture = loadTexture("resource/chara/roy/sprite/sprite.png", renderer);
}

void Roy::chara_id() {

}

void Roy::loadRoyACMD() {
	script("default", [this]() {
		return;
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 2)) {
			new_hitbox(1, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(3, 2)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 4)) {
			new_hitbox(1, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(3, 2)) {
			clear_hitbox_all();
		}
	});
	script("stand_hp", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 8)) {
			new_hitbox(1, 30, 5, 1.2, 1, GameCoordinate{ -5,70 }, GameCoordinate{ 60, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(3, 1)) {
			new_hitbox(2, 40, 10, 1.2, 1, GameCoordinate{ 50,70 }, GameCoordinate{ 70, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(4, 3)) {
			clear_hitbox_all();
		}
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {

	});
	script("crouch_lp", [this]() {

	});
	script("crouch_mp", [this]() {

	});
	script("crouch_hp", [this]() {

	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		if (is_excute_frame(1, 3)) {

		}
		if (is_excute_frame(2, 6)) {
			new_hitbox(1, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
	});
	script("jump_lp", [this]() {

	});
	script("jump_mp", [this]() {

	});
	script("jump_hp", [this]() {

	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("parry_start", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(2, 1)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(3, 4)) { //due to rates, this is actually active for 4 frames
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
}

void Roy::loadRoyStatusFunctions() {

}

RoyScript::RoyScript() {};

RoyScript::RoyScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->roy_script = move_script;
	this->id = id;
}


void Roy::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (roy_scripts[i].name == anim_name) {
			move_script = roy_scripts[i].roy_script;
			break;
		}
		else {
			move_script = roy_scripts[0].roy_script;
		}
	}
}

void Roy::script(string name, function<void()> move_script) {
	for (int i = 0; i < 256; i++) {
		if (roy_scripts[i].id == -1) {
			roy_scripts[i] = RoyScript(name, move_script, i);
			break;
		}
	}
}