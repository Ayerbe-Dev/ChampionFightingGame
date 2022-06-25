#include "Battle.h"

ComboCounter::ComboCounter() {
	font = nullptr;
	fighter = nullptr;
	alpha = 0;
	prev_value = 0;
	left_oriented = false;
}

void ComboCounter::init(Font* count_font, Font* hits_font, Fighter* fighter) {
	this->font = count_font;
	this->fighter = fighter;
	count_text.init(*count_font, "", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0);
	count_text.set_pos(glm::vec3(120.0, 350.0, 0.0));
	hits_text.init(*hits_font, "hits", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0);
	hits_text.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	count_text.set_alpha(0);
	hits_text.set_alpha(0);
	if (fighter->id == 0) {
		left_oriented = true;
		count_text.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		hits_text.set_pos(glm::vec3(275.0, 540.0, 0.0)); //Orientation for this text is based on
		//the number rather than the screen, so we can't just set a GameTexture orientation
	}
	else {
		count_text.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		hits_text.set_pos(glm::vec3(3565.0, 540.0, 0.0));
	}
}

void ComboCounter::render() {
	if (fighter->fighter_int[FIGHTER_INT_COMBO_COUNT] < 2) {
		if (alpha != 0) {
			alpha -= 51;
			count_text.set_alpha(alpha);
			hits_text.set_alpha(alpha);
		}
		else {
			if (prev_value > 9 && left_oriented) {
				hits_text.add_pos(glm::vec3(-65.0, 0.0, 0.0));
			}
			prev_value = 0;
		}
	}
	else {
		if (fighter->fighter_int[FIGHTER_INT_COMBO_COUNT] != prev_value) {
			if (alpha == 0) {
				alpha = 255;
				count_text.set_alpha(alpha);
				hits_text.set_alpha(alpha);
			}
			if (prev_value == 9 && left_oriented) {
				hits_text.add_pos(glm::vec3(65.0, 0.0, 0.0));
			}
			count_text.update_text(*font, std::to_string(fighter->fighter_int[FIGHTER_INT_COMBO_COUNT]), glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0);
			prev_value = fighter->fighter_int[FIGHTER_INT_COMBO_COUNT];
			count_text.scale_all_percent(0.8, false);
			hits_text.scale_all_percent(0.8, false);
			count_text.render();
			hits_text.render();
			count_text.scale_all_percent(1.0, false);
			hits_text.scale_all_percent(1.0, false);
			return;
		}
	}
	count_text.render();
	hits_text.render();
}