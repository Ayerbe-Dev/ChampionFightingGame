#include "Fighter.h"

void Fighter::update_jostle_rect() {
	glm::vec2 anchor = glm::vec2(fighter_float[FIGHTER_FLOAT_JOSTLE_X0], fighter_float[FIGHTER_FLOAT_JOSTLE_Y0]);
	glm::vec2 offset = glm::vec2(fighter_float[FIGHTER_FLOAT_JOSTLE_X1], fighter_float[FIGHTER_FLOAT_JOSTLE_Y1]);
	anchor.x *= facing_dir;
	offset.x *= facing_dir;
	anchor.x += pos.x;
	anchor.y += pos.y;
	offset.x += pos.x;
	offset.y += pos.y;
	jostle_box.update_corners(anchor, offset);
}

void Fighter::set_jostle_dimensions(float x0, float y0, float x1, float y1) {
	fighter_float[FIGHTER_FLOAT_JOSTLE_X0] = x0;
	fighter_float[FIGHTER_FLOAT_JOSTLE_Y0] = y0;
	fighter_float[FIGHTER_FLOAT_JOSTLE_X1] = x1;
	fighter_float[FIGHTER_FLOAT_JOSTLE_Y1] = y1;
}

void Fighter::reset_jostle_dimensions() {
	fighter_float[FIGHTER_FLOAT_JOSTLE_X0] = base_jostle_anchor.x;
	fighter_float[FIGHTER_FLOAT_JOSTLE_Y0] = base_jostle_anchor.y;
	fighter_float[FIGHTER_FLOAT_JOSTLE_X1] = base_jostle_offset.x;
	fighter_float[FIGHTER_FLOAT_JOSTLE_Y1] = base_jostle_offset.y;
}