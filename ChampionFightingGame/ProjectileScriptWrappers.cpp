#include "Projectile.h"

void Projectile::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, float);
	UNWRAP(chip_damage, float);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(situation_hit, SituationHit);
	UNWRAP(attack_level, AttackLevel);
	UNWRAP(attack_height, AttackHeight);
	UNWRAP(hitlag, int);
	UNWRAP(blocklag, int);
	UNWRAP(hitstun, int);
	UNWRAP(blockstun, int);
	UNWRAP(hit_pushback, float);
	UNWRAP(block_pushback, float);
	UNWRAP(hit_status, HitStatus);
	UNWRAP(counterhit_status, HitStatus);
	UNWRAP(counterhit_type, CounterhitType);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(juggle_max, int);
	UNWRAP(trade, bool);
	UNWRAP(ko_kind, DamageKind);
	UNWRAP(continue_launch, bool);
	UNWRAP(disable_hitstun_parry, bool);
	UNWRAP(launch_init_y, float);
	UNWRAP(launch_gravity_y, float);
	UNWRAP(launch_max_fall_speed, float);
	UNWRAP(launch_speed_x, float);

	new_hitbox(id, multihit, damage, chip_damage, damage_scale, meter_gain,
		anchor, offset, situation_hit, attack_level, attack_height, hitlag, blocklag, hitstun,
		blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type,
		juggle_start, juggle_increase, juggle_max, trade, ko_kind, continue_launch,
		disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x
	);
}

void Projectile::SET_INT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, int);

	set_int(target, val);
}

void Projectile::SET_FLOAT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, float);

	set_float(target, val);
}

void Projectile::SET_FLAG(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, bool);

	set_flag(target, val);
}

void Projectile::ADD_POS(ScriptArg args) {
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		add_pos(pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		add_pos(pos);
	}
}

void Projectile::SET_POS(ScriptArg args) {
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		set_pos(pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		set_pos(pos);
	}
}

void Projectile::CHANGE_STATUS(ScriptArg args) {
	UNWRAP(new_status_kind, int);
	bool call_end_status = true;
	UNWRAP_NO_DECL(call_end_status);
	bool require_different_status = true;
	UNWRAP_NO_DECL(require_different_status);
	change_status(new_status_kind, call_end_status, require_different_status);
}