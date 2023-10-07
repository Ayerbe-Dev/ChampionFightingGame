#include "EricFireball.h"
#include "EricFireballConstants.h"

void EricFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("move", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_LEVEL_HEAVY, HIT_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, 10.0, 0.0, 0.0, 1.0);
		});
	});
}