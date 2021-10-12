#include <iostream>
#include <functional>
#include <vector>
#include<string>
#include "PlayerInfo.h"
#include <SDL.h>
#include "Animation.h"
#include "Game.h"

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.


void set_status_functions(PlayerInfo* player_info) {
	(*player_info).status_pointer[CHARA_STATUS_WAIT] = status_wait;
	(*player_info).enter_status_pointer[CHARA_STATUS_WAIT] = enter_status_wait;
	(*player_info).exit_status_pointer[CHARA_STATUS_WAIT] = exit_status_wait;
	
	(*player_info).status_pointer[CHARA_STATUS_WALKF] = status_walkf;
	(*player_info).enter_status_pointer[CHARA_STATUS_WALKF] = enter_status_walkf;
	(*player_info).exit_status_pointer[CHARA_STATUS_WALKF] = exit_status_walkf;

	(*player_info).status_pointer[CHARA_STATUS_WALKB] = status_walkb;
	(*player_info).enter_status_pointer[CHARA_STATUS_WALKB] = enter_status_walkb;
	(*player_info).exit_status_pointer[CHARA_STATUS_WALKB] = exit_status_walkb;
	
	(*player_info).status_pointer[CHARA_STATUS_DASH] = status_dash;
	(*player_info).enter_status_pointer[CHARA_STATUS_DASH] = enter_status_dash;
	(*player_info).exit_status_pointer[CHARA_STATUS_DASH] = exit_status_dash;
	
	(*player_info).status_pointer[CHARA_STATUS_DASHB] = status_dashb;
	(*player_info).enter_status_pointer[CHARA_STATUS_DASHB] = enter_status_dashb;
	(*player_info).exit_status_pointer[CHARA_STATUS_DASHB] = exit_status_dashb;
	
	(*player_info).status_pointer[CHARA_STATUS_CROUCHD] = status_crouchd;
	(*player_info).enter_status_pointer[CHARA_STATUS_CROUCHD] = enter_status_crouchd;
	(*player_info).exit_status_pointer[CHARA_STATUS_CROUCHD] = exit_status_crouchd;
	
	(*player_info).status_pointer[CHARA_STATUS_CROUCH] = status_crouch;
	(*player_info).enter_status_pointer[CHARA_STATUS_CROUCH] = enter_status_crouch;
	(*player_info).exit_status_pointer[CHARA_STATUS_CROUCH] = exit_status_crouch;
	
	(*player_info).status_pointer[CHARA_STATUS_CROUCHU] = status_crouchu;
	(*player_info).enter_status_pointer[CHARA_STATUS_CROUCHU] = enter_status_crouchu;
	(*player_info).exit_status_pointer[CHARA_STATUS_CROUCHU] = exit_status_crouchu;
	
	(*player_info).status_pointer[CHARA_STATUS_JUMPSQUAT] = status_jumpsquat;
	(*player_info).enter_status_pointer[CHARA_STATUS_JUMPSQUAT] = enter_status_jumpsquat;
	(*player_info).exit_status_pointer[CHARA_STATUS_JUMPSQUAT] = exit_status_jumpsquat;
	
	(*player_info).status_pointer[CHARA_STATUS_JUMP] = status_jump;
	(*player_info).enter_status_pointer[CHARA_STATUS_JUMP] = enter_status_jump;
	(*player_info).exit_status_pointer[CHARA_STATUS_JUMP] = exit_status_jump;
	
	(*player_info).status_pointer[CHARA_STATUS_ATTACK] = status_attack;
	(*player_info).enter_status_pointer[CHARA_STATUS_ATTACK] = enter_status_attack;
	(*player_info).exit_status_pointer[CHARA_STATUS_ATTACK] = exit_status_attack;
	
	(*player_info).status_pointer[CHARA_STATUS_HITSTUN] = status_hitstun;
	(*player_info).enter_status_pointer[CHARA_STATUS_HITSTUN] = enter_status_hitstun;
	(*player_info).exit_status_pointer[CHARA_STATUS_HITSTUN] = exit_status_hitstun;
	
	(*player_info).status_pointer[CHARA_STATUS_BLOCKSTUN] = status_blockstun;
	(*player_info).enter_status_pointer[CHARA_STATUS_BLOCKSTUN] = enter_status_blockstun;
	(*player_info).exit_status_pointer[CHARA_STATUS_BLOCKSTUN] = exit_status_blockstun;
}



