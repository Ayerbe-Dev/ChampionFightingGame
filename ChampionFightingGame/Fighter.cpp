#pragma warning(disable : 4996)
#include "Fighter.h"
#include "Animation.h"
#include "Projectile.h"
#include <fstream>
#include "ObjectManager.h"
#include "AIManager.h"
#include "ParamAccessor.h"
#include "ThreadManager.h"
#include "utils.h"

Fighter::Fighter() {
	has_model = true;
	chara_kind = CHARA_KIND_ROWAN;
	chara_name = "Rowan";
	prev_stick_dir = 0;
	for (int i = 0; i < CANCEL_CAT_MAX; i++) {
		for (int i2 = 0; i2 < CANCEL_KIND_MAX; i2++) {
			cancel_flags[i][i2] = false;
		}
	}
	object_type = BATTLE_OBJECT_TYPE_FIGHTER;
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
		hitboxes[i].rect.destroy();
		hurtboxes[i].rect.destroy();
		grabboxes[i].rect.destroy();
		pushboxes[i].rect.destroy();
	}
	clear_effect_all();
	stop_sound_all();
	stop_reserved_sound();
	blockbox.rect.destroy();
	fighter_int.clear();
	fighter_float.clear();
	fighter_flag.clear();
	status_script.clear();
	enter_status_script.clear();
	exit_status_script.clear();
	model.unload_model_instance();
	anim_table.unload_animations();
	move_script_table.wipe_scripts();
	params.unload_params();
	stats.unload_params();
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

	process_animate();
	process_pre_position();
	process_pre_input();
	process_pre_status();
	chara_main();
	process_status();
	process_post_animate();
	process_projectiles();
	process_input();
	if (object_manager->is_allow_realtime_process(this)) {
		decrease_common_variables();
	}
	process_post_status();
	process_ai();
}

void Fighter::fighter_post() {
	if (is_status_delay()) {
		(this->*status_script[status_kind])();
	}
	process_post_position();
	if (anim_kind == nullptr) {
		rot.x -= glm::radians(90.0);
		rot.z += glm::radians(90.0 * facing_dir);
	}
	else {
		rot.z += glm::radians(90.0);
	}
	rot += extra_rot;
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
	if (fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] != 0) {
		if (anim_kind != nullptr && !anim_kind->flag_no_hitlag_interp) {
			frame += (0.2 / (float)(fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES])) * object_manager->get_world_rate(this);
		}
	}
	else {
		float add_frame = rate * object_manager->get_world_rate(this);
		frame += add_frame;
		if (add_frame != 0.0) {
			fighter_int[FIGHTER_INT_EXTERNAL_FRAME]++;
		}
		
	}

	if (internal_facing_right != facing_right 
		&& is_actionable()
		&& status_kind != FIGHTER_STATUS_JUMPSQUAT
		&& status_kind != FIGHTER_STATUS_TURN
		&& situation_kind == FIGHTER_SITUATION_GROUND) {
		change_status(FIGHTER_STATUS_TURN);
		return;
	}

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length) {
			frame = 0.0;
			fighter_int[FIGHTER_INT_EXTERNAL_FRAME] = 0;
			active_move_script.activate();
			clear_grabbox_all();
			clear_hurtbox_all();
			clear_hitbox_all();
			fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
			anim_end = true;
		}
		else {
			anim_end = false;
		}
	}
}

void Fighter::process_post_animate() {
	model.set_bones(frame, anim_kind);
	set_pos_anim();
}

void Fighter::process_pre_position() {
	rot = glm::vec3(0.0);
	extra_rot = glm::vec3(0.0);
	if (isnan(pos.x)) {
		pos.x = 0;
	}
	if (isnan(pos.y)) {
		pos.y = 0;
	}
	if (isnan(pos.z)) {
		pos.z = 0;
	}

	update_pushbox_pos();

	prev_pos = pos;
}

void Fighter::process_post_position() {
	Fighter* that = object_manager->fighter[!id];
	if (fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == FIGHTER_SITUATION_GROUND || fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL]) {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * that->facing_dir, 0, 0)) && !fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
					that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
					that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
					if (!(that->fighter_flag[FIGHTER_FLAG_SHORT_HOP] && that->fighter_flag[FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK])) {
						that->fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = true;
						//If an opponent does a fullhop air attack while we're cornered, we get pushed
						//back. If they do a shorthop instead, we get pushed back and up.
					}
				}
			}
			else {
				//This code was originally designed to push an aerial attacker up when they hit a
				//jumping opponent in the corner, but was later bugged to also execute when we hit
				//grounded opponents in the corner. Since aerial defenders no longer experience
				//normal pushback, it should be completely impossible to meet the conditions for
				//it to trigger at all, but I'm leaving it commented because it looked cool when we
				//did it to grounded opponents and we might want to use it for certain air attacks.

				float y_pushback = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
				if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > 0.0) {
					y_pushback = 0.0;
				}
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * that->internal_facing_dir, y_pushback, 0)) && !fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
					that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
					that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
					that->fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = true;
				}
			}
		}
	}
	else {
		fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0;
		fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = false;
	}
	update_pushbox_pos();
}

