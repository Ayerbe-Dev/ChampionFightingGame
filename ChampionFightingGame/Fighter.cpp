#pragma warning(disable : 4996)
#include "Fighter.h"
#include "Animation.h"
#include "Projectile.h"
#include <fstream>
#include "ObjectManager.h"
#include "ParamAccessor.h"
#include "ThreadManager.h"
#include "utils.h"

Fighter::Fighter() {
	object_type = BATTLE_OBJECT_TYPE_FIGHTER;
	has_model = true;
	chara_kind = CHARA_KIND_ROWAN;
	chara_name = "Rowan";
	prev_stick_dir = 0;
	fighter_context = FIGHTER_CONTEXT_GROUND;
	for (int i = 1; i < 10; i++) {
		if (i % 3 == 1) {
			throw_map_ground[i] = "throw_b";
			throw_map_air[i] = "throw_b_air";
		}
		else {
			throw_map_ground[i] = "throw_f";
			throw_map_air[i] = "throw_f_air";
		}
	}
}

Fighter::~Fighter() {
	projectiles.clear();
	for (int i = 0; i < 10; i++) {
		hitboxes[i].world_tex.destroy();
		hurtboxes[i].world_tex.destroy();
		grabboxes[i].world_tex.destroy();
		pushboxes[i].world_tex.destroy();
	}
	clear_effect_all();
	stop_sound_all();
	stop_reserved_sound();
	blockbox.world_tex.destroy();
	object_int.clear();
	object_float.clear();
	object_flag.clear();
	status_script.clear();
	enter_status_script.clear();
	exit_status_script.clear();
	model.unload_model_instance();
	anim_table.unload_animations();
	move_script_table.clear_scripts();
	param_table.unload_params();
}

void Fighter::fighter_main() {
		/*
				   _.-,
			  _ .-'  / .._
		   .-:'/ - - \:::::-.
		 .::: '  e e  ' '-::::.
		::::'(    ^    )_.::::::
	   ::::.' '.  o   '.::::'.'/_
   .  :::.'       -  .::::'_   _.:
 .-''---' .'|      .::::'   '''::::
'. ..-:::'  |    .::::'        ::::
 '.' ::::    \ .::::'          ::::
	  ::::   .::::'           ::::
	   ::::.::::'._          ::::
		::::::' /  '-      .::::
		 '::::-/__    __.-::::'
		   '-::::::::::::::-'
			   '''::::'''
	 */
	process_pre_common_fighter_vars();
	process_animate();
	process_position();
	process_pre_status();
	process_input();
	chara_main();
	process_status();
	process_post_animate();
	process_projectiles();
	process_post_common_fighter_vars();
	process_post_status();
}

void Fighter::fighter_post() {
	if (object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_POST_PROCESS) {
		(this->*status_script[status_kind])();
	}
	if (is_enable_free_cancel()) {
		object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	}
	process_post_position();
	if (anim_kind == nullptr) {
		add_rot(glm::vec3(0.0, 0.0, 90.0 * facing_dir));
	}
	else {
		add_rot(glm::vec3(0.0, 0.0, 90.0));
	}
	process_post_projectiles();
}

void Fighter::process_projectiles() {
	for (size_t i = 0; i < projectiles.size(); i++) {
		if (projectiles[i]->active) {
			projectiles[i]->projectile_main();
		}
	}
}

void Fighter::process_post_projectiles() {
	for (size_t i = 0; i < projectiles.size(); i++) {
		if (projectiles[i]->active) {
			projectiles[i]->projectile_post();
		}
	}
}

void Fighter::process_animate() {
	frame_delta = 0.0f;
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		if (anim_kind != nullptr && !anim_kind->flag_no_hitlag_interp) {
			frame_delta = (0.2 / (float)(object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES])) * object_manager->get_world_rate(this);
		}
	}
	else if (!(object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]
		&& object_int[FIGHTER_INT_STATUS_GROUP] == STATUS_GROUP_NONSTANDARD_HITSTUN)) {
		frame_delta = rate * object_manager->get_world_rate(this);
		if (frame_delta != 0.0) {
			object_int[FIGHTER_INT_EXTERNAL_FRAME]++;
		}
	}
	frame += frame_delta;

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length) {
			frame = 0.0;
			object_int[FIGHTER_INT_EXTERNAL_FRAME] = 0;
			move_script.activate();
			clear_grabbox_all();
			clear_hurtbox_all();
			clear_hitbox_all();
			object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
			anim_end = true;
		}
		else {
			anim_end = false;
		}
	}
}

