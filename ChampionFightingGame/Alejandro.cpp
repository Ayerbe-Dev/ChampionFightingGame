#include "Alejandro.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Alejandro::Alejandro() {

}

Alejandro::Alejandro(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/alejandro";
	chara_name = "alejandro";
	if (!crash_to_debug) {
		load_params();
	}
	loadAlejandroACMD();
	loadAlejandroStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ALEJANDRO;
	this->base_texture = loadTexture("resource/chara/alejandro/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}
}

void Alejandro::chara_id() {

}

void Alejandro::loadAlejandroStatusFunctions() {

}

void Alejandro::loadAlejandroACMD() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {

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

void Alejandro::chara_main() {}

void Alejandro::chara_status() {
	(this->*alejandro_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Alejandro::chara_enter_status() {
	(this->*alejandro_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Alejandro::chara_exit_status() {
	(this->*alejandro_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}

bool Alejandro::specific_ground_status_act() {
	return false;
}

bool Alejandro::specific_air_status_act() {
	return false;
}

bool Alejandro::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}