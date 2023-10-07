#include "BattleObject.h"

void BattleObject::new_pushbox(int id, glm::vec2 anchor, glm::vec2 offset) {
	if (id < 10) {
		pushboxes[id].activate(id, anchor, offset);
	}
}

/// <summary>
/// Updates the position of a pushbox relative to the object. 
/// </summary>
void BattleObject::update_pushbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (pushboxes[i].active) {
			pushboxes[i].update_pos();
		}
	}
}

/// <summary>
/// Clear the pushbox with the specified ID.
/// </summary>
/// <param name="id">: The ID of the pushbox to clear.</param>
void BattleObject::clear_pushbox(int id) {
	pushboxes[id].clear();
}

/// <summary>
/// Clear all active pushboxes.
/// </summary>
void BattleObject::clear_pushbox_all() {
	for (int i = 0; i < 10; i++) {
		pushboxes[i].clear();
	}
}

bool BattleObject::pushboxes_touching(BattleObject* object) {
	for (int i = 0; i < 10; i++) {
		if (!pushboxes[i].active) continue;
		for (int i2 = 0; i2 < 10; i2++) {
			if (!object->pushboxes[i2].active) continue;
			if (is_collide(pushboxes[i].rect, object->pushboxes[i2].rect)) {
				return true;
			}
		}
	}
	return false;
}

float BattleObject::get_pushbox_front(size_t id) {
	return pushboxes[id].rect.corners[2].x - pos.x;
}

float BattleObject::get_pushbox_back(size_t id) {
	return pushboxes[id].rect.corners[0].x - pos.x;
}