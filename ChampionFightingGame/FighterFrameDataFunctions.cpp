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
		float sim_rate = rate * battle_object_manager->get_time_multiplier(id);

		//TODO: This block assumes that our motion rate is constant, when we have the ability to change
		//it midway through. We should figure out a way to read through a script and check if it
		//will include changing the motion rate.

		//Update: I hope past Henry knows the bullshit he put me through to make the following possible.

		for (float sim_frame = frame + sim_rate; sim_frame < target_frame; sim_frame += sim_rate) {
			ScriptArg args;
			if (active_move_script.has_function(sim_frame, &BattleObject::SET_RATE, &args)) {
				UNWRAP_NO_DECL(sim_rate);
				sim_rate *= battle_object_manager->get_time_multiplier(id);
			}
			ret++;
		}

		//TODO: This block assumes that your air speed is completely constant, which is rarely going 
		//to be the case, so we'll have to figure out a proper way to handle that

		if (situation_kind == FIGHTER_SITUATION_AIR && pos.y + (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] * ret) <= FLOOR_GAMECOORD) {
			for (; pos.y + (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] * ret) <= FLOOR_GAMECOORD; ret--);

			//TODO: Figure out how to determine how many landing lag frames will be used, add that to
			//ret
		}

		return ret;
	}
}