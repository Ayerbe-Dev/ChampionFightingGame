#include "Object.h"

void Object::update_grabbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (grabboxes[i].id != -1) {
			grabboxes[i].update_pos(this);
		}
	}
}

void Object::clear_grabbox(int id) {
	grabboxes[id].clear();
}

void Object::clear_grabbox_all() {
	for (int i = 0; i < 10; i++) {
		grabboxes[i].clear();
	}
}

#include "Fighter.h"

void Fighter::new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	if (id < 10) {
		grabboxes[id] = Grabbox(this, id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
	}
}