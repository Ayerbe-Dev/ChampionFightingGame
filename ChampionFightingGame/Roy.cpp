#include "Roy.h"

RoyScript::RoyScript() {};

RoyScript::RoyScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->roy_script = move_script;
	this->id = id;
}

Roy::Roy() {

}

Roy::Roy(SDL_Renderer *renderer, int id) {
	resource_dir = "resource/chara/roy";
	superInit(id, renderer);
	load_roy_params();
	loadRoyACMD();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ROY;
	this->base_texture = loadTexture("resource/chara/roy/sprite/sprite.png", renderer);
}

void Roy::chara_id() {

}

void Roy::load_roy_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/params.yml");

	if (stats_table.fail())
	{
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	int i = 0;
	string stat;
	while (stats_table >> stat) {
		roy_table[i].stat = stat;
		stats_table >> roy_table[i].type;
		switch (roy_table[i].type) {
		case(PARAM_TYPE_INT): {
			stats_table >> roy_table[i].value_i;
		} break;
		case(PARAM_TYPE_FLOAT): {
			stats_table >> roy_table[i].value_f;
		} break;
		case(PARAM_TYPE_STRING): {
			stats_table >> roy_table[i].value_s;
		} break;
		case (PARAM_TYPE_BOOL): {
			stats_table >> roy_table[i].value_b;
		} break;
		default: {
			stats_table >> roy_table[i].value_i;
		} break;
		}
		i++;
	}

	stats_table.close();
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

void Roy::loadRoyStatusFunctions() {
	pStatus[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_status_fireball_start;
	pEnter_status[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_enter_status_fireball_start;
	pExit_status[CHARA_ROY_STATUS_FIREBALL_START] = &FighterInstance::roy_exit_status_fireball_start;

	pStatus[CHARA_ROY_STATUS_UPPERCUT_START] = &FighterInstance::roy_status_uppercut_start;
	pEnter_status[CHARA_ROY_STATUS_UPPERCUT_START] = &FighterInstance::roy_enter_status_uppercut_start;
	pExit_status[CHARA_ROY_STATUS_UPPERCUT_START] = &FighterInstance::roy_exit_status_uppercut_start;

	pStatus[CHARA_ROY_STATUS_UPPERCUT] = &FighterInstance::roy_status_uppercut;
	pEnter_status[CHARA_ROY_STATUS_UPPERCUT] = &FighterInstance::roy_enter_status_uppercut;
	pExit_status[CHARA_ROY_STATUS_UPPERCUT] = &FighterInstance::roy_exit_status_uppercut;

	pStatus[CHARA_ROY_STATUS_UPPERCUT_FALL] = &FighterInstance::roy_status_uppercut_fall;
	pEnter_status[CHARA_ROY_STATUS_UPPERCUT_FALL] = &FighterInstance::roy_enter_status_uppercut_fall;
	pExit_status[CHARA_ROY_STATUS_UPPERCUT_FALL] = &FighterInstance::roy_exit_status_uppercut_fall;
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
}

bool Roy::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_ROY_STATUS_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_ROY_STATUS_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_ROY_STATUS_UPPERCUT_START);
	}

	return false;
}

void Roy::roy_status_fireball_start() {

}

void Roy::roy_enter_status_fireball_start() {

}

void Roy::roy_exit_status_fireball_start() {

}

void Roy::roy_status_uppercut_start() {
	if (frame >= get_param_int("uppercut_transition_frame", roy_table) && !chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS]) {
		change_status(CHARA_ROY_STATUS_UPPERCUT);
		return;
	}
}

void Roy::roy_enter_status_uppercut_start() {
	change_anim("uppercut_start", 30);
}

void Roy::roy_exit_status_uppercut_start() {

}

void Roy::roy_status_uppercut() {
	if (is_anim_end) {
		change_status(CHARA_ROY_STATUS_UPPERCUT_FALL);
		return;
	}
}

void Roy::roy_enter_status_uppercut() {
	change_anim("uppercut");
}

void Roy::roy_exit_status_uppercut() {

}

void Roy::roy_status_uppercut_fall() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
}
void Roy::roy_enter_status_uppercut_fall() {
	change_anim("uppercut_fall");
}

void Roy::roy_exit_status_uppercut_fall() {

}