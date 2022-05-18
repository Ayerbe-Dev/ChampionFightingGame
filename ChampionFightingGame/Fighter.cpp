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
	process_projectiles();
	process_pre_status();
	chara_main();
	process_status();
	process_post_animate();
	process_position();
	process_input();
	decrease_common_variables();
	process_post_position();
	process_post_status();
	process_ai();
}

void Fighter::fighter_post() {
	if (is_status_delay()) {
		(this->*status_script[status_kind])();
	}
	update_hitbox_pos();
	update_grabbox_pos();
	update_hurtbox_pos();
	rot.z += glm::radians(90.0);
	rot += extra_rot;
	update_jostle_rect();
}

void Fighter::process_projectiles() {
	for (int i = 0; i < num_projectiles; i++) {
		if (projectiles[i]->active) {
			projectiles[i]->projectile_main();
		}
	}
}

void Fighter::process_animate() {
	if (get_anim_broad() == "hitstun_parry" && is_anim_end) {
		reenter_last_anim();
	}

	attempted_excutes = 0;
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		frame += 0.2 / (float)fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES];
	}
	else {
		frame += rate;
	}

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length) {
			frame = 0.0;
			excute_count = 0;
			clear_grabbox_all();
			clear_hurtbox_all();
			clear_hitbox_all();
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
	if (isnan(pos.y)) {
		pos.y = 0;
	}

	update_jostle_rect();

	prev_pos = pos;
}

void Fighter::process_position() {
	Fighter* that = battle_object_manager->fighter[!id];
	update_jostle_rect();
	if (situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND
	&& !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && !that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP]) {
		if (is_collide(jostle_box, that->jostle_box)) {
			if (that->status_kind == FIGHTER_STATUS_WAIT || that->get_status_group() == STATUS_GROUP_CROUCH) {
				if (!that->add_pos(glm::vec3(that->get_local_param_float("jostle_walk_b_speed") * facing_dir, 0.0, 0.0))) {
					add_pos(glm::vec3(get_local_param_float("jostle_walk_b_speed") * that->facing_dir, 0.0, 0.0));
				}
			}
			else {
				add_pos(glm::vec3(get_local_param_float("jostle_walk_b_speed") * that->facing_dir, 0.0, 0.0));
			}
		}
	}
}

void Fighter::process_post_position() {
	Fighter* that = battle_object_manager->fighter[!id];
	if (fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, 0, 0))) {
					that->add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir, 0, 0));
				}
			}
			else {
				if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME], 0))) {
					that->add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir, 0, 0));
				}
			}
		}
	}
	else {
		fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0;
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
	if (is_status_hitstun_enable_parry()) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				change_anim("hitstun_parry", 5);
			}
			else {
				change_anim("hitstun_parry_air", 5);
			}
			battle_object_manager->fighter[!id]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -5;
		}
	}
	if (!is_status_delay()) {
		(this->*status_script[status_kind])();
	}
	move_script.move_script();
}

void Fighter::process_post_status() {
	Fighter* that = battle_object_manager->fighter[!id];

	if (get_status_group() != STATUS_GROUP_HITSTUN && status_kind != FIGHTER_STATUS_GRABBED) {
		fighter_int[FIGHTER_INT_COMBO_COUNT] = 0;
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	}
	if (get_status_group() != STATUS_GROUP_ATTACK || is_actionable() || that->fighter_int[FIGHTER_INT_COMBO_COUNT] == 0) {
		fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = false;
	}

	if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0 && status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	}

	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1 && hitboxes[i].hitbox_kind != HITBOX_KIND_BLOCK) {
			fighter_flag[FIGHTER_FLAG_HAS_ATTACK] = true;
			fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS] = true;
			break;
		}
		else {
			fighter_flag[FIGHTER_FLAG_HAS_ATTACK] = false;
		}
	}
}

void Fighter::process_input() {
	int stick_dir = get_stick_dir();
	int dash_window = get_param_int("dash_window", PARAM_FIGHTER);
	int tech_window = get_param_int("tech_window", PARAM_FIGHTER);
	int motion_special_timer = get_param_int("motion_special_timer", PARAM_FIGHTER);

	//Dash Input

	if (get_flick_dir() == 6 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = dash_window;
	}
	if (get_flick_dir() == 4 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = dash_window;
	}
	if (status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		if (get_flick_dir() == 8 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = tech_window;
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_FAST;
		}
		if (get_flick_dir() == 2 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = tech_window;
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_SLOW;
		}
	}
	if (stick_dir != 6 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = 0;
	}
	if (stick_dir != 4 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = 0;
	}

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

	prev_stick_dir = get_stick_dir();
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