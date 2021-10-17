#include "PlayerInfo.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>

PlayerInfo::PlayerInfo() {}

PlayerInfo::PlayerInfo(SDL_Renderer *renderer)
{
	// runs on creation of instance;
	superInit(0, renderer);
}

void PlayerInfo::superInit(int id, SDL_Renderer *renderer)
{
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?
	if (id == 0)
	{
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -50, 0);
	}
	else if (id == 1)
	{
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 50, 0);
	}

	load_anim_list(renderer);
	loadDefaultButtonMap();
	load_params();
	loadStatusFunctions();
	change_status(CHARA_STATUS_WAIT, false, false);

	chara_int[CHARA_INT_DASH_F_WINDOW] = 0;
	chara_int[CHARA_INT_DASH_B_WINDOW] = 0;
}

//Setup

void PlayerInfo::load_anim_list(SDL_Renderer *renderer)
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
		string width;
		string height;
		string faf;
		anim_list >> name >> path >> frame_count >> width >> height >> faf;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		animation_table[i].sprite_width = ymlChopInt(width);
		animation_table[i].sprite_height = ymlChopInt(height);
		animation_table[i].faf = ymlChopInt(faf);
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void PlayerInfo::load_params()
{
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail())
	{
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	string walk_f_speed;
	string walk_b_speed;
	string dash_f_accel_frame;
	string dash_f_speed;
	string dash_f_maintain_speed_frame;
	string dash_b_accel_frame;
	string dash_b_speed;
	string dash_b_maintain_speed_frame;
	string dash_cancel_kind;
	string dash_f_cancel_frame;
	string dash_b_cancel_frame;
	string jump_y_init_speed;
	string jump_y_init_speed_s;
	string jump_x_speed;
	string gravity;
	string max_fall_speed;
	string empty_landing_lag;
	string lp_landing_lag;
	string mp_landing_lag;
	string hp_landing_lag;
	string lk_landing_lag;
	string mk_landing_lag;
	string hk_landing_lag;
	string health;
	string has_airdash;
	string meter_gain_on_parry;

	stats_table >> walk_f_speed >> walk_b_speed >> dash_f_accel_frame >> dash_f_speed >> dash_f_maintain_speed_frame >> dash_b_accel_frame >> dash_b_speed >> dash_b_maintain_speed_frame >> dash_cancel_kind >> dash_f_cancel_frame >> dash_b_cancel_frame >> jump_y_init_speed >> jump_y_init_speed_s >> jump_x_speed >> gravity >> max_fall_speed >> empty_landing_lag >> lp_landing_lag >> mp_landing_lag >> hp_landing_lag >> lk_landing_lag >> mk_landing_lag >> hk_landing_lag >> health >> has_airdash >> meter_gain_on_parry;

	stats.walk_f_speed = ymlChopFloat(walk_f_speed);
	stats.walk_b_speed = ymlChopFloat(walk_b_speed);
	stats.dash_f_accel_frame = ymlChopInt(dash_f_accel_frame);
	stats.dash_f_speed = ymlChopFloat(dash_f_speed);
	stats.dash_f_maintain_speed_frame = ymlChopInt(dash_f_maintain_speed_frame);
	stats.dash_b_accel_frame = ymlChopInt(dash_b_accel_frame);
	stats.dash_b_speed = ymlChopFloat(dash_b_speed);
	stats.dash_b_maintain_speed_frame = ymlChopInt(dash_b_maintain_speed_frame);
	stats.dash_cancel_kind = ymlChopInt(dash_cancel_kind);
	stats.dash_f_cancel_frame = ymlChopInt(dash_f_cancel_frame);
	stats.dash_b_cancel_frame = ymlChopInt(dash_b_cancel_frame);
	stats.jump_y_init_speed = ymlChopFloat(jump_y_init_speed);
	stats.jump_y_init_speed_s = ymlChopFloat(jump_y_init_speed_s);
	stats.jump_x_speed = ymlChopFloat(jump_x_speed);
	stats.gravity = ymlChopFloat(gravity);
	stats.max_fall_speed = ymlChopFloat(max_fall_speed);
	stats.empty_landing_lag = ymlChopInt(empty_landing_lag);
	stats.lp_landing_lag = ymlChopInt(lp_landing_lag);
	stats.mp_landing_lag = ymlChopInt(mp_landing_lag);
	stats.hp_landing_lag = ymlChopInt(hp_landing_lag);
	stats.lk_landing_lag = ymlChopInt(lk_landing_lag);
	stats.mk_landing_lag = ymlChopInt(mk_landing_lag);
	stats.hk_landing_lag = ymlChopInt(hk_landing_lag);
	stats.health = ymlChopInt(health);
	stats.has_airdash = (bool)ymlChopInt(has_airdash);
	stats.meter_gain_on_parry = ymlChopFloat(meter_gain_on_parry);

	stats_table.close();
}

