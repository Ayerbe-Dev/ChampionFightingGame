#include "Roy.h"
#include "Game.h"
#include "RoyFireball.fwd.h"
#include "RoyFireball.h"

Roy::Roy() {

}

Roy::Roy(SDL_Renderer *renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/chara/roy";
	superInit(id, renderer);
	load_unique_params();
	loadRoyACMD();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ROY;
	this->base_texture = loadTexture("resource/chara/roy/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}

	IObject* roy_fireball = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_ROY_FIREBALL, renderer, id, fighter_instance_accessor);
	this->projectile_objects[0] = roy_fireball->get_projectile();
	RoyFireball* roy_fireball_instance = (RoyFireball*)projectile_objects[0];
	roy_fireball_instance->roy = this;
}

void Roy::chara_id() {

}

void Roy::loadRoyStatusFunctions() {
	pStatus[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_status_special_fireball_start;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_enter_status_special_fireball_start;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_exit_status_special_fireball_start;

	pStatus[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_status_special_fireball_punch;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_enter_status_special_fireball_punch;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_exit_status_special_fireball_punch;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_status_special_uppercut_start;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_enter_status_special_uppercut_start;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_exit_status_special_uppercut_start;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_status_special_uppercut;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_enter_status_special_uppercut;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_exit_status_special_uppercut;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_status_special_uppercut_fall;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_enter_status_special_uppercut_fall;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_exit_status_special_uppercut_fall;
}

void Roy::loadRoyACMD() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0, true);
		}
		if (is_excute_frame(4)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0, true);
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
	});
	script("stand_hp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 50, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0, true);
		}
		if (is_excute_frame(10)) {
			new_hitbox(1, 50, 5, 1.2, 1, 0, GameCoordinate{ -5,70 }, GameCoordinate{ 60, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(2)) {
			new_hitbox(2, 60, 10, 1.2, 1, 0, GameCoordinate{ 50,70 }, GameCoordinate{ 70, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 20, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(3)) {
			clear_hitbox_all();
		}
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 0, 90 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(2)) {
			new_hurtbox(1, GameCoordinate{ -15, 100 }, GameCoordinate{ 25, 60 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 50, 10, 1.2, 1, 0, GameCoordinate{ 25,60 }, GameCoordinate{ 60, 120 }, HITBOX_KIND_NORMAL, 20, 40, 10, SITUATION_HIT_GROUND_AIR, 4, 15, 12, 70, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 3, 3, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 20, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_AERIAL, 20.0, 0.0, 2.0, 1.0, true);
		}
		if (is_excute_frame(8)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(12)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
	});
	script("crouch_lp", [this]() {

	});
	script("crouch_mp", [this]() {

	});
	script("crouch_hp", [this]() {
		if (is_excute_frame(9)) {
			new_hitbox(1, 50, 5, 1.2, 1, 0, GameCoordinate{ 25,75 }, GameCoordinate{ -10, 140 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NONE, 14.0, 0.0, 0.0, 1.0, true);
		}
	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		if (is_excute_frame(3)) {

		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
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
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(1)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(4)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
	script("hitstun_parry", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		}
		if (is_excute_wait(2)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
		if (is_excute_wait(9)) {
			reenter_last_anim();
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = false;
		}
	});
	script("grab", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW, CHARA_STATUS_GRABBED);
		}
	});
	script("grab_air", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_HITSTUN, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW_AIR, CHARA_STATUS_GRABBED);
		}
	});
	script("throw_f", [this]() {
		if (is_excute_frame(0)) {
			set_opponent_offset(GameCoordinate{ 40, 0 }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(13)) {
			damage_opponent(30.0, 2.0, 6.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("special_fireball_start", [this]() {
		if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
			if (is_excute_frame(11)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
			if (is_excute_frame(9)) {
				max_ticks++;
			}
			if (is_excute_frame(14)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
			if (is_excute_frame(8)) {
				max_ticks++;
			}
			if (is_excute_frame(15)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else {
			if (is_excute_frame(11)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
	});
	script("special_fireball_punch", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(12)) {
			projectile_objects[0]->change_status(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED);
		}
	});
	script("special_uppercut_start", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(5)) {
			max_ticks = 2;
		}
	});
}

bool Roy::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}

	if (get_special_input(SPECIAL_KIND_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}

	return false;
}

bool Roy::specific_status_attack() {
	if (chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Roy::roy_status_special_fireball_start() {
	int trans_frame = 0;
	if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		trans_frame = get_param_int("special_fireball_transition_frame_l", unique_param_table);
	}
	else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		trans_frame = get_param_int("special_fireball_transition_frame_m", unique_param_table);
	}
	else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		trans_frame = get_param_int("special_fireball_transition_frame_h", unique_param_table);
	}
	else {
		trans_frame = get_param_int("special_fireball_transition_frame_ex", unique_param_table);
	}
	if (frame >= trans_frame) {
		if (check_button_input(BUTTON_LP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_MP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_HP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}	
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
}

void Roy::roy_enter_status_special_fireball_start() {
	if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		change_anim("special_fireball_start", 2);
	}
	else {
		change_anim("special_fireball_start", 3);
	}
}

void Roy::roy_exit_status_special_fireball_start() {

}


void Roy::roy_status_special_fireball_punch() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void Roy::roy_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 2);
}

void Roy::roy_exit_status_special_fireball_punch() {

}

void Roy::roy_status_special_uppercut_start() {
	if (frame >= get_param_int("special_uppercut_transition_frame", unique_param_table) && !chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS]) {
		change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		return;
	}
}

void Roy::roy_enter_status_special_uppercut_start() {
	change_anim("special_uppercut_start", 1);
}

void Roy::roy_exit_status_special_uppercut_start() {

}

void Roy::roy_status_special_uppercut() {
	if (is_anim_end) {
		change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("special_uppercut_fall_speed", unique_param_table) * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("special_uppercut_gravity", unique_param_table);
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}

void Roy::roy_enter_status_special_uppercut() {
	situation_kind = CHARA_SITUATION_AIR;
	change_anim("special_uppercut");
	if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("special_uppercut_x_l", unique_param_table);
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("special_uppercut_init_y_l", unique_param_table);
	}
	else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("special_uppercut_x_m", unique_param_table);
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("special_uppercut_init_y_m", unique_param_table);
	}
	else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("special_uppercut_x_h", unique_param_table);
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("special_uppercut_init_y_h", unique_param_table);
	}
	else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("special_uppercut_x_ex", unique_param_table);
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("special_uppercut_init_y_ex", unique_param_table);
	}
}

void Roy::roy_exit_status_special_uppercut() {

}

void Roy::roy_status_special_uppercut_fall() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("special_uppercut_fall_speed", unique_param_table) * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("special_uppercut_gravity", unique_param_table);
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}
void Roy::roy_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Roy::roy_exit_status_special_uppercut_fall() {

}