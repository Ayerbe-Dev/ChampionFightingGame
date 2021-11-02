#include "Atlas.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Atlas::Atlas() {

}

Atlas::Atlas(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/chara/atlas";
	superInit(id, renderer);
	load_params();
	loadAtlasACMD();
	loadAtlasStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ATLAS;
	this->base_texture = loadTexture("resource/chara/atlas/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}
}

void Atlas::chara_id() {

}

void Atlas::loadAtlasStatusFunctions() {

}

void Atlas::loadAtlasACMD() { //todo: Fill this in with all of the common empty scripts
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 4;
		}
	});
	script("walk_f", [this]() {

	});
	script("walk_b", [this]() {

	});
	script("dash_f", [this]() {

	});
	script("dash_b", [this]() {

	});
	script("crouch_d", [this]() {

	});
	script("crouch", [this]() {

	});
	script("crouch_u", [this]() {

	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {

	});
	script("jump_f", [this]() {

	});
	script("jump_b", [this]() {

	});
	script("fall", [this]() {

	});
	script("stand_block", [this]() {

	});
	script("high_block", [this]() {

	});
	script("crouch_block", [this]() {

	});
	script("jump_block", [this]() {

	});
	script("parry_start", [this]() {

	});
	script("parry_start_air", [this]() {

	});
	script("parry_mid", [this]() {

	});
	script("parry_high", [this]() {

	});
	script("parry_low", [this]() {

	});
	script("parry_air", [this]() {

	});
	script("hitstun_parry", [this]() {

	});
	script("hitstun_parry_air", [this]() {

	});
	script("stand_lp", [this]() {

	});
	script("stand_mp", [this]() {

	});
	script("stand_hp", [this]() {

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
	script("grab", [this]() {

	});
	script("throw_f", [this]() {

	});
	script("throw_b", [this]() {

	});
	script("grab_air", [this]() {

	});
	script("throw_f_air", [this]() {

	});
	script("throw_b_air", [this]() {

	});
	script("stand_hitstun_l", [this]() {

	});
	script("stand_hitstun_m", [this]() {

	});
	script("stand_hitstun_h", [this]() {

	});
	script("crouch_hitstun_l", [this]() {

	});
	script("crouch_hitstun_m", [this]() {

	});
	script("crouch_hitstun_h", [this]() {

	});
	script("jump_hitstun", [this]() {

	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {

	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {

	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {

	});
	script("landing_hitstun", [this]() {

	});
}

void Atlas::chara_main() {}

void Atlas::chara_status() {
	(this->*atlas_status[status_kind - CHARA_STATUS_MAX])();
}

void Atlas::chara_enter_status() {
	(this->*atlas_enter_status[status_kind - CHARA_STATUS_MAX])();
}

void Atlas::chara_exit_status() {
	(this->*atlas_exit_status[status_kind - CHARA_STATUS_MAX])();
}

bool Atlas::specific_ground_status_act() {
	return false;
}

bool Atlas::specific_air_status_act() {
	return false;
}

bool Atlas::specific_status_attack() {
	if (chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (situation_kind == CHARA_SITUATION_GROUND && specific_ground_status_act()) {
			return true;
		}
		else if (situation_kind == CHARA_SITUATION_AIR && specific_air_status_act()) {
			return true;
		}
	}
	return false;
}