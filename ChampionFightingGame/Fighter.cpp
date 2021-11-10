#pragma warning(disable : 4996)
#include "Fighter.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>
#include <cmath>
//#include <format>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern SoundManager g_soundmanager;



Fighter::Fighter() {}

Fighter::Fighter(PlayerInfo* player_info) {
	this->player_info = player_info;
	// runs
		// yeah
	superInit(0);
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

	if (isnan(pos.y)) {
		pos.y = 0;
	}
	
	create_jostle_rect(GameCoordinate{ -20, 25 }, GameCoordinate{ 20, 0 });

	prevpos = pos;

	if (canStep()) {
		stepAnimation();
	}

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i]->id != -1) {
			projectiles[i]->projectile_main();
		}
	}

	if (fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] != FIGHTER_STATUS_MAX && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		change_status(fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS], fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_END], fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_SEPARATE]);
		fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] = FIGHTER_STATUS_MAX;
	}

	if (get_status_group() != STATUS_GROUP_HITSTUN && status_kind != FIGHTER_STATUS_GRABBED) {
		fighter_int[FIGHTER_INT_COMBO_COUNT] = 0;
	}

	if (get_anim_broad() == "hitstun_parry" && is_anim_end) {
		reenter_last_anim();
	}
	chara_main();
	playoutStatus();

	if (anim_kind->move_dir != 0) {
		if (add_pos((abs(getRenderPos(this, false).x - getRenderPos(this, true).x) / anim_kind->length + 1) * facing_dir * anim_kind->move_dir, 0, true)) {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
			if (anim_kind->force_center == 1) {
				fighter_flag[FIGHTER_FLAG_FORCE_ANIM_CENTER] = false;
			}
		}
	}
	if (fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION]) {
		add_pos(fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] / get_param_int(anim_kind->name + "_frames", param_table), 0);
	}

	create_jostle_rect(GameCoordinate{ -15, 25 }, GameCoordinate{ 15, 0 });
	Fighter* that = fighter_accessor->fighter[!id];
	if (situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND
	&& !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && !that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP]) {
		if (is_collide(jostle_box, that->jostle_box) && that->status_kind != FIGHTER_STATUS_WAIT && that->get_status_group() != STATUS_GROUP_CROUCH) {
			add_pos(get_param_float("jostle_walk_b_speed") * -1 * facing_dir, 0.0);
		}
	}

	processInput();

	prev_stick_dir = get_stick_dir();

	decrease_common_fighter_variables(this);

	if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0 && status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	}

	int width;
	int height;
	SDL_QueryTexture(base_texture, NULL, NULL, &width, &height);
	pos.x_spr_offset = width / 2;
	pos.y_spr_offset = height;

	update_hitbox_pos();
	update_grabbox_pos();
	update_hurtbox_pos();
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		if (fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				if (!add_pos(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, 0)) {
					fighter_accessor->fighter[!id]->add_pos(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir / 2, 0);
					//Note to self: Never try to use the FighterAccessor outside of a class method again, holy shit this is disgusting
				}
			}
			else {
				add_pos(fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME]);
			}
		}
	}
	else {
		fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0;
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
	create_jostle_rect(GameCoordinate{ -15, 25 }, GameCoordinate{ 15, 0 });
}

