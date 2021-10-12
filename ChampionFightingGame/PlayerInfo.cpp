#include "PlayerInfo.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>

PlayerInfo::PlayerInfo() { }

PlayerInfo::PlayerInfo(int id, string chara_kind) {
	// runs on creation of instance;	
	this->id = id;
	resource_dir = ("resource/chara/" + chara_kind);
	superInit();
}

void PlayerInfo::superInit() {
	load_anim_list();
	change_anim("wait", 0, 30);
	loadDefaultButtonMap();
	load_params();

	// set position
	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1){
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}
	chara_int[CHARA_INT_DASH_F_WINDOW] = 0;
	chara_int[CHARA_INT_DASH_B_WINDOW] = 0;
}

void PlayerInfo::load_anim_list() {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string line_1;
	anim_list >> line_1;
	int num_anims = stoi(line_1.substr(line_1.find("=") + 1));

	for (int i = 0; i < num_anims; i++) {
		string filename;
		string frame_count;
		string width;
		string height;
		string faf;
		anim_list >> filename >> frame_count >> width >> height >> faf;
		ANIM_TABLE[i][id].ANIMATION_DIR = (resource_dir + "/anims/" + filename.substr(filename.find("=") + 1));
		ANIM_TABLE[i][id].length = stoi(frame_count.substr(frame_count.find("=") + 1)) - 1;
		ANIM_TABLE[i][id].sprite_width = stoi(width.substr(width.find("=") + 1));
		ANIM_TABLE[i][id].sprite_height = stoi(height.substr(height.find("=") + 1));
		ANIM_TABLE[i][id].faf = stoi(faf.substr(faf.find("=") + 1));
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
		>> dash_b_speed >> dash_b_maintain_speed_frame >> dash_cancel_kind >> jump_y_init_speed >> jump_y_init_speed_s >> jump_x_speed >> gravity
		>> max_fall_speed >> empty_landing_lag >> lp_landing_lag >> mp_landing_lag >> hp_landing_lag >> lk_landing_lag >> mk_landing_lag
		>> hk_landing_lag >> health >> has_airdash;

	stats.walk_f_speed = stof(walk_f_speed.substr(walk_f_speed.find("=") + 1));
	stats.walk_b_speed = stof(walk_b_speed.substr(walk_b_speed.find("=") + 1));
	stats.dash_f_accel_frame = stoi(dash_f_accel_frame.substr(dash_f_accel_frame.find("=") + 1));
	stats.dash_f_speed = stof(dash_f_speed.substr(dash_f_speed.find("=") + 1));
	stats.dash_f_maintain_speed_frame = stoi(dash_f_maintain_speed_frame.substr(dash_f_maintain_speed_frame.find("=") + 1));
	stats.dash_b_accel_frame = stoi(dash_b_accel_frame.substr(dash_b_accel_frame.find("=") + 1));
	stats.dash_b_speed = stof(dash_b_speed.substr(dash_b_speed.find("=") + 1));
	stats.dash_b_maintain_speed_frame = stoi(dash_b_maintain_speed_frame.substr(dash_b_maintain_speed_frame.find("=") + 1));
	stats.dash_cancel_kind = stoi(dash_cancel_kind.substr(dash_cancel_kind.find("=") + 1));
	stats.jump_y_init_speed = stof(jump_y_init_speed.substr(jump_y_init_speed.find("=") + 1));
	stats.jump_y_init_speed_s = stof(jump_y_init_speed_s.substr(jump_y_init_speed_s.find("=") + 1));
	stats.jump_x_speed = stof(jump_x_speed.substr(jump_x_speed.find("=") + 1));
	stats.gravity = stof(gravity.substr(gravity.find("=") + 1));
	stats.max_fall_speed = stof(max_fall_speed.substr(max_fall_speed.find("=") + 1));
	stats.empty_landing_lag = stoi(empty_landing_lag.substr(empty_landing_lag.find("=") + 1));
	stats.lp_landing_lag = stoi(lp_landing_lag.substr(lp_landing_lag.find("=") + 1));
	stats.mp_landing_lag = stoi(mp_landing_lag.substr(mp_landing_lag.find("=") + 1));
	stats.hp_landing_lag = stoi(hp_landing_lag.substr(hp_landing_lag.find("=") + 1));
	stats.lk_landing_lag = stoi(lk_landing_lag.substr(lk_landing_lag.find("=") + 1));
	stats.mk_landing_lag = stoi(mk_landing_lag.substr(mk_landing_lag.find("=") + 1));
	stats.hk_landing_lag = stoi(hk_landing_lag.substr(hk_landing_lag.find("=") + 1));
	stats.health = stoi(health.substr(health.find("=") + 1));
	stats.has_airdash = (bool)stoi(has_airdash.substr(has_airdash.find("=") + 1));

	stats_table.close();
}

void PlayerInfo::change_anim(string new_anim_kind, int entry_frame, int div_rate) {
	int anim_to_use = -1;
	new_anim_kind = (resource_dir + "/anims/" + new_anim_kind + ".png");
	for (int i = 0; i < 60; i++) {
		if (new_anim_kind == ANIM_TABLE[i][id].ANIMATION_DIR) {
			anim_to_use = i;
			break;
		}
	}
	if (anim_to_use != -1) {
		frame = entry_frame;
		hold_ms = (1000 / div_rate);
		startAnimation(&ANIM_TABLE[anim_to_use][id]);
	}
	else {
		cout << "Invalid Animation" << endl;
	}
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

i32 PlayerInfo::get_stick_dir() { //Use this instead of check_button_on with left and right, it factors in the player's facing direction
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

i32 PlayerInfo::get_flick_dir() { //get_stick_dir, but only for stick inputs made on that frame
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

bool PlayerInfo::change_status(u32 new_status_kind) {
	//Call the exit status function for whatever we're leaving, change the value, then call the entry status to prepare any relevant info
	//This function will return whether or not a status was successfully changed, typically from the common_status_act functions.
	if (new_status_kind != status_kind) {
		exit_status_pointer[status_kind](this);
		status_kind = new_status_kind;
		enter_status_pointer[status_kind](this);
		return true;
	}
	else {
		return false;
	}
}

bool PlayerInfo::common_ground_status_act() { 
	//Handles all grounded options where the player is fully actionable
	//This function will return whether or not any status was successfully changed. If it was, the status script that called it will return.
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

