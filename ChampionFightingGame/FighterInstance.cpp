#include "FighterInstance.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>
#include <cmath>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;



FighterInstance::FighterInstance() {}

FighterInstance::FighterInstance(SDL_Renderer* renderer, PlayerInfo* player_info) {
	this->player_info = player_info;
	// runs on creation of instance;
		// no it doesn't
	superInit(0, renderer);
}

void FighterInstance::fighter_main() {
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
	create_jostle_rect(GameCoordinate{ -15, 25 }, GameCoordinate{ 15, 0 });

	prevpos = pos;

	if (canStep()) {
		stepAnimation();
	}

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectile_objects[i]->id != -1) {
			projectile_objects[i]->projectile_main();
		}
	}

	if (chara_int[CHARA_INT_BUFFER_HITLAG_STATUS] != CHARA_STATUS_MAX && chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
		change_status(chara_int[CHARA_INT_BUFFER_HITLAG_STATUS], chara_flag[CHARA_FLAG_BUFFER_HITLAG_STATUS_END], chara_flag[CHARA_FLAG_BUFFER_HITLAG_STATUS_SEPARATE]);
		chara_int[CHARA_INT_BUFFER_HITLAG_STATUS] = CHARA_STATUS_MAX;
	}

	playoutStatus();

	if (anim_kind->move_dir != 0) {
		if (!add_pos((abs(getRenderPos(this, false).x - getRenderPos(this, true).x) / anim_kind->length + 1) * facing_dir * anim_kind->move_dir, 0, true)) {
			/*
			if we need to modify a player's position and the game is saying that the position is invalid, the render position won't be
			compensated properly. In this situation, manually changing the render position every frame to look stationary would be a nightmare, so
			we'll just switch to a stationary version of the animation. If no such animation exists, nothing changes, which probably means it wasn't
			enough of a problem to be worth taking a look at.
			*/
			if (pos.x > 0) {
				chara_float[CHARA_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / 2) - pos.x;
			}
			else {
				chara_float[CHARA_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / -2) + pos.x;
			}
			if (change_anim_inherit_attributes(anim_kind->name + "_stationary", false)) {
				chara_flag[CHARA_FLAG_STATIONARY_ANIMATION] = true;
			}
			else {
				chara_flag[CHARA_FLAG_STATIONARY_ANIMATION] = false;
			}
		}
		else {
			chara_flag[CHARA_FLAG_STATIONARY_ANIMATION] = false;
			if (anim_kind->force_center == 1) {
				chara_flag[CHARA_FLAG_FORCE_ANIM_CENTER] = false;
			}
		}
	}

	if (chara_flag[CHARA_FLAG_STATIONARY_ANIMATION]) {
		add_pos(chara_float[CHARA_FLOAT_DISTANCE_TO_WALL] / get_param_int(anim_kind->name + "_frames", unique_param_table), 0, true);
	}

	create_jostle_rect(GameCoordinate{ -15, 25 }, GameCoordinate{ 15, 0 });
	FighterInstance* that = fighter_instance_accessor->fighter_instance[!id];
	if (situation_kind == CHARA_SITUATION_GROUND && that->situation_kind == CHARA_SITUATION_GROUND
	&& !chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] && !that->chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP]) {
		if (is_collide(jostle_box, that->jostle_box)) {
			add_pos(get_param_float("jostle_walk_b_speed") * -1 * facing_dir, 0.0);
		}
	}

	processInput();

	prev_stick_dir = get_stick_dir();

	decrease_common_fighter_variables(this);

	if (chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] == 0 && status_kind != CHARA_STATUS_KNOCKDOWN) {
		chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	}

	int width;
	int height;
	SDL_QueryTexture(base_texture, NULL, NULL, &width, &height);
	pos.x_spr_offset = width / 2;
	pos.y_spr_offset = height;

	update_hitbox_pos();
	update_grabbox_pos();
	update_hurtbox_pos();
	if (chara_int[CHARA_INT_HITLAG_FRAMES] != 0) {
		if (chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] != 0.0) {
			if (situation_kind == CHARA_SITUATION_GROUND) {
				if (!add_pos(chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, 0)) {
					fighter_instance_accessor->fighter_instance[!id]->add_pos(chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] * facing_dir / 2, 0);
					//Note to self: Never try to use the FighterInstanceAccessor outside of a class method again, holy shit this is disgusting
				}
			}
			else {
				add_pos(chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] * facing_dir * -1, chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME]);
			}
		}
	}
	else {
		chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = 0.0;
	}
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1 && hitboxes[i].hitbox_kind != HITBOX_KIND_BLOCK) {
			chara_flag[CHARA_FLAG_HAS_ATTACK] = true;
			chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS] = true;
			break;
		}
		else {
			chara_flag[CHARA_FLAG_HAS_ATTACK] = false;
		}
	}
	create_jostle_rect(GameCoordinate{ -15, 25 }, GameCoordinate{ 15, 0 });
}

