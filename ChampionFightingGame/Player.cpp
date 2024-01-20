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
	player_info = nullptr;
}

void Player::load_player(int index) {
	player_info = SaveManager::get_instance()->get_player_info(index);
	this->name = player_info->name;
	this->control_type = player_info->control_type;
	controller.reset_button_mappings();
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