void Fighter::create_jostle_rect(GameCoordinate anchor, GameCoordinate offset) {
	anchor.x = ((anchor.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - pos.y;
	offset.x = ((offset.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - pos.y;
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	jostle_box.x = anchor.x;
	jostle_box.y = anchor.y;
	jostle_box.w = offset.x;
	jostle_box.h = offset.y;
}

void Fighter::init_projectile(int id, GameCoordinate init_pos) {
	projectiles[id]->id = this->id;
	projectiles[id]->pos.x = ((init_pos.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	projectiles[id]->pos.y = (init_pos.y - WINDOW_HEIGHT) * -1.0 - this->pos.y;

	projectiles[id]->facing_right = facing_right;
	projectiles[id]->facing_dir = facing_dir;
	projectiles[id]->change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Fighter::destroy_projectile(int id) {
	projectiles[id]->id = -1;
}

void Fighter::superInit(int id) {
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?

	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}

	if (!crash_to_debug) {
		load_anim_list();
	}
	if (!crash_to_debug) {
		load_stats();
	}
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_param_float("health");
	loadStatusScripts();
	pos.y = FLOOR_GAMECOORD;
	if (!crash_to_debug) {
		change_anim("wait", 2, 0);
	}
	status_kind = FIGHTER_STATUS_WAIT;
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

//Setup

void Fighter::load_anim_list() {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		anim_list.close();
		char buffer[55];
		sprintf(buffer, "Character %d's resource directory was incorrectly set!", chara_kind);
		player_info->crash_reason = buffer;

		crash_to_debug = true;

		return;
	}

	string name;
	string path;
	string frame_count;
	string faf;
	string force_center;
	string move_dir;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> frame_count >> faf >> force_center >> move_dir;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		animation_table[i].faf = ymlChopInt(faf);
		animation_table[i].force_center = ymlChopInt(force_center);
		animation_table[i].move_dir = ymlChopInt(move_dir);
		load_anim_map(animation_table[i].anim_map, animation_table[i].name);
		loadAnimation(&animation_table[i]);
	}
	anim_list.close();
}

/*
	Check the map file matching the current animation.If no such map exists, we'll calculate the frame's position the way we've been doing. If it
	DOES exist, open it and read where each frame's render position will be.
*/
void Fighter::load_anim_map(SDL_Rect ret[MAX_ANIM_LENGTH], string anim_dir) {
	ifstream anim_map;
	anim_map.open(resource_dir + "/anims/" + anim_dir + ".yml");

	if (anim_map.fail()) { //If the map for that animation doesn't exist, don't worry about it. The code can still function without one, it just 
		//needs one if we want a spritesheet to be across multiple lines
		SDL_Rect empty_rect[MAX_ANIM_LENGTH];
		for (int i = 0; i < MAX_ANIM_LENGTH; i++) {
			empty_rect[i] = { 0, 0, 0, 0 };
		}
		ret = empty_rect;
	}
	else {
		int initial_index;
		anim_map >> initial_index;
		int i = initial_index;
		goto SKIP_READ_FRAME;
		while (anim_map >> i) { 
			/*
				Regardless of how the animation is indexed, the anim_map's first character on each line will be a 0. Since peek() only looks at a 
				character, not the whole string, and the number of 0s can vary, we can just read the entire segment in as an int, then skip over the 
				first iteration of the loop. A bit scuffed but it's reliable.
			*/
			SKIP_READ_FRAME:

			i -= initial_index;
			string equal_sign;
			anim_map >> equal_sign; //It is followed by a single equal sign. We can use this to make sure we're looking at the right fields.
			//If for whatever reason it's missing, that means the file got messed up somewhere, so we'll just crash to the debug menu with the 
			if (equal_sign != "=") {
				char buffer[58];
				sprintf(buffer, "Error at line %d of %s.yml: Expected equal sign, found %s", i, anim_dir.c_str(), equal_sign.c_str());
				player_info->crash_reason = buffer;
				crash_to_debug = true;
				anim_map.close();
				return;
			}
			anim_map >> ret[i].x >> ret[i].y >> ret[i].w >> ret[i].h;
		}
	}
	anim_map.close();
}

void Fighter::loadStatusScripts() {
	status_script[FIGHTER_STATUS_WAIT] = &Fighter::status_wait;
	enter_status_script[FIGHTER_STATUS_WAIT] = &Fighter::enter_status_wait;
	exit_status_script[FIGHTER_STATUS_WAIT] = &Fighter::exit_status_wait;

	status_script[FIGHTER_STATUS_WALKF] = &Fighter::status_walkf;
	enter_status_script[FIGHTER_STATUS_WALKF] = &Fighter::enter_status_walkf;
	exit_status_script[FIGHTER_STATUS_WALKF] = &Fighter::exit_status_walkf;

	status_script[FIGHTER_STATUS_WALKB] = &Fighter::status_walkb;
	enter_status_script[FIGHTER_STATUS_WALKB] = &Fighter::enter_status_walkb;
	exit_status_script[FIGHTER_STATUS_WALKB] = &Fighter::exit_status_walkb;

	status_script[FIGHTER_STATUS_DASH] = &Fighter::status_dash;
	enter_status_script[FIGHTER_STATUS_DASH] = &Fighter::enter_status_dash;
	exit_status_script[FIGHTER_STATUS_DASH] = &Fighter::exit_status_dash;

	status_script[FIGHTER_STATUS_DASHB] = &Fighter::status_dashb;
	enter_status_script[FIGHTER_STATUS_DASHB] = &Fighter::enter_status_dashb;
	exit_status_script[FIGHTER_STATUS_DASHB] = &Fighter::exit_status_dashb;

	status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::status_dash_air;
	enter_status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::enter_status_dash_air;
	exit_status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::exit_status_dash_air;

	status_script[FIGHTER_STATUS_CROUCHD] = &Fighter::status_crouchd;
	enter_status_script[FIGHTER_STATUS_CROUCHD] = &Fighter::enter_status_crouchd;
	exit_status_script[FIGHTER_STATUS_CROUCHD] = &Fighter::exit_status_crouchd;

	status_script[FIGHTER_STATUS_CROUCH] = &Fighter::status_crouch;
	enter_status_script[FIGHTER_STATUS_CROUCH] = &Fighter::enter_status_crouch;
	exit_status_script[FIGHTER_STATUS_CROUCH] = &Fighter::exit_status_crouch;

	status_script[FIGHTER_STATUS_CROUCHU] = &Fighter::status_crouchu;
	enter_status_script[FIGHTER_STATUS_CROUCHU] = &Fighter::enter_status_crouchu;
	exit_status_script[FIGHTER_STATUS_CROUCHU] = &Fighter::exit_status_crouchu;

	status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::status_jumpsquat;
	enter_status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::enter_status_jumpsquat;
	exit_status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::exit_status_jumpsquat;

	status_script[FIGHTER_STATUS_JUMP] = &Fighter::status_jump;
	enter_status_script[FIGHTER_STATUS_JUMP] = &Fighter::enter_status_jump;
	exit_status_script[FIGHTER_STATUS_JUMP] = &Fighter::exit_status_jump;

	status_script[FIGHTER_STATUS_FALL] = &Fighter::status_fall;
	enter_status_script[FIGHTER_STATUS_FALL] = &Fighter::enter_status_fall;
	exit_status_script[FIGHTER_STATUS_FALL] = &Fighter::exit_status_fall;

	status_script[FIGHTER_STATUS_ATTACK] = &Fighter::status_attack;
	enter_status_script[FIGHTER_STATUS_ATTACK] = &Fighter::enter_status_attack;
	exit_status_script[FIGHTER_STATUS_ATTACK] = &Fighter::exit_status_attack;

	status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::status_attack_air;
	enter_status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::enter_status_attack_air;
	exit_status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::exit_status_attack_air;

	status_script[FIGHTER_STATUS_GRAB] = &Fighter::status_grab;
	enter_status_script[FIGHTER_STATUS_GRAB] = &Fighter::enter_status_grab;
	exit_status_script[FIGHTER_STATUS_GRAB] = &Fighter::exit_status_grab;

	status_script[FIGHTER_STATUS_THROW] = &Fighter::status_throw;
	enter_status_script[FIGHTER_STATUS_THROW] = &Fighter::enter_status_throw;
	exit_status_script[FIGHTER_STATUS_THROW] = &Fighter::exit_status_throw;

	status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::status_grab_air;
	enter_status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::enter_status_grab_air;
	exit_status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::exit_status_grab_air;

	status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::status_throw_air;
	enter_status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::enter_status_throw_air;
	exit_status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::exit_status_throw_air;

	status_script[FIGHTER_STATUS_GRABBED] = &Fighter::status_grabbed;
	enter_status_script[FIGHTER_STATUS_GRABBED] = &Fighter::enter_status_grabbed;
	exit_status_script[FIGHTER_STATUS_GRABBED] = &Fighter::exit_status_grabbed;

	status_script[FIGHTER_STATUS_THROWN] = &Fighter::status_thrown;
	enter_status_script[FIGHTER_STATUS_THROWN] = &Fighter::enter_status_thrown;
	exit_status_script[FIGHTER_STATUS_THROWN] = &Fighter::exit_status_thrown;

	status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::status_hitstun;
	enter_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::enter_status_hitstun;
	exit_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::exit_status_hitstun;

	status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::status_hitstun_air;
	enter_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::enter_status_hitstun_air;
	exit_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::exit_status_hitstun_air;

	status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::status_blockstun;
	enter_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::enter_status_blockstun;
	exit_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::exit_status_blockstun;

	status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::status_parry_start;
	enter_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::enter_status_parry_start;
	exit_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::exit_status_parry_start;

	status_script[FIGHTER_STATUS_PARRY] = &Fighter::status_parry;
	enter_status_script[FIGHTER_STATUS_PARRY] = &Fighter::enter_status_parry;
	exit_status_script[FIGHTER_STATUS_PARRY] = &Fighter::exit_status_parry;

	status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::status_crumple;
	enter_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::enter_status_crumple;
	exit_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::exit_status_crumple;

	status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::status_launch_start;
	enter_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::enter_status_launch_start;
	exit_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::exit_status_launch_start;

	status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::status_launch;
	enter_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::enter_status_launch;
	exit_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::exit_status_launch;

	status_script[FIGHTER_STATUS_CLANK] = &Fighter::status_clank;
	enter_status_script[FIGHTER_STATUS_CLANK] = &Fighter::enter_status_clank;
	exit_status_script[FIGHTER_STATUS_CLANK] = &Fighter::exit_status_clank;

	status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::status_throw_tech;
	enter_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::enter_status_throw_tech;
	exit_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::exit_status_throw_tech;

	status_script[FIGHTER_STATUS_LANDING] = &Fighter::status_landing;
	enter_status_script[FIGHTER_STATUS_LANDING] = &Fighter::enter_status_landing;
	exit_status_script[FIGHTER_STATUS_LANDING] = &Fighter::exit_status_landing;

	status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::status_landing_attack;
	enter_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::enter_status_landing_attack;
	exit_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::exit_status_landing_attack;

	status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::status_landing_hitstun;
	enter_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::enter_status_landing_hitstun;
	exit_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::exit_status_landing_hitstun;

	status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::status_knockdown_start;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::enter_status_knockdown_start;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::exit_status_knockdown_start;

	status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::status_knockdown;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::enter_status_knockdown;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::exit_status_knockdown;

	status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::status_wakeup;
	enter_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::enter_status_wakeup;
	exit_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::exit_status_wakeup;
}

//Sound

int Fighter::playCommonSE(int se) {
	return g_soundmanager.playCommonSE(se, id);
}

int Fighter::playCharaSE(int se) {
	return g_soundmanager.playCharaSE(se, id);
}

int Fighter::playVoice(int voice) {
	return g_soundmanager.playVoice(voice, id);
}

void Fighter::endCommonSE(int se) {
	g_soundmanager.endCommonSE(se, id);
}

void Fighter::endCharaSE(int se) {
	g_soundmanager.endCharaSE(se, id);
}

void Fighter::endVoice(int voice) {
	g_soundmanager.endVoice(voice, id);
}

//Inputs

void Fighter::processInput() {
	int stick_dir = get_stick_dir();

	//Dash Input

	if (get_flick_dir() == 6 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = DASH_WINDOW;
	}
	if (get_flick_dir() == 4 && prev_stick_dir == 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = DASH_WINDOW;
	}
	if (status_kind != FIGHTER_STATUS_KNOCKDOWN) {
		if (get_flick_dir() == 8 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = TECH_WINDOW;
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_FAST;
		}
		if (get_flick_dir() == 2 && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = TECH_WINDOW;
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_SLOW;
		}
	}
	if (stick_dir != 6 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_F_WINDOW] = 0;
	}
	if (stick_dir != 4 && stick_dir != 5) {
		fighter_int[FIGHTER_INT_DASH_B_WINDOW] = 0;
	}

	//Special Inputs

	//Motion Inputs

	if (stick_dir == 1) {
		if (fighter_int[FIGHTER_INT_214_STEP] == 1) {
			fighter_int[FIGHTER_INT_214_STEP] ++;
			fighter_int[FIGHTER_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 1 || fighter_int[FIGHTER_INT_214214_STEP] == 4) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 1) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 3) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 2) {
		fighter_int[FIGHTER_INT_236_STEP] = 1;
		fighter_int[FIGHTER_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		fighter_int[FIGHTER_INT_214_STEP] = 1;
		fighter_int[FIGHTER_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		if (fighter_int[FIGHTER_INT_623_STEP] == 1) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 1) {
			fighter_int[FIGHTER_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 0 || fighter_int[FIGHTER_INT_214214_STEP] == 3) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 1) {
			fighter_int[FIGHTER_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 0 || fighter_int[FIGHTER_INT_236236_STEP] == 3) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 2) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 2) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 3) {
		if (fighter_int[FIGHTER_INT_236_STEP] == 1) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_623_STEP] == 2) {
			fighter_int[FIGHTER_INT_623_STEP] ++;
			fighter_int[FIGHTER_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 1 || fighter_int[FIGHTER_INT_236236_STEP] == 4) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 3) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 1) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 4) {
		fighter_int[FIGHTER_INT_41236_STEP] = 1;
		fighter_int[FIGHTER_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		if (fighter_int[FIGHTER_INT_214_STEP] == 2) {
			fighter_int[FIGHTER_INT_214_STEP] ++;
			fighter_int[FIGHTER_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_214214_STEP] == 2 || fighter_int[FIGHTER_INT_214214_STEP] == 5) {
			fighter_int[FIGHTER_INT_214214_STEP] ++;
			fighter_int[FIGHTER_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_63214_STEP] == 4) {
			fighter_int[FIGHTER_INT_63214_STEP] ++;
			fighter_int[FIGHTER_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 6) {
		fighter_int[FIGHTER_INT_623_STEP] = 1;
		fighter_int[FIGHTER_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		fighter_int[FIGHTER_INT_63214_STEP] = 1;
		fighter_int[FIGHTER_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		if (fighter_int[FIGHTER_INT_236_STEP] == 2) {
			fighter_int[FIGHTER_INT_236_STEP] ++;
			fighter_int[FIGHTER_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_236236_STEP] == 2 || fighter_int[FIGHTER_INT_236236_STEP] == 5) {
			fighter_int[FIGHTER_INT_236236_STEP] ++;
			fighter_int[FIGHTER_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (fighter_int[FIGHTER_INT_41236_STEP] == 4) {
			fighter_int[FIGHTER_INT_41236_STEP] ++;
			fighter_int[FIGHTER_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
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

	if (check_button_on(BUTTON_START)) {
		pos.y = FLOOR_GAMECOORD;
		if (id == 0) {
			pos.x = -200.0;
		}
		else {
			pos.x = 200.0;
		}
	}
}

bool Fighter::check_button_on(u32 button) {
	return player_info->check_button_on(button);
}

bool Fighter::check_button_input(u32 button) {
	return player_info->check_button_input(button);
}

bool Fighter::check_button_input(u32 button[], int length, int min_matches) {
	return player_info->check_button_input(button, length, min_matches);
}

bool Fighter::check_button_trigger(u32 button) {
	return player_info->check_button_trigger(button);
}

bool Fighter::check_button_release(u32 button) {
	return player_info->check_button_release(button);
}

int Fighter::get_stick_dir() {
	if (check_button_on(BUTTON_UP) && !check_button_on(BUTTON_DOWN)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 9;
			}
			else {
				return 7;
			}
		}
		else {
			return 8;
		}
	}
	else if (check_button_on(BUTTON_DOWN) && !check_button_on(BUTTON_UP)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 3;
			}
			else {
				return 1;
			}
		}
		else {
			return 2;
		}
	}
	else {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 6;
			}
			else {
				return 4;
			}
		}
		else {
			return 5;
		}
	}
}

int Fighter::get_flick_dir() {
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

int Fighter::get_special_input(int special_kind, u32 button, int charge_frames) {
	int button_check = 0;
	bool input_check = false;

	if (button == BUTTON_MACRO_P) {
		u32 ex_buttons[3] = { BUTTON_LP, BUTTON_MP, BUTTON_HP };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	else if (button == BUTTON_MACRO_K) {
		u32 ex_buttons[3] = { BUTTON_LK, BUTTON_MK, BUTTON_HK };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	else {
		if (check_button_input(button) || check_button_release(button)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	if (button_check) {
		if (special_kind == SPECIAL_KIND_236) {
			if (fighter_int[FIGHTER_INT_236_STEP] == 2 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_236_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_236_STEP] = 0;
				fighter_int[FIGHTER_INT_236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214) {
			if (fighter_int[FIGHTER_INT_214_STEP] == 2 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_214_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_214_STEP] = 0;
				fighter_int[FIGHTER_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_623) {
			if (fighter_int[FIGHTER_INT_623_STEP] == 2 && get_stick_dir() == 3) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_623_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_623_STEP] = 0;
				fighter_int[FIGHTER_INT_623_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_41236) {
			if (fighter_int[FIGHTER_INT_41236_STEP] == 4 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_41236_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_41236_STEP] = 0;
				fighter_int[FIGHTER_INT_41236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_63214) {
			if (fighter_int[FIGHTER_INT_63214_STEP] == 4 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_63214_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_63214_STEP] = 0;
				fighter_int[FIGHTER_INT_63214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_236236) {
			if (fighter_int[FIGHTER_INT_236236_STEP] == 5 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_236236_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_236236_STEP] = 0;
				fighter_int[FIGHTER_INT_236236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214214) {
			if (fighter_int[FIGHTER_INT_214214_STEP] == 5 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_214214_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_214_STEP] = 0;
				fighter_int[FIGHTER_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_DOWN) {
			input_check = (fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 8);
			if (input_check && get_flick_dir() == 8) {
				button_check == SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_BACK) {
			input_check = (fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 6);
			if (input_check && get_flick_dir() == 6) {
				button_check == SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = 0;
			}
		}
	}
	if (input_check) {
		return button_check;
	}
	else {
		return SPECIAL_INPUT_NONE;
	}
}

bool Fighter::get_normal_cancel(int attack_kind, u32 button, int situation_kind) {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == attack_kind && check_button_input(button) && situation_kind == this->situation_kind) {
		if (button == BUTTON_LP) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
		}
		if (button == BUTTON_MP) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
		}
		if (button == BUTTON_HP) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
		}
		if (button == BUTTON_LK) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
		}
		if (button == BUTTON_MK) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
		}
		if (button == BUTTON_HK) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
		}
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] <= BUFFER_WINDOW) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				return change_status_after_hitlag(FIGHTER_STATUS_ATTACK, true, false);
			}
			else {
				return change_status_after_hitlag(FIGHTER_STATUS_ATTACK_AIR, true, false);
			}
		}
	}
	else {
		return false;
	}
}

//Param

int Fighter::get_param_int_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_int(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_int(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_int(param + "_h", param_table);
	}
	else {
		return get_param_int(param + "_ex", param_table);
	}
}

float Fighter::get_param_float_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_float(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_float(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_float(param + "_h", param_table);
	}
	else {
		return get_param_float(param + "_ex", param_table);
	}
}

bool Fighter::get_param_bool_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_bool(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_bool(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_bool(param + "_h", param_table);
	}
	else {
		return get_param_bool(param + "_ex", param_table);
	}
}

string Fighter::get_param_string_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_string(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_string(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_string(param + "_h", param_table);
	}
	else {
		return get_param_string(param + "_ex", param_table);
	}
}

//Position

bool Fighter::add_pos(float x, float y, bool prev) {
	Fighter* that = fighter_accessor->fighter[!id];
	float prev_x = x;
	if (x == -0.0) {
		x = 0.0;
	}
	int prev_y = y;
	if (y == -0.0) {
		y = 0.0;
	}
	if (isnan(x) || isnan(y)) {
		char buffer_1[82];
		sprintf(buffer_1, "Player %d (Me) Status: %d. Pos X: %f, Pos Y: %f. You probably", (id + 1), status_kind, prev_x, (float)prev_y);
		char buffer_2[89];
		sprintf(buffer_2, "accidentally divided by 0 somewhere in that status. Player %d (Not Me) Status: %d. (Add)", ((!id) + 1), that->status_kind);
		fighter_accessor->fighter[0]->player_info->crash_reason = buffer_1;
		fighter_accessor->fighter[1]->player_info->crash_reason = buffer_2;

		crash_to_debug = true;
		return false;
	}

	GameCoordinate prevpos = pos;
	bool ret = true;
	bool opponent_right = pos.x > that->pos.x;
	pos.x += x;
	pos.y += y;
	if (pos.x + pos.x_spr_offset / 2 > WINDOW_WIDTH / 2) {
		if (prev) {
			pos.x = prevpos.x;
		}
		else {
			pos.x = WINDOW_WIDTH / 2 - pos.x_spr_offset / 2;
		}
		if (get_param_bool("has_wallbounce") && facing_right && status_kind == FIGHTER_STATUS_JUMP) {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (pos.x - pos.x_spr_offset / 2 < WINDOW_WIDTH / -2) {
		if (prev) {
			pos.x = prevpos.x;
		}
		else {
			pos.x = WINDOW_WIDTH / -2 + pos.x_spr_offset / 2;
		}
		if (get_param_bool("has_wallbounce") && !facing_right && status_kind == FIGHTER_STATUS_JUMP) {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (pos.y < 0) {
		if (prev) {
			pos.y = prevpos.y;
		}
		else {
			pos.y = 0;
		}
		ret = false;
	}
	if (pos.y > WINDOW_HEIGHT) {
		if (prev) {
			pos.y = prevpos.y;
		}
		else {
			pos.y = WINDOW_HEIGHT;
		}
		ret = false;
	}
	float opponent_x = that->pos.x + (that->pos.x_spr_offset * that->facing_dir / -2);
	float compare_x = pos.x + (pos.x_spr_offset * facing_dir / -2);
	float x_distance = std::max(opponent_x, compare_x) - std::min(opponent_x, compare_x);
	if (x_distance > CAMERA_MAX_ZOOM_OUT) {
		pos.x = prevpos.x;
		ret = false;
	}
	bool new_opponent_right = pos.x > that->pos.x;
	if (opponent_right != new_opponent_right && !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND) {
		pos.x = prevpos.x;
		ret = false;
	}

	return ret;
}

bool Fighter::set_pos(float x, float y, bool prev) {
	Fighter* that = fighter_accessor->fighter[!id];
	float prev_x = x;
	if (x == -0.0) {
		x = 0.0;
	}
	int prev_y = y;
	if (y == -0.0) {
		y = 0.0;
	}
	if (isnan(x) || isnan(y) || y != prev_y) {
		char buffer_1[82];
		sprintf(buffer_1, "Player %d (Me) Status: %d. Pos X: %f, Pos Y: %f. You probably", (id + 1), status_kind, prev_x, (float)prev_y);
		char buffer_2[89];
		sprintf(buffer_2, "accidentally divided by 0 somewhere in that status. Player %d (Not Me) Status: %d. (Set)", ((!id) + 1), that->status_kind);
		fighter_accessor->fighter[0]->player_info->crash_reason = buffer_1;
		fighter_accessor->fighter[1]->player_info->crash_reason = buffer_2;

		crash_to_debug = true;
		return false;
	}
	GameCoordinate prevpos = pos;
	bool ret = true;
	bool opponent_right = pos.x > that->pos.x;
	pos.x = x;
	pos.y = y;
	if (pos.x + pos.x_spr_offset / 2 > WINDOW_WIDTH / 2) {
		if (prev) {
			pos.x = prevpos.x;
		}
		else {
			pos.x = WINDOW_WIDTH / 2 - pos.x_spr_offset / 2;
		}
		ret = false;
	}
	if (pos.x - pos.x_spr_offset / 2 < WINDOW_WIDTH / -2) {
		if (prev) {
			pos.x = prevpos.x;
		}
		else {
			pos.x = WINDOW_WIDTH / -2 + pos.x_spr_offset / 2;
		}
		ret = false;
	}
	if (pos.y < 0) {
		if (prev) {
			pos.y = prevpos.y;
		}
		else {
			pos.y = 0;
		}
		ret = false;
	}
	if (pos.y > WINDOW_HEIGHT) {
		if (prev) {
			pos.y = prevpos.y;
		}
		else {
			pos.y = WINDOW_HEIGHT;
		}		
		ret = false;
	}
	float opponent_x = that->pos.x + (that->pos.x_spr_offset * that->facing_dir / -2);
	float compare_x = pos.x + (pos.x_spr_offset * facing_dir / -2);
	float x_distance = std::max(opponent_x, compare_x) - std::min(opponent_x, compare_x);
	if (x_distance > CAMERA_MAX_ZOOM_OUT) {
		pos.x = prevpos.x;
		ret = false;
	}
	bool new_opponent_right = pos.x > that->pos.x;
	if (opponent_right != new_opponent_right && !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND) {
		pos.x = prevpos.x;
		ret = false;
	}

	return ret;
}

//Opponent

void Fighter::set_opponent_offset(GameCoordinate offset, int frames) {
	GameCoordinate new_offset;
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES] = frames + 1;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::set_opponent_offset(GameCoordinate offset) {
	GameCoordinate new_offset;
	int frames = fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES];
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::change_opponent_status(u32 status_kind) {
	if ((this->status_kind == FIGHTER_STATUS_THROW || this->status_kind == FIGHTER_STATUS_THROW_AIR) && status_kind == FIGHTER_STATUS_THROWN) {
		fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = true;
	}
	fighter_accessor->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage, float facing_dir, float x_speed, float y_speed) {
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_HEALTH] -= damage;
	fighter_accessor->fighter[!id]->facing_dir = facing_dir * this->facing_dir * -1;
	fighter_accessor->fighter[!id]->facing_right = (facing_dir * this->facing_dir * -1) > 0;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = x_speed * this->facing_dir;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = y_speed;
}

void Fighter::set_opponent_angle(double angle) {
	fighter_accessor->fighter[!id]->angle = angle * facing_dir;
}

void Fighter::set_opponent_thrown_ticks() {
	fighter_accessor->fighter[!id]->max_ticks = (((anim_kind->length - 1) / max_ticks) / (fighter_accessor->fighter[!id]->anim_kind->length - 1)) - 1;
	fighter_accessor->fighter[!id]->ticks = 0;
}

void Fighter::change_opponent_anim(string anim_kind, int frame_rate, int entry_frame) {
	fighter_accessor->fighter[!id]->change_anim(anim_kind, frame_rate, entry_frame);
}

//Hitbox

/**
Creates a persistent hitbox until the status for the character changes.
\param id the uuid for the hitbox, call clear_hitbox(id) to delete it
\param anchor the x0 y0 values for the hitbox
\param offset the x1 y1 values for the hitbox
*/
void Fighter::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
								 GameCoordinate offset, int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag,
								 int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
								 int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y,
								 float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool use_player_pos) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
							  meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
							  attack_level, hit_pushback, block_pushback, clank_kind, juggle_set, max_juggle, hit_status, counterhit_status,
							  counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, continue_launch, use_player_pos);
	}
}

//Grabbox

void Fighter::new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
								  u32 defender_status_if_hit, bool use_player_pos) {

	if (id < 10) {
		grabboxes[id] = Grabbox(this, id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
	}
}

//Hurtbox

void Fighter::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind) {
	if (id < 10) {
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}

//Transitions

bool Fighter::is_actionable() {
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0 && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && status_kind != FIGHTER_STATUS_GRABBED && status_kind != FIGHTER_STATUS_THROWN) {
		if (anim_kind->faf == -1) {
			return render_frame >= anim_kind->length;
		}
		else {
			return frame >= anim_kind->faf;
		}
	}
	else {
		return false;
	}
}

bool Fighter::can_kara() {
	if (((fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) || kara_enabled) {
		return true;
	}
	else {
		return false;
	}
}

//Animations

void Fighter::reenter_last_anim() {
	max_ticks = prev_anim_max_ticks;
	render_frame = prev_anim_render_frame;
	frame = prev_anim_frame;
	set_current_move_script(prev_anim_kind->name);
	startAnimation(prev_anim_kind);
}

bool Fighter::change_anim(string animation_name, int frame_rate, int entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_max_ticks = max_ticks;
	prev_anim_frame = frame;
	prev_anim_render_frame = render_frame;

	int anim_to_use = -1;
	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			if (frame_rate != -1) {
				max_ticks = frame_rate;
				frame = entry_frame;
				render_frame = entry_frame;
			}
			else {
				max_ticks = ceil((float)entry_frame / (float)(animation_table[i].length));
				frame = 0;
				render_frame = 0;
			}
			ticks = 0;

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

void Fighter::startAnimation(Animation* animation) {
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
	fighter_flag[FIGHTER_FLAG_FORCE_ANIM_CENTER] = (anim_kind->force_center != 0);
	frame_rect = getFrame(render_frame, anim_kind);
	pos.x_anim_offset = frame_rect.w / 2;
	pos.y_anim_offset = frame_rect.h;
}

bool Fighter::canStep() {
	attempted_excutes = 0;
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		frame++;
		ticks++;

		if (ticks >= max_ticks) {
			ticks = 0;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void Fighter::stepAnimation() {
	int last_frame = render_frame;
	frame_rect = getFrame(render_frame, anim_kind);
	if (render_frame == anim_kind->length) {
		render_frame = 0;
		frame = 0;
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
		render_frame++;
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
 
//Status

bool Fighter::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		kara_enabled = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
		fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = 0.0;
		if (call_end_status) {
			if (status_kind < FIGHTER_STATUS_MAX) {
				(this->*exit_status_script[status_kind])();
			}
			else {
				chara_exit_status();
			}
		}
		status_kind = new_status_kind;
		if (status_kind < FIGHTER_STATUS_MAX) {
			(this->*enter_status_script[status_kind])();
		}
		else {
			chara_enter_status();
		}
		return true;
	}
	else {
		return false;
	}
}

bool Fighter::change_status_after_hitlag(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		return change_status(new_status_kind, call_end_status, require_different_status);
	}
	else {
		if (new_status_kind != status_kind || !require_different_status) {
			fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] = new_status_kind;
			fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_END] = call_end_status;
			fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_SEPARATE] = require_different_status;
			return true;
		}
		else {
			return false;
		}
	}
}

void Fighter::playoutStatus() {
	if (is_status_hitstun_enable_parry()) {
		u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				change_anim("hitstun_parry", 5);
			}
			else {
				change_anim("hitstun_parry_air", 5);
			}
			fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -5;
		}
	}
	if (status_kind < FIGHTER_STATUS_MAX) {
		(this->*status_script[status_kind])();
	}
	else {
		chara_status();
	}
	move_script();
}

bool Fighter::common_ground_status_act() {
	if (specific_ground_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			u32 grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_GRAB));
			}
			if (check_button_input(BUTTON_LP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
				}
			}
			if (check_button_input(BUTTON_LK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
				}
			}
			u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_PARRY_START));
			}
			if (check_button_input(BUTTON_MP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
				}
			}
			if (check_button_input(BUTTON_MK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
				}
			}
			if (check_button_input(BUTTON_HP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
				}
			}
			if (check_button_input(BUTTON_HK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
				}
			}
			return change_status(FIGHTER_STATUS_ATTACK, true, false);
		}
		if (get_stick_dir() == 6) {
			if (fighter_int[FIGHTER_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6) {
				return change_status(FIGHTER_STATUS_DASH);
			}
			else {
				return change_status(FIGHTER_STATUS_WALKF);
			}
		}
		if (get_stick_dir() == 4) {
			if (fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4) {
				return change_status(FIGHTER_STATUS_DASHB);
			}
			else {
				return change_status(FIGHTER_STATUS_WALKB);
			}
		}
		if (get_stick_dir() > 6) {
			return change_status(FIGHTER_STATUS_JUMPSQUAT);
		}
		if (get_stick_dir() < 4 && status_kind != FIGHTER_STATUS_CROUCH) {
			if (status_kind == FIGHTER_STATUS_ATTACK && fighter_int[FIGHTER_INT_ATTACK_KIND] >= ATTACK_KIND_HK) {
				return change_status(FIGHTER_STATUS_CROUCH);
			}
			else {
				return change_status(FIGHTER_STATUS_CROUCHD);
			}
		}
	}
	return false;
}

