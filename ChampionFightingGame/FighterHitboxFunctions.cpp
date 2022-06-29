#include "Fighter.h"

void Fighter::new_hitbox(int id, int multihit, float damage, float chip_damage,
	int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, SituationHit situation_hit,
	AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
	int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
	HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
	int juggle_max, ClankKind clank_kind, KoKind ko_kind, bool continue_launch,
	bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
	float launch_max_fall_speed, float launch_speed_x, bool use_player_pos) {
	if (id < 10) {
		hitboxes[id].activate(this, id, multihit, damage, chip_damage, damage_scale, meter_gain, 
		anchor, offset, situation_hit, attack_level, attack_height, hitlag, blocklag, hitstun,
		blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type, 
		juggle_start, juggle_increase, juggle_max, clank_kind, ko_kind, continue_launch,
		disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, 
		use_player_pos);
	}
}

/// <summary>
/// Clear the hitbox with the specified ID and reset the multihit flag if no other remaining hitboxes share its multihit value.
/// </summary>
/// <param name="id">: The ID of the hitbox to clear.</param>
void Fighter::clear_hitbox(int id) {
	int multihit = hitboxes[id].multihit;
	hitboxes[id].clear();
	hitboxes[id].multihit = 0;
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active && hitboxes[i].multihit == multihit) {
			return;
		}
	}
	multihit_connected[multihit] = false;
	if (!active_move_script.has_function(&Fighter::NEW_HITBOX)) {
		fighter_flag[FIGHTER_FLAG_IN_ENDLAG] = true;
	}
}

/// <summary>
/// Clear all active hitboxes and mark all hitboxes as having not connected.
/// </summary>
void Fighter::clear_hitbox_all() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].clear();
		hitboxes[i].multihit = 0;
		multihit_connected[i] = false;
	}
	clear_blockbox();
	if (!active_move_script.has_function(&Fighter::NEW_HITBOX)) {
		fighter_flag[FIGHTER_FLAG_IN_ENDLAG] = true;
	}
}