void PlayerInfo::loadDefaultButtonMap()
{
	if (id == 0)
	{
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_W;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Y;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_U;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_I;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_H;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_J;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_K;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
		button_info[BUTTON_DEBUG].mapping = SDL_SCANCODE_SLASH;
		button_info[BUTTON_DEBUG_2].mapping = SDL_SCANCODE_LCTRL;
	}
	else if (id == 1)
	{
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_UP;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;

		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Z;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_X;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_C;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_V;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_B;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_N;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_RETURN;
		button_info[BUTTON_DEBUG].mapping = SDL_SCANCODE_LSHIFT;
		button_info[BUTTON_DEBUG_2].mapping = SDL_SCANCODE_LCTRL;
	}
}

void PlayerInfo::loadStatusFunctions()
{
	pStatus[CHARA_STATUS_WAIT] = &PlayerInfo::status_wait;
	pEnter_status[CHARA_STATUS_WAIT] = &PlayerInfo::enter_status_wait;
	pExit_status[CHARA_STATUS_WAIT] = &PlayerInfo::exit_status_wait;

	pStatus[CHARA_STATUS_WALKF] = &PlayerInfo::status_walkf;
	pEnter_status[CHARA_STATUS_WALKF] = &PlayerInfo::enter_status_walkf;
	pExit_status[CHARA_STATUS_WALKF] = &PlayerInfo::exit_status_walkf;

	pStatus[CHARA_STATUS_WALKB] = &PlayerInfo::status_walkb;
	pEnter_status[CHARA_STATUS_WALKB] = &PlayerInfo::enter_status_walkb;
	pExit_status[CHARA_STATUS_WALKB] = &PlayerInfo::exit_status_walkb;

	pStatus[CHARA_STATUS_DASH] = &PlayerInfo::status_dash;
	pEnter_status[CHARA_STATUS_DASH] = &PlayerInfo::enter_status_dash;
	pExit_status[CHARA_STATUS_DASH] = &PlayerInfo::exit_status_dash;

	pStatus[CHARA_STATUS_DASHB] = &PlayerInfo::status_dashb;
	pEnter_status[CHARA_STATUS_DASHB] = &PlayerInfo::enter_status_dashb;
	pExit_status[CHARA_STATUS_DASHB] = &PlayerInfo::exit_status_dashb;

	pStatus[CHARA_STATUS_CROUCHD] = &PlayerInfo::status_crouchd;
	pEnter_status[CHARA_STATUS_CROUCHD] = &PlayerInfo::enter_status_crouchd;
	pExit_status[CHARA_STATUS_CROUCHD] = &PlayerInfo::exit_status_crouchd;

	pStatus[CHARA_STATUS_CROUCH] = &PlayerInfo::status_crouch;
	pEnter_status[CHARA_STATUS_CROUCH] = &PlayerInfo::enter_status_crouch;
	pExit_status[CHARA_STATUS_CROUCH] = &PlayerInfo::exit_status_crouch;

	pStatus[CHARA_STATUS_CROUCHU] = &PlayerInfo::status_crouchu;
	pEnter_status[CHARA_STATUS_CROUCHU] = &PlayerInfo::enter_status_crouchu;
	pExit_status[CHARA_STATUS_CROUCHU] = &PlayerInfo::exit_status_crouchu;

	pStatus[CHARA_STATUS_JUMPSQUAT] = &PlayerInfo::status_jumpsquat;
	pEnter_status[CHARA_STATUS_JUMPSQUAT] = &PlayerInfo::enter_status_jumpsquat;
	pExit_status[CHARA_STATUS_JUMPSQUAT] = &PlayerInfo::exit_status_jumpsquat;

	pStatus[CHARA_STATUS_JUMP] = &PlayerInfo::status_jump;
	pEnter_status[CHARA_STATUS_JUMP] = &PlayerInfo::enter_status_jump;
	pExit_status[CHARA_STATUS_JUMP] = &PlayerInfo::exit_status_jump;

	pStatus[CHARA_STATUS_FALL] = &PlayerInfo::status_fall;
	pEnter_status[CHARA_STATUS_FALL] = &PlayerInfo::enter_status_fall;
	pExit_status[CHARA_STATUS_FALL] = &PlayerInfo::exit_status_fall;

	pStatus[CHARA_STATUS_ATTACK] = &PlayerInfo::status_attack;
	pEnter_status[CHARA_STATUS_ATTACK] = &PlayerInfo::enter_status_attack;
	pExit_status[CHARA_STATUS_ATTACK] = &PlayerInfo::exit_status_attack;

	pStatus[CHARA_STATUS_ATTACK_AIR] = &PlayerInfo::status_attack_air;
	pEnter_status[CHARA_STATUS_ATTACK_AIR] = &PlayerInfo::enter_status_attack_air;
	pExit_status[CHARA_STATUS_ATTACK_AIR] = &PlayerInfo::exit_status_attack_air;

	pStatus[CHARA_STATUS_GRAB] = &PlayerInfo::status_grab;
	pEnter_status[CHARA_STATUS_GRAB] = &PlayerInfo::enter_status_grab;
	pExit_status[CHARA_STATUS_GRAB] = &PlayerInfo::exit_status_grab;

	pStatus[CHARA_STATUS_GRAB_AIR] = &PlayerInfo::status_grab_air;
	pEnter_status[CHARA_STATUS_GRAB_AIR] = &PlayerInfo::enter_status_grab_air;
	pExit_status[CHARA_STATUS_GRAB_AIR] = &PlayerInfo::exit_status_grab_air;

	pStatus[CHARA_STATUS_HITSTUN] = &PlayerInfo::status_hitstun;
	pEnter_status[CHARA_STATUS_HITSTUN] = &PlayerInfo::enter_status_hitstun;
	pExit_status[CHARA_STATUS_HITSTUN] = &PlayerInfo::exit_status_hitstun;

	pStatus[CHARA_STATUS_HITSTUN_AIR] = &PlayerInfo::status_hitstun_air;
	pEnter_status[CHARA_STATUS_HITSTUN_AIR] = &PlayerInfo::enter_status_hitstun_air;
	pExit_status[CHARA_STATUS_HITSTUN_AIR] = &PlayerInfo::exit_status_hitstun_air;

	pStatus[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::status_blockstun;
	pEnter_status[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::enter_status_blockstun;
	pExit_status[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::exit_status_blockstun;

	pStatus[CHARA_STATUS_PARRY_START] = &PlayerInfo::status_parry_start;
	pEnter_status[CHARA_STATUS_PARRY_START] = &PlayerInfo::enter_status_parry_start;
	pExit_status[CHARA_STATUS_PARRY_START] = &PlayerInfo::exit_status_parry_start;

	pStatus[CHARA_STATUS_PARRY] = &PlayerInfo::status_parry;
	pEnter_status[CHARA_STATUS_PARRY] = &PlayerInfo::enter_status_parry;
	pExit_status[CHARA_STATUS_PARRY] = &PlayerInfo::exit_status_parry;

	pStatus[CHARA_STATUS_CRUMPLE] = &PlayerInfo::status_crumple;
	pEnter_status[CHARA_STATUS_CRUMPLE] = &PlayerInfo::enter_status_crumple;
	pExit_status[CHARA_STATUS_CRUMPLE] = &PlayerInfo::exit_status_crumple;

	pStatus[CHARA_STATUS_LAUNCH_START] = &PlayerInfo::status_launch_start;
	pEnter_status[CHARA_STATUS_LAUNCH_START] = &PlayerInfo::enter_status_launch_start;
	pExit_status[CHARA_STATUS_LAUNCH_START] = &PlayerInfo::exit_status_launch_start;

	pStatus[CHARA_STATUS_LAUNCH] = &PlayerInfo::status_launch;
	pEnter_status[CHARA_STATUS_LAUNCH] = &PlayerInfo::enter_status_launch;
	pExit_status[CHARA_STATUS_LAUNCH] = &PlayerInfo::exit_status_launch;

	pStatus[CHARA_STATUS_CLANK] = &PlayerInfo::status_clank;
	pEnter_status[CHARA_STATUS_CLANK] = &PlayerInfo::enter_status_clank;
	pExit_status[CHARA_STATUS_CLANK] = &PlayerInfo::exit_status_clank;

	pStatus[CHARA_STATUS_THROW_TECH] = &PlayerInfo::status_throw_tech;
	pEnter_status[CHARA_STATUS_THROW_TECH] = &PlayerInfo::enter_status_throw_tech;
	pExit_status[CHARA_STATUS_THROW_TECH] = &PlayerInfo::exit_status_throw_tech;

	pStatus[CHARA_STATUS_LANDING] = &PlayerInfo::status_landing;
	pEnter_status[CHARA_STATUS_LANDING] = &PlayerInfo::enter_status_landing;
	pExit_status[CHARA_STATUS_LANDING] = &PlayerInfo::exit_status_landing;

	pStatus[CHARA_STATUS_LANDING_ATTACK] = &PlayerInfo::status_landing_attack;
	pEnter_status[CHARA_STATUS_LANDING_ATTACK] = &PlayerInfo::enter_status_landing_attack;
	pExit_status[CHARA_STATUS_LANDING_ATTACK] = &PlayerInfo::exit_status_landing_attack;

	pStatus[CHARA_STATUS_LANDING_HITSTUN] = &PlayerInfo::status_landing_hitstun;
	pEnter_status[CHARA_STATUS_LANDING_HITSTUN] = &PlayerInfo::enter_status_landing_hitstun;
	pExit_status[CHARA_STATUS_LANDING_HITSTUN] = &PlayerInfo::exit_status_landing_hitstun;
}

//Move Scripting

/*
	Basically, each instruction in move scripts should only be running once, so if the number of executed statements >= the one we're currently trying
	to execute, whatever's inside of the block won't run. Smash does something similar except it doesn't need to specify the number of executed
	statements, and I'm not entirely sure how that works, but this approach is still pretty solid. (It'll make more sense when we start putting more
	instructions outside of status scripts and inside of move scripts)

	P.S. It's misspelled in Smash as well. You can fix it if you want, idrc
*/

void PlayerInfo::set_current_move_script(string anim_name) {}

bool PlayerInfo::is_excute_frame(int excute_count, int frame)
{
	if (this->frame >= frame)
	{ //If we've reached the statement in question
		if (this->excute_count >= excute_count)
		{ //If we've already executed the statement
			return false;
		}
		else
		{ //If we are at the correct frame and haven't executed the statement yet
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else
	{ //If we still haven't reached the correct frame
		last_excute_frame = frame;
		return false;
	}
}

bool PlayerInfo::is_excute_wait(int excute_count, int frames)
{
	if (frame >= last_excute_frame + frames)
	{ //If it's been enough time since the last non-executed statement
		if (this->excute_count >= excute_count)
		{ //If we already executed the statement
			return false;
		}
		else
		{ //Success
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else
	{ //Still waiting
		return false;
	}
}

//Inputs

void PlayerInfo::processInput()
{
	update_hitbox_pos();
	update_grabbox_pos();
	update_hurtbox_pos();
	if (get_flick_dir() == 6)
	{
		chara_int[CHARA_INT_DASH_F_WINDOW] = 8;
	}
	if (get_flick_dir() == 4)
	{
		chara_int[CHARA_INT_DASH_B_WINDOW] = 8;
	}
	int stick_dir = get_stick_dir();
	if (stick_dir < 4)
	{ //disgusting
		chara_int[CHARA_INT_DOWN_CHARGE_TIMER] = 6;
		chara_int[CHARA_INT_DOWN_CHARGE_FRAMES]++;
	}
	else if (chara_int[CHARA_INT_DOWN_CHARGE_TIMER] != 0)
	{
		chara_int[CHARA_INT_DOWN_CHARGE_TIMER]--;
	}
	else
	{
		chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (stick_dir == 1 || stick_dir == 4 || stick_dir == 7)
	{
		chara_int[CHARA_INT_BACK_CHARGE_FRAMES]++;
		chara_int[CHARA_INT_BACK_CHARGE_TIMER] = 6;
	}
	else if (chara_int[CHARA_INT_BACK_CHARGE_TIMER] != 0)
	{
		chara_int[CHARA_INT_BACK_CHARGE_TIMER]--;
	}
	else
	{
		chara_int[CHARA_INT_BACK_CHARGE_FRAMES] = 0;
	}
	if (chara_int[CHARA_INT_HITLAG_FRAMES] != 0)
	{
		if (chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] != 0.0 && situation_kind == CHARA_SITUATION_GROUND)
		{
			pos.x -= chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] * facing_dir;
		}
	}
	else
	{
		chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = 0.0;
	}
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			chara_flag[CHARA_FLAG_HAS_ATTACK] = true;
			chara_flag[CHARA_FLAG_HAD_ATTACK_IN_STATUS] = true;
			break;
		}
		else
		{
			chara_flag[CHARA_FLAG_HAS_ATTACK] = false;
		}
	}
	if (check_button_on(BUTTON_START))
	{
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

bool PlayerInfo::check_button_on(u32 button)
{
	return button_info[button].button_on;
}

bool PlayerInfo::check_button_trigger(u32 button)
{
	return button_info[button].changed && button_info[button].button_on;
}

bool PlayerInfo::check_button_release(u32 button)
{
	return button_info[button].changed && !button_info[button].button_on;
}

i32 PlayerInfo::get_stick_dir()
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

i32 PlayerInfo::get_flick_dir()
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

//Position

bool PlayerInfo::add_pos(int x, int y)
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

bool PlayerInfo::set_pos(int x, int y)
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

bool PlayerInfo::invalid_x(f32 x)
{
	return x > WINDOW_WIDTH / 2 || x < WINDOW_WIDTH / -2;
}

bool PlayerInfo::invalid_y(f32 y)
{
	return y < 0 || y > WINDOW_HEIGHT;
}

//Hitbox

void PlayerInfo::new_hitbox(int id, f32 damage, f32 chip_damage, f32 counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
							int hitbox_kind, f32 meter_gain_on_hit, f32 meter_gain_on_counterhit, f32 meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
							int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, f32 hit_pushback, f32 block_pushback, int clank_kind,
							bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, f32 launch_init_y,
							f32 launch_gravity_y, f32 launch_max_fall_speed, f32 launch_speed_x, bool use_player_pos)
{
	if (id < 10)
	{
		hitboxes[id] = Hitbox(this, id, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
							  meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
							  attack_level, hit_pushback, block_pushback, clank_kind, success_hit, juggle_set, max_juggle, hit_status, counterhit_status,
							  counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, use_player_pos);
	}
}

void PlayerInfo::update_hitbox_connect()
{
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			hitboxes[i].update_connect();
		}
	}
}

void PlayerInfo::update_hitbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			hitboxes[i].update_pos(this);
		}
	}
}

void PlayerInfo::clear_hitbox(int id)
{
	hitboxes[id].clear();
}

void PlayerInfo::clear_hitbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		hitboxes[i].clear();
	}
}

