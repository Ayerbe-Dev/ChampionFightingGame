#include "Eric.h"
#include "EricFireball.h"

bool Eric::chara_ground_status_act() {
	return false;
}

bool Eric::chara_status_attack() {
	return false;
}

bool Eric::chara_status_attack_air() {
	return false;
}

void Eric::eric_status_special_fireball_start() {
	if (frame >= get_param_int("special_fireball_transition_frame")) {
		if (projectiles[0]->active) {
			if (check_button_input(BUTTON_LP)) {
				object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
				change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_MP)) {
				object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
				change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_HP)) {
				object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
				change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
		}
		if (check_button_input(BUTTON_LK)) {
			object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			object_int[CHARA_ERIC_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Eric::eric_enter_status_special_fireball_start() {
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_anim("special_fireball_start");
		change_script("special_fireball_start_l");
	} break;
	case (SPECIAL_LEVEL_M): {
		change_anim("special_fireball_start", 0.8);
		change_script("special_fireball_start_m");
	} break;
	case (SPECIAL_LEVEL_H): {
		change_anim("special_fireball_start", 0.3);
		change_script("special_fireball_start_h");
	} break;
	case (SPECIAL_LEVEL_EX): {
		change_anim("special_fireball_start");
		change_script("special_fireball_start_ex");
	} break;
	}
}

void Eric::eric_exit_status_special_fireball_start() {

}

void Eric::eric_status_special_fireball_punch() {
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch");
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_script("special_fireball_punch_l");
	} break;
	case (SPECIAL_LEVEL_M): {
		change_script("special_fireball_punch_m");
	} break;
	case (SPECIAL_LEVEL_H):
	case (SPECIAL_LEVEL_EX): {
		change_script("special_fireball_punch_h");
	} break;
	}
}

void Eric::eric_exit_status_special_fireball_punch() {

}

void Eric::eric_status_special_fireball_kick() {
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_special_fireball_kick() {
	change_anim("special_fireball_kick");
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_script("special_fireball_kick_l");
	} break;
	case (SPECIAL_LEVEL_M): {
		change_script("special_fireball_kick_m");
	} break;
	case (SPECIAL_LEVEL_H):
	case (SPECIAL_LEVEL_EX): {
		change_script("special_fireball_kick_h");
	} break;
	}
}

void Eric::eric_exit_status_special_fireball_kick() {

}

void Eric::eric_status_special_slide() {
	Fighter* that = object_manager->fighter[!id];
	if (is_status_end()) {
		return;
	}
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		if (frame >= get_param_int("special_slide_move_start_frame") && frame < get_param_int("special_slide_move_stop_frame")) {
			add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0.0, 0.0));
		}
		if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			int trans_frame = (object_flag[FIGHTER_FLAG_ATTACK_HIT] || object_flag[FIGHTER_FLAG_ATTACK_BLOCKED])
				? get_param_int("special_slide_transition_frame_contact") : get_param_int("special_slide_transition_frame");
			if (frame >= trans_frame) {
				change_status(CHARA_ERIC_STATUS_SPECIAL_SLIDE_FOLLOWUP);
			}
		}
	}
}

void Eric::eric_enter_status_special_slide() {
	change_anim("special_slide");
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_script("special_slide_l");
	} break;
	case (SPECIAL_LEVEL_M): {
		change_script("special_slide_m");
	} break;
	case (SPECIAL_LEVEL_H): {
		change_script("special_slide_h");
	} break;
	case (SPECIAL_LEVEL_EX): {
		change_script("special_slide_ex");
	} break;
	}
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float_special("special_slide_move_x") * facing_dir;
}