bool Fighter::common_air_status_act() {
	if (specific_air_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			u32 grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_GRAB_AIR, false, false));
			}
			if (check_button_input(BUTTON_LP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
			if (check_button_input(BUTTON_LK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
			u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_PARRY_START, false, false));
			}
			if (check_button_input(BUTTON_MP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
			}
			if (check_button_input(BUTTON_MK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
			}
			if (check_button_input(BUTTON_HP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
			}
			if (check_button_input(BUTTON_HK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
			}
			return change_status(FIGHTER_STATUS_ATTACK_AIR, false, false);
		}
		if (fighter_int[FIGHTER_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6 && get_param_bool("has_airdash") && !fighter_flag[FIGHTER_FLAG_USED_AIRDASH]) {
			fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 1;
			fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
			return change_status(FIGHTER_STATUS_DASH_AIR);
		}
		if (fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4 && get_param_bool("has_airdash") && !fighter_flag[FIGHTER_FLAG_USED_AIRDASH]) {
			fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 2;
			fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
			return change_status(FIGHTER_STATUS_DASH_AIR);
		}
	}
	return false;
}

bool Fighter::common_air_status_general() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
		}
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, 0);
		}
		add_pos(0, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0) {
		fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	}
	return false;
}

u32 Fighter::get_status_group() {
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH):
		case (FIGHTER_STATUS_CRUMPLE):
		case (FIGHTER_STATUS_THROWN): {
			return STATUS_GROUP_HITSTUN;
		}
		break;
		case (FIGHTER_STATUS_CROUCHD):
		case (FIGHTER_STATUS_CROUCH):
		case (FIGHTER_STATUS_CROUCHU): {
			return STATUS_GROUP_CROUCH;
		}
		break;
		case (FIGHTER_STATUS_WAIT):
		case (FIGHTER_STATUS_FALL):
		case (FIGHTER_STATUS_WALKF):
		case (FIGHTER_STATUS_WALKB):
		case (FIGHTER_STATUS_JUMP):
		{
			return STATUS_GROUP_NO_RENDER_PRIORITY;
		} 
		break;
		default: {
			return STATUS_GROUP_NORMAL;
		}
		break;
	}
}

