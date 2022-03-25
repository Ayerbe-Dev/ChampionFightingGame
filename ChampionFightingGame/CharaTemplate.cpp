#include "CharaTemplate.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
	chara_name = "chara_template";
	resource_dir = "resource/chara/chara_template";
	fighter_int.resize(CHARA_CHARA_TEMPLATE_INT_MAX, 0);
	fighter_float.resize(CHARA_CHARA_TEMPLATE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_CHARA_TEMPLATE_FLAG_MAX, false);
	load_params();
	load_move_scripts();
	loadCharaTemplateStatusFunctions();
	set_current_move_script("default");
}

void CharaTemplate::chara_id() {

}