void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer) {
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

	player_info->processInput();
	if (player_info->canStep()) {
		player_info->stepAnimation();
	}
	(*player_info).prev_stick_dir = (*player_info).get_stick_dir();
	if ((*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] --;
	}
	if ((*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] --;
	}
}


void status_wait(PlayerInfo* player_info) {
	if ((*player_info).common_ground_status_act()) {
		return;
	}
}

void enter_status_wait(PlayerInfo* player_info) {
	(*player_info).change_anim("wait", 30);
	(*player_info).situation_kind = CHARA_SITUATION_GROUND;
}

void exit_status_wait(PlayerInfo* player_info) {

}

void status_walkf(PlayerInfo* player_info) {
	if ((*player_info).common_ground_status_act()) {
		return;
	}
	if ((*player_info).get_stick_dir() == 5) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	(*player_info).pos.x += (*player_info).stats.walk_f_speed * (*player_info).facing_dir;
}

void enter_status_walkf(PlayerInfo* player_info) {
	(*player_info).change_anim("walk_f", 30);
}

void exit_status_walkf(PlayerInfo* player_info) {

}

void status_walkb(PlayerInfo* player_info) {
	if ((*player_info).common_ground_status_act()) {
		return;
	}
	if ((*player_info).get_stick_dir() == 5) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	(*player_info).pos.x -= (*player_info).stats.walk_b_speed * (*player_info).facing_dir;
}

void enter_status_walkb(PlayerInfo* player_info) {
	(*player_info).change_anim("walk_b", 30);
}

void exit_status_walkb(PlayerInfo* player_info) {

}

void status_dash(PlayerInfo* player_info) {
	if ((*player_info).is_anim_end) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	if ((*player_info).is_actionable() && (*player_info).common_ground_status_act()) {
		return;
	}
	int min_frame = (*player_info).stats.dash_f_accel_frame;
	int max_frame = min_frame + (*player_info).stats.dash_f_maintain_speed_frame;

	if ((*player_info).frame >= min_frame && (*player_info).frame < max_frame) {
		(*player_info).pos.x += (*player_info).stats.dash_f_speed * (*player_info).facing_dir;
	}
	else {
		(*player_info).pos.x += (*player_info).stats.walk_f_speed * (*player_info).facing_dir;
	}

	if ((*player_info).frame >= (*player_info).stats.dash_f_cancel_frame) {
		if (!(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL]) {
			if ((*player_info).get_flick_dir() == 4) {
				if ((*player_info).stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE) {
					(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL] = true;
				}
				(*player_info).change_status(CHARA_STATUS_DASHB, false);
				return;
			}
		}
		else if ((*player_info).stats.dash_cancel_kind == DASH_CANCEL_KIND_FOLLOWUP) {

		}
	}
}

void enter_status_dash(PlayerInfo* player_info) {
	(*player_info).change_anim("dash_f", 30);
}

void exit_status_dash(PlayerInfo* player_info) {
	(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL] = false;
}

void status_dashb(PlayerInfo* player_info) {
	if ((*player_info).is_anim_end) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	if ((*player_info).is_actionable() && (*player_info).common_ground_status_act()) {
		return;
	}
	int min_frame = (*player_info).stats.dash_b_accel_frame;
	int max_frame = min_frame + (*player_info).stats.dash_b_maintain_speed_frame;

	if ((*player_info).frame >= min_frame && (*player_info).frame < max_frame) {
		(*player_info).pos.x -= (*player_info).stats.dash_b_speed * (*player_info).facing_dir;
	}
	else {
		(*player_info).pos.x -= (*player_info).stats.walk_b_speed * (*player_info).facing_dir;
	}

	if (!(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL]) {
		if ((*player_info).frame >= (*player_info).stats.dash_b_cancel_frame) {
			if ((*player_info).get_flick_dir() == 6) {
				if ((*player_info).stats.dash_cancel_kind != DASH_CANCEL_KIND_INDEFINITE) {
					(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL] = true;
				}
				(*player_info).change_status(CHARA_STATUS_DASH, false);
				return;
			}
		}
	}
}

void enter_status_dashb(PlayerInfo* player_info) {
	(*player_info).change_anim("dash_b", 30);
}

void exit_status_dashb(PlayerInfo* player_info) {
	(*player_info).chara_bool[CHARA_BOOL_DASH_CANCEL] = false;
}

