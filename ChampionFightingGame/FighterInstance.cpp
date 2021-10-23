#include "FighterInstance.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;

FighterInstance::FighterInstance() {
}

FighterInstance::FighterInstance(SDL_Renderer* renderer, PlayerInfo *player_info) {
	this->player_info = player_info;
	// runs on creation of instance;
		// no it doesn't
	superInit(0, renderer);
}

void FighterInstance::init_projectile(int id, GameCoordinate pos) {
	projectile_objects[id]->id = this->id;
	projectile_objects[id]->pos.x = this->pos.x + pos.x * facing_dir;
	projectile_objects[id]->pos.y = this->pos.y + pos.y;
	projectile_objects[id]->facing_right = facing_right;
	projectile_objects[id]->facing_dir = facing_dir;
}

void FighterInstance::destroy_projectile(int id) {
	projectile_objects[id]->id = -1;
}

void FighterInstance::superInit(int id, SDL_Renderer *renderer)
{
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?
	if (id == 0)
	{
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1)
	{
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}

	load_anim_list(renderer);
	load_params();
	chara_float[CHARA_FLOAT_HEALTH] = get_param_float("health");
	loadStatusFunctions();
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait", 2, 0);
	status_kind = CHARA_STATUS_WAIT;

	chara_int[CHARA_INT_DASH_F_WINDOW] = 0;
	chara_int[CHARA_INT_DASH_B_WINDOW] = 0;
}

//Setup

void FighterInstance::load_anim_list(SDL_Renderer *renderer)
{
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail())
	{
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string line_1;
	anim_list >> line_1;
	int num_anims = ymlChopInt(line_1);

	for (int i = 0; i < num_anims; i++)
	{
		string name;
		string path;
		string frame_count;
		string faf;
		anim_list >> name >> path >> frame_count >> faf;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		animation_table[i].faf = ymlChopInt(faf);
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void FighterInstance::load_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	int i = 0;
	string stat;
	while (stats_table >> stat) {
		param_table[i].stat = stat;
		stats_table >> param_table[i].type;
		switch (param_table[i].type) {
			case(PARAM_TYPE_INT): {
				stats_table >> param_table[i].value_i;
			} break;
			case(PARAM_TYPE_FLOAT): {
				stats_table >> param_table[i].value_f;
			} break;
			case(PARAM_TYPE_STRING): {
				stats_table >> param_table[i].value_s;
			} break;
			case (PARAM_TYPE_BOOL): {
				stats_table >> param_table[i].value_b;
			} break;
			default: {
				stats_table >> param_table[i].value_i;
			} break;
		}
		i++;
	}

	stats_table.close();
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

	pStatus[CHARA_STATUS_GRAB_AIR] = &FighterInstance::status_grab_air;
	pEnter_status[CHARA_STATUS_GRAB_AIR] = &FighterInstance::enter_status_grab_air;
	pExit_status[CHARA_STATUS_GRAB_AIR] = &FighterInstance::exit_status_grab_air;

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
}

//Params

int FighterInstance::get_param_int(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->param_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_i;
		}
	}

	return 0;
}

float FighterInstance::get_param_float(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->param_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_f;
		}
	}

	return 0.0;
}

string FighterInstance::get_param_string(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->param_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_s;
		}
	}

	return "";
}

bool FighterInstance::get_param_bool(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->param_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_b;
		}
	}

	return false;
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

	//Dash Input

	if (get_flick_dir() == 6) {
		chara_int[CHARA_INT_DASH_F_WINDOW] = 9;
	}
	if (get_flick_dir() == 4)
	{
		chara_int[CHARA_INT_DASH_B_WINDOW] = 9;
	}

	//Special Inputs

	int stick_dir = get_stick_dir();

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
		if (id == 0)
		{
			pos.x = -200.0;
		}
		else
		{
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

int FighterInstance::get_stick_dir()
{
	if (check_button_on(BUTTON_UP) && !check_button_on(BUTTON_DOWN))
	{
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT)))
		{
			if (check_button_on(BUTTON_RIGHT) == facing_right)
			{
				return 9;
			}
			else
			{
				return 7;
			}
		}
		else
		{
			return 8;
		}
	}
	else if (check_button_on(BUTTON_DOWN) && !check_button_on(BUTTON_UP))
	{
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT)))
		{
			if (check_button_on(BUTTON_RIGHT) == facing_right)
			{
				return 3;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT)))
		{
			if (check_button_on(BUTTON_RIGHT) == facing_right)
			{
				return 6;
			}
			else
			{
				return 4;
			}
		}
		else
		{
			return 5;
		}
	}
}

