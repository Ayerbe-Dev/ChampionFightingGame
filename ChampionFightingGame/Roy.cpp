#include "Roy.h"
#include "Game.h"

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
}

void Roy::chara_id() {

}

void Roy::loadRoyStatusFunctions() {
	pStatus[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_status_fireball_start;
	pEnter_status[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_enter_status_fireball_start;
	pExit_status[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_exit_status_fireball_start;

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
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hitbox(0, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 4)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
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
			new_hitbox(0, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 6)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 60, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
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
			new_hitbox(0, 50, 5, 1.2, 1, 0, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 0.0, 0.0, 0.0, 0.0);
		}
		if (is_excute_frame(2, 10)) {
			new_hitbox(1, 50, 5, 1.2, 1, 0, GameCoordinate{ -5,70 }, GameCoordinate{ 60, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(3, 2)) {
			new_hitbox(2, 60, 10, 1.2, 1, 0, GameCoordinate{ 50,70 }, GameCoordinate{ 70, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
		if (is_excute_wait(4, 3)) {
			clear_hitbox_all();
		}
	});
	script("stand_lk", [this]() {
		if (is_excute_frame(1, 0)) {
			init_projectile(0, GameCoordinate{ 0,50 });
		}
	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 0, 90 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(2, 2)) {
			new_hurtbox(1, GameCoordinate{ -15, 100 }, GameCoordinate{ 25, 60 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(3, 6)) {
			new_hitbox(1, 50, 10, 1.2, 1, 0, GameCoordinate{ 25,60 }, GameCoordinate{ 60, 120 }, HITBOX_KIND_NORMAL, 20, 40, 10, SITUATION_HIT_GROUND_AIR, 4, 15, 12, 70, true, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 3, 3, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 20, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_AERIAL, 20.0, 0.0, 2.0, 1.0);
		}
		if (is_excute_frame(4, 8)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(5, 12)) {
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

	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		if (is_excute_frame(1, 3)) {

		}
		if (is_excute_frame(2, 6)) {
			new_hitbox(1, 30, 5, 1.2, 1, 0, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
	});
	script("jump_lp", [this]() {

	});
	script("jump_mp", [this]() {

	});
	script("jump_hp", [this]() {

	});
	script("jump_lk", [this]() {
		if (is_excute_frame(1, 0)) {
			init_projectile(0, GameCoordinate{ 0,50 });
		}
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
		if (is_excute_wait(3, 4)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
	script("grab", [this]() {
		if (is_excute_frame(1, 0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}
		if (is_excute_frame(2, 3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_WAIT, CHARA_STATUS_KNOCKDOWN_START);
		}
	});
	script("special_uppercut_start", [this]() {
		if (is_excute_frame(1, 0)) {

		}
		if (is_excute_frame(2, 5)) {
			max_ticks = 2;
		}
	});
}

bool Roy::specific_ground_status_act() {
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

void Roy::roy_status_fireball_start() {

}

void Roy::roy_enter_status_fireball_start() {

}

void Roy::roy_exit_status_fireball_start() {

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