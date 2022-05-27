#pragma warning(disable : 4996)
#include "Fighter.h"

void Fighter::reenter_last_anim() {
	rate = prev_anim_rate;
	frame = prev_anim_frame;
	set_current_move_script(prev_anim_kind->name);
	startAnimation(prev_anim_kind, true);
}

bool Fighter::change_anim(std::string animation_name, float rate, float frame, bool clear_buffer) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_rate = rate;
	prev_anim_frame = frame;

	set_current_move_script(animation_name);

	Animation* new_anim = anim_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		if (frame != -1) { //The frame can't go into the negatives, so if we call an animation with
			//frame -1, that's code for "scale the animation so that we reach a target frame within
			//[rate] frames
			this->rate = rate;
			this->frame = frame;
		}
		else {
			//If the animation has an faf, that's the target frame. If the faf is either 0 or -1, the target
			//frame is the anim length

			//In practice: For hitstun animations, if you pass the number of hitstun frames as the rate,
			//the hitstun animation should end when hitstun does no matter how many frames it is. 
			//For landing animations, if you pass the landing lag as the rate, the transition from landing
			//to idle will continue after landing lag ends (just looks prettier that way), but you'll reach
			//the interruptible frame of the landing animation on the frame landing lag ends
			float target_frame = new_anim->faf;
			if (target_frame < 1) {
				target_frame = new_anim->length;
			}
			this->rate = target_frame / (rate + 1.0);
			this->frame = 0.0;
		}
	}

	startAnimation(new_anim, clear_buffer);

	return new_anim != nullptr;
}

bool Fighter::change_anim_inherit_attributes(std::string animation_name, bool continue_script, bool clear_buffer, bool verbose) {
	if (!continue_script) {
		set_current_move_script(animation_name);
	}
	Animation* new_anim = anim_table.get_anim(animation_name, verbose);
	startAnimation(new_anim, clear_buffer);
	return new_anim != nullptr;
}

void Fighter::startAnimation(Animation* animation, bool clear_buffer) {
	if (clear_buffer) {
		player_info->controller.reset_buffer();
	}
	if (animation != nullptr) {
		model.set_move(animation->move);
	}
	prev_anim_offset = glm::vec3(0.0);
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

float Fighter::calc_launch_frames() {
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		char buffer[51];
		sprintf(buffer, "Player %d needs a gravity value on their launcher!", ((!id) + 1));
		player_info->crash_reason = buffer;

		crash_to_debug = true;
		return 1;
	}
	float airtime = 0.0;
	float simp_y = pos.y; //haha simp
	float sims_y = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	while (simp_y >= FLOOR_GAMECOORD) {
		if (sims_y > fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] * -1) {
			sims_y -= fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY];
		}
		simp_y += sims_y;
		airtime++;
	}
	return airtime;
}

std::string Fighter::get_anim() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		return anim_kind->name;
	}
}

std::string Fighter::get_anim_broad() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		std::string ret = anim_kind->name;
		if (ret.find("_air") != std::string::npos) {
			ret = Filter(ret, "_air");
		}
		return ret;
	}
}