//Grabbox

void PlayerInfo::update_grabbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (grabboxes[i].id != -1)
		{
			grabboxes[i].update_pos(this);
		}
	}
}

void PlayerInfo::clear_grabbox(int id)
{
	grabboxes[id].clear();
}

void PlayerInfo::clear_grabbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		grabboxes[i].clear();
	}
}

//Hurtbox

void PlayerInfo::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind)
{
	if (id < 10)
	{
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}

void PlayerInfo::update_hurtbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (hurtboxes[i].id != -1)
		{
			hurtboxes[i].update_pos(this);
		}
	}
}

void PlayerInfo::clear_hurtbox(int id)
{
	hurtboxes[id].clear();
}

void PlayerInfo::clear_hurtbox_all()
{
	for (int i = 0; i < 10; i++)
	{
		hurtboxes[i].clear();
	}
}

//Transitions

bool PlayerInfo::is_actionable()
{
	if (chara_int[CHARA_INT_HITSTUN_FRAMES] == 0 && chara_int[CHARA_INT_HITLAG_FRAMES] == 0)
	{
		if (anim_kind->faf == -1)
		{
			return is_anim_end;
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

bool PlayerInfo::can_kara()
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

void PlayerInfo::change_anim(string animation_name, int frame_rate, int entry_frame)
{
	excute_count = 0;
	last_excute_frame = 0;
	int anim_to_use = -1;
	for (int i = 0; i < 60; i++)
	{
		if (animation_table[i].name == animation_name)
		{
			render_frame = entry_frame;
			hold_ms = (1000 / frame_rate);
			frame = (render_frame * ((1000 / 60) / hold_ms));
			set_current_move_script(animation_name);
			startAnimation(&animation_table[i]);
			return;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
}

void PlayerInfo::startAnimation(Animation *animation)
{
	is_anim_end = false;
	anim_kind = animation;
	pos.x_spr_offset = animation->sprite_width / 2;
	pos.y_spr_offset = animation->sprite_height;
	last_frame_ms = SDL_GetTicks();
	frame_rect = getFrame(frame, anim_kind);
}

bool PlayerInfo::canStep()
{
	if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0)
	{
		frame++;
		u32 delta = SDL_GetTicks() - last_frame_ms;
		if (delta > hold_ms)
		{
			last_frame_ms = SDL_GetTicks();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void PlayerInfo::stepAnimation()
{
	int last_frame = render_frame;
	frame_rect = getFrame(render_frame, anim_kind);
	if (render_frame == anim_kind->length)
	{
		render_frame = 0;
		frame = 0;
	}
	else
	{
		render_frame++;
	}
	is_anim_end = last_frame > frame;
}

//Status

bool PlayerInfo::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status)
{
	if (new_status_kind != status_kind || !require_different_status)
	{
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		kara_enabled = false;
		chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = false;
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

void PlayerInfo::playoutStatus()
{
	(this->*pStatus[status_kind])();
	move_script();
}

bool PlayerInfo::common_ground_status_act()
{
	if (is_actionable())
	{
		if (check_button_trigger(BUTTON_LP) || check_button_trigger(BUTTON_MP) || check_button_trigger(BUTTON_HP) || check_button_trigger(BUTTON_LK) || check_button_trigger(BUTTON_MK) || check_button_trigger(BUTTON_HK))
		{
			if (check_button_trigger(BUTTON_LP))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CLP;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
				}
			}
			if (check_button_trigger(BUTTON_LK))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CLK;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
				}
			}
			if (check_button_trigger(BUTTON_MP))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CMP;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
				}
			}
			if (check_button_trigger(BUTTON_MK))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CMK;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MK;
				}
			}
			if (check_button_trigger(BUTTON_HP))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CHP;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
				}
			}
			if (check_button_trigger(BUTTON_HK))
			{
				if (get_stick_dir() < 4)
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_CHK;
				}
				else
				{
					chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HK;
				}
			}
			return change_status(CHARA_STATUS_ATTACK, true, false);
		}
		if (get_stick_dir() == 6)
		{
			if (chara_int[CHARA_INT_DASH_F_WINDOW] != 0 && get_flick_dir() == 6)
			{
				return change_status(CHARA_STATUS_DASH);
			}
			else
			{
				return change_status(CHARA_STATUS_WALKF);
			}
		}
		if (get_stick_dir() == 4)
		{
			if (chara_int[CHARA_INT_DASH_B_WINDOW] != 0 && get_flick_dir() == 4)
			{
				return change_status(CHARA_STATUS_DASHB);
			}
			else
			{
				return change_status(CHARA_STATUS_WALKB);
			}
		}
		if (get_stick_dir() > 6)
		{
			return change_status(CHARA_STATUS_JUMPSQUAT);
		}
		if (get_flick_dir() < 4 && get_flick_dir() != 0)
		{
			return change_status(CHARA_STATUS_CROUCHD);
		}
	}
	return false;
}