void FighterInstance::create_jostle_rect(GameCoordinate anchor, GameCoordinate offset) {
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

void FighterInstance::init_projectile(int id, GameCoordinate init_pos) {
	projectile_objects[id]->id = this->id;
	projectile_objects[id]->pos.x = ((init_pos.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	projectile_objects[id]->pos.y = (init_pos.y - WINDOW_HEIGHT) * -1.0 - this->pos.y;

	projectile_objects[id]->facing_right = facing_right;
	projectile_objects[id]->facing_dir = facing_dir;
	projectile_objects[id]->change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void FighterInstance::destroy_projectile(int id) {
	projectile_objects[id]->id = -1;
}

void FighterInstance::superInit(int id, SDL_Renderer* renderer) {
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?

	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}

	load_anim_list(renderer);
	load_params();
	chara_float[CHARA_FLOAT_HEALTH] = get_param_float("health");
	loadStatusFunctions();
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait", 2, 0);
	status_kind = CHARA_STATUS_WAIT;
	chara_flag[CHARA_FLAG_CAN_TECH] = true;
}

//Setup

void FighterInstance::load_anim_list(SDL_Renderer* renderer) {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		cerr << "Could not open anim_list! ->" << resource_dir + "/anims/anim_list.yml" <<endl;
		exit(1);
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
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void FighterInstance::loadStatusFunctions() {
	pStatus[CHARA_STATUS_WAIT] = &FighterInstance::status_wait;
	pEnter_status[CHARA_STATUS_WAIT] = &FighterInstance::enter_status_wait;
	pExit_status[CHARA_STATUS_WAIT] = &FighterInstance::exit_status_wait;

	pStatus[CHARA_STATUS_WALKF] = &FighterInstance::status_walkf;
	pEnter_status[CHARA_STATUS_WALKF] = &FighterInstance::enter_status_walkf;
	pExit_status[CHARA_STATUS_WALKF] = &FighterInstance::exit_status_walkf;

	pStatus[CHARA_STATUS_WALKB] = &FighterInstance::status_walkb;
	pEnter_status[CHARA_STATUS_WALKB] = &FighterInstance::enter_status_walkb;
	pExit_status[CHARA_STATUS_WALKB] = &FighterInstance::exit_status_walkb;

	pStatus[CHARA_STATUS_DASH] = &FighterInstance::status_dash;
	pEnter_status[CHARA_STATUS_DASH] = &FighterInstance::enter_status_dash;
	pExit_status[CHARA_STATUS_DASH] = &FighterInstance::exit_status_dash;

	pStatus[CHARA_STATUS_DASHB] = &FighterInstance::status_dashb;
	pEnter_status[CHARA_STATUS_DASHB] = &FighterInstance::enter_status_dashb;
	pExit_status[CHARA_STATUS_DASHB] = &FighterInstance::exit_status_dashb;

	pStatus[CHARA_STATUS_DASH_AIR] = &FighterInstance::status_dash_air;
	pEnter_status[CHARA_STATUS_DASH_AIR] = &FighterInstance::enter_status_dash_air;
	pExit_status[CHARA_STATUS_DASH_AIR] = &FighterInstance::exit_status_dash_air;

	pStatus[CHARA_STATUS_CROUCHD] = &FighterInstance::status_crouchd;
	pEnter_status[CHARA_STATUS_CROUCHD] = &FighterInstance::enter_status_crouchd;
	pExit_status[CHARA_STATUS_CROUCHD] = &FighterInstance::exit_status_crouchd;

	pStatus[CHARA_STATUS_CROUCH] = &FighterInstance::status_crouch;
	pEnter_status[CHARA_STATUS_CROUCH] = &FighterInstance::enter_status_crouch;
	pExit_status[CHARA_STATUS_CROUCH] = &FighterInstance::exit_status_crouch;

	pStatus[CHARA_STATUS_CROUCHU] = &FighterInstance::status_crouchu;
	pEnter_status[CHARA_STATUS_CROUCHU] = &FighterInstance::enter_status_crouchu;
	pExit_status[CHARA_STATUS_CROUCHU] = &FighterInstance::exit_status_crouchu;

	pStatus[CHARA_STATUS_JUMPSQUAT] = &FighterInstance::status_jumpsquat;
	pEnter_status[CHARA_STATUS_JUMPSQUAT] = &FighterInstance::enter_status_jumpsquat;
	pExit_status[CHARA_STATUS_JUMPSQUAT] = &FighterInstance::exit_status_jumpsquat;

	pStatus[CHARA_STATUS_JUMP] = &FighterInstance::status_jump;
	pEnter_status[CHARA_STATUS_JUMP] = &FighterInstance::enter_status_jump;
	pExit_status[CHARA_STATUS_JUMP] = &FighterInstance::exit_status_jump;

	pStatus[CHARA_STATUS_FALL] = &FighterInstance::status_fall;
	pEnter_status[CHARA_STATUS_FALL] = &FighterInstance::enter_status_fall;
	pExit_status[CHARA_STATUS_FALL] = &FighterInstance::exit_status_fall;

	pStatus[CHARA_STATUS_ATTACK] = &FighterInstance::status_attack;
	pEnter_status[CHARA_STATUS_ATTACK] = &FighterInstance::enter_status_attack;
	pExit_status[CHARA_STATUS_ATTACK] = &FighterInstance::exit_status_attack;

	pStatus[CHARA_STATUS_ATTACK_AIR] = &FighterInstance::status_attack_air;
	pEnter_status[CHARA_STATUS_ATTACK_AIR] = &FighterInstance::enter_status_attack_air;
	pExit_status[CHARA_STATUS_ATTACK_AIR] = &FighterInstance::exit_status_attack_air;

	pStatus[CHARA_STATUS_GRAB] = &FighterInstance::status_grab;
	pEnter_status[CHARA_STATUS_GRAB] = &FighterInstance::enter_status_grab;
	pExit_status[CHARA_STATUS_GRAB] = &FighterInstance::exit_status_grab;

	pStatus[CHARA_STATUS_THROW] = &FighterInstance::status_throw;
	pEnter_status[CHARA_STATUS_THROW] = &FighterInstance::enter_status_throw;
	pExit_status[CHARA_STATUS_THROW] = &FighterInstance::exit_status_throw;

	pStatus[CHARA_STATUS_GRAB_AIR] = &FighterInstance::status_grab_air;
	pEnter_status[CHARA_STATUS_GRAB_AIR] = &FighterInstance::enter_status_grab_air;
	pExit_status[CHARA_STATUS_GRAB_AIR] = &FighterInstance::exit_status_grab_air;

	pStatus[CHARA_STATUS_THROW_AIR] = &FighterInstance::status_throw_air;
	pEnter_status[CHARA_STATUS_THROW_AIR] = &FighterInstance::enter_status_throw_air;
	pExit_status[CHARA_STATUS_THROW_AIR] = &FighterInstance::exit_status_throw_air;

	pStatus[CHARA_STATUS_GRABBED] = &FighterInstance::status_grabbed;
	pEnter_status[CHARA_STATUS_GRABBED] = &FighterInstance::enter_status_grabbed;
	pExit_status[CHARA_STATUS_GRABBED] = &FighterInstance::exit_status_grabbed;

	pStatus[CHARA_STATUS_THROWN] = &FighterInstance::status_thrown;
	pEnter_status[CHARA_STATUS_THROWN] = &FighterInstance::enter_status_thrown;
	pExit_status[CHARA_STATUS_THROWN] = &FighterInstance::exit_status_thrown;

	pStatus[CHARA_STATUS_HITSTUN] = &FighterInstance::status_hitstun;
	pEnter_status[CHARA_STATUS_HITSTUN] = &FighterInstance::enter_status_hitstun;
	pExit_status[CHARA_STATUS_HITSTUN] = &FighterInstance::exit_status_hitstun;

	pStatus[CHARA_STATUS_HITSTUN_AIR] = &FighterInstance::status_hitstun_air;
	pEnter_status[CHARA_STATUS_HITSTUN_AIR] = &FighterInstance::enter_status_hitstun_air;
	pExit_status[CHARA_STATUS_HITSTUN_AIR] = &FighterInstance::exit_status_hitstun_air;

	pStatus[CHARA_STATUS_BLOCKSTUN] = &FighterInstance::status_blockstun;
	pEnter_status[CHARA_STATUS_BLOCKSTUN] = &FighterInstance::enter_status_blockstun;
	pExit_status[CHARA_STATUS_BLOCKSTUN] = &FighterInstance::exit_status_blockstun;

	pStatus[CHARA_STATUS_PARRY_START] = &FighterInstance::status_parry_start;
	pEnter_status[CHARA_STATUS_PARRY_START] = &FighterInstance::enter_status_parry_start;
	pExit_status[CHARA_STATUS_PARRY_START] = &FighterInstance::exit_status_parry_start;

	pStatus[CHARA_STATUS_PARRY] = &FighterInstance::status_parry;
	pEnter_status[CHARA_STATUS_PARRY] = &FighterInstance::enter_status_parry;
	pExit_status[CHARA_STATUS_PARRY] = &FighterInstance::exit_status_parry;

	pStatus[CHARA_STATUS_CRUMPLE] = &FighterInstance::status_crumple;
	pEnter_status[CHARA_STATUS_CRUMPLE] = &FighterInstance::enter_status_crumple;
	pExit_status[CHARA_STATUS_CRUMPLE] = &FighterInstance::exit_status_crumple;

	pStatus[CHARA_STATUS_LAUNCH_START] = &FighterInstance::status_launch_start;
	pEnter_status[CHARA_STATUS_LAUNCH_START] = &FighterInstance::enter_status_launch_start;
	pExit_status[CHARA_STATUS_LAUNCH_START] = &FighterInstance::exit_status_launch_start;

	pStatus[CHARA_STATUS_LAUNCH] = &FighterInstance::status_launch;
	pEnter_status[CHARA_STATUS_LAUNCH] = &FighterInstance::enter_status_launch;
	pExit_status[CHARA_STATUS_LAUNCH] = &FighterInstance::exit_status_launch;

	pStatus[CHARA_STATUS_CLANK] = &FighterInstance::status_clank;
	pEnter_status[CHARA_STATUS_CLANK] = &FighterInstance::enter_status_clank;
	pExit_status[CHARA_STATUS_CLANK] = &FighterInstance::exit_status_clank;

	pStatus[CHARA_STATUS_THROW_TECH] = &FighterInstance::status_throw_tech;
	pEnter_status[CHARA_STATUS_THROW_TECH] = &FighterInstance::enter_status_throw_tech;
	pExit_status[CHARA_STATUS_THROW_TECH] = &FighterInstance::exit_status_throw_tech;

	pStatus[CHARA_STATUS_LANDING] = &FighterInstance::status_landing;
	pEnter_status[CHARA_STATUS_LANDING] = &FighterInstance::enter_status_landing;
	pExit_status[CHARA_STATUS_LANDING] = &FighterInstance::exit_status_landing;

	pStatus[CHARA_STATUS_LANDING_ATTACK] = &FighterInstance::status_landing_attack;
	pEnter_status[CHARA_STATUS_LANDING_ATTACK] = &FighterInstance::enter_status_landing_attack;
	pExit_status[CHARA_STATUS_LANDING_ATTACK] = &FighterInstance::exit_status_landing_attack;

	pStatus[CHARA_STATUS_LANDING_HITSTUN] = &FighterInstance::status_landing_hitstun;
	pEnter_status[CHARA_STATUS_LANDING_HITSTUN] = &FighterInstance::enter_status_landing_hitstun;
	pExit_status[CHARA_STATUS_LANDING_HITSTUN] = &FighterInstance::exit_status_landing_hitstun;

	pStatus[CHARA_STATUS_KNOCKDOWN_START] = &FighterInstance::status_knockdown_start;
	pEnter_status[CHARA_STATUS_KNOCKDOWN_START] = &FighterInstance::enter_status_knockdown_start;
	pExit_status[CHARA_STATUS_KNOCKDOWN_START] = &FighterInstance::exit_status_knockdown_start;

	pStatus[CHARA_STATUS_KNOCKDOWN] = &FighterInstance::status_knockdown;
	pEnter_status[CHARA_STATUS_KNOCKDOWN] = &FighterInstance::enter_status_knockdown;
	pExit_status[CHARA_STATUS_KNOCKDOWN] = &FighterInstance::exit_status_knockdown;

	pStatus[CHARA_STATUS_WAKEUP] = &FighterInstance::status_wakeup;
	pEnter_status[CHARA_STATUS_WAKEUP] = &FighterInstance::enter_status_wakeup;
	pExit_status[CHARA_STATUS_WAKEUP] = &FighterInstance::exit_status_wakeup;
}

//Move Scripting

/*
	Basically, each instruction in move scripts should only be running once, so if the number of executed statements >= the one we're currently trying
	to execute, whatever's inside of the block won't run. Smash does something similar except it doesn't need to specify the number of executed
	statements, and I'm not entirely sure how that works, but this approach is still pretty solid. (It'll make more sense when we start putting more
	instructions outside of status scripts and inside of move scripts)

	P.S. It's misspelled in Smash as well. You can fix it if you want, idrc
*/

//Inputs

void FighterInstance::processInput() {
	int stick_dir = get_stick_dir();

	//Dash Input

	if (get_flick_dir() == 6 && prev_stick_dir == 5) {
		chara_int[CHARA_INT_DASH_F_WINDOW] = DASH_WINDOW;
	}
	if (get_flick_dir() == 4 && prev_stick_dir == 5) {
		chara_int[CHARA_INT_DASH_B_WINDOW] = DASH_WINDOW;
	}
	if (status_kind != CHARA_STATUS_KNOCKDOWN) {
		if (get_flick_dir() == 8 && chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] = TECH_WINDOW;
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_FAST;
		}
		if (get_flick_dir() == 2 && chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] = TECH_WINDOW;
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_SLOW;
		}
	}
	if (stick_dir != 6 && stick_dir != 5) {
		chara_int[CHARA_INT_DASH_F_WINDOW] = 0;
	}
	if (stick_dir != 4 && stick_dir != 5) {
		chara_int[CHARA_INT_DASH_B_WINDOW] = 0;
	}

	//Special Inputs

	//Motion Inputs

	if (stick_dir == 1) {
		if (chara_int[CHARA_INT_214_STEP] == 1) {
			chara_int[CHARA_INT_214_STEP] ++;
			chara_int[CHARA_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_214214_STEP] == 1 || chara_int[CHARA_INT_214214_STEP] == 4) {
			chara_int[CHARA_INT_214214_STEP] ++;
			chara_int[CHARA_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_41236_STEP] == 1) {
			chara_int[CHARA_INT_41236_STEP] ++;
			chara_int[CHARA_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_63214_STEP] == 3) {
			chara_int[CHARA_INT_63214_STEP] ++;
			chara_int[CHARA_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 2) {
		chara_int[CHARA_INT_236_STEP] = 1;
		chara_int[CHARA_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		chara_int[CHARA_INT_214_STEP] = 1;
		chara_int[CHARA_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		if (chara_int[CHARA_INT_623_STEP] == 1) {
			chara_int[CHARA_INT_623_STEP] ++;
			chara_int[CHARA_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_214214_STEP] == 1) {
			chara_int[CHARA_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_214214_STEP] == 0 || chara_int[CHARA_INT_214214_STEP] == 3) {
			chara_int[CHARA_INT_214214_STEP] ++;
			chara_int[CHARA_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_236236_STEP] == 1) {
			chara_int[CHARA_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_236236_STEP] == 0 || chara_int[CHARA_INT_236236_STEP] == 3) {
			chara_int[CHARA_INT_236236_STEP] ++;
			chara_int[CHARA_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_41236_STEP] == 2) {
			chara_int[CHARA_INT_41236_STEP] ++;
			chara_int[CHARA_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_63214_STEP] == 2) {
			chara_int[CHARA_INT_63214_STEP] ++;
			chara_int[CHARA_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 3) {
		if (chara_int[CHARA_INT_236_STEP] == 1) {
			chara_int[CHARA_INT_236_STEP] ++;
			chara_int[CHARA_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_623_STEP] == 2) {
			chara_int[CHARA_INT_623_STEP] ++;
			chara_int[CHARA_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_236236_STEP] == 1 || chara_int[CHARA_INT_236236_STEP] == 4) {
			chara_int[CHARA_INT_236236_STEP] ++;
			chara_int[CHARA_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_41236_STEP] == 3) {
			chara_int[CHARA_INT_41236_STEP] ++;
			chara_int[CHARA_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_63214_STEP] == 1) {
			chara_int[CHARA_INT_63214_STEP] ++;
			chara_int[CHARA_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 4) {
		chara_int[CHARA_INT_41236_STEP] = 1;
		chara_int[CHARA_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		if (chara_int[CHARA_INT_214_STEP] == 2) {
			chara_int[CHARA_INT_214_STEP] ++;
			chara_int[CHARA_INT_214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_214214_STEP] == 2 || chara_int[CHARA_INT_214214_STEP] == 5) {
			chara_int[CHARA_INT_214214_STEP] ++;
			chara_int[CHARA_INT_214214_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_63214_STEP] == 4) {
			chara_int[CHARA_INT_63214_STEP] ++;
			chara_int[CHARA_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}
	if (stick_dir == 6) {
		chara_int[CHARA_INT_623_STEP] = 1;
		chara_int[CHARA_INT_623_TIMER] = MOTION_SPECIAL_TIMER;
		chara_int[CHARA_INT_63214_STEP] = 1;
		chara_int[CHARA_INT_63214_TIMER] = MOTION_SPECIAL_TIMER;
		if (chara_int[CHARA_INT_236_STEP] == 2) {
			chara_int[CHARA_INT_236_STEP] ++;
			chara_int[CHARA_INT_236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_236236_STEP] == 2 || chara_int[CHARA_INT_236236_STEP] == 5) {
			chara_int[CHARA_INT_236236_STEP] ++;
			chara_int[CHARA_INT_236236_TIMER] = MOTION_SPECIAL_TIMER;
		}
		if (chara_int[CHARA_INT_41236_STEP] == 4) {
			chara_int[CHARA_INT_41236_STEP] ++;
			chara_int[CHARA_INT_41236_TIMER] = MOTION_SPECIAL_TIMER;
		}
	}

	//Charge Inputs (disgusting)

	if (stick_dir < 4) {
		chara_int[CHARA_INT_DOWN_CHARGE_TIMER] = 6;
		chara_int[CHARA_INT_DOWN_CHARGE_FRAMES]++;
	}

	if (stick_dir == 1 || stick_dir == 4 || stick_dir == 7) {
		chara_int[CHARA_INT_BACK_CHARGE_FRAMES]++;
		chara_int[CHARA_INT_BACK_CHARGE_TIMER] = 6;
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

bool FighterInstance::check_button_on(u32 button) {
	return player_info->check_button_on(button);
}

bool FighterInstance::check_button_input(u32 button) {
	return player_info->check_button_input(button);
}

bool FighterInstance::check_button_input(u32 button[], int length, int min_matches) {
	return player_info->check_button_input(button, length, min_matches);
}

bool FighterInstance::check_button_trigger(u32 button) {
	return player_info->check_button_trigger(button);
}

bool FighterInstance::check_button_release(u32 button) {
	return player_info->check_button_release(button);
}

int FighterInstance::get_stick_dir() {
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

int FighterInstance::get_flick_dir() {
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

int FighterInstance::get_special_input(int special_kind, u32 button, int charge_frames) {
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
			if (chara_int[CHARA_INT_236_STEP] == 2 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_236_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_236_STEP] = 0;
				chara_int[CHARA_INT_236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214) {
			if (chara_int[CHARA_INT_214_STEP] == 2 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_214_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_214_STEP] = 0;
				chara_int[CHARA_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_623) {
			if (chara_int[CHARA_INT_623_STEP] == 2 && get_stick_dir() == 3) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_623_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_623_STEP] = 0;
				chara_int[CHARA_INT_623_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_41236) {
			if (chara_int[CHARA_INT_41236_STEP] == 4 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_41236_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_41236_STEP] = 0;
				chara_int[CHARA_INT_41236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_63214) {
			if (chara_int[CHARA_INT_63214_STEP] == 4 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_63214_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_63214_STEP] = 0;
				chara_int[CHARA_INT_63214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_236236) {
			if (chara_int[CHARA_INT_236236_STEP] == 5 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_236236_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_236236_STEP] = 0;
				chara_int[CHARA_INT_236236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214214) {
			if (chara_int[CHARA_INT_214214_STEP] == 5 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (chara_int[CHARA_INT_214214_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				chara_int[CHARA_INT_214_STEP] = 0;
				chara_int[CHARA_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_DOWN) {
			input_check = (chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 8);
			if (input_check && get_flick_dir() == 8) {
				button_check == SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] = 0;
				chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] = 0;
				chara_int[CHARA_INT_DOWN_CHARGE_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_BACK) {
			input_check = (chara_int[CHARA_INT_BACK_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 6);
			if (input_check && get_flick_dir() == 6) {
				button_check == SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				chara_int[CHARA_INT_BACK_CHARGE_FRAMES] = 0;
				chara_int[CHARA_INT_BACK_CHARGE_TIMER] = 0;
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

bool FighterInstance::get_normal_cancel(int attack_kind, u32 button, int situation_kind) {
	if (chara_int[CHARA_INT_ATTACK_KIND] == attack_kind && check_button_input(button) && situation_kind == this->situation_kind) {
		if (button == BUTTON_LP) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
		}
		if (button == BUTTON_MP) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
		}
		if (button == BUTTON_HP) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
		}
		if (button == BUTTON_LK) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
		}
		if (button == BUTTON_MK) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MK;
		}
		if (button == BUTTON_HK) {
			chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HK;
		}
		if (chara_int[CHARA_INT_HITLAG_FRAMES] <= BUFFER_WINDOW) {
			if (situation_kind == CHARA_SITUATION_GROUND) {
				return change_status_after_hitlag(CHARA_STATUS_ATTACK, true, false);
			}
			else {
				return change_status_after_hitlag(CHARA_STATUS_ATTACK_AIR, true, false);
			}
		}
	}
	else {
		return false;
	}
}

//Position

bool FighterInstance::add_pos(float x, float y, bool prev) {
	GameCoordinate prevpos = pos;
	FighterInstance* that = fighter_instance_accessor->fighter_instance[!id];
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
		if (get_param_bool("has_wallbounce") && facing_right && status_kind == CHARA_STATUS_JUMP) {
			chara_float[CHARA_FLOAT_CURRENT_X_SPEED] *= -1;
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
		if (get_param_bool("has_wallbounce") && !facing_right && status_kind == CHARA_STATUS_JUMP) {
			chara_float[CHARA_FLOAT_CURRENT_X_SPEED] *= -1;
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
	if (opponent_right != new_opponent_right && !chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == CHARA_SITUATION_GROUND && that->situation_kind == CHARA_SITUATION_GROUND) {
		pos.x = prevpos.x;
		ret = false;
	}

	return ret;
}

bool FighterInstance::set_pos(float x, float y, bool prev) {
	GameCoordinate prevpos = pos;
	FighterInstance* that = fighter_instance_accessor->fighter_instance[!id];
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
	if (opponent_right != new_opponent_right && !chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == CHARA_SITUATION_GROUND && that->situation_kind == CHARA_SITUATION_GROUND) {
		pos.x = prevpos.x;
		ret = false;
	}

	return ret;
}

//Opponent

void FighterInstance::set_opponent_offset(GameCoordinate offset, int frames) {
	GameCoordinate new_offset;
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_instance_accessor->fighter_instance[!id]->pos.x - new_offset.x) * fighter_instance_accessor->fighter_instance[!id]->facing_dir / frames;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_instance_accessor->fighter_instance[!id]->pos.y - new_offset.y) / frames;
	fighter_instance_accessor->fighter_instance[!id]->chara_int[CHARA_INT_MANUAL_POS_CHANGE_FRAMES] = frames;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void FighterInstance::set_opponent_offset(GameCoordinate offset) {
	GameCoordinate new_offset;
	int frames = fighter_instance_accessor->fighter_instance[!id]->chara_int[CHARA_INT_MANUAL_POS_CHANGE_FRAMES];
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_instance_accessor->fighter_instance[!id]->pos.x - new_offset.x) * fighter_instance_accessor->fighter_instance[!id]->facing_dir / frames;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_instance_accessor->fighter_instance[!id]->pos.y - new_offset.y) / frames;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void FighterInstance::change_opponent_status(u32 status_kind) {
	if ((this->status_kind == CHARA_STATUS_THROW || this->status_kind == CHARA_STATUS_THROW_AIR) && status_kind == CHARA_STATUS_THROWN) {
		chara_flag[CHARA_FLAG_THREW_OPPONENT] = true;
	}
	fighter_instance_accessor->fighter_instance[!id]->change_status(status_kind);
}

void FighterInstance::damage_opponent(float damage, float facing_dir, float x_speed, float y_speed) {
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_HEALTH] -= damage;
	fighter_instance_accessor->fighter_instance[!id]->facing_dir = facing_dir * this->facing_dir * -1;
	fighter_instance_accessor->fighter_instance[!id]->facing_right = (facing_dir * this->facing_dir * -1) > 0;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = x_speed * this->facing_dir;
	fighter_instance_accessor->fighter_instance[!id]->chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = y_speed;
}

void FighterInstance::set_opponent_angle(double angle) {
	fighter_instance_accessor->fighter_instance[!id]->angle = angle * facing_dir;
}

void FighterInstance::set_opponent_thrown_ticks() {
	fighter_instance_accessor->fighter_instance[!id]->max_ticks = (((anim_kind->length - 1) / max_ticks) / (fighter_instance_accessor->fighter_instance[!id]->anim_kind->length - 1)) - 1;
	fighter_instance_accessor->fighter_instance[!id]->ticks = 0;
}

void FighterInstance::change_opponent_anim(string anim_kind, int frame_rate, int entry_frame) {
	fighter_instance_accessor->fighter_instance[!id]->change_anim(anim_kind, frame_rate, entry_frame);
}

//Hitbox

void FighterInstance::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
								 GameCoordinate offset, int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag,
								 int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
								 bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y,
								 float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool use_player_pos) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
							  meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
							  attack_level, hit_pushback, block_pushback, clank_kind, success_hit, juggle_set, max_juggle, hit_status, counterhit_status,
							  counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, continue_launch, use_player_pos);
	}
}

//Grabbox

void FighterInstance::new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
								  u32 defender_status_if_hit, bool use_player_pos) {

	if (id < 10) {
		grabboxes[id] = Grabbox(this, id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
	}
}

//Hurtbox

void FighterInstance::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind) {
	if (id < 10) {
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}

//Transitions

bool FighterInstance::is_actionable() {
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0 && chara_int[CHARA_INT_HITLAG_FRAMES] == 0 && status_kind != CHARA_STATUS_GRABBED && status_kind != CHARA_STATUS_THROWN) {
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

bool FighterInstance::can_kara() {
	if (((chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS]) || kara_enabled) {
		return true;
	}
	else {
		return false;
	}
}

//Animations

void FighterInstance::reenter_last_anim() {
	max_ticks = prev_anim_max_ticks;
	render_frame = prev_anim_render_frame;
	frame = prev_anim_frame;
	set_current_move_script(prev_anim_kind->name);
	startAnimation(prev_anim_kind);
}

bool FighterInstance::change_anim(string animation_name, int frame_rate, int entry_frame) {
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

bool FighterInstance::change_anim_inherit_attributes(string animation_name, bool verbose, bool continue_script) {
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

void FighterInstance::startAnimation(Animation* animation) {
	is_anim_end = false;
	prev_anim_kind = anim_kind;
	anim_kind = animation;
	int width;
	int height;
	SDL_QueryTexture(animation->SPRITESHEET, NULL, NULL, &width, &height);
	chara_flag[CHARA_FLAG_FORCE_ANIM_CENTER] = (anim_kind->force_center != 0);
	pos.x_anim_offset = width / (anim_kind->length + 1) / 2;
	pos.y_anim_offset = height;
	frame_rect = getFrame(render_frame, anim_kind);
}

bool FighterInstance::canStep() {
	attempted_excutes = 0;
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
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

void FighterInstance::stepAnimation() {
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
	}
	else {
		render_frame++;
	}
	is_anim_end = last_frame > frame;
}

void FighterInstance::forceStepThroughHitlag() {
	int curr_hitlag_frames = chara_int[CHARA_INT_HITLAG_FRAMES];
	chara_int[CHARA_INT_HITLAG_FRAMES] = 0;
	canStep();
	stepAnimation();
	chara_int[CHARA_INT_HITLAG_FRAMES] = curr_hitlag_frames;
}

bool FighterInstance::beginning_hitlag(int frames) {
	return chara_int[CHARA_INT_HITLAG_FRAMES] + frames >= chara_int[CHARA_INT_INIT_HITLAG_FRAMES] && chara_int[CHARA_INT_HITLAG_FRAMES] != 0;
}

bool FighterInstance::ending_hitlag(int frames) {
	return chara_int[CHARA_INT_HITLAG_FRAMES] - frames <= 0 && chara_int[CHARA_INT_HITLAG_FRAMES] != 0;
}
 
//Status

bool FighterInstance::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		kara_enabled = false;
		chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = false;
		chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS] = false;
		chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS] = false;
		chara_flag[CHARA_FLAG_STATIONARY_ANIMATION] = false;
		if (call_end_status) {
			(this->*pExit_status[status_kind])();
		}
		status_kind = new_status_kind;
		(this->*pEnter_status[status_kind])();
		return true;
	}
	else {
		return false;
	}
}

bool FighterInstance::change_status_after_hitlag(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
		return change_status(new_status_kind, call_end_status, require_different_status);
	}
	else {
		if (new_status_kind != status_kind || !require_different_status) {
			chara_int[CHARA_INT_BUFFER_HITLAG_STATUS] = new_status_kind;
			chara_flag[CHARA_FLAG_BUFFER_HITLAG_STATUS_END] = call_end_status;
			chara_flag[CHARA_FLAG_BUFFER_HITLAG_STATUS_SEPARATE] = require_different_status;
			return true;
		}
		else {
			return false;
		}
	}
}

void FighterInstance::playoutStatus() {
	if (is_status_hitstun_enable_parry()) {
		u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			if (situation_kind == CHARA_SITUATION_GROUND) {
				change_anim("hitstun_parry", 5);
			}
			else {
				change_anim("hitstun_parry_air", 5);
			}
		}
	}
	(this->*pStatus[status_kind])();
	move_script();
}

bool FighterInstance::common_ground_status_act() {
	if (specific_ground_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			u32 grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(CHARA_STATUS_GRAB));
			}
			if (check_button_input(BUTTON_LP)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CLP;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
				}
			}
			if (check_button_input(BUTTON_LK)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CLK;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
				}
			}
			u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(CHARA_STATUS_PARRY_START));
			}
			if (check_button_input(BUTTON_MP)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CMP;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
				}
			}
			if (check_button_input(BUTTON_MK)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CMK;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MK;
				}
			}
			if (check_button_input(BUTTON_HP)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CHP;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
				}
			}
			if (check_button_input(BUTTON_HK)) {
				if (get_stick_dir() < 4) {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CHK;
				}
				else {
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HK;
				}
			}
			return change_status(CHARA_STATUS_ATTACK, true, false);
		}
		if (get_stick_dir() == 6) {
			if (chara_int[CHARA_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6) {
				return change_status(CHARA_STATUS_DASH);
			}
			else {
				return change_status(CHARA_STATUS_WALKF);
			}
		}
		if (get_stick_dir() == 4) {
			if (chara_int[CHARA_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4) {
				return change_status(CHARA_STATUS_DASHB);
			}
			else {
				return change_status(CHARA_STATUS_WALKB);
			}
		}
		if (get_stick_dir() > 6) {
			return change_status(CHARA_STATUS_JUMPSQUAT);
		}
		if (get_stick_dir() < 4 && status_kind != CHARA_STATUS_CROUCH) {
			if (status_kind == CHARA_STATUS_ATTACK && chara_int[CHARA_INT_ATTACK_KIND] >= ATTACK_KIND_HK) {
				return change_status(CHARA_STATUS_CROUCH);
			}
			else {
				return change_status(CHARA_STATUS_CROUCHD);
			}
		}
	}
	return false;
}

bool FighterInstance::common_air_status_act() {
	if (specific_air_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			u32 grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(CHARA_STATUS_GRAB_AIR, false, false));
			}
			if (check_button_input(BUTTON_LP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
			if (check_button_input(BUTTON_LK)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
			u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(CHARA_STATUS_PARRY_START, false, false));
			}
			if (check_button_input(BUTTON_MP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
			}
			if (check_button_input(BUTTON_MK)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MK;
			}
			if (check_button_input(BUTTON_HP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
			}
			if (check_button_input(BUTTON_HK)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HK;
			}
			return change_status(CHARA_STATUS_ATTACK_AIR, false, false);
		}
		if (chara_int[CHARA_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6 && get_param_bool("has_airdash") && !chara_flag[CHARA_FLAG_USED_AIRDASH]) {
			chara_int[CHARA_INT_DASH_AIR_DIR] = 1;
			chara_flag[CHARA_FLAG_USED_AIRDASH] = true;
			return change_status(CHARA_STATUS_DASH_AIR);
		}
		if (chara_int[CHARA_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4 && get_param_bool("has_airdash") && !chara_flag[CHARA_FLAG_USED_AIRDASH]) {
			chara_int[CHARA_INT_DASH_AIR_DIR] = 2;
			chara_flag[CHARA_FLAG_USED_AIRDASH] = true;
			return change_status(CHARA_STATUS_DASH_AIR);
		}
	}
	return false;
}

bool FighterInstance::common_air_status_general() {
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
		if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
		}
		if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, 0);
		}
		add_pos(0, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	}
	return false;
}

u32 FighterInstance::get_status_group() {
	switch (status_kind) {
		case (CHARA_STATUS_HITSTUN):
		case (CHARA_STATUS_HITSTUN_AIR):
		case (CHARA_STATUS_LAUNCH_START):
		case (CHARA_STATUS_LAUNCH):
		case (CHARA_STATUS_CRUMPLE):
		case (CHARA_STATUS_THROWN): {
			return STATUS_GROUP_HITSTUN;
		}
		break;
		case (CHARA_STATUS_CROUCHD):
		case (CHARA_STATUS_CROUCH):
		case (CHARA_STATUS_CROUCHU): {
			return STATUS_GROUP_CROUCH;
		}
		break;
		default: {
			return STATUS_GROUP_NORMAL;
		}
		break;
	}
}

bool FighterInstance::is_status_hitstun_enable_parry() {
	if (chara_int[CHARA_INT_HITLAG_FRAMES] != 0 || anim_kind->name == "hitstun_parry" || anim_kind->name == "hitstun_parry_air") {
		return false;
	}
	switch (status_kind) {
		case (CHARA_STATUS_HITSTUN):
		case (CHARA_STATUS_HITSTUN_AIR):
		case (CHARA_STATUS_LAUNCH): {
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

void FighterInstance::status_wait() {
	if (common_ground_status_act()) {
		return;
	}
}


void FighterInstance::enter_status_wait() {
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait");


	situation_kind = CHARA_SITUATION_GROUND;
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
	common_ground_status_act();
}

void FighterInstance::exit_status_wait() {}

void FighterInstance::status_walkf() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_collide(jostle_box, fighter_instance_accessor->fighter_instance[!id]->jostle_box)) {
		add_pos(get_param_float("jostle_walk_f_speed") * facing_dir, 0);
	}
	else {
		add_pos(get_param_float("walk_f_speed") * facing_dir, 0);
	}
}

void FighterInstance::enter_status_walkf() {
	change_anim("walk_f");
}

void FighterInstance::exit_status_walkf() {}

void FighterInstance::status_walkb() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (chara_flag[CHARA_FLAG_PROX_GUARD]) {
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

void FighterInstance::enter_status_walkb() {
	change_anim("walk_b");
}

void FighterInstance::exit_status_walkb() {}

void FighterInstance::status_dash() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
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
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 4) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASHB, false);
				return;
			}
		}
		else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
		}
	}
}

void FighterInstance::enter_status_dash() {
	change_anim("dash_f");
}

void FighterInstance::exit_status_dash() {
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
}

void FighterInstance::status_dashb() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
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
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 6) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
	}
}

