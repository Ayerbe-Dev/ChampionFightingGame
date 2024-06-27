#pragma warning(disable : 4996)
#include "Fighter.h"
#include "GameManager.h"

bool Fighter::change_anim(std::string animation_name, float rate, float frame) {
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	prev_anim_rate = this->rate;
	prev_anim_frame = this->frame;

	Animation* new_anim = anim_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		if (frame != -1) { //The frame can't go into the negatives, so if we call an animation with
			//frame -1, that's code for "scale the animation so that we reach a target frame within
			//[rate] frames
			this->rate = rate;
			this->frame = frame;
			object_int[FIGHTER_INT_EXTERNAL_FRAME] = std::floor(frame);
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
			if (rate == 1.0) {
				this->rate = 1.0;
			}
			else {
				this->rate = target_frame / rate;
			}
			this->frame = 0.0;
			object_int[FIGHTER_INT_EXTERNAL_FRAME] = 0;
		}
		model.set_move(new_anim->flag_move);
		model.set_flip(!facing_right);
		prev_anim_kind = anim_kind;
	}
	
	prev_anim_offset = glm::vec3(0.0);
	anim_end = false;
	anim_kind = new_anim;

	set_active_script(animation_name);

	return new_anim != nullptr;
}

bool Fighter::change_anim_inherit_attributes(std::string animation_name, bool continue_script, bool verbose) {
	if (!continue_script) {
		set_active_script(animation_name);
	}
	Animation* new_anim = anim_table.get_anim(animation_name, verbose);

	if (new_anim != nullptr) {
		model.set_move(new_anim->flag_move);
		model.set_flip(!facing_right);
	}
	anim_end = false;
	if (anim_kind != new_anim) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = new_anim;

	return new_anim != nullptr;
}

bool Fighter::beginning_hitlag(int frames) {
	return object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] + frames >= object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] && object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0;
}

bool Fighter::ending_hitlag(int frames) {
	return object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] - frames <= 0 && object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0;
}