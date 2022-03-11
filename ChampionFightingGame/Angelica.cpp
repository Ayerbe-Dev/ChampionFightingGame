#include "Angelica.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Angelica::Angelica() {

}

Angelica::Angelica(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/angelica";
	chara_name = "angelica";
	fighter_int.resize(CHARA_ANGELICA_INT_MAX, 0);
	fighter_float.resize(CHARA_ANGELICA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ANGELICA_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAngelicaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ANGELICA;
}

void Angelica::chara_id() {

}