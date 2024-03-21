#include "Rowan.h"
#include "RowanFireball.h"

bool Rowan::chara_ground_status_act() {
	return false;
}

bool Rowan::chara_status_attack() {
	return false;
}

bool Rowan::chara_status_attack_air() {
	return false;
}

void Rowan::rowan_status_special_fireball_start() {
	if (projectiles[0]->active) {
		if (frame >= get_param_int("special_fireball_transition_frame_punch")) {
			if (check_button_input(BUTTON_LP)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_MP)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_HP)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
		}
		if (frame >= get_param_int("special_fireball_transition_frame_kick")) {
			if (check_button_input(BUTTON_LK)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
				return;
			}
			if (check_button_input(BUTTON_MK)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
				return;
			}
			if (check_button_input(BUTTON_HK)) {
				fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
				return;
			}
		}
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_start() {
	switch (fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
		case (SPECIAL_LEVEL_L):
		case (SPECIAL_LEVEL_EX): {
			change_anim("special_fireball_start");
		} break;
		case (SPECIAL_LEVEL_M): {
			change_anim("special_fireball_start", 0.8);
		} break;
		case (SPECIAL_LEVEL_H): {
			change_anim("special_fireball_start", 0.3);
		} break;
	}
}

void Rowan::rowan_exit_status_special_fireball_start() {

}

void Rowan::rowan_status_special_fireball_punch() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 1.0);
}

void Rowan::rowan_exit_status_special_fireball_punch() {

}

void Rowan::rowan_status_special_fireball_kick() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_kick() {
	change_anim("special_fireball_kick", 1.0);
}

void Rowan::rowan_exit_status_special_fireball_kick() {

}

void Rowan::rowan_status_special_slide() {
	Fighter* that = object_manager->fighter[!id];
	if (is_status_end()) {
		return;
	}
	if (!fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		if (frame >= get_param_int("special_slide_move_start_frame") && frame < get_param_int("special_slide_move_stop_frame")) {
			add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0.0, 0.0));
		}
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			if (frame >= get_param_int("special_slide_transition_frame")) {
				change_status(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP);
			}
		}
	}
}

void Rowan::rowan_enter_status_special_slide() {
	change_anim("special_slide");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_slide_move_x") * facing_dir;
}

void Rowan::rowan_exit_status_special_slide() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Rowan::rowan_status_special_slide_followup() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_slide_followup() {
	change_anim("special_slide_followup");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Rowan::rowan_exit_status_special_slide_followup() {

}

void Rowan::rowan_status_special_uppercut_start() {
	if (!fighter_int[FIGHTER_INT_HITLAG_FRAMES] && (fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || 
		(frame >= get_param_int("special_uppercut_transition_frame") 
			&& !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED]))
		) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_uppercut_start() {
	switch (fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
		case (SPECIAL_LEVEL_L):
		case (SPECIAL_LEVEL_EX): {
			change_anim("special_uppercut_start");
		} break;
		case (SPECIAL_LEVEL_M):
		case (SPECIAL_LEVEL_H): {
			change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		} break;
		default: {
		} break;
	}
}

void Rowan::rowan_exit_status_special_uppercut_start() {

}

void Rowan::rowan_status_special_uppercut() {
	fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	if (anim_end) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > 4.0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			change_situation(FIGHTER_SITUATION_AIR);
			add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
			if (check_landing()) {
				return;
			}
		}
	}
	else {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));
			if (frame == 4.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] /= 2.3;
			}
		}
	}
}

void Rowan::rowan_enter_status_special_uppercut() {
	fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = false;
	if (situation_kind == FIGHTER_SITUATION_GROUND) { //Not sure if we want air dp to be a thing but if we do, this is designed to account for it
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			change_anim("special_uppercut", 1.0, 1.0);
		}
		else {
			change_anim("special_uppercut", 1.0, 1.0);
		}
	}
	else {
		change_anim("special_uppercut", 1.0, 4.0);
	}

	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x") * facing_dir * 2.3;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Rowan::rowan_exit_status_special_uppercut() {

}

void Rowan::rowan_status_special_uppercut_fall() {
	if (check_landing(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND)) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
}

void Rowan::rowan_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
}

void Rowan::rowan_exit_status_special_uppercut_fall() {
	fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = false;
}

void Rowan::rowan_status_special_uppercut_land() {
	if (fighter_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] == 0) {
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
			fighter_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
		}

	}
}

void Rowan::rowan_enter_status_special_uppercut_land() {
	landing_crossup();
	fighter_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = get_param_int_special("special_uppercut_landing_lag");
	change_anim("special_uppercut_land", fighter_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	change_situation(FIGHTER_SITUATION_GROUND);
	pos.y = 0.0f;
}

void Rowan::rowan_exit_status_special_uppercut_land() {

}

void Rowan::rowan_status_special_install() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_install() {
	change_anim("special_install");
}

void Rowan::rowan_exit_status_special_install() {

}

void Rowan::load_chara_status_scripts() {
	status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_status_special_fireball_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_enter_status_special_fireball_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_exit_status_special_fireball_start);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_status_special_fireball_punch);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_enter_status_special_fireball_punch);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_exit_status_special_fireball_punch);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_status_special_fireball_kick);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_enter_status_special_fireball_kick);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_exit_status_special_fireball_kick);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_status_special_slide);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_enter_status_special_slide);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_exit_status_special_slide);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_status_special_slide_followup);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_enter_status_special_slide_followup);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_exit_status_special_slide_followup);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_status_special_uppercut_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_enter_status_special_uppercut_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_exit_status_special_uppercut_start);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_status_special_uppercut);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_enter_status_special_uppercut);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_exit_status_special_uppercut);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_status_special_uppercut_fall);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_enter_status_special_uppercut_fall);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_exit_status_special_uppercut_fall);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_status_special_uppercut_land);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_enter_status_special_uppercut_land);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_exit_status_special_uppercut_land);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_status_special_install);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_enter_status_special_install);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_exit_status_special_install);
}