void FighterInstance::enter_status_dashb() {
	change_anim("dash_b");
}

void FighterInstance::exit_status_dashb() {
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
}

void FighterInstance::status_dash_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_FALL);
		return;
	}
	if (is_actionable() && common_air_status_act()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	if (frame < max_frame) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0.0;
	}
	else {
		if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			if (chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed")) {
				chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * 0.95, chara_float[CHARA_FLOAT_CURRENT_X_SPEED]);
			}
		}
		else if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			if (chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed") * -1.0) {
				chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * 0.95, chara_float[CHARA_FLOAT_CURRENT_X_SPEED]);
			}
		}
	}
	if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	}
	else {
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.0, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	}
}

void FighterInstance::enter_status_dash_air() {
	if (chara_int[CHARA_INT_DASH_AIR_DIR] == 1) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_f_speed");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
		change_anim("dash_f");
	}
	if (chara_int[CHARA_INT_DASH_AIR_DIR] == 2) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_b_speed");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		change_anim("dash_b");
	}
}

void FighterInstance::exit_status_dash_air() {

}


void FighterInstance::status_crouchd() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() > 3) {
		change_status(CHARA_STATUS_CROUCHU);
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_CROUCH);
		return;
	}
}

void FighterInstance::enter_status_crouchd() {
	change_anim("crouch_d");
}

