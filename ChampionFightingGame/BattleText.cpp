#include "Battle.h"

BattleText::BattleText() {
	this->duration = -1;
	this->font = nullptr;
}

void BattleText::init(Font* font, std::string text, int duration, Fighter* fighter, glm::vec2 pos) {
	this->font = font;
	this->duration = duration;

	GameTexture::init(*font, text, glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	set_pos(glm::vec3(pos, 0.0));
	if (fighter->id) {
		set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
	}
	else {
		set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	}
	scale_all_percent(0.8, false);
}

void BattleText::update(std::string text, int duration) {
	this->duration = duration;
	update_text(*font, text, glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	scale_all_percent(0.8, false);
}