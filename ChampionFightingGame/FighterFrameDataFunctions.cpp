#include "Fighter.h"
#include "ParamAccessor.h"

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
				if ((flag == FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY) && val) {
					break;
				}
			}
			ret++;
		}

		//TODO: This block assumes that your air speed is completely constant, which is rarely going 
		//to be the case, so we'll have to figure out a proper way to handle that

		if (situation_kind == FIGHTER_SITUATION_AIR && pos.y + (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] * (ret - 1)) <= 0.0f) {
			for (; pos.y + (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] * (ret - 1)) <= 0.0f; ret--);
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_LP): {
					ret += get_local_param_int("lp_landing_lag");
				} break;
				case (ATTACK_KIND_MP): {
					ret += get_local_param_int("mp_landing_lag");
				} break;
				case (ATTACK_KIND_HP): {
					ret += get_local_param_int("hp_landing_lag");
				} break;
				case (ATTACK_KIND_LK): {
					ret += get_local_param_int("lk_landing_lag");
				} break;
				case (ATTACK_KIND_MK): {
					ret += get_local_param_int("mk_landing_lag");
				} break;
				case (ATTACK_KIND_HK): {
					ret += get_local_param_int("hk_landing_lag");
				} break;
				default: {
					ret += get_local_param_int("empty_landing_lag");
				} break;
			}
		}

		return ret;
	}
}