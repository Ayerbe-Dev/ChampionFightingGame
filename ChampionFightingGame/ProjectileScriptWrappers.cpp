#include "Projectile.h"

void Projectile::HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, float);
	UNWRAP(chip_damage, float);
	UNWRAP(counterhit_damage_mul, float);
	UNWRAP(scale, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(meter_gain_on_hit, float);
	UNWRAP(meter_gain_on_counterhit, float);
	UNWRAP(meter_gain_on_block, float);
	UNWRAP(situation_hit, int);
	UNWRAP(hitlag, int);
	UNWRAP(hitstun, int);
	UNWRAP(blocklag, int);
	UNWRAP(blockstun, int);
	UNWRAP(unblockable, bool);
	UNWRAP(hit_pushback, float);
	UNWRAP(block_pushback, float);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(max_juggle, int);
	UNWRAP(hit_status, int);
	UNWRAP(counterhit_status, int);
	UNWRAP(counterhit_type, int);
	UNWRAP(launch_init_y, float);
	UNWRAP(launch_gravity_y, float);
	UNWRAP(launch_max_fall_speed, float);
	UNWRAP(launch_speed_x, float);
	UNWRAP(trade, bool);
	UNWRAP(continue_launch, bool);
	UNWRAP(can_chip_ko, bool);
	UNWRAP(can_ko, bool);

	new_hitbox(id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, 
	meter_gain_on_hit, meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, 
	blocklag, blockstun, unblockable, hit_pushback, block_pushback, juggle_start, juggle_increase, 
	max_juggle, hit_status, counterhit_status, counterhit_type, launch_init_y, launch_gravity_y, 
	launch_max_fall_speed, launch_speed_x, trade, continue_launch, can_chip_ko, can_ko);
}