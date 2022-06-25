#include "Battle.h"

PlayerIndicator::PlayerIndicator() {
	fighter = nullptr;
}

PlayerIndicator::PlayerIndicator(Fighter* fighter, std::string nametag) {
	this->fighter = fighter;
	this->nametag = nametag;
	std::string resource_dir = "resource/ui/battle/tag/";
	if (fighter->id == 0) {
		resource_dir += "p1_tag";
	}
	else {
		resource_dir += "p2_tag";
	}
	if (nametag == "") {
		resource_dir += "_no_nametag";
	}
	resource_dir += ".png";
	this->texture.init(resource_dir);
}