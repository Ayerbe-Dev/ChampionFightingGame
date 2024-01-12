#include "Player.h"
#include "CharaKind.h"
#include "StageKind.h"
#include "ParamAccessor.h"
#include "utils.h"
#include "PlayerInfo.h"
#include "SaveManager.h"

Player::Player() {
	id = -1;
	chara_kind = CHARA_KIND_ROWAN;
	alt_costume = 0;
	alt_color = 0;
	control_type = CONTROL_TYPE_ADVANCE;

	int timer = get_param_int(PARAM_MENU, "stick_hold_timer");
	controller.set_stick_hold_timer(timer, timer);
	player_info = nullptr;
}

Player::Player(int id) {
	this->id = id;
	int timer = get_param_int(PARAM_MENU, "stick_hold_timer");
	controller.set_id(id);
	controller.set_stick_hold_timer(timer, timer);
	chara_kind = CHARA_KIND_MAX;
	stage_info = StageInfo(STAGE_KIND_TRAINING, "training_room"); //Todo: Overwrite this value while on the stage select
	set_default_button_mappings(id);
	player_info = nullptr;
}

void Player::set_default_button_mappings(int id) {
	if (id == 0) {
		controller.add_button_mapping(BUTTON_UP, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP);
		controller.add_button_mapping(BUTTON_LEFT, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		controller.add_button_mapping(BUTTON_DOWN, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		controller.add_button_mapping(BUTTON_RIGHT, SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		controller.add_button_mapping(BUTTON_LP, SDL_SCANCODE_Y, SDL_CONTROLLER_BUTTON_A);
		controller.add_button_mapping(BUTTON_MP, SDL_SCANCODE_U, SDL_CONTROLLER_BUTTON_Y);
		controller.add_button_mapping(BUTTON_HP, SDL_SCANCODE_I, SDL_CONTROLLER_BUTTON_B);
		controller.add_button_mapping(BUTTON_LK, SDL_SCANCODE_H, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		controller.add_button_mapping(BUTTON_MK, SDL_SCANCODE_J, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		controller.add_button_mapping(BUTTON_HK, SDL_SCANCODE_K, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		controller.add_button_mapping(BUTTON_3P, SDL_SCANCODE_O, SDL_CONTROLLER_BUTTON_X);
		controller.add_button_mapping(BUTTON_3K, SDL_SCANCODE_L, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
		controller.add_button_mapping(BUTTON_2L);
		controller.add_button_mapping(BUTTON_2M);
		controller.add_button_mapping(BUTTON_2H);
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
		controller.add_button_mapping(BUTTON_3P, SDL_SCANCODE_V, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		controller.add_button_mapping(BUTTON_3K, SDL_SCANCODE_COMMA, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		controller.add_button_mapping(BUTTON_2L);
		controller.add_button_mapping(BUTTON_2M);
		controller.add_button_mapping(BUTTON_2H);
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

void Player::load_player(int index) {
	set_default_button_mappings(id);
	player_info = SaveManager::get_instance()->get_player_info(index);
	this->name = player_info->name;
	this->control_type = player_info->control_type;
	for (size_t i = 0, max = player_info->custom_mappings.size(); i < max; i++) {
		controller.set_button_mapping(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].k_mapping);
		controller.set_button_mapping(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].c_mapping);
		controller.set_button_mapping(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].c_axis);
	}
	this->chara_kind = player_info->preferred_chara;
	set_alt_for_chara();
}

void Player::set_alt_for_chara() {
	alt_costume = player_info->preferred_costume[chara_kind];
	alt_color = player_info->preferred_color[chara_kind];
}