void Fighter::process_post_animate() {
	model.set_bones(frame, anim_kind);
	apply_trans_to_pos();
}

void Fighter::process_position() {
	set_rot(glm::vec3(0.0));

	update_pushbox_pos();

	prev_pos = get_pos();
}

void Fighter::process_post_position() {
	Fighter* that = object_manager->fighter[!id];
	if (object_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0 
			&& object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0f
			&& fighter_context != FIGHTER_CONTEXT_AIR) {
			//TODO: Modify add_pos so that the returned failure code can distinguish between failures 
			//due to positional differences and failures due to OoB movement
			if (!add_pos_validate(glm::vec3(object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * that->facing_dir, 0, 0)) 
				&& !object_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
				that->object_int[FIGHTER_INT_PUSHBACK_FRAMES] = object_int[FIGHTER_INT_PUSHBACK_FRAMES];
				that->object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
				object_int[FIGHTER_INT_PUSHBACK_FRAMES] = 0;
				object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0f;
			}
		}
	}
	else {
		object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0f;
	}
	update_pushbox_pos();
}

void Fighter::process_pre_status() {
	if (object_flag[FIGHTER_FLAG_STATUS_CHANGED]) {
		prev_status_kind = status_kind;
		object_int[FIGHTER_INT_PREV_STATUS_GROUP] = object_int[FIGHTER_INT_STATUS_GROUP];
		object_flag[FIGHTER_FLAG_STATUS_CHANGED] = false;
	}
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
	}
	if (internal_facing_right != facing_right
		&& is_actionable()
		&& status_kind != FIGHTER_STATUS_JUMPSQUAT
		&& status_kind != FIGHTER_STATUS_TURN
		&& fighter_context == FIGHTER_CONTEXT_GROUND) {
		change_status(FIGHTER_STATUS_TURN);
	}
}

void Fighter::process_status() {
	bool execute_after_status = anim_end;
	if (!execute_after_status) {
		execute_active_script();
	}
	if (!(object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_POST_PROCESS)) {
		(this->*status_script[status_kind])();
	}
	if (execute_after_status) {
		execute_active_script();
	}
}

void Fighter::process_post_status() {
	Fighter* that = object_manager->fighter[!id];

	object_flag[FIGHTER_FLAG_ENDED_HITSTUN] = object_flag[FIGHTER_FLAG_HITSTUN_PARRY] 
		|| ((object_int[FIGHTER_INT_PREV_STATUS_GROUP] & STATUS_GROUP_HITSTUN) 
			&& !(object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN));
	object_flag[FIGHTER_FLAG_HITSTUN_PARRY] = false;

	if (object_flag[FIGHTER_FLAG_ENDED_HITSTUN]) {
		object_flag[FIGHTER_FLAG_HIT_BY_EX_SUPER] = false;
		object_flag[FIGHTER_FLAG_HIT_BY_CHAMPION_SUPER] = false;
		that->object_int[FIGHTER_INT_COMBO_COUNT] = 0;
		that->object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING] = 0.0;
		object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.0f;
		object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] = 1.0f;
		object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	}
	if (object_float[FIGHTER_FLOAT_DAMAGE_SCALE] < 0.1f) {
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 0.1f;
	}

	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active) {
			object_flag[FIGHTER_FLAG_ACTIVE_HITBOX] = true;
			object_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = true;
			break;
		}
		else {
			object_flag[FIGHTER_FLAG_ACTIVE_HITBOX] = false;
		}
	}
	prev_stick_dir = get_stick_dir();
}