void Fighter::process_pre_status() {
	if (!fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE_FOR_UI] = fighter_int[FIGHTER_INT_DAMAGE_SCALE];
		if (fighter_int[FIGHTER_INT_BUFFER_STATUS] != FIGHTER_STATUS_MAX
			&& is_enable_cancel(fighter_int[FIGHTER_INT_BUFFER_CANCEL_KIND])
			&& fighter_int[FIGHTER_INT_BUFFER_CANCEL_TIMER]) {
			if (fighter_int[FIGHTER_INT_BUFFER_STATUS] == FIGHTER_STATUS_ATTACK
			|| fighter_int[FIGHTER_INT_BUFFER_STATUS] == FIGHTER_STATUS_ATTACK_AIR) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND];
				if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_OTHER) {
					fighter_int[FIGHTER_INT_ATTACK_OTHER_KIND] = fighter_int[FIGHTER_INT_BUFFER_ATTACK_OTHER_INFO];
				}
			}
			else if (fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND]) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND];
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = fighter_int[FIGHTER_INT_BUFFER_ATTACK_OTHER_INFO];
			}
			change_status(fighter_int[FIGHTER_INT_BUFFER_STATUS], fighter_flag[FIGHTER_FLAG_BUFFER_STATUS_END], fighter_flag[FIGHTER_FLAG_BUFFER_STATUS_SEPARATE]);
		}
		if (fighter_int[FIGHTER_INT_BUFFER_CANCEL_TIMER] > 0) {
			fighter_int[FIGHTER_INT_BUFFER_CANCEL_TIMER]--;
		}
		else {
			fighter_int[FIGHTER_INT_BUFFER_STATUS] = FIGHTER_STATUS_MAX;
		}
	}
}

void Fighter::process_status() {
	bool execute_after_status = anim_end;
	if (!execute_after_status) {
		active_move_script.execute(this, frame);
	}
	if (!is_status_delay()) {
		(this->*status_script[status_kind])();
	}
	if (execute_after_status) {
		active_move_script.execute(this, frame);
	}
}

void Fighter::process_post_status() {
	Fighter* that = object_manager->fighter[!id];
	fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN] = false;
	if (get_status_group() != STATUS_GROUP_HITSTUN && status_kind != FIGHTER_STATUS_GRABBED) {
		if (that->fighter_int[FIGHTER_INT_COMBO_COUNT] != 0) {
			fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN] = true;
		}
		that->fighter_int[FIGHTER_INT_COMBO_COUNT] = 0;
		that->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] = 0.0;
		if (situation_kind == FIGHTER_SITUATION_GROUND) {
			fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		}
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
		fighter_int[FIGHTER_INT_PREV_DAMAGE_SCALE] = 0;
		if (fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] != 0) {
			fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER]--;
		}
	}
	if (get_status_group() != STATUS_GROUP_ATTACK || is_actionable() || that->fighter_int[FIGHTER_INT_COMBO_COUNT] == 0) {
		fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = false;
	}

	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active) {
			fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX] = true;
			fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = true;
			break;
		}
		else {
			fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX] = false;
		}
	}
}

void Fighter::process_pre_input() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (!get_local_param_bool("has_airdash") || fighter_flag[FIGHTER_FLAG_USED_AIRDASH]) {
			fighter_int[FIGHTER_INT_DASH_F_BUFFER] = 0;
			fighter_int[FIGHTER_INT_DASH_B_BUFFER] = 0;
			return;
		}
	}
	if (fighter_int[FIGHTER_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6) {
		fighter_int[FIGHTER_INT_DASH_F_BUFFER] = get_param_int(PARAM_FIGHTER, "buffer_window");
	}
	if (fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4) {
		fighter_int[FIGHTER_INT_DASH_B_BUFFER] = get_param_int(PARAM_FIGHTER, "buffer_window");
	}
}