bool PlayerInfo::common_air_status_act()
{
	if (is_actionable())
	{
		if (check_button_trigger(BUTTON_LP) || check_button_trigger(BUTTON_MP) || check_button_trigger(BUTTON_HP) || check_button_trigger(BUTTON_LK) || check_button_trigger(BUTTON_MK) || check_button_trigger(BUTTON_HK))
		{
			if (check_button_trigger(BUTTON_LP))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
			if (check_button_trigger(BUTTON_LK))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
			if (check_button_trigger(BUTTON_MP))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
			}
			if (check_button_trigger(BUTTON_MK))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MK;
			}
			if (check_button_trigger(BUTTON_HP))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
			}
			if (check_button_trigger(BUTTON_HK))
			{
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HK;
			}
			return change_status(CHARA_STATUS_ATTACK_AIR, false, false);
		}
	}
	return false;
}

bool PlayerInfo::common_air_status_general()
{
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > stats.max_fall_speed * -1.0)
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= stats.gravity;
	}
	if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F)
	{
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
	}
	if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_B)
	{
		add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, 0);
	}
	add_pos(0, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
	return false;
}

u32 PlayerInfo::get_status_group(u32 status)
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

void PlayerInfo::status_wait()
{
	if (common_ground_status_act())
	{
		return;
	}
}

