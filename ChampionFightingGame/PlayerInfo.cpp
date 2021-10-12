#include "PlayerInfo.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>

PlayerInfo::PlayerInfo() { }

PlayerInfo::PlayerInfo(int id) {
	// runs on creation of instance;	
	this->id = id;
	resource_dir = ("resource/chara/testchar");
	superInit();
}

void PlayerInfo::superInit() {
	load_anim_list();
	change_anim("wait", 0, 30);
	loadDefaultButtonMap();

	// set position
	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1){
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}
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
	anim_kind = animation;
	pos.x_spr_offset = animation->sprite_width / 2;
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
	is_anim_end = last_frame >= frame; //This needs to be here or else is_anim_end will never be reset back to false
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

void PlayerInfo::change_status(u32 new_status_kind) {
	//Call the exit status function for whatever we're leaving, change the value, then call the entry status to prepare any relevant info
	if (new_status_kind != status_kind) {
		exit_status_pointer[status_kind](this);
		status_kind = new_status_kind;
		enter_status_pointer[status_kind](this);
	}
}

void PlayerInfo::common_ground_status_act() { //Handles all grounded options where the player is fully actionable
	if (get_stick_dir() == 6) {
		change_status(CHARA_STATUS_WALKF);
	}
	if (get_stick_dir() == 4) {
		change_status(CHARA_STATUS_WALKB);
	}
	if (get_stick_dir() > 6) {
		change_status(CHARA_STATUS_JUMPSQUAT);
	}
	if (get_stick_dir() < 4) {
		change_status(CHARA_STATUS_CROUCHD);
	}
}

void PlayerInfo::processInput() {
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