void Fighter::process_input() {
	int stick_dir = get_stick_dir();
	int flick_dir = get_flick_dir();
	int dash_window = get_param_int(PARAM_FIGHTER, "dash_window");
	int motion_special_timer = get_param_int(PARAM_FIGHTER, "motion_special_timer");

	//Dash Input

	if (flick_dir == 6 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = dash_window;
	}
	if (flick_dir == 4 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = dash_window;
	}
	if (stick_dir != 6 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = 0;
	}
	if (stick_dir != 4 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = 0;
	}

	//Motion Inputs

	if (stick_dir == 1) {
		if (fighter_int[FIGHTER_INT_214_STEP] == 1) {
			fighter_int[FIGHTER_INT_214_STEP] ++;
			fighter_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 1) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 3) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 1 || fighter_int[FIGHTER_INT_214214_STEP] == 4) {
			fighter_int[FIGHTER_INT_214214_STEP]++;
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 2) {
		if (fighter_int[FIGHTER_INT_236_STEP] == 0) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_214_STEP] == 0) {
			fighter_int[FIGHTER_INT_214_STEP]++;
			fighter_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
		}		
		if (fighter_int[FIGHTER_INT_623_STEP] == 1) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 2) {
			fighter_int[FIGHTER_INT_41236_STEP]++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 2) {
			fighter_int[FIGHTER_INT_63214_STEP]++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_632_STEP] == 2) {
			fighter_int[FIGHTER_INT_632_STEP]++;
			fighter_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_22_STEP] == 0 || fighter_int[FIGHTER_INT_22_STEP] == 2) {
			if (prev_stick_dir == 5) {
				fighter_int[FIGHTER_INT_22_STEP] ++;
				fighter_int[FIGHTER_INT_22_TIMER] = motion_special_timer;
			}
			else {
				fighter_int[FIGHTER_INT_22_STEP] = 0;
				fighter_int[FIGHTER_INT_22_TIMER] = 0;
			}
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 1) {
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 0 || fighter_int[FIGHTER_INT_214214_STEP] == 3) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 1) {
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 0 || fighter_int[FIGHTER_INT_236236_STEP] == 3) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 3) {
		if (fighter_int[FIGHTER_INT_236_STEP] == 1) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236_STEP] == 3) { 
			//This check is weird, but it essentially prevents conflicts with DP motions by making
			//it so finishing a QCF motion and then inputting Down Forward makes it so you have to
			//input forward again in order to QCF. This both makes it so inputting double DP doesn't
			//get read as QCF on characters where QCF has priority, and prevents obviously 
			//unintended QCFs from being buffered if you do crouch -> walk forward -> crouch again
			//before pressing a button.
			fighter_int[FIGHTER_INT_236_STEP]--;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_623_STEP] == 0 || fighter_int[FIGHTER_INT_623_STEP] == 2) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 3) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 1) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_632_STEP] == 1) {
			fighter_int[FIGHTER_INT_632_STEP]++;
			fighter_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
		}
		//Same deal as before but like the other way
		if (fighter_int[FIGHTER_INT_632_STEP] == 3) {
			fighter_int[FIGHTER_INT_632_STEP]--;
			fighter_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 1 || fighter_int[FIGHTER_INT_236236_STEP] == 4) {
			fighter_int[FIGHTER_INT_236236_STEP]++;
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 4) {
		if (fighter_int[FIGHTER_INT_214_STEP] == 2) {
			fighter_int[FIGHTER_INT_214_STEP]++;
			fighter_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 0) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 4) {
			fighter_int[FIGHTER_INT_63214_STEP]++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 2 || fighter_int[FIGHTER_INT_214214_STEP] == 5) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_4646_STEP] == 2) {
			fighter_int[FIGHTER_INT_4646_STEP]++;
			fighter_int[FIGHTER_INT_4646_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 5) {
		if (fighter_int[FIGHTER_INT_22_STEP] == 1) {
			fighter_int[FIGHTER_INT_22_STEP]++;
			fighter_int[FIGHTER_INT_22_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 6) {
		if (fighter_int[FIGHTER_INT_236_STEP] == 2) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_623_STEP] == 0) {
			fighter_int[FIGHTER_INT_623_STEP]++;
			fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 4) {
			fighter_int[FIGHTER_INT_41236_STEP]++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 0) {
			fighter_int[FIGHTER_INT_63214_STEP]++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_632_STEP] == 0) {
			fighter_int[FIGHTER_INT_632_STEP]++;
			fighter_int[FIGHTER_INT_632_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 2 || fighter_int[FIGHTER_INT_236236_STEP] == 5) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_4646_STEP] == 1 || fighter_int[FIGHTER_INT_4646_STEP] == 3) {
			fighter_int[FIGHTER_INT_4646_STEP]++;
			fighter_int[FIGHTER_INT_4646_TIMER] = motion_special_timer;
		}

		//If you're inputting forward, it's very unlikely that you still want to do an input that
		//ends in a quarter circle back. We shouldn't completely rule out the possibility of 
		//microwalk qcb inputs, but we'll make it so the qcb input only stays in the buffer for
		//3 frames as opposed to the usual 11.

		//Note: We aren't doing this for QCD motions because the act of inputting a 3 already 
		//decrements the step by 1. If you did a QCD and then immediately hit forward without
		//ever hitting down forward, that was definitely planned.

		if (fighter_int[FIGHTER_INT_214_STEP] == 3 && fighter_int[FIGHTER_INT_214_TIMER] > 3) {
			fighter_int[FIGHTER_INT_214_TIMER] = 3;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 6 && fighter_int[FIGHTER_INT_214214_TIMER] > 3) {
			fighter_int[FIGHTER_INT_214214_TIMER] = 3;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 5 && fighter_int[FIGHTER_INT_63214_TIMER] > 3) {
			fighter_int[FIGHTER_INT_63214_TIMER] = 3;
		}
	}
	if (stick_dir < 4) {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = motion_special_timer;
		if (prev_stick_dir < 4) {
			fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES]++;
		}
		else {
			fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
		}
	}

	if (stick_dir % 3 == 1) {
		fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = motion_special_timer;
		if (prev_stick_dir % 3 == 1) {
			fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES]++;
		}
		else {
			fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
		}
	}

	prev_stick_dir = stick_dir;
}