void Fighter::process_input() {
	int motion_special_timer = get_global_param_int(PARAM_FIGHTER, "motion_special_timer");
	int flick_special_timer = get_global_param_int(PARAM_FIGHTER, "flick_special_timer");

	//Motion Inputs

	switch (get_stick_dir()) {
		case (1): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
			if (object_int[FIGHTER_INT_214_STEP] == 1) {
				object_int[FIGHTER_INT_214_STEP]++;
				object_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_41236_STEP] == 1) {
				object_int[FIGHTER_INT_41236_STEP]++;
				object_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 3) {
				object_int[FIGHTER_INT_63214_STEP]++;
				object_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_214214_STEP] == 1 || object_int[FIGHTER_INT_214214_STEP] == 4) {
				object_int[FIGHTER_INT_214214_STEP]++;
				object_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
			}
			object_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir < 4) {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
			}
			object_int[FIGHTER_INT_BACK_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir % 3 == 1) {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
			}
		} break;
		case (2): {
			if (object_int[FIGHTER_INT_236_STEP] == 0) {
				object_int[FIGHTER_INT_236_STEP]++;
				object_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_214_STEP] == 0) {
				object_int[FIGHTER_INT_214_STEP]++;
				object_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_623_STEP] == 1) {
				object_int[FIGHTER_INT_623_STEP]++;
				object_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_41236_STEP] == 2) {
				object_int[FIGHTER_INT_41236_STEP]++;
				object_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 2) {
				object_int[FIGHTER_INT_63214_STEP]++;
				object_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_632_STEP] == 2) {
				object_int[FIGHTER_INT_632_STEP]++;
				object_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
			}
			if (get_flick_dir() == 2 && (object_int[FIGHTER_INT_22_STEP] == 0 || object_int[FIGHTER_INT_22_STEP] == 2)) {
				object_int[FIGHTER_INT_22_STEP]++;
				object_int[FIGHTER_INT_22_TIMER] = flick_special_timer;
			}
			if (object_int[FIGHTER_INT_214214_STEP] == 1) {
				object_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_214214_STEP] == 0 || object_int[FIGHTER_INT_214214_STEP] == 3) {
				object_int[FIGHTER_INT_214214_STEP]++;
				object_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_236236_STEP] == 1) {
				object_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_236236_STEP] == 0 || object_int[FIGHTER_INT_236236_STEP] == 3) {
				object_int[FIGHTER_INT_236236_STEP]++;
				object_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
			}
			object_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir < 4) {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
			}
		} break;
		case (3): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
			if (object_int[FIGHTER_INT_236_STEP] == 1) {
				object_int[FIGHTER_INT_236_STEP]++;
				object_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_236_STEP] == 3) {
				//This check is weird, but it essentially prevents conflicts with DP motions by making
				//it so finishing a QCF motion and then inputting Down Forward makes it so you have to
				//input forward again in order to QCF. This both makes it so inputting double DP doesn't
				//get read as QCF on characters where QCF has priority, and prevents obviously 
				//unintended QCFs from being buffered if you do crouch -> walk forward -> crouch again
				//before pressing a button.
				object_int[FIGHTER_INT_236_STEP]--;
				object_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_623_STEP] == 0 || object_int[FIGHTER_INT_623_STEP] == 2) {
				object_int[FIGHTER_INT_623_STEP]++;
				object_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_41236_STEP] == 3) {
				object_int[FIGHTER_INT_41236_STEP]++;
				object_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 1) {
				object_int[FIGHTER_INT_63214_STEP]++;
				object_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_632_STEP] == 1) {
				object_int[FIGHTER_INT_632_STEP]++;
				object_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
			}
			//Same deal as before but like the other way
			if (object_int[FIGHTER_INT_632_STEP] == 3) {
				object_int[FIGHTER_INT_632_STEP]--;
				object_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_236236_STEP] == 1 || object_int[FIGHTER_INT_236236_STEP] == 4) {
				object_int[FIGHTER_INT_236236_STEP]++;
				object_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
			}
			object_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir < 4) {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
			}
		} break;
		case (4): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
			if (object_int[FIGHTER_INT_214_STEP] == 2) {
				object_int[FIGHTER_INT_214_STEP]++;
				object_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_41236_STEP] == 0) {
				object_int[FIGHTER_INT_41236_STEP]++;
				object_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 4) {
				object_int[FIGHTER_INT_63214_STEP]++;
				object_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_44_STEP] == 0 || object_int[FIGHTER_INT_44_STEP] == 2) {
				if (prev_stick_dir % 3 != 1) {
					object_int[FIGHTER_INT_44_STEP]++;
					object_int[FIGHTER_INT_44_TIMER] = flick_special_timer;
				}
				else {
					object_int[FIGHTER_INT_44_STEP] = 0;
					object_int[FIGHTER_INT_44_TIMER] = 0;
				}
			}
			if (object_int[FIGHTER_INT_214214_STEP] == 2 || object_int[FIGHTER_INT_214214_STEP] == 5) {
				object_int[FIGHTER_INT_214214_STEP]++;
				object_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_4646_STEP] == 0 || object_int[FIGHTER_INT_4646_STEP] == 2) {
				object_int[FIGHTER_INT_4646_STEP]++;
				object_int[FIGHTER_INT_4646_TIMER] = motion_special_timer;
			}

			//Uncommenting the following block removes some really cool charging tech. Charge Supers
			//check both that you inputted the correct sequence and that you charged for a specific
			//amount of time, but it doesn't require that you finish charging before you input 4646.
			//If you 4646, then start inputting back to finish the charge, you can finish it after 
			//and activate the super while blocking.

		/*
			if (object_int[FIGHTER_INT_4646_STEP] == 5) {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
			}
		*/
			object_int[FIGHTER_INT_BACK_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir % 3 == 1) {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
			}
		} break;
		case (5): {
			if (object_int[FIGHTER_INT_66_STEP] == 1) {
				object_int[FIGHTER_INT_66_STEP]++;
				object_int[FIGHTER_INT_66_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_44_STEP] == 1) {
				object_int[FIGHTER_INT_44_STEP]++;
				object_int[FIGHTER_INT_44_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_22_STEP] == 1) {
				object_int[FIGHTER_INT_22_STEP]++;
				object_int[FIGHTER_INT_22_TIMER] = flick_special_timer;
			}
		} break;
		case (6): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
			if (object_int[FIGHTER_INT_236_STEP] == 2) {
				object_int[FIGHTER_INT_236_STEP]++;
				object_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_623_STEP] == 0) {
				object_int[FIGHTER_INT_623_STEP]++;
				object_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_41236_STEP] == 4) {
				object_int[FIGHTER_INT_41236_STEP]++;
				object_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 0) {
				object_int[FIGHTER_INT_63214_STEP]++;
				object_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_632_STEP] == 0) {
				object_int[FIGHTER_INT_632_STEP]++;
				object_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_66_STEP] == 0 || object_int[FIGHTER_INT_66_STEP] == 2) {
				if (prev_stick_dir % 3) {
					object_int[FIGHTER_INT_66_STEP]++;
					object_int[FIGHTER_INT_66_TIMER] = flick_special_timer;
				}
				else {
					object_int[FIGHTER_INT_66_STEP] = 0;
					object_int[FIGHTER_INT_66_TIMER] = 0;
				}
			}
			if (object_int[FIGHTER_INT_236236_STEP] == 2 || object_int[FIGHTER_INT_236236_STEP] == 5) {
				object_int[FIGHTER_INT_236236_STEP]++;
				object_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
			}
			if (object_int[FIGHTER_INT_4646_STEP] == 1 || object_int[FIGHTER_INT_4646_STEP] == 3) {
				object_int[FIGHTER_INT_4646_STEP]++;
				object_int[FIGHTER_INT_4646_TIMER] = motion_special_timer;
			}

			//If you're inputting forward, it's very unlikely that you still want to do an input that
			//ends in a quarter circle back. We shouldn't completely rule out the possibility of 
			//microwalk qcb inputs, but we'll make it so the qcb input only stays in the buffer for
			//3 frames as opposed to the usual 11.

			//Note: We aren't doing this for QCD motions because the act of inputting a 3 already 
			//decrements the step by 1. If you did a QCD and then immediately hit forward without
			//ever hitting down forward, that was definitely planned.

			if (object_int[FIGHTER_INT_214_STEP] == 3 && object_int[FIGHTER_INT_214_TIMER] > 3) {
				object_int[FIGHTER_INT_214_TIMER] = 3;
			}
			if (object_int[FIGHTER_INT_214214_STEP] == 6 && object_int[FIGHTER_INT_214214_TIMER] > 3) {
				object_int[FIGHTER_INT_214214_TIMER] = 3;
			}
			if (object_int[FIGHTER_INT_63214_STEP] == 5 && object_int[FIGHTER_INT_63214_TIMER] > 3) {
				object_int[FIGHTER_INT_63214_TIMER] = 3;
			}
		} break;
		case (7): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
			object_int[FIGHTER_INT_BACK_CHARGE_TIMER] = motion_special_timer;
			if (prev_stick_dir % 3 == 1) {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES]++;
			}
			else {
				object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
			}
		} break;
		case (8): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
		} break;
		case (9): {
			object_int[FIGHTER_INT_22_STEP] = 0;
			object_int[FIGHTER_INT_22_TIMER] = 0;
		} break;
	}
	if (player->controller.get_buffer_code() & BUTTON_6B_BIT
	&& object_int[FIGHTER_INT_22_STEP] != 3) {
		object_int[FIGHTER_INT_22_STEP] = 0;
		object_int[FIGHTER_INT_22_TIMER] = 0;
	}

	check_movelist_inputs();
}

