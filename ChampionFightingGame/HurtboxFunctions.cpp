#include "Object.h"

void Object::update_hurtbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (hurtboxes[i].id != -1) {
			hurtboxes[i].update_pos(this);
		}
	}
}

void Object::clear_hurtbox(int id) {
	hurtboxes[id].clear();
}

void Object::clear_hurtbox_all() {
	for (int i = 0; i < 10; i++) {
		hurtboxes[i].clear();
	}
}

#include "Fighter.h"

void Fighter::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind) {
	if (id < 10) {
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}