#include "BattleObject.h"
#include "ObjectManager.h"

void BattleObject::add_pos(glm::vec3 pos) {
	pos *= object_manager->get_world_rate(this);
	this->pos += pos;
}

void BattleObject::set_pos(glm::vec3 pos) {
	this->pos = pos;
}