#include "BattleObject.h"

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