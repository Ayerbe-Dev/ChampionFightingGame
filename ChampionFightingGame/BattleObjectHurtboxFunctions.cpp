#include "BattleObject.h"

/// <summary>
/// Updates the position of a hurtbox relative to the object. 
/// </summary>
void BattleObject::update_hurtbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (hurtboxes[i].id != -1) {
			hurtboxes[i].update_pos();
		}
	}
}

/// <summary>
/// Clear the hurtbox with the specified ID.
/// </summary>
/// <param name="id">: The ID of the hurtbox to clear.</param>
void BattleObject::clear_hurtbox(int id) {
	hurtboxes[id].clear();
}

/// <summary>
/// Clear all active hurtboxes.
/// </summary>
void BattleObject::clear_hurtbox_all() {
	for (int i = 0; i < 10; i++) {
		hurtboxes[i].clear();
	}
}
