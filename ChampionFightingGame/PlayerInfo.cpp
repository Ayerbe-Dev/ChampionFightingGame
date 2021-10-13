#include "PlayerInfo.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>

PlayerInfo::PlayerInfo() { }

PlayerInfo::PlayerInfo(int id, string chara_kind, SDL_Renderer *renderer) {
	// runs on creation of instance;	
	this->id = id;
	resource_dir = ("resource/chara/" + chara_kind);
	superInit(renderer);
}

void PlayerInfo::superInit(SDL_Renderer* renderer) {
	// set position
	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 320);
	}
	else if (id == 1) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 320);
	}
	load_anim_list(renderer);
	change_anim("wait", 30);
	loadDefaultButtonMap();
	loadStatusFunctions();
	load_params();

	chara_int[CHARA_INT_DASH_F_WINDOW] = 0;
	chara_int[CHARA_INT_DASH_B_WINDOW] = 0;
}

void PlayerInfo::load_anim_list(SDL_Renderer *renderer) {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string line_1;
	anim_list >> line_1;
	int num_anims = ymlChopInt(line_1);

	for (int i = 0; i < num_anims; i++) {
		string filename;
		string frame_count;
		string width;
		string height;
		string faf;
		anim_list >> filename >> frame_count >> width >> height >> faf;
		animation_table[i].name = ymlChopString(filename);
		animation_table[i].path = (resource_dir + "/anims/" + animation_table[i].name + ".png");
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		animation_table[i].sprite_width = ymlChopInt(width);
		animation_table[i].sprite_height = ymlChopInt(height);
		animation_table[i].faf = ymlChopInt(faf);
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void PlayerInfo::load_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
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

	stats_table >> walk_f_speed >> walk_b_speed >> dash_f_accel_frame >> dash_f_speed >> dash_f_maintain_speed_frame >> dash_b_accel_frame
		>> dash_b_speed >> dash_b_maintain_speed_frame >> dash_cancel_kind >> dash_f_cancel_frame >> dash_b_cancel_frame >> jump_y_init_speed
		>> jump_y_init_speed_s >> jump_x_speed >> gravity >> max_fall_speed >> empty_landing_lag >> lp_landing_lag >> mp_landing_lag
		>> hp_landing_lag >> lk_landing_lag >> mk_landing_lag >> hk_landing_lag >> health >> has_airdash;

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

	stats_table.close();
}

bool PlayerInfo::is_actionable() {
	if (anim_kind->faf == -1) {
		return is_anim_end;
	}
	else {
		return frame >= anim_kind->faf;
	}
}

void PlayerInfo::change_anim(string animation_name, int frame_rate, int entry_frame) {
	int anim_to_use = -1;
	for (int i = 0; i < 60; i++) {
		if (animation_table[i].name == animation_name) {
			frame = entry_frame;
			hold_ms = (1000 / frame_rate);
			startAnimation(&animation_table[i]);
			return;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
}

void PlayerInfo::startAnimation(Animation* animation) {
	is_anim_end = false;
	anim_kind = animation;
	pos.x_spr_offset = animation->sprite_width / 2;
	pos.y_spr_offset = animation->sprite_height;
	last_frame_ms = SDL_GetTicks();
	frame_rect = getFrame(frame, anim_kind); // needs to be here in case the player was in the middle of an animation.
}

bool PlayerInfo::canStep() {
	u32 delta = SDL_GetTicks() - last_frame_ms;
	if (delta > hold_ms) {
		last_frame_ms = SDL_GetTicks();
		return true;
	}
	else {
		return false;
	}
}

void PlayerInfo::stepAnimation() {
	//tbh i think this impl isn't that bad
	int last_frame = frame;
	frame_rect = getFrame(frame, anim_kind);
	if (frame == anim_kind->length) {
		frame = 0;
		/*
			Instead of going back to idle after reaching the end of an animation, go back to 0. If the animation is designed to loop, nothing
			happens, but if it isn't, we still save that the end of the animation was reached. From there if we want to make certain status
			changes based on the animation ending, doing so is a pretty simple check.
		*/
	}
	else {
		frame ++;
	}
	is_anim_end = last_frame > frame; //This needs to be here or else is_anim_end will never be reset back to false
}

void PlayerInfo::loadDefaultButtonMap() {
	if (id == 0) {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_W;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Z;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
	}
	else if (id == 1) {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_UP;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_RETURN;
	}
}

bool PlayerInfo::check_button_on(u32 button) {
	return button_info[button].button_on;
}

bool PlayerInfo::check_button_trigger(u32 button) {
	return button_info[button].changed && button_info[button].button_on;
}

bool PlayerInfo::check_button_release(u32 button) {
	return button_info[button].changed && !button_info[button].button_on;
}

i32 PlayerInfo::get_stick_dir() {
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

i32 PlayerInfo::get_flick_dir() { 
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

bool PlayerInfo::change_status(u32 new_status_kind, bool call_end_status) {
	if (new_status_kind != status_kind) {
		if (call_end_status) {
			(this->*pExit_status[status_kind])(); //hey it's fine because now we don't have to think about it
		}
		status_kind = new_status_kind;
		(this->*pEnter_status[status_kind])();
		return true;
	}
	else {
		return false;
	}
}

void PlayerInfo::playoutStatus() {
	(this->*pStatus[status_kind])();
}

bool PlayerInfo::common_ground_status_act() { 
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
	if (get_stick_dir() < 4) {
		return change_status(CHARA_STATUS_CROUCHD);
	}
	return false;
}

void PlayerInfo::processInput() {
	if (get_flick_dir() == 6) {
		chara_int[CHARA_INT_DASH_F_WINDOW] = 8;
	}
	if (get_flick_dir() == 4) {
		chara_int[CHARA_INT_DASH_B_WINDOW] = 8;
	}
	int stick_dir = get_stick_dir();
	if (stick_dir < 4) {
		chara_int[CHARA_INT_DOWN_CHARGE_TIMER] = 6;
		chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] ++;
	}
	else if (chara_int[CHARA_INT_DOWN_CHARGE_TIMER] != 0) {
		chara_int[CHARA_INT_DOWN_CHARGE_TIMER] --;
	}
	else {
		chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (stick_dir == 1 || stick_dir == 4 || stick_dir == 7) {
		chara_int[CHARA_INT_BACK_CHARGE_FRAMES] ++;
		chara_int[CHARA_INT_BACK_CHARGE_TIMER] = 6;
	}
	else if (chara_int[CHARA_INT_BACK_CHARGE_TIMER] != 0) {
		chara_int[CHARA_INT_BACK_CHARGE_TIMER] --;
	}
	else {
		chara_int[CHARA_INT_BACK_CHARGE_FRAMES] = 0;
	}
	if (check_button_on(BUTTON_START)) {
		pos.y = 0.0;
		if (id == 0) {
			pos.x = -200.0;
		}
		else {
			pos.x = 200.0;
		}
	}
}

// STATUS 

/*
	 ..=====.. |==|
	|| sans || |= |
 _  ||      || |^*| _
|=| o==,===,=o |__||=|
|_|  ________)~`)  |_|
	[========]  ()
*/

void PlayerInfo::loadStatusFunctions() {
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

	pStatus[CHARA_STATUS_ATTACK] = &PlayerInfo::status_attack;
	pEnter_status[CHARA_STATUS_ATTACK] = &PlayerInfo::enter_status_attack;
	pExit_status[CHARA_STATUS_ATTACK] = &PlayerInfo::exit_status_attack;

	pStatus[CHARA_STATUS_HITSTUN] = &PlayerInfo::status_hitstun;
	pEnter_status[CHARA_STATUS_HITSTUN] = &PlayerInfo::enter_status_hitstun;
	pExit_status[CHARA_STATUS_HITSTUN] = &PlayerInfo::exit_status_hitstun;

	pStatus[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::status_blockstun;
	pEnter_status[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::enter_status_blockstun;
	pExit_status[CHARA_STATUS_BLOCKSTUN] = &PlayerInfo::exit_status_blockstun;
}

void PlayerInfo::status_wait() {
	if (common_ground_status_act()) {
		return;
	}
}

void PlayerInfo::enter_status_wait() {
	change_anim("wait", 30, 0);

	situation_kind = CHARA_SITUATION_GROUND;
}

void PlayerInfo::exit_status_wait() {

}

void PlayerInfo::status_walkf() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	pos.x += stats.walk_f_speed * facing_dir;
}

void PlayerInfo::enter_status_walkf() {
	change_anim("walk_f", 30, 0);
}

void PlayerInfo::exit_status_walkf() {

}

void PlayerInfo::status_walkb() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	pos.x -= stats.walk_b_speed * facing_dir;
}

void PlayerInfo::enter_status_walkb() {
	change_anim("walk_b", 30);
}

void PlayerInfo::exit_status_walkb() {

}

void PlayerInfo::status_dash() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
		return;
	}
	int min_frame = stats.dash_f_accel_frame;
	int max_frame = min_frame + stats.dash_f_maintain_speed_frame;

	if (frame >= min_frame && frame < max_frame) {
		pos.x += stats.dash_f_speed * facing_dir;
	}
	else {
		pos.x += stats.walk_f_speed * facing_dir;
	}

	if (frame >= stats.dash_f_cancel_frame) {
		if (!chara_bool[CHARA_BOOL_DASH_CANCEL]) {
			if (get_flick_dir() == 4) {
				if (stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE) {
					chara_bool[CHARA_BOOL_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASHB, false);
				return;
			}
		}
		else if (stats.dash_cancel_kind == DASH_CANCEL_KIND_FOLLOWUP) {

		}
	}
}

void PlayerInfo::enter_status_dash() {
	change_anim("dash_f", 30, 0);
}

void PlayerInfo::exit_status_dash() {
	chara_bool[CHARA_BOOL_DASH_CANCEL] = false;
}

void PlayerInfo::status_dashb() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
		return;
	}
	int min_frame = stats.dash_b_accel_frame;
	int max_frame = min_frame + stats.dash_b_maintain_speed_frame;

	if (frame >= min_frame && frame < max_frame) {
		pos.x -= stats.dash_b_speed * facing_dir;
	}
	else {
		pos.x -= stats.walk_b_speed * facing_dir;
	}

	if (frame >= stats.dash_f_cancel_frame) {
		if (!chara_bool[CHARA_BOOL_DASH_CANCEL]) {
			if (get_flick_dir() == 6) {
				if (stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE) {
					chara_bool[CHARA_BOOL_DASH_CANCEL] = true;
				}
				change_status(CHARA_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (stats.dash_cancel_kind == DASH_CANCEL_KIND_FOLLOWUP) {

	}
}

void PlayerInfo::enter_status_dashb() {
	change_anim("dash_b", 30);
}

void PlayerInfo::exit_status_dashb() {
	chara_bool[CHARA_BOOL_DASH_CANCEL] = false;
}

void PlayerInfo::status_crouchd() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_CROUCH);
		return;
	}
}

void PlayerInfo::enter_status_crouchd() {
	change_anim("crouch_d", 30);
}

void PlayerInfo::exit_status_crouchd() {

}

void PlayerInfo::status_crouch() {
	if (get_stick_dir() == 6) {
		change_status(CHARA_STATUS_WALKF);
		return;
	}
	if (get_stick_dir() == 4) {
		change_status(CHARA_STATUS_WALKB);
		return;
	}
	if (get_stick_dir() > 6) {
		change_status(CHARA_STATUS_JUMPSQUAT);
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(CHARA_STATUS_CROUCHU);
		return;
	}
}

void PlayerInfo::enter_status_crouch() {
	change_anim("crouch", 30);
}

void PlayerInfo::exit_status_crouch() {

}

void PlayerInfo::status_crouchu() {
	if (common_ground_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
	}
}

void PlayerInfo::enter_status_crouchu() {
	change_anim("crouch_u", 30);
}

void PlayerInfo::exit_status_crouchu() {

}

void PlayerInfo::status_jumpsquat() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_JUMP);
		return;
	}
}