void FighterInstance::exit_status_crouchd() {}

void FighterInstance::status_crouch() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_CROUCHU);
		return;
	}
}

void FighterInstance::enter_status_crouch() {
	change_anim("crouch");
}

void FighterInstance::exit_status_crouch() {}

void FighterInstance::status_crouchu() {
	if (common_ground_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
	}
}

void FighterInstance::enter_status_crouchu() {
	change_anim("crouch_u");
}

void FighterInstance::exit_status_crouchu() {}

void FighterInstance::status_jumpsquat() {
	if (frame == 4 || is_anim_end) {
		change_status(CHARA_STATUS_JUMP);
		return;
	}
}

void FighterInstance::enter_status_jumpsquat() {
	change_anim("jump_squat");
}

void FighterInstance::exit_status_jumpsquat() {
	chara_flag[CHARA_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	chara_flag[CHARA_FLAG_USED_AIRDASH] = false;
	situation_kind = CHARA_SITUATION_AIR;
}

void FighterInstance::status_jump() {
	if (common_air_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_FALL, false);
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	common_air_status_general();
	chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > 0;
}

void FighterInstance::enter_status_jump() {
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1) {
		change_anim("jump_b");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3) {
		change_anim("jump_f");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
	}
	else {
		change_anim("jump");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if (chara_flag[CHARA_FLAG_SHORT_HOP]) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed_s");
	}
	else {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed");
	}
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("jump_x_speed");
	common_air_status_act();
	chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
}

