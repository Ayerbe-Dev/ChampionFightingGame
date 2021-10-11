#include "PlayerInfo.h"
#include "GameCoordinate.h"
#include "Animation.h"

PlayerInfo::PlayerInfo() { }

PlayerInfo::PlayerInfo(std::string chara_kind, SDL_Renderer* renderer) {
	// runs on creation of instance;	
	startAnimation(&TEST_IDLE_ANIMATION);

	//other numbers
	height = 100;
	width = 100;
}

void PlayerInfo::startAnimation(Animation* animation) {
	anim_kind = animation;
	frame = 0;
	pos.x_spr_offset = animation->sprite_width / 2;

}

void PlayerInfo::stepAnimation() {
	//tbh i think this impl isn't that bad
	int prev_frame = frame;
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
		frame++;
	}
	is_anim_end = prev_frame > frame;
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

void PlayerInfo::setStateLikePlayer1() {
	id = 0;
	pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
}

void PlayerInfo::setStateLikePlayer2() {
	id = 1;
	pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
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
	if (check_button_on(BUTTON_UP)) {
		pos.y -= 1.0;
	}
	if (check_button_on(BUTTON_DOWN)) {
		pos.y += 1.0;
	}
}