int FighterInstance::get_flick_dir()
{
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir)
	{
		return 0;
	}
	else
	{
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
		return button_check && chara_int[CHARA_INT_HITLAG_FRAMES] == 0;
	}
	else {
		return SPECIAL_INPUT_NONE;
	}
}

//Position

bool FighterInstance::add_pos(int x, int y)
{
	pos.x += x;
	pos.y += y;
	if (invalid_x(pos.x) || invalid_y(pos.y))
	{
		if (invalid_x(pos.x))
		{
			pos.x = prevpos.x;
		}
		if (invalid_y(pos.x))
		{
			pos.y = prevpos.y;
		}
		return false;
	}
	else
	{
		return true;
	}
}

bool FighterInstance::set_pos(int x, int y)
{
	pos.x = x;
	pos.y = y;
	if (invalid_x(pos.x) || invalid_y(pos.y))
	{
		if (invalid_x(pos.x))
		{
			pos.x = prevpos.x;
		}
		if (invalid_y(pos.y))
		{
			pos.y = prevpos.y;
		}
		return false;
	}
	else
	{
		return true;
	}
}

bool FighterInstance::invalid_x(float x) {
	float opponent_x = fighter_instance_accessor->fighter_instance[!id]->pos.x;
	float x_distance = std::max(opponent_x, x) - std::min(opponent_x, x);

	return x > WINDOW_WIDTH / 2 || x < WINDOW_WIDTH / -2 || x_distance > CAMERA_MAX_ZOOM_OUT;
}

bool FighterInstance::invalid_y(float y)
{
	return y < 0 || y > WINDOW_HEIGHT;
}

//Hitbox

void FighterInstance::new_hitbox(int id, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
							int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
							int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
							bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
							float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool use_player_pos)
{
	if (id < 10)
	{
		hitboxes[id] = Hitbox(this, id, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
							  meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
							  attack_level, hit_pushback, block_pushback, clank_kind, success_hit, juggle_set, max_juggle, hit_status, counterhit_status,
							  counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, use_player_pos);
	}
}

void FighterInstance::update_hitbox_connect()
{
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			hitboxes[i].update_connect();
		}
	}
}

void FighterInstance::update_hitbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			hitboxes[i].update_pos(this);
		}
	}
}

void FighterInstance::clear_hitbox(int id)
{
	hitboxes[id].clear();
}

void FighterInstance::clear_hitbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		hitboxes[i].clear();
	}
}

//Grabbox

void FighterInstance::new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	
	if (id < 10) {
		grabboxes[id] = Grabbox(this, id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
	}
}

void FighterInstance::update_grabbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (grabboxes[i].id != -1)
		{
			grabboxes[i].update_pos(this);
		}
	}
}

void FighterInstance::clear_grabbox(int id)
{
	grabboxes[id].clear();
}

void FighterInstance::clear_grabbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		grabboxes[i].clear();
	}
}

//Hurtbox

void FighterInstance::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind)
{
	if (id < 10)
	{
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}

void FighterInstance::update_hurtbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (hurtboxes[i].id != -1)
		{
			hurtboxes[i].update_pos(this);
		}
	}
}

void FighterInstance::clear_hurtbox(int id)
{
	hurtboxes[id].clear();
}

void FighterInstance::clear_hurtbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		hurtboxes[i].clear();
	}
}

//Transitions

bool FighterInstance::is_actionable()
{
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0 && chara_int[CHARA_INT_HITLAG_FRAMES] == 0)
	{
		if (anim_kind->faf == -1) {
			return render_frame >= anim_kind->length;
		}
		else
		{
			return frame >= anim_kind->faf;
		}
	}
	else
	{
		return false;
	}
}

