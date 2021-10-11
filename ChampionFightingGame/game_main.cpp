using namespace std;
#include <iostream>
#include <functional>
#include <vector>
#include<string>
#include "animations.cpp"

/*
For each of the user's controls, track what key they're assigned to, whether or not that button is being pressed, and whether or not a change was made
on the current frame
*/



struct Buttons {
	u32 mapping = 0;
	bool button_on = false;
	bool changed = false;
};

class GameCoordinate {
public:
	f32 x;
	f32 y;
	f32 x_offset;
	f32 y_offset;
	f32 x_spr_offset;

	GameCoordinate() {
		x = 0.0;
		y = 0.0;
		x_offset = 0.0;
		y_offset = 0.0;
	}

	GameCoordinate(f32 window_width, f32 window_height) {
		x = 0.0;
		y = 0.0;
		x_offset = window_width / 2;
		y_offset = window_height / 2;
	}

	GameCoordinate(f32 window_width, f32 window_height, f32 start_x, f32 start_y) {
		x = start_x;
		y = start_y;
		x_offset = window_width / 2;
		y_offset = window_height / 2;
	}

	f32 getRenderCoodrinateX() {
		return x + x_offset - x_spr_offset;
	}

	f32 getRenderCoodrinateY() {
		return y + y_offset;
	}
};

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.
class PlayerInfo {
public:
	i64 id;
	string chara_kind;
	GameCoordinate pos;
	GameCoordinate prevpos;
	f32 height;
	f32 width;
	bool facing_right{ true };
	f32 facing_dir{ 1.0 };
	u32 status_kind{ CHARA_STATUS_WAIT };
	void (*status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	void (*enter_status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	void (*exit_status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	Buttons button_info[BUTTON_MAX];
	string resource_dir;
	SDL_Texture* current_texture;
	int frame;
	bool is_anim_end{ false };
	Animation* anim_kind;
	SDL_Rect frame_rect;

	PlayerInfo() { }

	PlayerInfo(string chara_kind, SDL_Renderer* renderer) {
		// runs on creation of instance;	
		startAnimation(&TEST_IDLE_ANIMATION);
		
		//other numbers
		height = 100;
		width = 100;
	}

	void startAnimation(Animation* animation) {
		anim_kind = animation;
		frame = 0;
		pos.x_spr_offset = animation->sprite_width / 2;
		
	}

	void stepAnimation() {
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
		if (prev_frame > frame) {
			is_anim_end = true;
		}
		else {
			is_anim_end = false;
		}
	}
		
	void loadDefaultButtonMap() {
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

	void setStateLikePlayer1() {
		id = 0;
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}

	void setStateLikePlayer2() {
		id = 1;
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
	}

	bool check_button_on(u32 button) {
		return button_info[button].button_on;
	}

	bool check_button_trigger(u32 button) {
		return button_info[button].changed && button_info[button].button_on;
	}

	bool check_button_release(u32 button) {
		return button_info[button].changed && !button_info[button].button_on;
	}

	i32 get_stick_dir() { //Use this instead of check_button_on with left and right, it factors in the player's facing direction
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

	void change_status(u32 new_status_kind) {
		//Call the exit status function for whatever we're leaving, change the value, then call the entry status to prepare any relevant info
		if (new_status_kind != status_kind) {
			exit_status_pointer[status_kind](this);
			status_kind = new_status_kind;
			enter_status_pointer[status_kind](this);
		}
	}

	void processInput() {
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

		if (get_stick_dir() == 6) {
			change_status(CHARA_STATUS_WALKF);
		}
		if (get_stick_dir() == 4) {
			change_status(CHARA_STATUS_WALKB);
		}
	}

	function<void(PlayerInfo*)> wait;
	function<void(PlayerInfo*)> enter_wait;
	function<void(PlayerInfo*)> exit_wait;
	function<void(PlayerInfo*)> walkf;
	function<void(PlayerInfo*)> enter_walkf;
	function<void(PlayerInfo*)> exit_walkf;
	function<void(PlayerInfo*)> walkb;
	function<void(PlayerInfo*)> enter_walkb;
	function<void(PlayerInfo*)> exit_walkb;
	function<void(PlayerInfo*)> dash;
	function<void(PlayerInfo*)> enter_dash;
	function<void(PlayerInfo*)> exit_dash;
	function<void(PlayerInfo*)> dashb;
	function<void(PlayerInfo*)> enter_dashb;
	function<void(PlayerInfo*)> exit_dashb;
	function<void(PlayerInfo*)> crouch;
	function<void(PlayerInfo*)> enter_crouch;
	function<void(PlayerInfo*)> exit_crouch;
	function<void(PlayerInfo*)> crouchs;
	function<void(PlayerInfo*)> enter_crouchs;
	function<void(PlayerInfo*)> exit_crouchs;
	function<void(PlayerInfo*)> jumpsquat;
	function<void(PlayerInfo*)> enter_jumpsquat;
	function<void(PlayerInfo*)> exit_jumpsquat;
	function<void(PlayerInfo*)> jump;
	function<void(PlayerInfo*)> enter_jump;
	function<void(PlayerInfo*)> exit_jump;
	function<void(PlayerInfo*)> attack;
	function<void(PlayerInfo*)> enter_attack;
	function<void(PlayerInfo*)> exit_attack;
	function<void(PlayerInfo*)> hitstun;
	function<void(PlayerInfo*)> enter_hitstun;
	function<void(PlayerInfo*)> exit_hitstun;
	function<void(PlayerInfo*)> blockstun;
	function<void(PlayerInfo*)> enter_blockstun;
	function<void(PlayerInfo*)> exit_blockstun;
};

void status_wait(PlayerInfo* player_info);
void enter_status_wait(PlayerInfo* player_info);
void exit_status_wait(PlayerInfo* player_info);
void status_walkf(PlayerInfo* player_info);
void enter_status_walkf(PlayerInfo* player_info);
void exit_status_walkf(PlayerInfo* player_info);
void status_walkb(PlayerInfo* player_info);
void enter_status_walkb(PlayerInfo* player_info);
void exit_status_walkb(PlayerInfo* player_info);
void status_dash(PlayerInfo* player_info);
void enter_status_dash(PlayerInfo* player_info);
void exit_status_dash(PlayerInfo* player_info);
void status_dashb(PlayerInfo* player_info);
void enter_status_dashb(PlayerInfo* player_info);
void exit_status_dashb(PlayerInfo* player_info);
void status_crouch(PlayerInfo* player_info);
void enter_status_crouch(PlayerInfo* player_info);
void exit_status_crouch(PlayerInfo* player_info);
void status_crouchs(PlayerInfo* player_info);
void enter_status_crouchs(PlayerInfo* player_info);
void exit_status_crouchs(PlayerInfo* player_info);
void status_jumpsquat(PlayerInfo* player_info);
void enter_status_jumpsquat(PlayerInfo* player_info);
void exit_status_jumpsquat(PlayerInfo* player_info);
void status_jump(PlayerInfo* player_info);
void enter_status_jump(PlayerInfo* player_info);
void exit_status_jump(PlayerInfo* player_info);
void status_attack(PlayerInfo* player_info);
void enter_status_attack(PlayerInfo* player_info);
void exit_status_attack(PlayerInfo* player_info);
void status_hitstun(PlayerInfo* player_info);
void enter_status_hitstun(PlayerInfo* player_info);
void exit_status_hitstun(PlayerInfo* player_info);
void status_blockstun(PlayerInfo* player_info);
void enter_status_blockstun(PlayerInfo* player_info);
void exit_status_blockstun(PlayerInfo* player_info);

void set_status_functions(PlayerInfo* player_info) {
	(*player_info).wait = &status_wait;
	(*player_info).enter_wait = &enter_status_wait;
	(*player_info).exit_wait = &exit_status_wait;
	(*player_info).status_pointer[CHARA_STATUS_WAIT] = status_wait;
	(*player_info).enter_status_pointer[CHARA_STATUS_WAIT] = enter_status_wait;
	(*player_info).exit_status_pointer[CHARA_STATUS_WAIT] = exit_status_wait;
	(*player_info).walkf = &status_walkf;
	(*player_info).enter_walkf = &enter_status_walkf;
	(*player_info).exit_walkf = &exit_status_walkf;
	(*player_info).status_pointer[CHARA_STATUS_WALKF] = status_walkf;
	(*player_info).enter_status_pointer[CHARA_STATUS_WALKF] = enter_status_walkf;
	(*player_info).exit_status_pointer[CHARA_STATUS_WALKF] = exit_status_walkf;
	(*player_info).walkb = &status_walkb;
	(*player_info).enter_walkb = &enter_status_walkb;
	(*player_info).exit_walkb = &exit_status_walkb;
	(*player_info).status_pointer[CHARA_STATUS_WALKB] = status_walkb;
	(*player_info).enter_status_pointer[CHARA_STATUS_WALKB] = enter_status_walkb;
	(*player_info).exit_status_pointer[CHARA_STATUS_WALKB] = exit_status_walkb;
	(*player_info).dash = &status_dash;
	(*player_info).status_pointer[CHARA_STATUS_DASH] = status_dash;
	(*player_info).enter_status_pointer[CHARA_STATUS_DASH] = enter_status_dash;
	(*player_info).exit_status_pointer[CHARA_STATUS_DASH] = exit_status_dash;
	(*player_info).dashb = &status_dashb;
	(*player_info).status_pointer[CHARA_STATUS_DASHB] = status_dashb;
	(*player_info).enter_status_pointer[CHARA_STATUS_DASHB] = enter_status_dashb;
	(*player_info).exit_status_pointer[CHARA_STATUS_DASHB] = exit_status_dashb;
	(*player_info).crouch = &status_crouch;
	(*player_info).status_pointer[CHARA_STATUS_CROUCH] = status_crouch;
	(*player_info).enter_status_pointer[CHARA_STATUS_CROUCH] = enter_status_crouch;
	(*player_info).exit_status_pointer[CHARA_STATUS_CROUCH] = exit_status_crouch;
	(*player_info).crouchs = &status_crouchs;
	(*player_info).status_pointer[CHARA_STATUS_CROUCHS] = status_crouchs;
	(*player_info).enter_status_pointer[CHARA_STATUS_CROUCHS] = enter_status_crouchs;
	(*player_info).exit_status_pointer[CHARA_STATUS_CROUCHS] = exit_status_crouchs;
	(*player_info).jumpsquat = &status_jumpsquat;
	(*player_info).status_pointer[CHARA_STATUS_JUMPSQUAT] = status_jumpsquat;
	(*player_info).enter_status_pointer[CHARA_STATUS_JUMPSQUAT] = enter_status_jumpsquat;
	(*player_info).exit_status_pointer[CHARA_STATUS_JUMPSQUAT] = exit_status_jumpsquat;
	(*player_info).jump = &status_jump;
	(*player_info).status_pointer[CHARA_STATUS_JUMP] = status_jump;
	(*player_info).enter_status_pointer[CHARA_STATUS_JUMP] = enter_status_jump;
	(*player_info).exit_status_pointer[CHARA_STATUS_JUMP] = exit_status_jump;
	(*player_info).attack = &status_attack;
	(*player_info).status_pointer[CHARA_STATUS_ATTACK] = status_attack;
	(*player_info).enter_status_pointer[CHARA_STATUS_ATTACK] = enter_status_attack;
	(*player_info).exit_status_pointer[CHARA_STATUS_ATTACK] = exit_status_attack;
	(*player_info).hitstun = &status_hitstun;
	(*player_info).status_pointer[CHARA_STATUS_HITSTUN] = status_hitstun;
	(*player_info).enter_status_pointer[CHARA_STATUS_HITSTUN] = enter_status_hitstun;
	(*player_info).exit_status_pointer[CHARA_STATUS_HITSTUN] = exit_status_hitstun;
	(*player_info).blockstun = &status_blockstun;
	(*player_info).status_pointer[CHARA_STATUS_BLOCKSTUN] = status_blockstun;
	(*player_info).enter_status_pointer[CHARA_STATUS_BLOCKSTUN] = enter_status_blockstun;
	(*player_info).exit_status_pointer[CHARA_STATUS_BLOCKSTUN] = exit_status_blockstun;
}



void game_main(PlayerInfo* player_info, SDL_Renderer* renderer) {
	//Marks down the addresses of all of the player's status functions and puts them all on their info table. 

	/*
		TODO: Make it so this only runs once, probably while initializing the player_info class but it could be somewhere else.Eventually there's
		going to be a version of this function for each character that may or may not overwrite the global one, so ideally they aren't constantly
		overwriting each other every frame.
	*/
	set_status_functions(player_info);

	//Calls the looping status function for whatever the player's current status_kind is.
	(*player_info).status_pointer[(*player_info).status_kind](player_info);

	/*
		Get the player's inputs and increment the frame.
	*/

	player_info->stepAnimation();
	player_info->processInput();
	
	
}


void status_wait(PlayerInfo* player_info) {

}

void enter_status_wait(PlayerInfo* player_info) {
	(*player_info).startAnimation(&TEST_IDLE_ANIMATION);
}

void exit_status_wait(PlayerInfo* player_info) {

}

void status_walkf(PlayerInfo* player_info) {
	if ((*player_info).get_stick_dir() != 6) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	(*player_info).pos.x += 3.0;
}

void enter_status_walkf(PlayerInfo* player_info) {
	(*player_info).startAnimation(&TEST_WALK_ANIMATION);
}

void exit_status_walkf(PlayerInfo* player_info) {

}

void status_walkb(PlayerInfo* player_info) {
	if ((*player_info).get_stick_dir() != 4) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	(*player_info).pos.x -= 3.0;
}

void enter_status_walkb(PlayerInfo* player_info) {

}

void exit_status_walkb(PlayerInfo* player_info) {

}

void status_dash(PlayerInfo* player_info) {

}

void enter_status_dash(PlayerInfo* player_info) {

}

void exit_status_dash(PlayerInfo* player_info) {

}

void status_dashb(PlayerInfo* player_info) {

}

void enter_status_dashb(PlayerInfo* player_info) {

}

void exit_status_dashb(PlayerInfo* player_info) {

}

void status_crouch(PlayerInfo* player_info) {

}

void enter_status_crouch(PlayerInfo* player_info) {

}

void exit_status_crouch(PlayerInfo* player_info) {

}

void status_crouchs(PlayerInfo* player_info) {

}

void enter_status_crouchs(PlayerInfo* player_info) {

}

void exit_status_crouchs(PlayerInfo* player_info) {

}

void status_jumpsquat(PlayerInfo* player_info) {

}

void enter_status_jumpsquat(PlayerInfo* player_info) {

}

void exit_status_jumpsquat(PlayerInfo* player_info) {

}

void status_jump(PlayerInfo* player_info) {

}

void enter_status_jump(PlayerInfo* player_info) {

}

void exit_status_jump(PlayerInfo* player_info) {

}

void status_attack(PlayerInfo* player_info) {

}

void enter_status_attack(PlayerInfo* player_info) {

}

void exit_status_attack(PlayerInfo* player_info) {

}

void status_hitstun(PlayerInfo* player_info) {

}

void enter_status_hitstun(PlayerInfo* player_info) {

}

void exit_status_hitstun(PlayerInfo* player_info) {

}

void status_blockstun(PlayerInfo* player_info) {

}

void enter_status_blockstun(PlayerInfo* player_info) {

}

void exit_status_blockstun(PlayerInfo* player_info) {

}