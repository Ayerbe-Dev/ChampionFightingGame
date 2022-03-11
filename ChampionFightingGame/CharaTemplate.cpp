#include "CharaTemplate.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/chara_template";
	chara_name = "chara_template";
	fighter_int.resize(CHARA_CHARA_TEMPLATE_INT_MAX, 0);
	fighter_float.resize(CHARA_CHARA_TEMPLATE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_CHARA_TEMPLATE_FLAG_MAX, false);
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