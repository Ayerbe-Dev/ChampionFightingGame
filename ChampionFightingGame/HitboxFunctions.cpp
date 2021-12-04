#include "Object.h"

void Object::update_hitbox_pos(bool add_window_width) {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1) {
			hitboxes[i].update_pos(this, add_window_width);
		}
	}
}

bool Object::is_hitbox_active(int multihit) {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1) {
			if (hitboxes[i].multihit == multihit) {
				return true;
			}
		}
	}
	return false;
}

void Object::clear_hitbox(int id) {
	int multihit = hitboxes[id].multihit;
	hitboxes[id].clear();
	hitboxes[id].multihit = 0;
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1 && hitboxes[i].multihit == multihit) {
			return;
		}
	}
	multihit_connected[multihit] = false;
}

void Object::clear_hitbox_all() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].clear();
		hitboxes[i].multihit = 0;
		multihit_connected[i] = false;
	}
}

void Object::update_hitbox_connect(int multihit_index) {
	multihit_connected[multihit_index] = true;
}

#include "Fighter.h"

/**
Creates a persistent hitbox until the status for the character changes.
\param id the uuid for the hitbox, call clear_hitbox(id) to delete it
\param anchor the x0 y0 values for the hitbox
\param offset the x1 y1 values for the hitbox
*/
void Fighter::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
	GameCoordinate offset, int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit,
	int hitlag, int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback,
	int clank_kind, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y,
	float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool use_player_pos) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
							  meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
							  attack_level, hit_pushback, block_pushback, clank_kind, juggle_set, max_juggle, hit_status, counterhit_status,
							  counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, continue_launch, can_chip_ko, use_player_pos);
	}
}

#include "Projectile.h"

void Projectile::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
	float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x,
	bool trade, bool continue_launch, bool can_chip_ko) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, meter_gain_on_hit,
			meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable,
			hit_pushback, block_pushback, juggle_set, max_juggle, hit_status, counterhit_status,
			counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, trade, continue_launch, can_chip_ko);
	}
}