void Eric::eric_exit_status_special_slide() {
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Eric::eric_status_special_slide_followup() {
	if (frame >= get_param_int("special_slide_transition_frame_enhanced_start")
		&& frame < get_param_int("special_slide_transition_frame_enhanced_end")
		&& object_flag[CHARA_ERIC_FLAG_INSTALL_ACTIVE]
		&& (check_button_on(BUTTON_LK) || check_button_on(BUTTON_MK) || check_button_on(BUTTON_HK))) {
		change_status(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED_START);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_special_slide_followup() {
	change_anim("special_slide_followup");
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L):
	case (SPECIAL_LEVEL_M):
	case (SPECIAL_LEVEL_H): {
		change_script("special_slide_followup_l");
	} break;
	case (SPECIAL_LEVEL_EX): {
		change_script("special_slide_followup_ex");
	} break;
	}
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Eric::eric_exit_status_special_slide_followup() {

}

void Eric::eric_status_special_slide_enhanced_start() {
	if (is_status_end(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED)) {
		return;
	}
}

void Eric::eric_enter_status_special_slide_enhanced_start() {
	change_anim("special_slide_enhanced_start");
	change_script("special_slide_enhanced_start");
}

void Eric::eric_exit_status_special_slide_enhanced_start() {
	object_flag[CHARA_ERIC_FLAG_INSTALL_ACTIVE] = false;
}


void Eric::eric_status_special_slide_enhanced() {
	common_air_status_general();
	if (is_status_end(FIGHTER_STATUS_FALL)) {
		return;
	}
}

void Eric::eric_enter_status_special_slide_enhanced() {
	change_anim("special_slide_enhanced");
	change_script("special_slide_enhanced");
	change_context(FIGHTER_CONTEXT_AIR);
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("special_slide_enhanced_x") * facing_dir;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float("special_slide_enhanced_y");
	object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = get_param_float("special_slide_enhanced_gravity");
	object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = get_param_float("special_slide_enhanced_max_fall");
}

void Eric::eric_exit_status_special_slide_enhanced() {

}

void Eric::eric_status_special_uppercut_start() {
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] && (object_flag[FIGHTER_FLAG_ATTACK_HIT] ||
		(frame >= get_param_int("special_uppercut_transition_frame")
			&& !object_flag[FIGHTER_FLAG_ATTACK_BLOCKED]))
		) {
		change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_special_uppercut_start() {
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_anim("special_uppercut_start");
		change_script("special_uppercut_start_l");
	} break;
	case (SPECIAL_LEVEL_M):
	case (SPECIAL_LEVEL_H): {
		change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT);
	} break;
	case (SPECIAL_LEVEL_EX): {
		change_anim("special_uppercut_start");
		change_script("special_uppercut_start_ex");
	} break;
	default: {
	} break;
	}
}

void Eric::eric_exit_status_special_uppercut_start() {

}

void Eric::eric_status_special_uppercut() {
	object_flag[CHARA_ERIC_FLAG_BLAZING_UPPER_HIT] = object_flag[FIGHTER_FLAG_ATTACK_HIT] || object_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	if (anim_end) {
		change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > get_param_int("special_uppercut_move_frame")) {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			change_context(FIGHTER_CONTEXT_AIR);
			add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
			if (check_landing()) {
				return;
			}
		}
	}
	else {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
			if (frame == 4.0) {
				object_float[BATTLE_OBJECT_FLOAT_X_SPEED] /= 2.3;
			}
		}
	}
}

void Eric::eric_enter_status_special_uppercut() {
	object_flag[CHARA_ERIC_FLAG_BLAZING_UPPER_HIT] = false;
	change_anim("special_uppercut");
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
	case (SPECIAL_LEVEL_L): {
		change_script("special_uppercut_l");
	} break;
	case (SPECIAL_LEVEL_M): {
		change_script("special_uppercut_m");
	} break;
	case (SPECIAL_LEVEL_H): {
		change_script("special_uppercut_h");
	} break;
	case (SPECIAL_LEVEL_EX): {
		change_script("special_uppercut_ex");
	} break;
	}

	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float_special("special_uppercut_x") * facing_dir * 2.3;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Eric::eric_exit_status_special_uppercut() {

}

void Eric::eric_status_special_uppercut_fall() {
	if (check_landing(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_LAND)) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
}

void Eric::eric_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	change_script("special_uppercut_fall");
}

void Eric::eric_exit_status_special_uppercut_fall() {
	object_flag[CHARA_ERIC_FLAG_BLAZING_UPPER_HIT] = false;
}