bool FighterInstance::can_kara()
{
	if (((chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS]) || kara_enabled)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Animations

void FighterInstance::change_anim(string animation_name, int frame_rate, int entry_frame) {
	excute_count = 0;
	last_excute_frame = 0;
	int anim_to_use = -1;
	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			render_frame = entry_frame;
			frame = entry_frame;
			max_ticks = frame_rate;
			ticks = 0;
      
			set_current_move_script(animation_name);
			startAnimation(&animation_table[i]);
			return;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
}

void FighterInstance::startAnimation(Animation *animation)
{
	is_anim_end = false;
	anim_kind = animation;
	int width;
	int height;
	SDL_QueryTexture(animation->SPRITESHEET, NULL, NULL, &width, &height);
	pos.x_anim_offset = width / (anim_kind->length + 1) / 2;
	pos.y_anim_offset = height;
	frame_rect = getFrame(frame, anim_kind);
}

bool FighterInstance::canStep() {
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
	}
	else {
		render_frame++;
	}
	is_anim_end = last_frame > frame;
}

//Status

bool FighterInstance::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status)
{
	if (new_status_kind != status_kind || !require_different_status)
	{
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		kara_enabled = false;
		chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = false;
		chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS] = false;
		chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS] = false;
		if (call_end_status)
		{
			(this->*pExit_status[status_kind])();
		}
		status_kind = new_status_kind;
		(this->*pEnter_status[status_kind])();
		return true;
	}
	else
	{
		return false;
	}
}

void FighterInstance::playoutStatus() {
	(this->*pStatus[status_kind])();
	move_script();
}

bool FighterInstance::common_ground_status_act() {
	if (is_actionable() && !specific_ground_status_act()) {
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
		if (get_flick_dir() < 4 && get_flick_dir() != 0 && get_status_group(status_kind) != STATUS_GROUP_CROUCH) {
			return change_status(CHARA_STATUS_CROUCHD);
		}
	}
	return false;
}

