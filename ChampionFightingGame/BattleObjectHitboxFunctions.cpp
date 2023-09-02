#include "BattleObject.h"

void BattleObject::new_hitbox(int id, int multihit, float damage, float chip_damage,
	int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, HitKind hit_kind,
	AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
	int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
	HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
	int juggle_max, ClankKind clank_kind, DamageKind ko_kind, bool continue_launch,
	bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
	float launch_max_fall_speed, float launch_speed_x) {
	if (id < 10) {
		hitboxes[id].activate(this, id, multihit, damage, chip_damage, damage_scale, meter_gain,
			anchor, offset, hit_kind, attack_level, attack_height, hitlag, blocklag, hitstun,
			blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type,
			juggle_start, juggle_increase, juggle_max, clank_kind, ko_kind, continue_launch,
			disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x);
	}
}

/// <summary>
/// Updates the position of a hitbox relative to the object. 
/// </summary>
/// <param name="add_window_width">: Whether or not to add the window width to the hitbox's new position, true by default. When called by projectiles,
/// this arg should be false instead.</param>
void BattleObject::update_hitbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active) {
			hitboxes[i].update_pos();
		}
	}
}

/// <summary>
/// Check if any active hitboxes share the specified multihit index. If -1 is passed as the arg (or no arg is passed), this function will return true 
/// if any hitboxes are active.
/// </summary>
/// <param name="multihit">: The multihit index to check the activity of, -1 by default</param>
/// <returns></returns>
bool BattleObject::is_hitbox_active(int multihit) {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active) {
			if (hitboxes[i].multihit == multihit || multihit == -1) {
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// Marks the multihit index as having connected, preventing any other hitboxes that share that multihit value from connecting until there has been a
/// point where there were no active hitboxes with that multihit value.
/// </summary>
/// <param name="multihit_index">: The index to mark as having connected.</param>
void BattleObject::update_hitbox_connect(int multihit_index) {
	multihit_connected[multihit_index] = true;
}

/// <summary>
/// Clear the hitbox with the specified ID and reset the multihit flag if no other remaining hitboxes share its multihit value.
/// </summary>
/// <param name="id">: The ID of the hitbox to clear.</param>
void BattleObject::clear_hitbox(int id) {
	int multihit = hitboxes[id].multihit;
	hitboxes[id].clear();
	hitboxes[id].multihit = 0;
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].active && hitboxes[i].multihit == multihit) {
			return;
		}
	}
	multihit_connected[multihit] = false;
}

/// <summary>
/// Clear all active hitboxes and mark all hitboxes as having not connected.
/// </summary>
void BattleObject::clear_hitbox_all() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].clear();
		hitboxes[i].multihit = 0;
		multihit_connected[i] = false;
	}
	clear_blockbox();
}