void PlayerInfo::enter_status_wait()
{
	set_pos(pos.x, FLOOR_GAMECOORD);
	change_anim("wait", 30, 0);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);

	situation_kind = CHARA_SITUATION_GROUND;
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
}

void PlayerInfo::exit_status_wait()
{
}

void PlayerInfo::status_walkf()
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
	add_pos(stats.walk_f_speed * facing_dir, 0);
}

void PlayerInfo::enter_status_walkf()
{
	change_anim("walk_f", 30);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_walkf()
{
}

void PlayerInfo::status_walkb()
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
		add_pos(stats.walk_f_speed * facing_dir * -1, 0);
	}
}

void PlayerInfo::enter_status_walkb()
{
	change_anim("walk_b", 30);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_walkb()
{
}

void PlayerInfo::status_dash()
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
	int min_frame = stats.dash_f_accel_frame;
	int max_frame = min_frame + stats.dash_f_maintain_speed_frame;

	if (frame >= min_frame && frame < max_frame)
	{
		add_pos(stats.dash_f_speed * facing_dir, 0);
	}
	else if (frame < max_frame + 2)
	{
		add_pos((stats.walk_f_speed + stats.dash_f_speed) * facing_dir / 2, 0);
	}

	if (frame >= stats.dash_f_cancel_frame)
	{
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL])
		{
			if (get_flick_dir() == 4)
			{
				if (stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE)
				{
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASHB, false);
				return;
			}
		}
		else if (stats.dash_cancel_kind == DASH_CANCEL_KIND_FOLLOWUP)
		{
		}
	}
}

