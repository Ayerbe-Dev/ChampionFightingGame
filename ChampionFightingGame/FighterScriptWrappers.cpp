#include "Fighter.h"

void Fighter::NEW_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);

	HurtboxKind hurtbox_kind = HURTBOX_KIND_NORMAL;
	bool armor = false;
	IntangibleKind intangible_kind = INTANGIBLE_KIND_NONE;

	UNWRAP_NO_DECL(hurtbox_kind);
	UNWRAP_NO_DECL(armor);
	UNWRAP_NO_DECL(intangible_kind);

	new_hurtbox(id, anchor, offset, hurtbox_kind, armor, intangible_kind);
}

void Fighter::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, double);
	UNWRAP(chip_damage, double);
	UNWRAP(counterhit_damage_mul, double);
	UNWRAP(scale, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(hitbox_kind, HitboxKind);
	UNWRAP(meter_gain_on_hit, double);
	UNWRAP(meter_gain_on_counterhit, double);
	UNWRAP(meter_gain_on_block, double);
	UNWRAP(situation_hit, SituationHit);
	UNWRAP(hitlag, int);
	UNWRAP(hitstun, int);
	UNWRAP(blocklag, int);
	UNWRAP(blockstun, int);
	UNWRAP(unblockable, bool);
	UNWRAP(attack_height, AttackHeight);
	UNWRAP(attack_level, AttackLevel);
	UNWRAP(hit_pushback, double);
	UNWRAP(block_pushback, double);
	UNWRAP(clank_kind, ClankKind);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(max_juggle, int);
	UNWRAP(hit_status, HitStatus);
	UNWRAP(counterhit_status, HitStatus);
	UNWRAP(counterhit_type, CounterhitType);
	UNWRAP(launch_init_y, double);
	UNWRAP(launch_gravity_y, double);
	UNWRAP(launch_max_fall_speed, double);
	UNWRAP(launch_speed_x, double);
	UNWRAP(continue_launch, bool);
	UNWRAP(can_chip_ko, bool);
	UNWRAP(can_ko, bool);
	bool use_player_pos = true;
	UNWRAP_NO_DECL(use_player_pos);
	new_hitbox(id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
		meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
		attack_level, hit_pushback, block_pushback, clank_kind, juggle_start, juggle_increase, max_juggle, hit_status, counterhit_status,
		counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, continue_launch, can_chip_ko, can_ko, use_player_pos);
}