void FighterInstance::exit_status_jump() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void FighterInstance::status_fall() {
	if (common_air_status_act()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	common_air_status_general();
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] < 0) {
		chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = false;
	}
}

void FighterInstance::enter_status_fall() {
	change_anim("fall");
}


void FighterInstance::exit_status_fall() {
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void FighterInstance::status_attack() {
	if (specific_status_attack()) {
		return;
	}
	if (is_anim_end) {
		if (get_stick_dir() < 4) {
			if (chara_int[CHARA_INT_ATTACK_KIND] <= ATTACK_KIND_HK) {
				if (change_status(CHARA_STATUS_CROUCHD)) {
					return;
				}
			}
			else {
				if (change_status(CHARA_STATUS_CROUCH)) {
					return;
				}
			}
		}
		else if (change_status(CHARA_STATUS_WAIT)) {
			return;
		}
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(CHARA_STATUS_GRAB);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (chara_flag[CHARA_FLAG_HAS_ATTACK] && chara_float[CHARA_FLOAT_SUPER_METER] >= 30.0) {
				chara_float[CHARA_FLOAT_SUPER_METER] -= 30.0;
				change_status(CHARA_STATUS_PARRY_START);
				return;
			}
			else if (!chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(CHARA_STATUS_PARRY_START);
				return;
			}
		}
	}
}