/*
	 ..=====.. |==|
	|| sus  || |= |
 _  ||      || |^*| _
|=| o==,===,=o |__||=|
|_|  ________)~`)  |_|
	[========]  ()
*/

void Fighter::reset() {
	set_pos(glm::vec3(stage->start_pos[id], 0.0, 0.0));
	set_default_vars();
	change_status(FIGHTER_STATUS_WAIT);
}

void Fighter::process_pre_common_fighter_vars() {
	if (!object_manager->is_allow_realtime_process(this)) return;
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0) {
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]--;
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0 && anim_kind != nullptr) {
			if (!anim_kind->flag_no_hitlag_interp) {
				frame = object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME];
			}
		}
	}
}

void Fighter::process_post_common_fighter_vars() {
	if (!object_manager->is_allow_realtime_process(this)) return;
	if (object_int[FIGHTER_INT_236_TIMER] != 0) {
		object_int[FIGHTER_INT_236_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_236_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_214_TIMER] != 0) {
		object_int[FIGHTER_INT_214_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_214_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_623_TIMER] != 0) {
		object_int[FIGHTER_INT_623_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_623_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_41236_TIMER] != 0) {
		object_int[FIGHTER_INT_41236_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_41236_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_63214_TIMER] != 0) {
		object_int[FIGHTER_INT_63214_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_63214_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_632_TIMER] != 0) {
		object_int[FIGHTER_INT_632_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_632_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_66_TIMER] != 0) {
		object_int[FIGHTER_INT_66_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_66_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_44_TIMER] != 0) {
		object_int[FIGHTER_INT_44_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_44_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_22_TIMER] != 0) {
		object_int[FIGHTER_INT_22_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_22_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_236236_TIMER] != 0) {
		object_int[FIGHTER_INT_236236_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_236236_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_214214_TIMER] != 0) {
		object_int[FIGHTER_INT_214214_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_214214_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_4646_TIMER] != 0) {
		object_int[FIGHTER_INT_4646_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_4646_STEP] = 0;
	}
	if (object_int[FIGHTER_INT_DOWN_CHARGE_TIMER] != 0) {
		object_int[FIGHTER_INT_DOWN_CHARGE_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (object_int[FIGHTER_INT_BACK_CHARGE_TIMER] != 0) {
		object_int[FIGHTER_INT_BACK_CHARGE_TIMER]--;
	}
	else {
		object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
	}
	if (object_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] != 0) {
		object_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW]--;
	}
	if (object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] != 0) {
		object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES]--;
	}
	else {
		object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(object_float[FIGHTER_FLOAT_PARTIAL_HEALTH], object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] + 1.0, object_float[FIGHTER_FLOAT_HEALTH]);
	}
	if (object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] > 0) {
		object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER]--;
	}
	if (object_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] != 0) {
		object_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER]--;
	}
	if (object_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER] != 0) {
		object_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER]--;
		if (object_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER] == 0) {
			object_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = true;
		}
	}
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		if (status_kind == FIGHTER_STATUS_HITSTUN
		||	status_kind == FIGHTER_STATUS_HITSTUN_AIR
		||	status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] != 0) {
				object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
			}
		}
		if (object_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
			object_int[FIGHTER_INT_PUSHBACK_FRAMES]--;
		}
		object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 0;
	}
}

void Fighter::process_cpu() {
	cpu.process();
}