void Fighter::process_ai() {
	AIManager* ai_manager = AIManager::get_instance();
	ai_manager->ai_mutex.lock();
	ai_manager->ai_info[id].insert(AIInfo(pos.x, pos.y, get_anim(), frame, rate));
	ai_manager->ai_mutex.unlock();
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
	pos.x = stage->start_pos[id];
	pos.y = 0.0f;
	pos.z = 0.0f;
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_local_param_float("health");
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	change_status(FIGHTER_STATUS_WAIT);
}


void Fighter::decrease_common_variables() {
	if (fighter_int[FIGHTER_INT_236_TIMER] != 0) {
		fighter_int[FIGHTER_INT_236_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_214_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_623_TIMER] != 0) {
		fighter_int[FIGHTER_INT_623_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_623_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_41236_TIMER] != 0) {
		fighter_int[FIGHTER_INT_41236_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_41236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_63214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_63214_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_63214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_632_TIMER] != 0) {
		fighter_int[FIGHTER_INT_632_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_632_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_22_TIMER] != 0) {
		fighter_int[FIGHTER_INT_22_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_22_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_236236_TIMER] != 0) {
		fighter_int[FIGHTER_INT_236236_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_236236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_214214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_214214_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_214214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_4646_TIMER] != 0) {
		fighter_int[FIGHTER_INT_4646_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_4646_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] != 0) {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] != 0) {
		fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		fighter_int[FIGHTER_INT_HITLAG_FRAMES]--;
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && anim_kind != nullptr) {
			if (!anim_kind->flag_no_hitlag_interp) {
				frame -= 0.2;
			}			
			fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 0;
		}
	}
	else {
		if (status_kind == FIGHTER_STATUS_HITSTUN || status_kind == FIGHTER_STATUS_HITSTUN_AIR
			|| status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] != 0) {
				fighter_int[FIGHTER_INT_HITSTUN_FRAMES]--;
			}
		}
		if (fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
			fighter_int[FIGHTER_INT_PUSHBACK_FRAMES]--;
		}
	}
	if (fighter_int[FIGHTER_INT_DASH_F_WINDOW] != 0) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW]--;
	}
	if (fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW]--;
	}
	if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] != 0) {
		fighter_int[FIGHTER_INT_DASH_F_BUFFER]--;
	}
	if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] != 0) {
		fighter_int[FIGHTER_INT_DASH_B_BUFFER]--;
	}
	if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] != 0) {
		fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW]--;
	}
	if (fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] != 0) {
		fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES]--;
	}
	else {
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH], fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] + 1.0, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	if (fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] != 0) {
		fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER]--;
	}
	if (fighter_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER] != 0) {
		fighter_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER]--;
		if (fighter_int[FIGHTER_INT_ATTACK_ENABLE_CANCEL_TIMER] == 0) {
			fighter_flag[FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY] = true;
		}
	}
}