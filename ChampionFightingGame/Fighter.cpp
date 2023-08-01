#pragma warning(disable : 4996)
#include "Fighter.h"
#include "Animation.h"
#include "Projectile.h"
#include <fstream>
#include "BattleObjectManager.h"
#include "AIManager.h"
#include "ParamAccessor.h"
#include "ThreadManager.h"
#include "utils.h"

Fighter::Fighter() {
	has_model = true;
	chara_kind = CHARA_KIND_ROWAN;
	for (int i = 0; i < CANCEL_CAT_MAX; i++) {
		for (int i2 = 0; i2 < CANCEL_KIND_MAX; i2++) {
			cancel_flags[i][i2] = false;
		}
	}
}

Fighter::~Fighter() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i] != nullptr) {
			delete projectiles[i];
		}
	}
	for (int i = 0, max = HITBOX_COUNT_MAX + 1; i < max; i++) {
		hitboxes[i].rect.destroy();
		hurtboxes[i].rect.destroy();
		grabboxes[i].rect.destroy();
	}
	clear_effect_all();
	stop_se_all();
	stop_vc_all();
	blockbox.rect.destroy();
	jostle_box.destroy();
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
	process_position();
	process_input();
	if (battle_object_manager->allow_dec_var(id)) {
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
	update_hitbox_pos();
	update_grabbox_pos();
	update_hurtbox_pos();
	update_blockbox_pos();
	update_jostle_rect();
	rot.z += glm::radians(90.0);
	rot += extra_rot;
	process_post_projectiles();
}

void Fighter::process_projectiles() {
	for (int i = 0; i < num_projectiles; i++) {
		if (projectiles[i]->active) {
			projectiles[i]->projectile_main();
		}
	}
}

void Fighter::process_post_projectiles() {
	for (int i = 0; i < num_projectiles; i++) {
		if (projectiles[i]->active) {
			projectiles[i]->projectile_post();
		}
	}
}

void Fighter::process_animate() {
	if (fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] != 0) {
		if (anim_kind != nullptr && !anim_kind->flag_no_hitlag_interp) {
			frame += (0.2 / (float)(fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES])) * battle_object_manager->get_world_rate(id);
		}
	}
	else {
		float add_frame = rate * battle_object_manager->get_world_rate(id);
		frame += add_frame;
		if (add_frame != 0.0) {
			fighter_int[FIGHTER_INT_EXTERNAL_FRAME]++;
		}
		
	}

	if (internal_facing_right != facing_right 
		&& is_actionable() 
		&& status_kind != FIGHTER_STATUS_TURN 
		&& status_kind != FIGHTER_STATUS_CRUMPLE
		&& status_kind != FIGHTER_STATUS_KNOCKDOWN_START
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
			fighter_flag[FIGHTER_FLAG_IN_ENDLAG] = false;
			fighter_flag[FIGHTER_FLAG_KARA_ENABLED] = false;
			is_anim_end = true;
		}
		else {
			is_anim_end = false;
		}
	}
}

void Fighter::process_post_animate() {
	model.set_bones(frame, anim_kind, !facing_right);
	set_pos_anim();
}

void Fighter::process_pre_position() {
	rot = glm::vec3(0.0);
	extra_rot = glm::vec3(0.0);
	if (isnan(pos.y)) {
		pos.y = 0;
	}

	update_jostle_rect();

	prev_pos = pos;
}

void Fighter::process_position() {
	Fighter* that = battle_object_manager->fighter[!id];
	update_jostle_rect();
	if ((situation_kind == FIGHTER_SITUATION_GROUND || situation_kind == FIGHTER_SITUATION_DOWN) && 
		(that->situation_kind == FIGHTER_SITUATION_GROUND || that->situation_kind == FIGHTER_SITUATION_DOWN)
	&& !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && !that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP]) {
		if (is_collide(jostle_box, that->jostle_box) && status_kind != FIGHTER_STATUS_WALK_B) {
			float x_diff = (pos.x - prev_pos.x) * internal_facing_dir;
			float that_x_diff = (that->pos.x - that->prev_pos.x) * that->internal_facing_dir;

			//TODO: This code is ok, but it has a side effect where if one player is walking towards
			//another, cornered player and the cornered player starts walking towards them, the
			//cornered player can push them back and cause some weird stutters for a bit before
			//it corrects itself. This is because when we walk toward a cornered opponent, our
			//x_diff will always be 0.0 even though we're attempting to move. I'm not 100% sure how
			//to fix this issue, but will revisit in the future.

			if (x_diff != 0.0 && that_x_diff != 0.0) {
				if (abs(x_diff) > abs(that_x_diff)) {
					if (!that->add_pos(glm::vec3(-that_x_diff * internal_facing_dir, 0, 0), true)) {
						add_pos(glm::vec3(-x_diff * that->internal_facing_dir, 0, 0), true);
					}
				}
				else if (x_diff == that_x_diff) {
					pos.x = prev_pos.x;
					that->pos.x = that->prev_pos.x;
				}
			}
			else {
				if (x_diff == 0.0 && that_x_diff == 0.0) {
					if (!add_pos(glm::vec3(get_local_param_float("jostle_walk_b_speed") * that->internal_facing_dir, 0.0, 0.0)) && !fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
						that->add_pos(glm::vec3(get_local_param_float("jostle_walk_f_speed") * internal_facing_dir, 0.0, 0.0));

						that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
						that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
						that->fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = true;
					}
				}
				else if (that_x_diff == 0.0) {
					if (!that->add_pos(glm::vec3(
						get_local_param_float("jostle_walk_f_speed") * internal_facing_dir, 
						0.0, 0.0)
					)) {
						pos.x = prev_pos.x;
					}
					else {
						add_pos(glm::vec3((get_local_param_float("jostle_walk_f_speed") -
							get_local_param_float("walk_f_speed")) * internal_facing_dir,
							0.0, 0.0)
						);
					}
				}
			}
		}
	}
	update_jostle_rect();
}

