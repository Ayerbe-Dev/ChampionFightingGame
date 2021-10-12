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
	load_anim_list(renderer);
	startAnimation("wait", 30);
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

	stats.walk_f_speed = ymlChopFloat (walk_f_speed);
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

void PlayerInfo::startAnimation(string animation_name, int frame_rate , int entry_frame) {
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

bool PlayerInfo::change_status(u32 new_status_kind, bool call_end_status) {
	//Call the exit status function for whatever we're leaving, change the value, then call the entry status to prepare any relevant info
	//This function will return whether or not a status was successfully changed, typically from the common_status_act functions.
	if (new_status_kind != status_kind) {
		if (call_end_status) { //For some related statuses, not everything should be reset
			exit_status_pointer[status_kind](this);
		}
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

