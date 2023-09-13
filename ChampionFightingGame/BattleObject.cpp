#include "BattleObject.h"
#include "BattleObjectManager.h"

BattleObject::BattleObject() {
	id = -1;
	object_type = BATTLE_OBJECT_TYPE_MAX;
	has_model = false;
	prev_pos = glm::vec3(0.0);
	extra_rot = glm::vec3(0.0);
	facing_dir = 1.0;
	internal_facing_dir = 1.0;
	facing_right = true;
	internal_facing_right = true;
	status_kind = 0;
	situation_kind = 0;
	for (int i = 0; i < 10; i++) {
		multihit_connected[i] = false;
	}
	active_script_condition = nullptr;
	connected_hitbox = nullptr;
	connected_grabbox = nullptr;
	incoming_collision_kind = INCOMING_COLLISION_KIND_NONE;
	post_collision_status = 0;
	last_execute_frame = 0.0;
	player = nullptr;
	stage = nullptr;
	battle_object_manager = BattleObjectManager::get_instance();
}