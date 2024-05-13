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
	if (frame >= get_param_int("special_fireball_transition_frame")) {
		if (projectiles[0]->active) {	
			if (check_button_input(BUTTON_LP)) {
				object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_MP)) {
				object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
			if (check_button_input(BUTTON_HP)) {
				object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
				change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
				return;
			}
		}
		if (check_button_input(BUTTON_LK)) {
			object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_start() {
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
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
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		if (frame >= get_param_int("special_slide_move_start_frame") && frame < get_param_int("special_slide_move_stop_frame")) {
			add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0.0, 0.0));
		}
		if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			int trans_frame = (object_flag[FIGHTER_FLAG_ATTACK_HIT] || object_flag[FIGHTER_FLAG_ATTACK_BLOCKED]) 
				? get_param_int("special_slide_transition_frame_contact") : get_param_int("special_slide_transition_frame");
			if (frame >= trans_frame) {
				change_status(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP);
			}
		}
	}
}

void Rowan::rowan_enter_status_special_slide() {
	change_anim("special_slide");
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float_special("special_slide_move_x") * facing_dir;
}

void Rowan::rowan_exit_status_special_slide() {
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Rowan::rowan_status_special_slide_followup() {
	if (frame >= get_param_int("special_slide_transition_frame_enhanced_start")
	&& frame < get_param_int("special_slide_transition_frame_enhanced_end")
	&& object_flag[CHARA_ROWAN_FLAG_INSTALL_ACTIVE]
	&& (check_button_on(BUTTON_LK) || check_button_on(BUTTON_MK) || check_button_on(BUTTON_HK))) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED_START);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_slide_followup() {
	change_anim("special_slide_followup");
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Rowan::rowan_exit_status_special_slide_followup() {

}

void Rowan::rowan_status_special_slide_enhanced_start() {
	if (is_status_end(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED)) {
		return;
	}
}

void Rowan::rowan_enter_status_special_slide_enhanced_start() {
	change_anim("special_slide_enhanced_start");
}

void Rowan::rowan_exit_status_special_slide_enhanced_start() {
	object_flag[CHARA_ROWAN_FLAG_INSTALL_ACTIVE] = false;
}


void Rowan::rowan_status_special_slide_enhanced() {
	common_air_status_general();
	if (is_status_end(FIGHTER_STATUS_FALL)) {
		return;
	}
}

void Rowan::rowan_enter_status_special_slide_enhanced() {
	change_anim("special_slide_enhanced");
	change_context(FIGHTER_CONTEXT_AIR);
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("special_slide_enhanced_x") * facing_dir;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float("special_slide_enhanced_y");
	object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = get_param_float("special_slide_enhanced_gravity");
	object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = get_param_float("special_slide_enhanced_max_fall");
}

void Rowan::rowan_exit_status_special_slide_enhanced() {

}

void Rowan::rowan_status_special_uppercut_start() {
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] && (object_flag[FIGHTER_FLAG_ATTACK_HIT] || 
		(frame >= get_param_int("special_uppercut_transition_frame") 
			&& !object_flag[FIGHTER_FLAG_ATTACK_BLOCKED]))
		) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_uppercut_start() {
	switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
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
	object_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = object_flag[FIGHTER_FLAG_ATTACK_HIT] || object_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	if (anim_end) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > get_param_int("special_uppercut_move_frame")) {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			change_context(FIGHTER_CONTEXT_AIR);
			add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
			if (check_landing()) {
				return;
			}
		}
	}
	else {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
			add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
			if (frame == 4.0) {
				object_float[BATTLE_OBJECT_FLOAT_X_SPEED] /= 2.3;
			}
		}
	}
}

void Rowan::rowan_enter_status_special_uppercut() {
	object_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = false;
	if (fighter_context == FIGHTER_CONTEXT_GROUND) { //Not sure if we want air dp to be a thing but if we do, this is designed to account for it
		if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			change_anim("special_uppercut", 1.0, 1.0);
		}
		else {
			change_anim("special_uppercut", 1.0, 1.0);
		}
	}
	else {
		change_anim("special_uppercut", 1.0, 4.0);
	}

	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float_special("special_uppercut_x") * facing_dir * 2.3;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Rowan::rowan_exit_status_special_uppercut() {

}

void Rowan::rowan_status_special_uppercut_fall() {
	if (check_landing(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND)) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
}

void Rowan::rowan_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
}

void Rowan::rowan_exit_status_special_uppercut_fall() {
	object_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT] = false;
}

void Rowan::rowan_status_special_uppercut_land() {
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

void Rowan::rowan_enter_status_special_uppercut_land() {
	landing_crossup();
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = get_param_int_special("special_uppercut_landing_lag");
	change_anim("special_uppercut_land", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1);
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0;
	change_context(FIGHTER_CONTEXT_GROUND);
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

void Rowan::rowan_status_champion_super_start() {
	if (object_flag[FIGHTER_FLAG_ATTACK_HIT] && !object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		change_status(CHARA_ROWAN_STATUS_CHAMPION_SUPER);
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_champion_super_start() {
	object_float[FIGHTER_FLOAT_SUPER_METER] = 0.0f;
	change_anim("champion_super_start");
}

void Rowan::rowan_exit_status_champion_super_start() {

}

void Rowan::rowan_status_champion_super() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_champion_super() {
	change_anim("champion_super");
}

void Rowan::rowan_exit_status_champion_super() {

}

void Rowan::load_chara_status_scripts() {
	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_status_special_fireball_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_enter_status_special_fireball_start);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_exit_status_special_fireball_start);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_status_special_fireball_punch);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_enter_status_special_fireball_punch);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_exit_status_special_fireball_punch);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_status_special_fireball_kick);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_enter_status_special_fireball_kick);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_exit_status_special_fireball_kick);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_status_special_slide);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_enter_status_special_slide);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_exit_status_special_slide);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_status_special_slide_followup);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_enter_status_special_slide_followup);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_exit_status_special_slide_followup);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Rowan::rowan_status_special_slide_enhanced_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Rowan::rowan_enter_status_special_slide_enhanced_start);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED_START, &Rowan::rowan_exit_status_special_slide_enhanced_start);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED, &Rowan::rowan_status_special_slide_enhanced);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED, &Rowan::rowan_enter_status_special_slide_enhanced);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED, &Rowan::rowan_exit_status_special_slide_enhanced);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_status_special_uppercut_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_enter_status_special_uppercut_start);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_exit_status_special_uppercut_start);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_status_special_uppercut);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_enter_status_special_uppercut);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_exit_status_special_uppercut);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_status_special_uppercut_fall);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_enter_status_special_uppercut_fall);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_exit_status_special_uppercut_fall);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_status_special_uppercut_land);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_enter_status_special_uppercut_land);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND, &Rowan::rowan_exit_status_special_uppercut_land);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_status_special_install);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_enter_status_special_install);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_SPECIAL_INSTALL, &Rowan::rowan_exit_status_special_install);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER_START, &Rowan::rowan_status_champion_super_start);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER_START, &Rowan::rowan_enter_status_champion_super_start);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER_START, &Rowan::rowan_exit_status_champion_super_start);

	SET_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER, &Rowan::rowan_status_champion_super);
	SET_ENTRY_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER, &Rowan::rowan_enter_status_champion_super);
	SET_EXIT_STATUS_FUNC(CHARA_ROWAN_STATUS_CHAMPION_SUPER, &Rowan::rowan_exit_status_champion_super);
}