/// When it's time to transition out of the current status, this changes to the proper status. If no arg is given, this will change status to wait or
/// fall based on your situation_kind
bool Fighter::is_status_end(u32 post_status_kind, bool call_end_status, bool require_different_status) {
	if (is_anim_end) {
		if (situation_kind == FIGHTER_SITUATION_AIR && post_status_kind == FIGHTER_STATUS_WAIT) {
			post_status_kind = FIGHTER_STATUS_FALL;
		}
		return change_status(post_status_kind, call_end_status, require_different_status);
	}
	else if (is_actionable()) {
		if (situation_kind == FIGHTER_SITUATION_GROUND) {
			return common_ground_status_act();
		}
		else {
			return common_air_status_act();
		}
	}
	return false;
}

bool Fighter::is_status_hitstun_enable_parry() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0 || fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] == -5) {
		return false;
	}
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_LAUNCH): {
			return true;
		}
		break;
		default: {
			return false;
		}
		break;
	}
}

// STATUS FUNCTIONS

/*
	 ..=====.. |==|
	|| sus  || |= |
 _  ||      || |^*| _
|=| o==,===,=o |__||=|
|_|  ________)~`)  |_|
	[========]  ()
*/

void Fighter::status_wait() {
	if (common_ground_status_act()) {
		return;
	}
}


