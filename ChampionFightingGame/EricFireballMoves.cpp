#include "EricFireball.h"
#include "EricFireballConstants.h"

void EricFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("move", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, KO_KIND_CHIP, true, 10.0, 0.0, 0.0, 1.0);
		});
	});
}