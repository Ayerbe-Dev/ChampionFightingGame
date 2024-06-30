#include "Fighter.h"
#include "ParamAccessor.h"
#include "utils.h"

int Fighter::get_frames_until_actionable() {
	if (anim_kind == nullptr) {
		return 0;
	}
	else {
		float target_frame;

		if (anim_kind->faf == -1) {
			target_frame = anim_kind->length;
		}
		else {
			target_frame = anim_kind->faf;
		}

		int ret = 0;
		float sim_rate = rate * object_manager->get_world_rate(this);

		//TODO: This block assumes that our motion rate is constant, when we have the ability to change
		//it midway through. We should figure out a way to read through a script and check if it
		//will include changing the motion rate.

		//Update: I hope past Henry knows the bullshit he put me through to make the following possible.

		for (float sim_frame = frame + sim_rate; sim_frame < target_frame; sim_frame += sim_rate) {
			ScriptArg args;
			if (active_move_script.has_function(sim_frame, &BattleObject::SET_RATE, &args)) {
				UNWRAP_NO_DECL(sim_rate);
				sim_rate *= object_manager->get_world_rate(this);
			}
			if (active_move_script.has_function(sim_frame, (void (BattleObject::*)(ScriptArg)) &Fighter::SET_FLAG, &args)) {
				UNWRAP(flag, int);
				UNWRAP(val, bool);
				if ((flag == FIGHTER_FLAG_ALLOW_FREE_CANCEL) && val) {
					break;
				}
			}
			ret++;
		}

		//TODO: This block assumes that your air speed is completely constant, which is rarely going 
		//to be the case, so we'll have to figure out a proper way to handle that

		if (fighter_context == FIGHTER_CONTEXT_AIR && get_pos().y + (object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] * (ret - 1)) <= 0.0f) {
			for (; get_pos().y + (object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] * (ret - 1)) <= 0.0f; ret--);
			if (object_string[FIGHTER_STRING_MOVE_KIND] == "") {
				ret += get_param_int(object_string[FIGHTER_STRING_MOVE_KIND] + "empty_landing_lag");
			}
			else {
				ret += get_param_int(object_string[FIGHTER_STRING_MOVE_KIND] + "_landing_lag");
			}
		}

		return ret;
	}
}

int Fighter::calc_launch_frames() {
	float sim_gravity = object_float[FIGHTER_FLOAT_CURRENT_GRAVITY];
	if (object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] == 0 || object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] == 0) {
		return 1;
	}
	int airtime = 0;
	float simp_y = get_pos().y;
	float sims_y = object_float[BATTLE_OBJECT_FLOAT_Y_SPEED];
	while (simp_y >= 0.0f) {
		sims_y = clampf(object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] * -1,
			sims_y - object_float[FIGHTER_FLOAT_CURRENT_GRAVITY], sims_y);
		simp_y += sims_y;
		airtime++;
	}
	return airtime;
}