void FighterInstance::enter_status_attack() {
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("stand_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("stand_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("stand_hp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("stand_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("stand_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("stand_hk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CLP) {
		change_anim("crouch_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMP) {
		change_anim("crouch_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CHP) {
		change_anim("crouch_hp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CLK) {
		change_anim("crouch_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		change_anim("crouch_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CHK) {
		change_anim("crouch_hk");
	}
}

void FighterInstance::exit_status_attack() {}

void FighterInstance::status_attack_air() {
	if (specific_status_attack()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING_ATTACK);
		return;
	}
	if (is_anim_end) {
		if (change_status(CHARA_STATUS_FALL)) {
			return;
		}
	}
	if (is_actionable()) {
		if (common_air_status_act()) {
			return;
		}
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(CHARA_STATUS_GRAB_AIR);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (!chara_flag[CHARA_FLAG_HAS_ATTACK] && !chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(CHARA_STATUS_PARRY_START);
				return;
			}
		}
	}
	common_air_status_general();
}

void FighterInstance::enter_status_attack_air() {
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("jump_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("jump_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("jump_hp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("jump_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("jump_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("jump_hk");
	}
}

void FighterInstance::exit_status_attack_air() {}

void FighterInstance::status_grab() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void FighterInstance::enter_status_grab() {
	change_anim("grab");
}

void FighterInstance::exit_status_grab() {}

void FighterInstance::status_throw() {
	if (!chara_flag[CHARA_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_Y] });
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void FighterInstance::enter_status_throw() {
	chara_flag[CHARA_FLAG_THREW_OPPONENT] = false;
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b");
	}
	else {
		change_anim("throw_f");
	}
	add_pos(get_param_float(anim_kind->name + "_move_offset", unique_param_table) * facing_dir, 0);
}

void FighterInstance::exit_status_throw() {
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
	if (anim_kind->name == "throw_b") {
		facing_right = !facing_right;
		facing_dir *= -1;
	}
}

void FighterInstance::status_grab_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_FALL);
		return;
	}
	if (is_actionable()) {
		if (common_air_status_act()) {
			return;
		}
	}
	common_air_status_general();
}

void FighterInstance::enter_status_grab_air() {
	change_anim("grab_air");
}

void FighterInstance::exit_status_grab_air() {}

void FighterInstance::status_throw_air() {
	if (!chara_flag[CHARA_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_Y] });
	}
	if (pos.y <= FLOOR_GAMECOORD) {
		set_pos(pos.x, FLOOR_GAMECOORD);
		situation_kind = CHARA_SITUATION_GROUND;
		if (is_anim_end) {
			change_status(CHARA_STATUS_WAIT);
			return;
		}
		if (is_actionable()) {
			if (common_ground_status_act()) {
				return;
			}
		}
	}
	else {
		if (is_anim_end) {
			change_status(CHARA_STATUS_FALL);
			return;
		}
		if (is_actionable()) {
			if (common_air_status_act()) {
				return;
			}
		}
	}
	common_air_status_general();
}