void PlayerInfo::enter_status_dash()
{
	change_anim("dash_f", 30);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_dash()
{
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
}

void PlayerInfo::status_dashb()
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
	int min_frame = stats.dash_b_accel_frame;
	int max_frame = min_frame + stats.dash_b_maintain_speed_frame;

	if (frame >= min_frame && frame < max_frame)
	{
		add_pos(stats.dash_b_speed * facing_dir * -1, 0);
	}
	else if (frame < max_frame + 2)
	{
		add_pos((stats.walk_b_speed + stats.dash_b_speed) * facing_dir / -2, 0);
	}

	if (frame >= stats.dash_f_cancel_frame)
	{
		if (!chara_flag[CHARA_FLAG_DASH_CANCEL])
		{
			if (get_flick_dir() == 6)
			{
				if (stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE)
				{
					chara_flag[CHARA_FLAG_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (stats.dash_cancel_kind == DASH_CANCEL_KIND_FOLLOWUP)
	{
	}
}

void PlayerInfo::enter_status_dashb()
{
	change_anim("dash_b", 30);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_dashb()
{
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
}

void PlayerInfo::status_crouchd()
{
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_CROUCH);
		return;
	}
}

void PlayerInfo::enter_status_crouchd()
{
	change_anim("crouch_d", 120);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_crouchd()
{
}

void PlayerInfo::status_crouch()
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

void PlayerInfo::enter_status_crouch()
{
	change_anim("crouch", 30);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_crouch()
{
}

void PlayerInfo::status_crouchu()
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

void PlayerInfo::enter_status_crouchu()
{
	change_anim("crouch_u", 120);
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_crouchu()
{
}

void PlayerInfo::status_jumpsquat()
{
	if (is_anim_end)
	{
		change_status(CHARA_STATUS_JUMP);
		return;
	}
}

void PlayerInfo::enter_status_jumpsquat()
{
	change_anim("jump_squat", 120);
}

void PlayerInfo::exit_status_jumpsquat()
{
	chara_flag[CHARA_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	situation_kind = CHARA_SITUATION_AIR;
}

void PlayerInfo::status_jump()
{
	if (common_air_status_act())
	{
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

void PlayerInfo::enter_status_jump()
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
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = stats.jump_y_init_speed_s;
	}
	else
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = stats.jump_y_init_speed;
	}
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = stats.jump_x_speed;
}

void PlayerInfo::exit_status_jump()
{
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void PlayerInfo::status_fall()
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

void PlayerInfo::enter_status_fall()
{
	change_anim("fall");
	new_hurtbox(0, GameCoordinate{-25, 0}, GameCoordinate{20, 120}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-35, 75}, GameCoordinate{35, 115}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_fall()
{
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
	chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void PlayerInfo::status_attack()
{
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

void PlayerInfo::enter_status_attack()
{
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP)
	{
		change_anim("stand_lp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP)
	{
		change_anim("stand_mp");
	}
	if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_HP)
	{
		change_anim("stand_hp");
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

void PlayerInfo::exit_status_attack()
{
}

void PlayerInfo::status_attack_air()
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

void PlayerInfo::enter_status_attack_air()
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

void PlayerInfo::exit_status_attack_air()
{
}

void PlayerInfo::status_grab()
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

void PlayerInfo::enter_status_grab()
{
	change_anim("grab");
}

void PlayerInfo::exit_status_grab()
{
}

void PlayerInfo::status_grab_air()
{
}

void PlayerInfo::enter_status_grab_air()
{
}

void PlayerInfo::exit_status_grab_air()
{
}

void PlayerInfo::status_hitstun()
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

void PlayerInfo::enter_status_hitstun()
{
	if (get_stick_dir() < 4)
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT)
		{
			change_anim("crouch_hitstun_l", 30);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM)
		{
			change_anim("crouch_hitstun_m", 30);
		}
		else
		{
			change_anim("crouch_hitstun_h", 30);
		}
	}
	else
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT)
		{
			change_anim("stand_hitstun_l", 30);
		}
		else if (chara_int[CHARA_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM)
		{
			change_anim("stand_hitstun_m", 30);
		}
		else
		{
			change_anim("stand_hitstun_h", 30);
		}
	}
}

void PlayerInfo::exit_status_hitstun()
{
}

void PlayerInfo::status_hitstun_air()
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
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > stats.max_fall_speed * -1.0)
	{
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= stats.gravity;
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}

void PlayerInfo::enter_status_hitstun_air()
{
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = chara_float[CHARA_FLOAT_LAUNCH_SPEED_X];
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("jump_hitstun", 30);
}

void PlayerInfo::exit_status_hitstun_air()
{
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0;
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = 0;
}

void PlayerInfo::status_blockstun()
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
	else
	{
		if (get_stick_dir() < 4)
		{
			change_anim("crouch_block", 30);
		}
		else
		{
			change_anim("stand_block", 30);
		}
	}
}

void PlayerInfo::enter_status_blockstun()
{
	if (get_stick_dir() < 4)
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 70}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		change_anim("crouch_block", 30);
	}
	else if (chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH)
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		change_anim("high_block", 30);
	}
	else
	{
		new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		change_anim("stand_block", 30);
	}
}

void PlayerInfo::exit_status_blockstun()
{
}

void PlayerInfo::status_parry_start()
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

void PlayerInfo::enter_status_parry_start()
{
	change_anim("parry_start", 15);
}

void PlayerInfo::exit_status_parry_start()
{
}

void PlayerInfo::status_parry()
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

void PlayerInfo::enter_status_parry()
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

void PlayerInfo::exit_status_parry()
{
}

void PlayerInfo::status_crumple()
{
}

void PlayerInfo::enter_status_crumple()
{
}

void PlayerInfo::exit_status_crumple()
{
}

void PlayerInfo::status_launch_start()
{
}

void PlayerInfo::enter_status_launch_start()
{
}

void PlayerInfo::exit_status_launch_start()
{
}

void PlayerInfo::status_launch()
{
}

void PlayerInfo::enter_status_launch()
{
}

void PlayerInfo::exit_status_launch()
{
}

void PlayerInfo::status_clank()
{
}

void PlayerInfo::enter_status_clank()
{
}

void PlayerInfo::exit_status_clank()
{
}

void PlayerInfo::status_throw_tech()
{
}

void PlayerInfo::enter_status_throw_tech()
{
}

void PlayerInfo::exit_status_throw_tech()
{
}

void PlayerInfo::status_landing()
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

void PlayerInfo::enter_status_landing()
{
	change_anim("landing", 30);
	chara_int[CHARA_INT_LANDING_LAG] = 2;
	situation_kind = CHARA_SITUATION_GROUND;
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_landing()
{
}

void PlayerInfo::status_landing_attack()
{
}

void PlayerInfo::enter_status_landing_attack()
{
}

void PlayerInfo::exit_status_landing_attack()
{
}

void PlayerInfo::status_landing_hitstun()
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

void PlayerInfo::enter_status_landing_hitstun()
{
	change_anim("landing_hitstun", 30);
	chara_int[CHARA_INT_LANDING_LAG] = 4;
	chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
	situation_kind = CHARA_SITUATION_GROUND;
	new_hurtbox(0, GameCoordinate{-35, 0}, GameCoordinate{37, 35}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(1, GameCoordinate{-25, 0}, GameCoordinate{20, 110}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
	new_hurtbox(2, GameCoordinate{-15, 55}, GameCoordinate{35, 95}, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
}

void PlayerInfo::exit_status_landing_hitstun()
{
}