bool FighterInstance::common_air_status_act() {
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			u32 grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(CHARA_STATUS_GRAB_AIR));
			}
			if (check_button_input(BUTTON_LP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
			if (check_button_input(BUTTON_LK)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
			u32 parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(CHARA_STATUS_PARRY_START));
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

bool FighterInstance::common_air_status_general()
{
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
	return false;
}

u32 FighterInstance::get_status_group(u32 status)
{
	switch (status)
	{
	case (CHARA_STATUS_HITSTUN):
	case (CHARA_STATUS_HITSTUN_AIR):
	case (CHARA_STATUS_LAUNCH):
	case (CHARA_STATUS_CRUMPLE):
	{
		return STATUS_GROUP_HITSTUN;
	}
	break;
	case (CHARA_STATUS_CROUCHD):
	case (CHARA_STATUS_CROUCH):
	case (CHARA_STATUS_CROUCHU):
	{
		return STATUS_GROUP_CROUCH;
	}
	break;
	default:
	{
		return STATUS_GROUP_NORMAL;
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

void FighterInstance::status_wait()
{
	if (common_ground_status_act())
	{
		return;
	}
}


void FighterInstance::enter_status_wait()
{
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait", 2, 0);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);


	situation_kind = CHARA_SITUATION_GROUND;
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
	common_ground_status_act();
}

void FighterInstance::exit_status_wait()
{
}

void FighterInstance::status_walkf()
{
	if (common_ground_status_act())
	{
		return;
	}
	if (get_stick_dir() == 5)
	{
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	add_pos(get_param_float("walk_f_speed") * facing_dir, 0);
}

void FighterInstance::enter_status_walkf()
{
	change_anim("walk_f", 2);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_walkf()
{
}

void FighterInstance::status_walkb()
{
	if (common_ground_status_act())
	{
		return;
	}
	if (get_stick_dir() == 5)
	{
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (chara_flag[CHARA_FLAG_PROX_GUARD])
	{
		if (anim_kind->name != "stand_block")
		{
			change_anim("stand_block");
		}
	}
	else
	{
		if (anim_kind->name == "stand_block")
		{
			change_anim("walk_b");
		}
		add_pos(get_param_float("walk_f_speed") * facing_dir * -1, 0);
	}
}

void FighterInstance::enter_status_walkb()
{
	change_anim("walk_b", 2);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_walkb()
{
}

void FighterInstance::status_dash()
{
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act())
	{
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame)
	{
		add_pos(get_param_float("dash_f_speed") * facing_dir, 0);
	}
	else if (frame < max_frame + 2)
	{
		add_pos((get_param_float("walk_f_speed") + get_param_float("dash_f_speed")) * facing_dir / 2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame"))
	{
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL])
		{
			if (get_stick_dir() == 4)
			{
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE)
				{
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASHB, false);
				return;
			}
		}
		else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP)
		{
		}
	}
}

void FighterInstance::enter_status_dash()
{
	change_anim("dash_f", 2);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_dash()
{
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
}

void FighterInstance::status_dashb()
{
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act())
	{
		return;
	}
	int min_frame = get_param_int("dash_b_accel_frame");
	int max_frame = min_frame + get_param_int("dash_b_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame)
	{
		add_pos(get_param_float("dash_b_speed") * facing_dir * -1, 0);
	}
	else if (frame < max_frame + 2)
	{
		add_pos((get_param_float("walk_b_speed") + get_param_float("dash_b_speed")) * facing_dir / -2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame"))
	{
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL])
		{
			if (get_stick_dir() == 6)
			{
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE)
				{
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP)
	{
	}
}

void FighterInstance::enter_status_dashb()
{
	change_anim("dash_b", 2);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_dashb()
{
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
		change_anim("dash_f", 2);
	}
	if (chara_int[CHARA_INT_DASH_AIR_DIR] == 2) {
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_b_speed");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		change_anim("dash_b", 2);
	}
}

void FighterInstance::exit_status_dash_air() {

}


void FighterInstance::status_crouchd() {
	if (common_ground_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_CROUCH);
		return;
	}
}

void FighterInstance::enter_status_crouchd()
{
	change_anim("crouch_d");
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_crouchd()
{
}

void FighterInstance::status_crouch()
{
	if (common_ground_status_act())
	{
		return;
	}
	if (get_stick_dir() == 5)
	{
		change_status(CHARA_STATUS_CROUCHU);
		return;
	}
}

void FighterInstance::enter_status_crouch()
{
	change_anim("crouch", 2);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_crouch()
{
}

void FighterInstance::status_crouchu()
{
	if (common_ground_status_act())
	{
		return;
	}
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_WAIT);
	}
}

void FighterInstance::enter_status_crouchu()
{
	change_anim("crouch_u");
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_crouchu()
{
}

void FighterInstance::status_jumpsquat()
{
	if (frame == 4 || is_anim_end) {
		change_status(CHARA_STATUS_JUMP);
		return;
	}
}

void FighterInstance::enter_status_jumpsquat() {
	change_anim("jump_squat");
}

void FighterInstance::exit_status_jumpsquat()
{
	chara_flag[CHARA_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	chara_flag[CHARA_FLAG_USED_AIRDASH] = false;
	situation_kind = CHARA_SITUATION_AIR;
}

void FighterInstance::status_jump()
{
	if (common_air_status_act()) {
		return;
	}
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_FALL, false);
		return;
	}
	if (pos.y < FLOOR_GAMECOORD)
	{
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	common_air_status_general();
}

void FighterInstance::enter_status_jump()
{
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1)
	{
		change_anim("jump_b");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		new_hurtbox(0, GameCoordinate{-35, 40}, GameCoordinate{35, 105}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3)
	{
		change_anim("jump_f");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
		new_hurtbox(0, GameCoordinate{-35, 40}, GameCoordinate{35, 105}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	}
	else
	{
		change_anim("jump");
		new_hurtbox(0, GameCoordinate{-25, 20}, GameCoordinate{20, 120}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(1, GameCoordinate{-35, 75}, GameCoordinate{35, 115}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if (chara_flag[CHARA_FLAG_SHORT_HOP])
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed_s");
	}
	else
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed");
	}
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float("jump_x_speed");
	common_air_status_act();
}

void FighterInstance::exit_status_jump()
{
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void FighterInstance::status_fall()
{
	if (common_air_status_act())
	{
		return;
	}
	if (pos.y < FLOOR_GAMECOORD)
	{
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	common_air_status_general();
}

void FighterInstance::enter_status_fall()
{
	change_anim("fall");
	new_hurtbox(0, GameCoordinate{-25, 0}, GameCoordinate{20, 120}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-35, 75}, GameCoordinate{35, 115}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}


void FighterInstance::exit_status_fall() {
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void FighterInstance::status_attack() {
	if (specific_status_attack()) {
		return;
	}
	if (is_anim_end)
	{
		if (get_stick_dir() < 4)
		{
			if (change_status(CHARA_STATUS_CROUCH))
			{
				return;
			}
		}
		else if (change_status(CHARA_STATUS_WAIT))
		{
			return;
		}
	}
	if (is_actionable())
	{
		if (common_ground_status_act())
		{
			return;
		}
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK))
	{
		change_status(CHARA_STATUS_GRAB);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMP || chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMK)
	{
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK))
		{
			if (chara_flag[CHARA_FLAG_HAS_ATTACK] && chara_float[CHARA_FLOAT_SUPER_METER] >= 30.0)
			{
				chara_float[CHARA_FLOAT_SUPER_METER] -= 30.0;
				change_status(CHARA_STATUS_PARRY_START);
				return;
			}
			else if (!chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS])
			{
				change_status(CHARA_STATUS_PARRY_START);
				return;
			}
		}
	}
}

void FighterInstance::enter_status_attack()
{
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP)
	{
		change_anim("stand_lp", 2);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP)
	{
		change_anim("stand_mp", 2);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP)
	{
		change_anim("stand_hp", 2);
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK)
	{
		change_anim("stand_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK)
	{
		change_anim("stand_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HK)
	{
		change_anim("stand_hk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CLP)
	{
		change_anim("crouch_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMP)
	{
		change_anim("crouch_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CHP)
	{
		change_anim("crouch_hp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CLK)
	{
		change_anim("crouch_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CMK)
	{
		change_anim("crouch_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_CHK)
	{
		change_anim("crouch_hk");
	}
}

void FighterInstance::exit_status_attack()
{
}

void FighterInstance::status_attack_air()
{
	if (pos.y < FLOOR_GAMECOORD)
	{
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	if (is_anim_end)
	{
		if (change_status(CHARA_STATUS_FALL))
		{
			return;
		}
	}
	if (is_actionable())
	{
		if (common_air_status_act())
		{
			return;
		}
	}
	common_air_status_general();
}

void FighterInstance::enter_status_attack_air()
{
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP)
	{
		change_anim("jump_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP)
	{
		change_anim("jump_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP)
	{
		change_anim("jump_hp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LK)
	{
		change_anim("jump_lk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MK)
	{
		change_anim("jump_mk");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HK)
	{
		change_anim("jump_hk");
	}
}

void FighterInstance::exit_status_attack_air()
{
}

void FighterInstance::status_grab()
{
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable())
	{
		if (common_ground_status_act())
		{
			return;
		}
	}
}

void FighterInstance::enter_status_grab() {
	change_anim("grab", 2);
}

void FighterInstance::exit_status_grab()
{
}

void FighterInstance::status_grab_air()
{
}

void FighterInstance::enter_status_grab_air()
{
}

void FighterInstance::exit_status_grab_air()
{
}

void FighterInstance::status_hitstun()
{
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0)
	{
		if (get_stick_dir() < 4)
		{
			if (change_status(CHARA_STATUS_CROUCH))
			{
				return;
			}
		}
		else
		{
			if (change_status(CHARA_STATUS_WAIT))
			{
				return;
			}
		}
	}
}

void FighterInstance::enter_status_hitstun() {
	if (get_stick_dir() < 4) {
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("crouch_hitstun_l", 2);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("crouch_hitstun_m", 2);
		}
		else
		{
			change_anim("crouch_hitstun_h", 2);
		}
	}
	else
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT)
		{
			change_anim("stand_hitstun_l", 2);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM)
		{
			change_anim("stand_hitstun_m", 2);
		}
		else
		{
			change_anim("stand_hitstun_h", 2);
		}
	}
}

void FighterInstance::exit_status_hitstun() {
}

void FighterInstance::status_hitstun_air()
{
	if (pos.y < FLOOR_GAMECOORD)
	{
		change_status(CHARA_STATUS_LANDING_HITSTUN);
		return;
	}
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0)
	{
		change_status(CHARA_STATUS_FALL, false);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0)
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}

void FighterInstance::enter_status_hitstun_air()
{
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = chara_float[CHARA_FLOAT_LAUNCH_SPEED_X];
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("jump_hitstun", 2);
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
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			change_anim("crouch_block", 2);
		}
		else if (chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			change_anim("high_block", 2);
		}
		else {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			change_anim("stand_block", 2);
		}
	}
	else {
		change_anim("stand_block", 2);
		chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	}
}

void FighterInstance::exit_status_blockstun()
{
}

void FighterInstance::status_parry_start()
{
	if (is_actionable())
	{
		if (common_ground_status_act())
		{
			return;
		}
	}
	if (is_anim_end)
	{
		if (get_stick_dir() < 4)
		{
			if (change_status(CHARA_STATUS_CROUCH))
			{
				return;
			}
		}
		else if (change_status(CHARA_STATUS_WAIT))
		{
			return;
		}
	}
	if (get_stick_dir() < 4)
	{
		chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
	}
	else if (get_stick_dir() < 7)
	{
		chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
	}
	else
	{
		chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
	}
}

void FighterInstance::enter_status_parry_start()
{
	change_anim("parry_start", 4);
}

void FighterInstance::exit_status_parry_start()
{
}

void FighterInstance::status_parry()
{
	if (is_actionable())
	{
		if (common_ground_status_act())
		{
			return;
		}
	}
	if (is_anim_end)
	{
		if (get_stick_dir() < 4)
		{
			if (change_status(CHARA_STATUS_CROUCH))
			{
				return;
			}
		}
		else if (change_status(CHARA_STATUS_WAIT))
		{
			return;
		}
	}
}

void FighterInstance::enter_status_parry()
{
	if (chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW)
	{
		change_anim("parry_low");
	}
	else if (chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID)
	{
		change_anim("parry_mid");
	}
	else
	{
		change_anim("parry_high");
	}
}

void FighterInstance::exit_status_parry()
{
}

void FighterInstance::status_crumple()
{
}

void FighterInstance::enter_status_crumple()
{
}

void FighterInstance::exit_status_crumple()
{
}

void FighterInstance::status_launch_start()
{
}

void FighterInstance::enter_status_launch_start()
{
}

void FighterInstance::exit_status_launch_start()
{
}

void FighterInstance::status_launch()
{
}

void FighterInstance::enter_status_launch()
{
}

void FighterInstance::exit_status_launch()
{
}

void FighterInstance::status_clank()
{
}

void FighterInstance::enter_status_clank()
{
}

void FighterInstance::exit_status_clank()
{
}

void FighterInstance::status_throw_tech()
{
}

void FighterInstance::enter_status_throw_tech()
{
}

void FighterInstance::exit_status_throw_tech()
{
}

void FighterInstance::status_landing()
{
	if (chara_int[CHARA_INT_LANDING_LAG] == 0)
	{
		if (common_ground_status_act())
		{
			return;
		}
		else if (change_status(CHARA_STATUS_WAIT))
		{
			return;
		}
	}
	else
	{
		chara_int[CHARA_INT_LANDING_LAG]--;
	}
}

void FighterInstance::enter_status_landing()
{
	change_anim("landing", 2);
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_LANDING_LAG] = 2;
	situation_kind = CHARA_SITUATION_GROUND;
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void FighterInstance::exit_status_landing()
{
}

void FighterInstance::status_landing_attack()
{
}


void FighterInstance::enter_status_landing_attack() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
}

void FighterInstance::exit_status_landing_attack()
{
}

void FighterInstance::status_landing_hitstun()
{
	if (chara_int[CHARA_INT_LANDING_LAG] == 0)
	{
		if (common_ground_status_act())
		{
			return;
		}
		else if (change_status(CHARA_STATUS_WAIT))
		{
			return;
		}
	}
	else
	{
		chara_int[CHARA_INT_LANDING_LAG]--;
	}
}


void FighterInstance::enter_status_landing_hitstun() {
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	change_anim("landing_hitstun", 2);
	chara_int[CHARA_INT_LANDING_LAG] = 4;
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
	situation_kind = CHARA_SITUATION_GROUND;
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}


void FighterInstance::exit_status_landing_hitstun() {

}

void FighterInstance::status_knockdown_start() {
	if (frame >= anim_kind->length - 10) {
		if (get_stick_dir() == 8) {
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_FAST;
		}
		else if (get_stick_dir() == 2) {
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_SLOW;
		}
		else {
			chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
		}
	}
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
		if (chara_int[CHARA_INT_WAKEUP_SPEED] == WAKEUP_SPEED_MIN) {
			change_status(CHARA_STATUS_WAIT); //this will be wakeup in the future but there's no wakeup animation atm
		}
		else {
			chara_int[CHARA_INT_WAKEUP_SPEED] --;
		}
	}
}

void FighterInstance::enter_status_knockdown() {
	change_anim("knockdown_wait");
	situation_kind = CHARA_SITUATION_DOWN;
}

void FighterInstance::exit_status_knockdown() {
	chara_int[CHARA_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	situation_kind = CHARA_SITUATION_GROUND;
}