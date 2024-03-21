#include "BattleObject.h"

void BattleObject::new_hitbox(int id, int multihit, glm::vec2 anchor, glm::vec2 offset,
	CollisionKind collision_kind, HitResult hit_result, HitStatus hit_status, 
	unsigned int custom_hit_status, HitMove hit_move, HitFlag hit_flags,
	CriticalCondition critical_condition, HitStatus critical_status,
	unsigned int custom_critical_status, HitMove critical_hit_move,
	HitFlag critical_hit_flags, HitHeight hit_height, DamageKind damage_kind, 
	std::string hit_effect, std::string hit_sound) {
	if (id < 10) {
		hitboxes[id].activate(id, multihit, anchor, offset, collision_kind,
			hit_result, hit_status, custom_hit_status, hit_move, hit_flags, critical_condition,
			critical_status, custom_critical_status, critical_hit_move, critical_hit_flags,
			hit_height, damage_kind, hit_effect, hit_sound
		);
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