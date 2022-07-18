#include "Player.h"
#include "CharaKind.h"
#include "StageKind.h"
#include "ParamAccessor.h"
#include "utils.h"
#include "PlayerInfo.h"

Player::Player() {
	id = -1;
	int timer = get_param_int("stick_hold_timer", PARAM_MENU);
	controller.stick_hold_v_timer = timer;
	controller.stick_hold_h_timer = timer;
}

Player::Player(int id) {
	this->id = id;
	int timer = get_param_int("stick_hold_timer", PARAM_MENU);
	controller.stick_hold_v_timer = timer;
	controller.stick_hold_h_timer = timer;
	chara_kind = CHARA_KIND_MAX;
	stage_info = StageInfo(STAGE_KIND_TRAINING, "training_room"); //Todo: Overwrite this value while on the stage select
	set_default_button_mappings(id);
}

void Player::set_default_button_mappings(int id) {
	if (id == 0) {
		controller.add_button_mapping(BUTTON_UP, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP);
		controller.add_button_mapping(BUTTON_LEFT, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		controller.add_button_mapping(BUTTON_DOWN, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		controller.add_button_mapping(BUTTON_RIGHT, SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		controller.add_button_mapping(BUTTON_LP, SDL_SCANCODE_Y, SDL_CONTROLLER_BUTTON_A);
		controller.add_button_mapping(BUTTON_MP, SDL_SCANCODE_U, SDL_CONTROLLER_BUTTON_X);
		controller.add_button_mapping(BUTTON_HP, SDL_SCANCODE_I, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
		controller.add_button_mapping(BUTTON_LK, SDL_SCANCODE_H, SDL_CONTROLLER_BUTTON_B);
		controller.add_button_mapping(BUTTON_MK, SDL_SCANCODE_J, SDL_CONTROLLER_BUTTON_Y);
		controller.add_button_mapping(BUTTON_HK, SDL_SCANCODE_K, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		controller.add_button_mapping(BUTTON_MACRO_P, SDL_SCANCODE_O, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		controller.add_button_mapping(BUTTON_MACRO_K, SDL_SCANCODE_L, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		controller.add_button_mapping(BUTTON_MACRO_L);
		controller.add_button_mapping(BUTTON_MACRO_M);
		controller.add_button_mapping(BUTTON_MACRO_H);
		controller.add_button_mapping(BUTTON_START, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_START);
		controller.add_button_mapping(BUTTON_MENU_UP, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP);
		controller.add_button_mapping(BUTTON_MENU_LEFT, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		controller.add_button_mapping(BUTTON_MENU_DOWN, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		controller.add_button_mapping(BUTTON_MENU_RIGHT, SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		controller.add_button_mapping(BUTTON_MENU_SELECT, SDL_SCANCODE_Y, SDL_CONTROLLER_BUTTON_A);
		controller.add_button_mapping(BUTTON_MENU_BACK, SDL_SCANCODE_U, SDL_CONTROLLER_BUTTON_B);
		controller.add_button_mapping(BUTTON_MENU_START, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_START);
	}
	else {
		controller.add_button_mapping(BUTTON_UP, SDL_SCANCODE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
		controller.add_button_mapping(BUTTON_LEFT, SDL_SCANCODE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		controller.add_button_mapping(BUTTON_DOWN, SDL_SCANCODE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		controller.add_button_mapping(BUTTON_RIGHT, SDL_SCANCODE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		controller.add_button_mapping(BUTTON_LP, SDL_SCANCODE_Z, SDL_CONTROLLER_BUTTON_A);
		controller.add_button_mapping(BUTTON_MP, SDL_SCANCODE_X, SDL_CONTROLLER_BUTTON_X);
		controller.add_button_mapping(BUTTON_HP, SDL_SCANCODE_C, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
		controller.add_button_mapping(BUTTON_LK, SDL_SCANCODE_B, SDL_CONTROLLER_BUTTON_B);
		controller.add_button_mapping(BUTTON_MK, SDL_SCANCODE_N, SDL_CONTROLLER_BUTTON_Y);
		controller.add_button_mapping(BUTTON_HK, SDL_SCANCODE_M, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		controller.add_button_mapping(BUTTON_MACRO_P, SDL_SCANCODE_V, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		controller.add_button_mapping(BUTTON_MACRO_K, SDL_SCANCODE_COMMA, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		controller.add_button_mapping(BUTTON_MACRO_L);
		controller.add_button_mapping(BUTTON_MACRO_M);
		controller.add_button_mapping(BUTTON_MACRO_H);
		controller.add_button_mapping(BUTTON_START, SDL_SCANCODE_RETURN, SDL_CONTROLLER_BUTTON_START);
		controller.add_button_mapping(BUTTON_MENU_UP, SDL_SCANCODE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
		controller.add_button_mapping(BUTTON_MENU_LEFT, SDL_SCANCODE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		controller.add_button_mapping(BUTTON_MENU_DOWN, SDL_SCANCODE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		controller.add_button_mapping(BUTTON_MENU_RIGHT, SDL_SCANCODE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		controller.add_button_mapping(BUTTON_MENU_SELECT, SDL_SCANCODE_Z, SDL_CONTROLLER_BUTTON_A);
		controller.add_button_mapping(BUTTON_MENU_BACK, SDL_SCANCODE_X, SDL_CONTROLLER_BUTTON_B);
		controller.add_button_mapping(BUTTON_MENU_START, SDL_SCANCODE_RETURN, SDL_CONTROLLER_BUTTON_START);
	}
}

void Player::load_player(PlayerInfo* player_info) {

}