void FighterInstance::enter_status_throw_air() {
	chara_flag[CHARA_FLAG_THREW_OPPONENT] = false;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b_air");
	}
	else {
		change_anim("throw_f_air");
	}
	add_pos(get_param_float(anim_kind->name + "_move_offset", unique_param_table) * facing_dir, 0);
}

void FighterInstance::exit_status_throw_air() {
	if (anim_kind->name == "throw_b_air") {
		facing_dir *= -1;
		facing_right = !facing_right;
	}
}

void FighterInstance::status_grabbed() {
	if (chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] != 0 && chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] != 0) {
		if (chara_int[CHARA_INT_MANUAL_POS_CHANGE_FRAMES] != 0) {
			add_pos(chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_X] * facing_dir * -1, chara_float[CHARA_FLOAT_MANUAL_POS_CHANGE_Y] * -1);
			chara_int[CHARA_INT_MANUAL_POS_CHANGE_FRAMES]--;
		}
		else {
			set_pos(chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y]);
		}
	}
}

void FighterInstance::enter_status_grabbed() {
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
}

void FighterInstance::exit_status_grabbed() {
	angle = 0;
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
	chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = 0;
	chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
}

void FighterInstance::status_thrown() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_KNOCKDOWN);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED], chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}

void FighterInstance::enter_status_thrown() {
	situation_kind = CHARA_SITUATION_AIR;
	change_anim("knockdown_start");
}

void FighterInstance::exit_status_thrown() {

}

void FighterInstance::status_hitstun() {
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0) {
		if (get_stick_dir() < 4) {
			if (change_status(CHARA_STATUS_CROUCH)) {
				return;
			}
		}
		else {
			if (change_status(CHARA_STATUS_WAIT)) {
				return;
			}
		}
	}
}

void FighterInstance::enter_status_hitstun() {
	if (get_stick_dir() < 4) {
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("crouch_hitstun_l", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("crouch_hitstun_m", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("crouch_hitstun_h", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("stand_hitstun_l", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("stand_hitstun_m", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_hitstun_h", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
	}
}

void FighterInstance::exit_status_hitstun() {}

void FighterInstance::status_hitstun_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING_HITSTUN);
		return;
	}
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0) {
		change_status(CHARA_STATUS_FALL, false);
		return;
	}
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
		if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	}
}

void FighterInstance::enter_status_hitstun_air() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = chara_float[CHARA_FLOAT_LAUNCH_SPEED_X];
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("jump_hitstun", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
}


void FighterInstance::exit_status_hitstun_air() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] /= 2;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] /= 2;
}

void FighterInstance::status_blockstun() {
	if (situation_kind == CHARA_SITUATION_GROUND) {
		if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0) {
			if (get_stick_dir() < 4) {
				if (change_status(CHARA_STATUS_CROUCH)) {
					return;
				}
			}
			else {
				if (change_status(CHARA_STATUS_WAIT)) {
					return;
				}
			}
		}
	}
	else {
		if (pos.y < 50.0) {
			change_status(CHARA_STATUS_LANDING_HITSTUN);
			return;
		}
		if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
			if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
				chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
			}
			add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.5, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
		}
		if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0) {
			change_status(CHARA_STATUS_FALL);

		}
	}
}


