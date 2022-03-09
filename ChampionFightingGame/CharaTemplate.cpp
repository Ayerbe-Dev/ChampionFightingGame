#include "CharaTemplate.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/chara_template";
	chara_name = "chara_template";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadCharaTemplateStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
}

void CharaTemplate::chara_id() {

}

void CharaTemplate::chara_status() {
	(this->*chara_template_status[status_kind - FIGHTER_STATUS_MAX])();
}

void CharaTemplate::chara_enter_status() {
	(this->*chara_template_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void CharaTemplate::chara_exit_status() {
	(this->*chara_template_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}