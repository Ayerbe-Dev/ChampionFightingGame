#include "Aziel.h"

Aziel::Aziel() {

}

Aziel::Aziel(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_AZIEL;
	chara_name = "aziel";
	resource_dir = "resource/chara/aziel";
	fighter_int.resize(CHARA_AZIEL_INT_MAX, 0);
	fighter_float.resize(CHARA_AZIEL_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_AZIEL_FLAG_MAX, false);
	loadAzielStatusFunctions();
}

void Aziel::chara_id() {

}