void FighterInstance::enter_status_blockstun() {
	if (situation_kind == CHARA_SITUATION_GROUND) {
		if (get_stick_dir() < 4) {
			change_anim("crouch_block", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else if (chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH) {
			change_anim("high_block", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_block", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		change_anim("jump_block", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	}
}

void FighterInstance::exit_status_blockstun() {}

void FighterInstance::status_parry_start() {
	if (situation_kind == CHARA_SITUATION_AIR) {
		if (pos.y < FLOOR_GAMECOORD) {
			change_status(CHARA_STATUS_LANDING);
			return;
		}
		if (is_actionable()) {
			if (common_air_status_act()) {
				return;
			}
		}
		if (is_anim_end) {
			if (change_status(CHARA_STATUS_FALL)) {
				return;
			}
		}
		chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		common_air_status_general();
	}
	else {
		if (is_actionable()) {
			if (common_ground_status_act()) {
				return;
			}
		}
		if (is_anim_end) {
			if (get_stick_dir() < 4) {
				if (change_status(CHARA_STATUS_CROUCH)) {
					return;
				}
			}
			else if (change_status(CHARA_STATUS_WAIT)) {
				return;
			}
		}
		if (get_stick_dir() < 4) {
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
		}
		else if (get_stick_dir() < 7) {
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
		}
		else {
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
		}
	}
}

void FighterInstance::enter_status_parry_start() {
	if (situation_kind == CHARA_SITUATION_AIR) {
		change_anim("parry_start_air");
	}
	else {
		change_anim("parry_start");
	}
}

void FighterInstance::exit_status_parry_start() {}

void FighterInstance::status_parry() {
	if (beginning_hitlag(1) || ending_hitlag(2)) {
		forceStepThroughHitlag();
	}
	if (is_actionable()) {
		if (situation_kind == CHARA_SITUATION_GROUND && common_ground_status_act()) {
			return;
		}
		if (situation_kind == CHARA_SITUATION_AIR && common_air_status_act()) {
			return;
		}
	}
	if (is_anim_end) {
		if (situation_kind == CHARA_SITUATION_AIR) {
			change_status(CHARA_STATUS_FALL);
			return;
		}
		else {
			if (get_stick_dir() < 4) {
				if (chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
					if (change_status(CHARA_STATUS_CROUCH)) {
						return;
					}
				}
				else if (change_status(CHARA_STATUS_CROUCHD)) {
					return;
				}
			}
			else if (change_status(CHARA_STATUS_WAIT)) {
				return;
			}
		}
	}
}

void FighterInstance::enter_status_parry() {
	chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
	if (situation_kind == CHARA_SITUATION_AIR) {
		change_anim("parry_air");
	}
	else {
		if (chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
			change_anim("parry_low");
		}
		else if (chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID) {
			change_anim("parry_mid");
		}
		else {
			change_anim("parry_high");
		}
	}
}

void FighterInstance::exit_status_parry() {}

void FighterInstance::status_crumple() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_KNOCKDOWN);
		return;
	}
}

void FighterInstance::enter_status_crumple() {
	change_anim("crumple");
}

void FighterInstance::exit_status_crumple() {
	chara_flag[CHARA_FLAG_CAN_TECH] = false;
}

void FighterInstance::status_launch_start() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_LAUNCH);
	}
}

void FighterInstance::enter_status_launch_start() {
	change_anim("launch_start");
}

void FighterInstance::exit_status_launch_start() {
	situation_kind = CHARA_SITUATION_AIR; //I love pretending the exit statuses are actually useful :)
}

void FighterInstance::status_launch() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_KNOCKDOWN);
		return;
	}
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
		if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	}
}

void FighterInstance::enter_status_launch() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = chara_float[CHARA_FLOAT_LAUNCH_SPEED_X];
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("launch", -1, chara_int[CHARA_INT_HITSTUN_FRAMES]);
}

void FighterInstance::exit_status_launch() {
	chara_flag[CHARA_FLAG_CAN_TECH] = false;
}

void FighterInstance::status_clank() {

}

void FighterInstance::enter_status_clank() {

}

void FighterInstance::exit_status_clank() {}

void FighterInstance::status_throw_tech() {}

void FighterInstance::enter_status_throw_tech() {}

void FighterInstance::exit_status_throw_tech() {}

void FighterInstance::status_landing() {
	if (chara_int[CHARA_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(CHARA_STATUS_WAIT)) {
			return;
		}
	}
	else {
		chara_int[CHARA_INT_LANDING_LAG]--;
	}
}

void FighterInstance::enter_status_landing() {
	FighterInstance* that = fighter_instance_accessor->fighter_instance[!id];
	if (that->pos.x - that->pos.x_spr_offset / 2 == WINDOW_WIDTH / -2 && pos.x - pos.x_spr_offset / 2 == WINDOW_WIDTH / -2) {
		that->pos.x += 20;
	}
	if (that->pos.x + that->pos.x_spr_offset / 2 == WINDOW_WIDTH / 2 && pos.x + pos.x_spr_offset / 2 == WINDOW_WIDTH / 2) {
		that->pos.x -= 20;
	}
	chara_int[CHARA_INT_LANDING_LAG] = 2;
	change_anim("landing", -1, chara_int[CHARA_INT_LANDING_LAG]);
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = CHARA_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void FighterInstance::exit_status_landing() {}

void FighterInstance::status_landing_attack() {
	if (chara_int[CHARA_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(CHARA_STATUS_WAIT)) {
			return;
		}
	}
	else {
		chara_int[CHARA_INT_LANDING_LAG]--;
	}
}


void FighterInstance::enter_status_landing_attack() {
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("lp_landing_lag");
	}
	else if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("mp_landing_lag");
	}
	else if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("hp_landing_lag");
	}
	else if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("lk_landing_lag");
	}
	else if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("mk_landing_lag");
	}
	else if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		chara_int[CHARA_INT_LANDING_LAG] = get_param_int("hk_landing_lag");
	}
	change_anim("landing", -1, chara_int[CHARA_INT_LANDING_LAG]);
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = CHARA_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void FighterInstance::exit_status_landing_attack() {}

void FighterInstance::status_landing_hitstun() {
	if (chara_int[CHARA_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(CHARA_STATUS_WAIT)) {
			return;
		}
	}
	else {
		chara_int[CHARA_INT_LANDING_LAG]--;
	}
}


void FighterInstance::enter_status_landing_hitstun() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_LANDING_LAG] = 4;
	change_anim("landing_hitstun", -1, chara_int[CHARA_INT_LANDING_LAG]);
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
	situation_kind = CHARA_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void FighterInstance::exit_status_landing_hitstun() {

}

void FighterInstance::status_knockdown_start() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_KNOCKDOWN);
	}
}

void FighterInstance::enter_status_knockdown_start() {
	change_anim("knockdown_start");
}

void FighterInstance::exit_status_knockdown_start() {

}

void FighterInstance::status_knockdown() {
	if (is_anim_end) {
		if (chara_int[CHARA_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) { 
			//Hacky impl for 3 different wakeup speeds, in the future we can just make a different animation for each speed
			change_status(CHARA_STATUS_WAKEUP);
		}
		else {
			chara_int[CHARA_INT_WAKEUP_SPEED] --;
		}
	}
}

void FighterInstance::enter_status_knockdown() {
	if (chara_flag[CHARA_FLAG_CAN_TECH]) {
		if (chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
		}
		if (chara_int[CHARA_INT_WAKEUP_SPEED] == WAKEUP_SPEED_SLOW) {
			change_anim("knockdown_wait");
		}
		if (chara_int[CHARA_INT_WAKEUP_SPEED] == WAKEUP_SPEED_DEFAULT) {
			change_anim("knockdown_wait");
		}
		if (chara_int[CHARA_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) {
			change_anim("knockdown_wait");
		}
	}
	else {
		change_anim("knockdown_wait");
	}
	situation_kind = CHARA_SITUATION_DOWN;
}

void FighterInstance::exit_status_knockdown() {
	chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	situation_kind = CHARA_SITUATION_GROUND;
	chara_flag[CHARA_FLAG_CAN_TECH] = true;
}

void FighterInstance::status_wakeup() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void FighterInstance::enter_status_wakeup() {
	change_anim("wakeup");
}

void FighterInstance::exit_status_wakeup() {

}