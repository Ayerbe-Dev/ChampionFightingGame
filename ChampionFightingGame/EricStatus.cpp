#include "Eric.h"

bool Eric::chara_ground_status_act() {
	return false;
}

bool Eric::chara_status_attack() {
	return false;
}

bool Eric::chara_status_attack_air() {
	return false;
}

void Eric::enter_status_dash_f() {
	change_anim("dash_f");
}

void Eric::exit_status_dash_f() {
	object_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Eric::enter_status_dash_b() {
	change_anim("dash_b");
}

void Eric::exit_status_dash_b() {
	object_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Eric::eric_status_special_uppercut_start() {
	if (frame >= get_param_int("special_uppercut_transition_frame") && !object_flag[FIGHTER_FLAG_ATTACK_BLOCKED]) {
		change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
		return;
	}
}

void Eric::eric_enter_status_special_uppercut_start() {
	change_anim("special_uppercut_start", 1);
}

void Eric::eric_exit_status_special_uppercut_start() {

}

void Eric::eric_status_special_uppercut() {
	if (get_anim() == "special_uppercut") {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			change_context(FIGHTER_CONTEXT_AIR);
			add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * facing_dir, object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
			if (anim_end) {
				change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL);
				return;
			}
		}
	}
	else {
		if (anim_end) {
			change_anim("special_uppercut");
		}
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * facing_dir, 0, 0));
		}
	}
}

void Eric::eric_enter_status_special_uppercut() {
	if (fighter_context == FIGHTER_CONTEXT_GROUND) {
		change_anim("special_uppercut_rise", 2);
	}
	else {
		change_anim("special_uppercut");
	}

	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float_special("special_uppercut_x");
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Eric::eric_exit_status_special_uppercut() {

}

void Eric::eric_status_special_uppercut_fall() {
	if (check_landing()) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * facing_dir, object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
}
void Eric::eric_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Eric::eric_exit_status_special_uppercut_fall() {

}

void Eric::load_chara_status_scripts() {
	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_status_special_uppercut_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_enter_status_special_uppercut_start);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_exit_status_special_uppercut_start);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT, &Eric::eric_enter_status_special_uppercut);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT , &Eric::eric_enter_status_special_uppercut);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT , &Eric::eric_exit_status_special_uppercut);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_status_special_uppercut_fall);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_enter_status_special_uppercut_fall);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_exit_status_special_uppercut_fall);
}