#include "Fighter.h"

void Fighter::create_jostle_rect(vec2 anchor, vec2 offset) {
	anchor.x *= facing_dir;
	offset.x *= facing_dir;
	anchor.x += pos.x;
	anchor.y += pos.y;
	offset.x += pos.x;
	offset.y += pos.y;
	jostle_box.update_corners(anchor, offset);
}