void status_crouchd(PlayerInfo* player_info) {
	if ((*player_info).is_anim_end) {
		(*player_info).change_status(CHARA_STATUS_CROUCH);
		return;
	}
}

void enter_status_crouchd(PlayerInfo* player_info) {
	(*player_info).change_anim("crouch_d", 30);
}

void exit_status_crouchd(PlayerInfo* player_info) {

}

void status_crouch(PlayerInfo* player_info) {
	if ((*player_info).get_stick_dir() == 6) {
		(*player_info).change_status(CHARA_STATUS_WALKF);
		return;
	}
	if ((*player_info).get_stick_dir() == 4) {
		(*player_info).change_status(CHARA_STATUS_WALKB);
		return;
	}
	if ((*player_info).get_stick_dir() > 6) {
		(*player_info).change_status(CHARA_STATUS_JUMPSQUAT);
		return;
	}
	if ((*player_info).get_stick_dir() == 5) {
		(*player_info).change_status(CHARA_STATUS_CROUCHU);
		return;
	}
}

void enter_status_crouch(PlayerInfo* player_info) {
	(*player_info).change_anim("crouch", 30);
}

void exit_status_crouch(PlayerInfo* player_info) {

}

void status_crouchu(PlayerInfo* player_info) {
	if ((*player_info).common_ground_status_act()) {
		return;
	}
	if ((*player_info).is_anim_end) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
	}
}

void enter_status_crouchu(PlayerInfo* player_info) {
	(*player_info).change_anim("crouch_u", 30);
}

void exit_status_crouchu(PlayerInfo* player_info) {

}

void status_jumpsquat(PlayerInfo* player_info) {
	if ((*player_info).is_anim_end) {
		(*player_info).change_status(CHARA_STATUS_JUMP);
		return;
	}
}

void enter_status_jumpsquat(PlayerInfo* player_info) {
	(*player_info).change_anim("jump_squat", 120);
}

void exit_status_jumpsquat(PlayerInfo* player_info) {
	(*player_info).chara_bool[CHARA_BOOL_SHORT_HOP] = (*player_info).get_stick_dir() < 7;
	(*player_info).situation_kind = CHARA_SITUATION_AIR;
	(*player_info).chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y] = (*player_info).pos.y; //This will eventually be replaced by a proper ground collision check
}

void status_jump(PlayerInfo* player_info) {
	if ((*player_info).pos.y > (*player_info).chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y]) {
		(*player_info).change_status(CHARA_STATUS_WAIT);
		return;
	}
	if ((*player_info).chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > (*player_info).stats.max_fall_speed * -1.0) {
		(*player_info).chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= (*player_info).stats.gravity;
	}
	if ((*player_info).chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
		(*player_info).pos.x += (*player_info).stats.jump_x_speed * (*player_info).facing_dir;
	}
	if ((*player_info).chara_int[CHARA_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
		(*player_info).pos.x -= (*player_info).stats.jump_x_speed * (*player_info).facing_dir;
	}
	(*player_info).pos.y -= (*player_info).chara_float[CHARA_FLOAT_CURRENT_Y_SPEED];
}

void enter_status_jump(PlayerInfo* player_info) {
	if ((*player_info).get_stick_dir() == 7
	|| (*player_info).get_stick_dir() == 4 
	|| (*player_info).get_stick_dir() == 1) {
		(*player_info).change_anim("jump_b");
		(*player_info).chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
	}
	else if ((*player_info).get_stick_dir() == 9
	|| (*player_info).get_stick_dir() == 6
	|| (*player_info).get_stick_dir() == 3) {
		(*player_info).change_anim("jump_f");
		(*player_info).chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
	}
	else {
		(*player_info).change_anim("jump");
		(*player_info).chara_int[CHARA_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if ((*player_info).chara_bool[CHARA_BOOL_SHORT_HOP]) {
		(*player_info).chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = (*player_info).stats.jump_y_init_speed_s;
	}
	else {
		(*player_info).chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = (*player_info).stats.jump_y_init_speed;
	}
}

void exit_status_jump(PlayerInfo* player_info) {
	(*player_info).pos.y = (*player_info).chara_float[CHARA_FLOAT_INIT_POS_JUMP_Y];
	(*player_info).situation_kind = CHARA_SITUATION_GROUND;
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