void Fighter::enter_status_wait() {
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait");


	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	common_ground_status_act();
}

void Fighter::exit_status_wait() {}

void Fighter::status_walkf() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (is_collide(jostle_box, fighter_accessor->fighter[!id]->jostle_box)) {
		add_pos(get_param_float("jostle_walk_f_speed") * facing_dir, 0);
	}
	else {
		add_pos(get_param_float("walk_f_speed") * facing_dir, 0);
	}
}

void Fighter::enter_status_walkf() {
	change_anim("walk_f");
}

void Fighter::exit_status_walkf() {}

void Fighter::status_walkb() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (fighter_flag[FIGHTER_FLAG_PROX_GUARD]) {
		if (anim_kind->name != "stand_block") {
			change_anim("stand_block");
		}
	}
	else {
		if (anim_kind->name == "stand_block") {
			change_anim("walk_b");
		}
		add_pos(get_param_float("walk_f_speed") * facing_dir * -1, 0);
	}
}

void Fighter::enter_status_walkb() {
	change_anim("walk_b");
}

void Fighter::exit_status_walkb() {}

void Fighter::status_dash() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_param_float("dash_f_speed") * facing_dir, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_param_float("walk_f_speed") + get_param_float("dash_f_speed")) * facing_dir / 2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 4) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASHB, false);
				return;
			}
		}
		else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
		}
	}
}

