#include "BattleObject.h"

void BattleObject::new_grabbox(int id, glm::vec2 anchor, glm::vec2 offset, GrabboxKind grabbox_kind,
	CollisionKind hit_kind, unsigned int attacker_status_if_hit, unsigned int defender_status_if_hit) {
	if (id < 10) {
		grabboxes[id].activate(id, anchor, offset, grabbox_kind, hit_kind, 
			attacker_status_if_hit, defender_status_if_hit);
	}
}

/// <summary>
/// Updates the position of a grabbox relative to the object. 
/// </summary>
void BattleObject::update_grabbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (grabboxes[i].id != -1) {
			grabboxes[i].update_pos();
		}
	}
}

/// <summary>
/// Clear the grabbox with the specified ID.
/// </summary>
/// <param name="id">: The ID of the grabbox to clear.</param>
void BattleObject::clear_grabbox(int id) {
	grabboxes[id].clear();
}

/// <summary>
/// Clear all active grabboxes.
/// </summary>
void BattleObject::clear_grabbox_all() {
	for (int i = 0; i < 10; i++) {
		grabboxes[i].clear();
	}
}