void Eric::eric_status_special_uppercut_land() {
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] == 0) {
		if (common_ground_status_act()) {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
			return;
		}
		else if (anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
	}
	else {
		if (object_manager->is_allow_realtime_process(this)) {
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
		}

	}
}

void Eric::eric_enter_status_special_uppercut_land() {
	landing_crossup();
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = get_param_int_special("special_uppercut_landing_lag");
	change_anim("special_uppercut_land", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1);
	change_script("special_uppercut_land");
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0;
	change_context(FIGHTER_CONTEXT_GROUND);
	set_pos_y(0.0);
}

void Eric::eric_exit_status_special_uppercut_land() {

}

void Eric::eric_status_special_install() {
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_special_install() {
	change_anim("special_install");
	change_script("special_install");
}

void Eric::eric_exit_status_special_install() {

}

void Eric::eric_status_champion_super_start() {
	if (object_flag[FIGHTER_FLAG_ATTACK_HIT] && !object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		change_status(CHARA_ERIC_STATUS_CHAMPION_SUPER);
	}
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_champion_super_start() {
	object_float[FIGHTER_FLOAT_SUPER_METER] = 0.0f;
	change_anim("champion_super_start");
	change_script("champion_super_start");
}

void Eric::eric_exit_status_champion_super_start() {

}

void Eric::eric_status_champion_super() {
	if (is_status_end()) {
		return;
	}
}

void Eric::eric_enter_status_champion_super() {
	change_anim("champion_super");
	change_script("champion_super");
}

void Eric::eric_exit_status_champion_super() {

}

void Eric::load_chara_status_scripts() {
	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_START, &Eric::eric_status_special_fireball_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_START, &Eric::eric_enter_status_special_fireball_start);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_START, &Eric::eric_exit_status_special_fireball_start);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH, &Eric::eric_status_special_fireball_punch);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH, &Eric::eric_enter_status_special_fireball_punch);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH, &Eric::eric_exit_status_special_fireball_punch);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK, &Eric::eric_status_special_fireball_kick);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK, &Eric::eric_enter_status_special_fireball_kick);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK, &Eric::eric_exit_status_special_fireball_kick);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE, &Eric::eric_status_special_slide);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE, &Eric::eric_enter_status_special_slide);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE, &Eric::eric_exit_status_special_slide);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Eric::eric_status_special_slide_followup);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Eric::eric_enter_status_special_slide_followup);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Eric::eric_exit_status_special_slide_followup);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Eric::eric_status_special_slide_enhanced_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Eric::eric_enter_status_special_slide_enhanced_start);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Eric::eric_exit_status_special_slide_enhanced_start);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED, &Eric::eric_status_special_slide_enhanced);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED, &Eric::eric_enter_status_special_slide_enhanced);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED, &Eric::eric_exit_status_special_slide_enhanced);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_status_special_uppercut_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_enter_status_special_uppercut_start);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_exit_status_special_uppercut_start);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT, &Eric::eric_status_special_uppercut);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT, &Eric::eric_enter_status_special_uppercut);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT, &Eric::eric_exit_status_special_uppercut);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL, &Eric::eric_status_special_uppercut_fall);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL, &Eric::eric_enter_status_special_uppercut_fall);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL, &Eric::eric_exit_status_special_uppercut_fall);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_LAND, &Eric::eric_status_special_uppercut_land);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_LAND, &Eric::eric_enter_status_special_uppercut_land);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_LAND, &Eric::eric_exit_status_special_uppercut_land);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_INSTALL, &Eric::eric_status_special_install);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_INSTALL, &Eric::eric_enter_status_special_install);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_SPECIAL_INSTALL, &Eric::eric_exit_status_special_install);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER_START, &Eric::eric_status_champion_super_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER_START, &Eric::eric_enter_status_champion_super_start);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER_START, &Eric::eric_exit_status_champion_super_start);

	SET_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER, &Eric::eric_status_champion_super);
	SET_ENTRY_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER, &Eric::eric_enter_status_champion_super);
	SET_EXIT_STATUS_FUNC(CHARA_ERIC_STATUS_CHAMPION_SUPER, &Eric::eric_exit_status_champion_super);
}