void Fighter::enter_status_dash() {
	change_anim("dash_f");
}

void Fighter::exit_status_dash() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Fighter::status_dashb() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_b_accel_frame");
	int max_frame = min_frame + get_param_int("dash_b_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_param_float("dash_b_speed") * facing_dir * -1, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_param_float("walk_b_speed") + get_param_float("dash_b_speed")) * facing_dir / -2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 6) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
	}
}

void Fighter::enter_status_dashb() {
	change_anim("dash_b");
}

void Fighter::exit_status_dashb() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Fighter::status_dash_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	if (frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0.0;
	}
	else {
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed")) {
				fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * 0.95, fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED]);
			}
		}
		else if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * 0.95, fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED]);
			}
		}
	}
	if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
	else {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.0, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
}

void Fighter::enter_status_dash_air() {
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 1) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_f_speed");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
		change_anim("dash_f");
	}
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 2) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_b_speed");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		change_anim("dash_b");
	}
}

void Fighter::exit_status_dash_air() {

}


void Fighter::status_crouchd() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() > 3) {
		change_status(FIGHTER_STATUS_CROUCHU);
		return;
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_CROUCH);
		return;
	}
}

void Fighter::enter_status_crouchd() {
	change_anim("crouch_d");
}

void Fighter::exit_status_crouchd() {}