void PlayerInfo::enter_status_jumpsquat() {
	change_anim("jump_squat", 120);
}

void PlayerInfo::exit_status_jumpsquat() {
	chara_bool[CHARA_BOOL_SHORT_HOP] = get_stick_dir() < 7;
	situation_kind = CHARA_SITUATION_AIR;
	chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y] = pos.y; //This will eventually be replaced by a proper ground collision check
}

void PlayerInfo::status_jump() {
	if (pos.y > chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y]) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > stats.max_fall_speed * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= stats.gravity;
	}
	if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
		pos.x += stats.jump_x_speed * facing_dir;
	}
	if (chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
		pos.x -= stats.jump_x_speed * facing_dir;
	}
	pos.y -= chara_float[CHARA_FLOAT_CURRENT_Y_SPEED];
}

void PlayerInfo::enter_status_jump() {
	if (get_stick_dir() == 7
		|| get_stick_dir() == 4
		|| get_stick_dir() == 1) {
		change_anim("jump_b");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9
		|| get_stick_dir() == 6
		|| get_stick_dir() == 3) {
		change_anim("jump_f");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
	}
	else {
		change_anim("jump");
		chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if (chara_bool[CHARA_BOOL_SHORT_HOP]) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = stats.jump_y_init_speed_s;
	}
	else {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = stats.jump_y_init_speed;
	}
}

void PlayerInfo::exit_status_jump() {
	pos.y = chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y];
	situation_kind = CHARA_SITUATION_GROUND;
}

void PlayerInfo::status_attack() {

}

void PlayerInfo::enter_status_attack() {

}

void PlayerInfo::exit_status_attack() {

}

void PlayerInfo::status_hitstun() {

}

void PlayerInfo::enter_status_hitstun() {

}

void PlayerInfo::exit_status_hitstun() {

}

void PlayerInfo::status_blockstun() {

}

void PlayerInfo::enter_status_blockstun() {

}

void PlayerInfo::exit_status_blockstun() {

}
