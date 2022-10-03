#pragma warning(disable : 4996)
#include "Fighter.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include "Projectile.h"
#include <fstream>
#include "BattleObjectManager.h"
#include "AIManager.h"
#include "ParamAccessor.h"
#include "ThreadManager.h"

Fighter::Fighter() {
	has_model = true;
	chara_kind = CHARA_KIND_ROWAN;
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
	blockbox.rect.destroy();
	jostle_box.destroy();
	fighter_int.clear();
	fighter_float.clear();
	fighter_flag.clear();
	status_script.clear();
	enter_status_script.clear();
	exit_status_script.clear();
	model.unload_model();
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
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		frame += (0.2 / (float)(fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES])) * battle_object_manager->get_time_multiplier(id);
	}
	else {
		frame += rate * battle_object_manager->get_time_multiplier(id);
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
		if (is_collide(jostle_box, that->jostle_box)) {
			if (!((status_kind != FIGHTER_STATUS_WAIT && get_status_group() != STATUS_GROUP_CROUCH) &&
				(that->status_kind == FIGHTER_STATUS_WAIT || that->get_status_group() == STATUS_GROUP_CROUCH))) {
				if (!add_pos(glm::vec3(get_local_param_float("jostle_walk_b_speed") * that->internal_facing_dir, 0.0, 0.0))) {
					that->add_pos(glm::vec3(that->get_local_param_float("jostle_walk_b_speed") * internal_facing_dir, 0.0, 0.0));
				}
			}
		}
	}
}

void Fighter::process_post_position() {
	Fighter* that = battle_object_manager->fighter[!id];
	if (fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == FIGHTER_SITUATION_GROUND || fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL]) {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * that->facing_dir, 0, 0))) {
					if (!fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
						that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
						that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
						that->fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = true; //Necessary for making sure an opponent
						//who jumped in on us while we were at the wall only gets pushed back and not up
					}
				}
			}
			else {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * that->facing_dir, fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME], 0))) {
					if (!fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE]) {
						that->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
						that->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME];
						that->fighter_flag[FIGHTER_FLAG_PUSHBACK_FROM_OPPONENT_AT_WALL] = true;
					}
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

	if (get_status_group() != STATUS_GROUP_HITSTUN && status_kind != FIGHTER_STATUS_GRABBED) {
		that->fighter_int[FIGHTER_INT_COMBO_COUNT] = 0;
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
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
		fighter_int[FIGHTER_INT_DASH_F_BUFFER] = get_param_int("buffer_window", PARAM_FIGHTER);
	}
	if (fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4) {
		fighter_int[FIGHTER_INT_DASH_B_BUFFER] = get_param_int("buffer_window", PARAM_FIGHTER);
	}
}

void Fighter::process_input() {
	int stick_dir = get_stick_dir();
	int flick_dir = get_flick_dir();
	int dash_window = get_param_int("dash_window", PARAM_FIGHTER);
	int tech_window = get_param_int("tech_window", PARAM_FIGHTER);
	int motion_special_timer = get_param_int("motion_special_timer", PARAM_FIGHTER);

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

	//Charge Inputs (disgusting)

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
	pos.y = FLOOR_GAMECOORD;
	pos.z = 0;
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_local_param_float("health");
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
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			frame -= 0.2;
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
}