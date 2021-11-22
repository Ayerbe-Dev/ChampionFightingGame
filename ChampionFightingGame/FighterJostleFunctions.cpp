#include "Fighter.h"

void Fighter::create_jostle_rect(GameCoordinate anchor, GameCoordinate offset) {
	anchor.x = ((anchor.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - pos.y;
	offset.x = ((offset.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - pos.y;
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	jostle_box.x = anchor.x;
	jostle_box.y = anchor.y;
	jostle_box.w = offset.x;
	jostle_box.h = offset.y;
}