void Fighter::process_post_position() {
	Fighter* that = battle_object_manager->fighter[!id];
	if (fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == FIGHTER_SITUATION_GROUND || fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL]) {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * -facing_dir, 0, 0)) && !fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
					that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
					that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
					if (!that->fighter_flag[FIGHTER_FLAG_SHORT_HOP]) {
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
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * -facing_dir, y_pushback, 0)) && !fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
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
	update_jostle_rect();
}

void Fighter::process_pre_status() {
	if (fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] != FIGHTER_STATUS_MAX && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		change_status(fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS], fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_END], fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_SEPARATE]);
		fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] = FIGHTER_STATUS_MAX;
	}
}

void Fighter::process_status() {
	bool execute_after_status = is_anim_end;
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
	Fighter* that = battle_object_manager->fighter[!id];
	fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN] = false;
	if (get_status_group() != STATUS_GROUP_HITSTUN && status_kind != FIGHTER_STATUS_GRABBED) {
		if (that->fighter_int[FIGHTER_INT_COMBO_COUNT] != 0) {
			fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN] = true;
		}
		that->fighter_int[FIGHTER_INT_COMBO_COUNT] = 0;
		that->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] = 0.0;
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = false;
		if (fighter_int[FIGHTER_INT_POST_HITSTUN_TIMER] != 0) {
			fighter_int[FIGHTER_INT_POST_HITSTUN_TIMER]--;
		}
	}
	else {
		fighter_int[FIGHTER_INT_POST_HITSTUN_TIMER] = 60;
	}
	if (get_status_group() != STATUS_GROUP_ATTACK || is_actionable() || that->fighter_int[FIGHTER_INT_COMBO_COUNT] == 0) {
		fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = false;
	}

	if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0 && status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
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
	int tech_window = get_param_int(PARAM_FIGHTER, "tech_window");
	int motion_special_timer = get_param_int(PARAM_FIGHTER, "motion_special_timer");

	//Dash Input

	if (flick_dir == 6 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = dash_window;
	}
	if (flick_dir == 4 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = dash_window;
	}
	if (status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		if (flick_dir == 8 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = tech_window;
			fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_FAST;
		}
		if (flick_dir == 4 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = tech_window;
			fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_BACK;
		}
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
		if (fighter_int[FIGHTER_INT_214214_STEP] == 1 || fighter_int[FIGHTER_INT_214214_STEP] == 4) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 1) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 3) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 2) {
		fighter_int[FIGHTER_INT_236_STEP] = 1;
		fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		fighter_int[FIGHTER_INT_214_STEP] = 1;
		fighter_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
		if (fighter_int[FIGHTER_INT_623_STEP] == 1) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
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
		if (fighter_int[FIGHTER_INT_41236_STEP] == 2) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 2) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 3) {
		if (fighter_int[FIGHTER_INT_236_STEP] == 1) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_623_STEP] == 2) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 1 || fighter_int[FIGHTER_INT_236236_STEP] == 4) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 3) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 1) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 4) {
		fighter_int[FIGHTER_INT_41236_STEP] = 1;
		fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		if (fighter_int[FIGHTER_INT_214_STEP] == 2) {
			fighter_int[FIGHTER_INT_214_STEP] ++;
			fighter_int[FIGHTER_INT_214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 2 || fighter_int[FIGHTER_INT_214214_STEP] == 5) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 4) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir == 6) {
		fighter_int[FIGHTER_INT_623_STEP] = 1;
		fighter_int[FIGHTER_INT_623_TIMER] = motion_special_timer;
		fighter_int[FIGHTER_INT_63214_STEP] = 1;
		fighter_int[FIGHTER_INT_63214_TIMER] = motion_special_timer;
		if (fighter_int[FIGHTER_INT_236_STEP] == 2) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 2 || fighter_int[FIGHTER_INT_236236_STEP] == 5) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = motion_special_timer;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 4) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = motion_special_timer;
		}
	}
	if (stick_dir < 4) {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = 6;
		fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES]++;
	}

	if (stick_dir == 1 || stick_dir == 4 || stick_dir == 7) {
		fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES]++;
		fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = 6;
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
		fighter_int[FIGHTER_INT_236_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_214_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_623_TIMER] != 0) {
		fighter_int[FIGHTER_INT_623_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_623_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_41236_TIMER] != 0) {
		fighter_int[FIGHTER_INT_41236_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_41236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_63214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_63214_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_63214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_236236_TIMER] != 0) {
		fighter_int[FIGHTER_INT_236236_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_236236_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_214214_TIMER] != 0) {
		fighter_int[FIGHTER_INT_214214_TIMER] --;
	}
	else {
		fighter_int[FIGHTER_INT_214214_STEP] = 0;
	}
	if (fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] != 0) {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		fighter_int[FIGHTER_INT_HITLAG_FRAMES]--;
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && anim_kind != nullptr 
			&& !anim_kind->flag_no_hitlag_interp) {
			frame -= 0.2;
			fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 0;
		}
	}
	else {
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] != 0) {
			fighter_int[FIGHTER_INT_HITSTUN_FRAMES]--;
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
	if (fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] != 0) {
		fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER]--;
	}
	else {
		fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
	}
	if (fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] != 0) {
		fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES]--;
	}
	else {
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH], fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] + 1.0, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
}