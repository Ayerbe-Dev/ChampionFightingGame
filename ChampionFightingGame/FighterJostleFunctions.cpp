#include "Fighter.h"

void Fighter::update_jostle_rect() {
	glm::vec2 anchor = base_jostle_anchor;
	glm::vec2 offset = base_jostle_offset;
	offset.x += fighter_float[FIGHTER_FLOAT_JOSTLE_OFFSET_X];
	anchor.x *= facing_dir;
	offset.x *= facing_dir;
	anchor.x += pos.x;
	anchor.y += pos.y;
	offset.x += pos.x;
	offset.y += pos.y;
	jostle_box.update_corners(anchor, offset);
}

void Fighter::set_jostle_offset(float offset) {
	fighter_float[FIGHTER_FLOAT_JOSTLE_OFFSET_X] = offset;
}