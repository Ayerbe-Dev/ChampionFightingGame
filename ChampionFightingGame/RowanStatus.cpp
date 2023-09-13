#include "Rowan.h"
#include "RowanFireball.h"

void Rowan::chara_main() {

}

bool Rowan::chara_ground_status_act() {
	if (!projectiles[0]->active) {
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
		}
		else {
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		}
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_MACRO_K, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPKICK);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_LK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPKICK);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_MK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPKICK);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_HK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPKICK);
	}

	if (get_attack_input(ATTACK_KIND_OTHER, BUTTON_HP, 6)) {
		fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
		fighter_int[FIGHTER_INT_ATTACK_OTHER_KIND] = 1;
		return change_status_after_hitlag(FIGHTER_STATUS_ATTACK);
	}

	return false;
}

bool Rowan::chara_air_status_act() {
	return false;
}

bool Rowan::chara_status_attack() {
	if (!projectiles[0]->active) {
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, CANCEL_KIND_236P);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, CANCEL_KIND_236P);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, CANCEL_KIND_236P);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, CANCEL_KIND_236P);
		}
	}
	
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(false);
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, CANCEL_KIND_214K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, CANCEL_KIND_214K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, CANCEL_KIND_214K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, CANCEL_KIND_214K);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, CANCEL_KIND_623P);
		}
		else {
			return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, CANCEL_KIND_623P);
		}
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, CANCEL_KIND_623P);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, CANCEL_KIND_623P);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, CANCEL_KIND_623P);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_MACRO_K, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(false);
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, CANCEL_KIND_28K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_LK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, CANCEL_KIND_28K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_MK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, CANCEL_KIND_28K);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_28, BUTTON_HK, 45) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return buffer_status_pre_enable_cancel(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, CANCEL_KIND_28K);
	}
	
	return false;
}

void Rowan::chara_enter_status_attack_other() {
	switch (fighter_int[FIGHTER_INT_ATTACK_OTHER_KIND]) {
		default: {

		} break;
		case (1): {
			change_anim("forward_hp");
		} break;
	}
}

void Rowan::rowan_status_special_fireball_start() {
	if (frame >= get_param_int_special("special_fireball_transition_frame") && projectiles[0]->active) {
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
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_start() {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		spend_meter(2);
	}
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_236;
	change_anim("special_fireball_start", 1.0);	
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
	if (is_status_end()) {
		return;
	}
	if (!fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		if (frame >= get_local_param_int("special_slide_move_start_frame", params) && frame < get_local_param_int("special_slide_move_stop_frame", params)) {
			add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0.0, 0.0));
		}
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			if (frame >= get_local_param_int("special_slide_transition_frame", params)) {
				change_status(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP);
			}
		}
	}
}

void Rowan::rowan_enter_status_special_slide() {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		spend_meter(2);
	}
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_214;
	change_anim("special_slide");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_slide_move_x") * facing_dir;
}

void Rowan::rowan_exit_status_special_slide() {

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
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || (frame >= get_local_param_int("special_uppercut_transition_frame", params) && !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED])) {
		change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_uppercut_start() {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		spend_meter(2);
	}
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_623;
	change_anim("special_uppercut_start");
}

void Rowan::rowan_exit_status_special_uppercut_start() {

}

void Rowan::rowan_status_special_uppercut() {
	fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	if (is_anim_end) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > 4.0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			situation_kind = FIGHTER_SITUATION_AIR;
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
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_623;
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
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
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
		else if (is_anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
	}
	else {
		if (battle_object_manager->allow_dec_var(id)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
}

void Rowan::rowan_enter_status_special_uppercut_land() {
	landing_crossup();
	fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int_special("special_uppercut_landing_lag");
	change_anim("special_uppercut_land", fighter_int[FIGHTER_INT_LANDING_LAG], -1);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = 0.0f;
}

void Rowan::rowan_exit_status_special_uppercut_land() {

}

void Rowan::rowan_status_special_upkick() {
	if (is_anim_end) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > 4.0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			situation_kind = FIGHTER_SITUATION_AIR;
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

void Rowan::rowan_enter_status_special_upkick() {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		spend_meter(2);
	}
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_28;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x") * facing_dir * 2.3;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y") * 1.3;
	switch (fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
		case (SPECIAL_LEVEL_L): {
			change_anim("special_upkick_l");
		} break;
		case (SPECIAL_LEVEL_M): {
			change_anim("special_upkick_m");
		} break;
		case (SPECIAL_LEVEL_H): {
			change_anim("special_upkick_h");
		} break;
		case (SPECIAL_LEVEL_EX): {
			change_anim("special_upkick_ex");
		} break;
	}
}

void Rowan::rowan_exit_status_special_upkick() {

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

	//TODO: Delete this, Rowan isn't actually a charge character lol

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, &Rowan::rowan_status_special_upkick);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, &Rowan::rowan_enter_status_special_upkick);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPKICK, &Rowan::rowan_exit_status_special_upkick);
}