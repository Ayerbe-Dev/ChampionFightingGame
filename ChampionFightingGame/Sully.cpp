#include "Sully.h"

Sully::Sully() {

}

Sully::Sully(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_SULLY;
	chara_name = "sully";
	resource_dir = "resource/chara/sully";
	fighter_int.resize(CHARA_SULLY_INT_MAX, 0);
	fighter_float.resize(CHARA_SULLY_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_SULLY_FLAG_MAX, false);
	loadSullyStatusFunctions();
}

void Sully::chara_id() {

}