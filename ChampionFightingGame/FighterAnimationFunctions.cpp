#pragma warning(disable : 4996)
#include "Fighter.h"

void Fighter::reenter_last_anim() {
	rate = prev_anim_rate;
	frame = prev_anim_frame;
	set_current_move_script(prev_anim_kind->name);
	startAnimation(prev_anim_kind);
}

bool Fighter::change_anim(string animation_name, float frame_rate, float entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_rate = rate;
	prev_anim_frame = frame;

	set_current_move_script(animation_name);

	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			if (frame_rate != -1) {
				rate = frame_rate;
				frame = entry_frame;
			}
			else {
				rate = ceil((float)entry_frame / (float)(animation_table[i].length));
				frame = 0;
			}

			startAnimation(&animation_table[i]);
			return true;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
	startAnimation(nullptr);
	return false;
}

bool Fighter::change_anim_inherit_attributes(string animation_name, bool verbose, bool continue_script) {
	int anim_to_use = -1;
	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			if (!continue_script) {
				set_current_move_script(animation_name);
			}
			startAnimation(&animation_table[i]);
			return true;
		}
	}
	if (verbose) {
		cout << "Invalid Animation '" << animation_name << "'" << endl;
	}
	startAnimation(nullptr);
	return false;
}

void Fighter::startAnimation(Animation* animation) {
	is_anim_end = false;
	if (anim_kind != animation) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = animation;
}

bool Fighter::beginning_hitlag(int frames) {
	return fighter_int[FIGHTER_INT_HITLAG_FRAMES] + frames >= fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] && fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0;
}

bool Fighter::ending_hitlag(int frames) {
	return fighter_int[FIGHTER_INT_HITLAG_FRAMES] - frames <= 0 && fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0;
}

int Fighter::get_launch_ticks() {
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		char buffer[51];
		sprintf(buffer, "Player %d needs a gravity value on their launcher!", ((!id) + 1));
		player_info->crash_reason = buffer;

		crash_to_debug = true;
		return 1;
	}
	int airtime = 0;
	int simp_y = pos.y; //haha simp
	int sims_y = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	while (simp_y >= FLOOR_GAMECOORD) {
		if (sims_y > fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] * -1) {
			sims_y -= fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY];
		}
		simp_y += sims_y;
		airtime++;
	}
	return airtime;
}

string Fighter::get_anim() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		return anim_kind->name;
	}
}

string Fighter::get_anim_broad() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		string ret = anim_kind->name;
		if (ret.find("_air") != string::npos) {
			ret = Filter(ret, "_air");
		}
		if (ret.find("_stationary") != string::npos) {
			ret = Filter(ret, "_stationary");
		}
		return ret;
	}
}