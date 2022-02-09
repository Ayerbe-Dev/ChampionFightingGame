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

	int anim_to_use = -1;
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

			set_current_move_script(animation_name);
			startAnimation(&animation_table[i]);
			return true;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
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
	return false;
}

void Fighter::startAnimation(Animation3D* animation) {
	int group = get_status_group();
	if (group != STATUS_GROUP_NO_RENDER_PRIORITY && group != STATUS_GROUP_CROUCH && group != STATUS_GROUP_HITSTUN) {
		fighter_accessor->render_priority = id;
		requesting_priority = true;
	}
	else {
		fighter_accessor->render_priority_no_req = id;
		requesting_priority = false;
	}
	is_anim_end = false;
	if (anim_kind != animation) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = animation;
}

bool Fighter::canStep() {
	attempted_excutes = 0;
	return fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0;
}

void Fighter::stepAnimation() {
	float last_frame = frame;
	if (frame >= anim_kind->length) {
		frame = 0.0;
		excute_count = 0;
		clear_grabbox_all();
		clear_hurtbox_all();
		clear_hitbox_all();
		kara_enabled = false;
		if (id == 0) {
			int group = get_status_group();
			if (group == STATUS_GROUP_NO_RENDER_PRIORITY || group == STATUS_GROUP_CROUCH) {
				fighter_accessor->render_priority_no_req = !fighter_accessor->render_priority_no_req;
			}
		}
	}
	else {
		frame += rate;
	}
	is_anim_end = last_frame > frame;
}

void Fighter::forceStepThroughHitlag() {
	int curr_hitlag_frames = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 0;
	canStep();
	stepAnimation();
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = curr_hitlag_frames;
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
	return anim_kind->name;
}

string Fighter::get_anim_broad() {
	string ret = anim_kind->name;
	if (ret.find("_air") != string::npos) {
		ret = Filter(ret, "_air");
	}
	if (ret.find("_stationary") != string::npos) {
		ret = Filter(ret, "_stationary");
	}
	return ret;
}