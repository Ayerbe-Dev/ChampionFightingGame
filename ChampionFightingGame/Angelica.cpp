#include "Angelica.h"

Angelica::Angelica() {

}

Angelica::Angelica(int id, Player* player) {
	this->id = id;
	this->player = player;
	chara_kind = CHARA_KIND_ANGELICA;
	chara_name = "angelica";
	resource_dir = "resource/chara/angelica";
	fighter_int.resize(CHARA_ANGELICA_INT_MAX, 0);
	fighter_float.resize(CHARA_ANGELICA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ANGELICA_FLAG_MAX, false);
}