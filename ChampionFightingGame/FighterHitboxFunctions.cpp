#include "Fighter.h"

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