void Fighter::status_crouch() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_CROUCHU);
		return;
	}
}

void Fighter::enter_status_crouch() {
	change_anim("crouch");
}

void Fighter::exit_status_crouch() {}

void Fighter::status_crouchu() {
	if (common_ground_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
	}
}

void Fighter::enter_status_crouchu() {
	change_anim("crouch_u");
}

void Fighter::exit_status_crouchu() {}

void Fighter::status_jumpsquat() {
	if (frame == 4 || is_anim_end) {
		change_status(FIGHTER_STATUS_JUMP);
		return;
	}
}

void Fighter::enter_status_jumpsquat() {
	change_anim("jump_squat");
}

void Fighter::exit_status_jumpsquat() {
	fighter_flag[FIGHTER_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = false;
	situation_kind = FIGHTER_SITUATION_AIR;
}

void Fighter::status_jump() {
	if (is_status_end(FIGHTER_STATUS_FALL, false)) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	common_air_status_general();
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > 0;
}

void Fighter::enter_status_jump() {
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1) {
		change_anim("jump_b");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3) {
		change_anim("jump_f");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
	}
	else {
		change_anim("jump");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if (fighter_flag[FIGHTER_FLAG_SHORT_HOP]) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed_s");
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed");
	}
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("jump_x_speed");
	common_air_status_act();
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
}

void Fighter::exit_status_jump() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void Fighter::status_fall() {
	if (common_air_status_act()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_fall() {
	change_anim("fall");
}


void Fighter::exit_status_fall() {
	fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void Fighter::status_attack() {
	if (specific_status_attack()) {
		return;
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] > ATTACK_KIND_HK) { //If we're in a crouching attack, change to the crouch animation on animation end. 
		if (is_status_end(FIGHTER_STATUS_CROUCH)) {
			return;
		}
	}
	else { //Otherwise, just transition to wait
		if (is_status_end()) {
			return;
		}
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(FIGHTER_STATUS_GRAB);
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (fighter_flag[FIGHTER_FLAG_HAS_ATTACK] && fighter_float[FIGHTER_FLOAT_SUPER_METER] >= 30.0) {
				fighter_float[FIGHTER_FLOAT_SUPER_METER] -= 30.0;
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
			else if (!fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
		}
	}
}

void Fighter::enter_status_attack() {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("stand_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("stand_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("stand_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("stand_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("stand_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("stand_hk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CLP) {
		change_anim("crouch_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP) {
		change_anim("crouch_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CHP) {
		change_anim("crouch_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CLK) {
		change_anim("crouch_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		change_anim("crouch_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CHK) {
		change_anim("crouch_hk");
	}
}

void Fighter::exit_status_attack() {}

void Fighter::status_attack_air() {
	if (specific_status_attack()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING_ATTACK);
		return;
	}
	if (is_status_end()) {
		return;
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(FIGHTER_STATUS_GRAB_AIR);
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (!fighter_flag[FIGHTER_FLAG_HAS_ATTACK] && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
		}
	}
	common_air_status_general();
}

void Fighter::enter_status_attack_air() {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("jump_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("jump_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("jump_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("jump_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("jump_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("jump_hk");
	}
}

void Fighter::exit_status_attack_air() {}

void Fighter::status_grab() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_grab() {
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab");
}

void Fighter::exit_status_grab() {}

void Fighter::status_throw() {
	if (!fighter_flag[FIGHTER_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] });
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_throw() {
	fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = false;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b");
	}
	else {
		change_anim("throw_f");
	}
	if (!add_pos(get_param_float(anim_kind->name + "_move_offset", param_table) * facing_dir, 0, true)) {
		if (pos.x > 0) {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / 2) - pos.x;
		}
		else {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / -2) + pos.x;
		}
		if (change_anim_inherit_attributes(anim_kind->name + "_stationary", false)) {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = true;
		}
		else {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		}
	}
}

void Fighter::exit_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	if (anim_kind->name == "throw_b") {
		facing_right = !facing_right;
		facing_dir *= -1;
	}
}

void Fighter::status_grab_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_grab_air() {
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_air");
}

void Fighter::exit_status_grab_air() {}

void Fighter::status_throw_air() {
	if (!fighter_flag[FIGHTER_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] });
	}
	if (pos.y <= FLOOR_GAMECOORD) {
		set_pos(pos.x, FLOOR_GAMECOORD);
		situation_kind = FIGHTER_SITUATION_GROUND;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_throw_air() {
	fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = false;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b_air");
	}
	else {
		change_anim("throw_f_air");
	}
	if (!add_pos(get_param_float(anim_kind->name + "_move_offset", param_table) * facing_dir, 0, true)) {
		if (pos.x > 0) {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / 2) - pos.x;
		}
		else {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / -2) + pos.x;
		}
		if (change_anim_inherit_attributes(anim_kind->name + "_stationary", false)) {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = true;
		}
		else {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		}
	}
}

void Fighter::exit_status_throw_air() {
	if (anim_kind->name == "throw_b_air") {
		facing_dir *= -1;
		facing_right = !facing_right;
	}
}

void Fighter::status_grabbed() {
	if (fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] != 0 && fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] != 0) {
		if (fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES] != 0) {
			add_pos(fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] * -1);
			fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES]--;
		}
		else {
			set_pos(fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y]);
		}
	}
}

void Fighter::enter_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
}

void Fighter::exit_status_grabbed() {
	angle = 0;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = 0;
	fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
}

void Fighter::status_thrown() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}

