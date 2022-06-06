#include "Alejandro.h"

Alejandro::Alejandro() {

}

Alejandro::Alejandro(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_ALEJANDRO;
	chara_name = "alejandro";
	resource_dir = "resource/chara/alejandro";
	fighter_int.resize(CHARA_ALEJANDRO_INT_MAX, 0);
	fighter_float.resize(CHARA_ALEJANDRO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ALEJANDRO_FLAG_MAX, false);
	loadAlejandroStatusFunctions();
}

void Alejandro::chara_id() {

}