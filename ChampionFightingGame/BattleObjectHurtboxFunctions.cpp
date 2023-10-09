#include "BattleObject.h"

void BattleObject::new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, HurtboxKind hurtbox_kind, int armor_hits, IntangibleKind intangible_kind) {
	if (id < 10) {
		hurtboxes[id].activate(id, anchor, offset, hurtbox_kind, armor_hits, intangible_kind);
	}
}

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

void BattleObject::set_hurtbox_intangible_kind(int id, IntangibleKind intangible_kind) {
	hurtboxes[id].intangible_kind = intangible_kind;
}