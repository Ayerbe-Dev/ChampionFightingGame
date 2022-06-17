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

		for (float sim_frame = frame + sim_rate; sim_frame < target_frame; sim_frame += sim_rate) {
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

bool Fighter::is_actionable() {
	if (battle_object_manager->world_rate == 0.0 && battle_object_manager->real_time_id != id) {
		return false; //Disable all actions in super freeze no matter what
	}
	if (anim_kind == nullptr) {
		return true;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0 && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && status_kind != FIGHTER_STATUS_GRABBED && status_kind != FIGHTER_STATUS_THROWN) {
		if (anim_kind->faf == -1) {
			return std::ceil(frame) >= anim_kind->length;
		}
		else {
			return std::ceil(frame) >= anim_kind->faf;
		}
	}
	else {
		return false;
	}
}

bool Fighter::can_kara() {
	if (((fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) || fighter_flag[FIGHTER_FLAG_KARA_ENABLED]) {
		return true;
	}
	else {
		return false;
	}
}

bool Fighter::has_meter(int bars) {
	int ex_meter_size = get_param_int("ex_meter_size", PARAM_FIGHTER);
	int ex_meter_bars = get_param_int("ex_meter_bars", PARAM_FIGHTER);
	if (fighter_float[FIGHTER_FLOAT_SUPER_METER] >= ex_meter_size / (ex_meter_bars / bars)) {
		fighter_float[FIGHTER_FLOAT_SUPER_METER] -= ex_meter_size / (ex_meter_bars / bars);
		return true;
	}
	return false;
}