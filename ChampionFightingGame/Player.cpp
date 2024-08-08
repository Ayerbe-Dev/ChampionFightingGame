#include "Player.h"
#include "CharaKind.h"
#include "StageKind.h"
#include "ParamAccessor.h"
#include "utils.h"
#include "PlayerInfo.h"
#include "SaveManager.h"
#include "GameManager.h"
#include "Battle.h"

Player::Player() {
	id = -1;
	chara_kind = CHARA_KIND_ROWAN;
	alt_costume = 0;
	alt_color = 0;
	control_type = CONTROL_TYPE_ADVANCE;
	stage_info = StageInfo(STAGE_KIND_TRAINING, "training_room");
	int timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
	controller.set_stick_hold_timer(timer, timer);
	player_info = SaveManager::get_instance()->get_player_info(-1);
	input_mode = INPUT_MODE_POLL;
	rollback_frames = 0;
	player_kind = PLAYER_KIND_PLAYER;
}

Player::Player(int id) {
	this->id = id;
	int timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
	controller.set_stick_hold_timer(timer, timer);
	chara_kind = CHARA_KIND_MAX;
	alt_costume = 0;
	alt_color = 0;
	control_type = CONTROL_TYPE_ADVANCE;
	stage_info = StageInfo(STAGE_KIND_TRAINING, "training_room");
	player_info = SaveManager::get_instance()->get_player_info(-1);
	input_mode = INPUT_MODE_POLL;
	rollback_frames = 0;
	player_kind = PLAYER_KIND_PLAYER;
}

void Player::load_player_info(int index) {
	player_info = SaveManager::get_instance()->get_player_info(index);
	this->name = player_info->name;
	this->control_type = player_info->control_type;
	chara_kind = player_info->preferred_chara;
	alt_costume = player_info->preferred_costume[chara_kind];
	alt_color = player_info->preferred_color[chara_kind];
	controller.reset_button_mappings();
	for (size_t i = 0, max = player_info->custom_mappings.size(); i < max; i++) {
		controller.set_button_k_mapping(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].k_mapping);
		controller.set_button_c_mapping(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].c_mapping);
		controller.set_button_c_axis(player_info->custom_mappings[i].button_kind, player_info->custom_mappings[i].c_axis);
	}
}

void Player::update_player_info() {
	player_info->preferred_chara = chara_kind;
	player_info->preferred_costume[chara_kind] = alt_costume;
	player_info->preferred_color[chara_kind] = alt_color;
	//TODO: Add playerinfo mappings here
	SaveManager::get_instance()->sort_player_info(player_info);
}

void Player::poll_controller_menu() {
	controller.check_controllers();
	controller.poll_menu();
}

void Player::poll_controller_fighter() {
	switch (input_mode) {
		default:
		case (INPUT_MODE_POLL): { //Normal
			if (player_kind == PLAYER_KIND_CPU) {
				controller.poll_from_input_code(manual_seq.get_curr_input_code_cpu());
				replay_seq.add_inputs(controller.get_input_code());
			}
			else {
				controller.poll_fighter();
				replay_seq.add_inputs(controller.get_input_code());
			}
		} break;
		case (INPUT_MODE_RECORD_SEQ): { //CPU Input Recording
			controller.poll_fighter();
			manual_seq.add_inputs(controller.get_input_code());
			replay_seq.add_inputs(controller.get_input_code());
		} break;
		case (INPUT_MODE_PLAY_SEQ): { //CPU Input Replay
			controller.poll_from_input_code(manual_seq.get_curr_input_code());
			replay_seq.add_inputs(controller.get_input_code());
		} break;
		case (INPUT_MODE_REPLAY):
		case (INPUT_MODE_ATLAS_REWIND): { //Standard Replay + Atlas
			controller.poll_from_input_code(replay_seq.get_curr_input_code());
		} break;
		case (INPUT_MODE_ROLLBACK): { //Rollback
			controller.poll_from_input_code(replay_seq.get_prev_input_code(rollback_frames--));
		} break;
	}
}