void Fighter::enter_status_thrown() {
	situation_kind = FIGHTER_SITUATION_AIR;
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	}
	change_anim("knockdown_start", -1, get_launch_ticks());
}

void Fighter::exit_status_thrown() {

}

void Fighter::status_hitstun() {
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
		if (get_stick_dir() < 4) {
			if (change_status(FIGHTER_STATUS_CROUCH)) {
				return;
			}
		}
		else {
			if (change_status(FIGHTER_STATUS_WAIT)) {
				return;
			}
		}
	}
}

void Fighter::enter_status_hitstun() {
	if (get_stick_dir() < 4) {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("crouch_hitstun_l", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("crouch_hitstun_m", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("crouch_hitstun_h", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("stand_hitstun_l", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("stand_hitstun_m", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_hitstun_h", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
}

void Fighter::exit_status_hitstun() {}

void Fighter::status_hitstun_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING_HITSTUN);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
		change_status(FIGHTER_STATUS_FALL, false);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
}

void Fighter::enter_status_hitstun_air() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("jump_hitstun", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
}


void Fighter::exit_status_hitstun_air() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] /= 2;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] /= 2;
}

void Fighter::status_blockstun() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			if (get_stick_dir() < 4) {
				if (change_status(FIGHTER_STATUS_CROUCH)) {
					return;
				}
			}
			else {
				if (change_status(FIGHTER_STATUS_WAIT)) {
					return;
				}
			}
		}
	}
	else {
		if (pos.y < 50.0) {
			change_status(FIGHTER_STATUS_LANDING_HITSTUN);
			return;
		}
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
			}
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.5, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
		}
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			change_status(FIGHTER_STATUS_FALL);

		}
	}
}


void Fighter::enter_status_blockstun() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		if (get_stick_dir() < 4) {
			change_anim("crouch_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH) {
			change_anim("high_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		change_anim("jump_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	}
}

void Fighter::exit_status_blockstun() {}

void Fighter::status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (pos.y < FLOOR_GAMECOORD) {
			change_status(FIGHTER_STATUS_LANDING);
			return;
		}
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		common_air_status_general();
	}
	else {
		if (get_stick_dir() < 4) {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
		}
		else if (get_stick_dir() < 7) {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
		}
		else {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
		}
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_start_air");
	}
	else {
		change_anim("parry_start");
	}
}

void Fighter::exit_status_parry_start() {}

void Fighter::status_parry() {
	if (beginning_hitlag(1) || ending_hitlag(2)) {
		forceStepThroughHitlag();
	}
	if (situation_kind == FIGHTER_SITUATION_GROUND && fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
		if (is_status_end(FIGHTER_STATUS_CROUCH)) {
			return;
		}
	}
	else {
		if (is_status_end()) {
			return;
		}
	}
}

void Fighter::enter_status_parry() {
	fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_air");
	}
	else {
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
			change_anim("parry_low");
		}
		else if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID) {
			change_anim("parry_mid");
		}
		else {
			change_anim("parry_high");
		}
	}
}

void Fighter::exit_status_parry() {
	fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MAX;
}

void Fighter::status_crumple() {
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
}

void Fighter::enter_status_crumple() {
	change_anim("crumple");
}

void Fighter::exit_status_crumple() {
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = false;
}

void Fighter::status_launch_start() {
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_LAUNCH);
	}
}

void Fighter::enter_status_launch_start() {
	change_anim("launch_start");
}

void Fighter::exit_status_launch_start() {
	situation_kind = FIGHTER_SITUATION_AIR;
}

void Fighter::status_launch() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 1) {
		max_ticks = ceil((float)get_launch_ticks() / (float)(anim_kind->length));
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] * -1.0) {
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY];
		}
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
		if (pos.y < FLOOR_GAMECOORD) {
			change_status(FIGHTER_STATUS_KNOCKDOWN);
			return;
		}
	}
}

void Fighter::enter_status_launch() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	}
	change_anim("launch", -1, get_launch_ticks());
}

void Fighter::exit_status_launch() {
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = false;
}

void Fighter::status_clank() {

}

void Fighter::enter_status_clank() {

}

void Fighter::exit_status_clank() {}

void Fighter::status_throw_tech() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 15 || fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 16) {
		forceStepThroughHitlag();
	}
	else if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] < 15) {
		add_pos(-4 * facing_dir, 0);
	}
	else {
		add_pos(-1 * facing_dir, 0);
	}
	if (is_anim_end) {
		change_status(situation_kind == FIGHTER_SITUATION_GROUND ? FIGHTER_STATUS_WAIT : FIGHTER_STATUS_FALL); //woah i can do this
	}
}

void Fighter::enter_status_throw_tech() {
	change_anim("throw_tech");
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 20;
}

void Fighter::exit_status_throw_tech() {}

void Fighter::status_landing() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}

void Fighter::enter_status_landing() {
	Fighter* that = fighter_accessor->fighter[!id];
	if (that->pos.x - that->pos.x_spr_offset / 2 == WINDOW_WIDTH / -2 && pos.x - pos.x_spr_offset / 2 == WINDOW_WIDTH / -2) {
		that->pos.x += 20;
	}
	if (that->pos.x + that->pos.x_spr_offset / 2 == WINDOW_WIDTH / 2 && pos.x + pos.x_spr_offset / 2 == WINDOW_WIDTH / 2) {
		that->pos.x -= 20;
	}
	fighter_int[FIGHTER_INT_LANDING_LAG] = 2;
	change_anim("landing", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing() {}

void Fighter::status_landing_attack() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}


void Fighter::enter_status_landing_attack() {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("lp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("mp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("hp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("lk_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("mk_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("hk_landing_lag");
	}
	change_anim("landing", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing_attack() {}

void Fighter::status_landing_hitstun() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}


void Fighter::enter_status_landing_hitstun() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_LANDING_LAG] = 4;
	change_anim("landing_hitstun", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing_hitstun() {

}

void Fighter::status_knockdown_start() {
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
	}
}

void Fighter::enter_status_knockdown_start() {
	change_anim("knockdown_start");
}

void Fighter::exit_status_knockdown_start() {

}

void Fighter::status_knockdown() {
	if (is_anim_end) {
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) { 
			//Hacky impl for 3 different wakeup speeds, in the future we can just make a different animation for each speed
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		else {
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] --;
		}
	}
}

void Fighter::enter_status_knockdown() {
	if (fighter_flag[FIGHTER_FLAG_CAN_TECH]) {
		if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_SLOW) {
			change_anim("knockdown_wait");
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_DEFAULT) {
			change_anim("knockdown_wait");
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) {
			change_anim("knockdown_wait");
		}
	}
	else {
		change_anim("knockdown_wait");
	}
	situation_kind = FIGHTER_SITUATION_DOWN;
}

void Fighter::exit_status_knockdown() {
	fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

void Fighter::status_wakeup() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_wakeup() {
	change_anim("wakeup");
}

void Fighter::exit_status_wakeup() {

}