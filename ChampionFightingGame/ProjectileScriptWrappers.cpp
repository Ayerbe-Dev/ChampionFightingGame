#include "Projectile.h"

void Projectile::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, double);
	UNWRAP(chip_damage, double);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, double);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(situation_hit, SituationHit);
	UNWRAP(attack_level, AttackLevel);
	UNWRAP(attack_height, AttackHeight);
	UNWRAP(hitlag, int);
	UNWRAP(blocklag, int);
	UNWRAP(hitstun, int);
	UNWRAP(blockstun, int);
	UNWRAP(hit_pushback, double);
	UNWRAP(block_pushback, double);
	UNWRAP(hit_status, HitStatus);
	UNWRAP(counterhit_status, HitStatus);
	UNWRAP(counterhit_type, CounterhitType);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(juggle_max, int);
	UNWRAP(trade, bool);
	UNWRAP(ko_kind, KoKind);
	UNWRAP(continue_launch, bool);
	UNWRAP(disable_hitstun_parry, bool);
	UNWRAP(launch_init_y, double);
	UNWRAP(launch_gravity_y, double);
	UNWRAP(launch_max_fall_speed, double);
	UNWRAP(launch_speed_x, double);

	new_hitbox(id, multihit, damage, chip_damage, damage_scale, meter_gain,
		anchor, offset, situation_hit, attack_level, attack_height, hitlag, blocklag, hitstun,
		blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type,
		juggle_start, juggle_increase, juggle_max, trade, ko_kind, continue_launch,
		disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x
	);
}