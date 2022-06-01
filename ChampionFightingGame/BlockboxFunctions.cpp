#include "BattleObject.h"

void BattleObject::new_blockbox(glm::vec2 anchor, glm::vec2 offset) {
	blockbox.activate(this, anchor, offset);
}

void BattleObject::update_blockbox_pos() {
	if (blockbox.active) {
		blockbox.update_pos();
	}
}

void BattleObject::clear_blockbox() {
	blockbox.clear();
}