#include "Battle.h"

HitboxSim::HitboxSim() {
	active_box = nullptr;
}

void HitboxSim::destroy() {
	active_box = nullptr;
	hitboxes.clear();
	hurtboxes.clear();
	grabboxes.clear();
	pushboxes.clear();
}

void HitboxSim::update(BattleObject* active_object) {
	for (size_t i = 0, max = hitboxes.size(); i < max; i++) {
		hitboxes[i].update_rect(active_object);
	}
	for (size_t i = 0, max = hurtboxes.size(); i < max; i++) {
		hurtboxes[i].update_rect(active_object);
	}
	for (size_t i = 0, max = grabboxes.size(); i < max; i++) {
		grabboxes[i].update_rect(active_object);
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		pushboxes[i].update_rect(active_object);
	}
}

void HitboxSim::render(BattleObject* active_object) {
	for (size_t i = 0, max = hitboxes.size(); i < max; i++) {
		hitboxes[i].render(active_object);
	}
	for (size_t i = 0, max = hurtboxes.size(); i < max; i++) {
		hurtboxes[i].render(active_object);
	}
	for (size_t i = 0, max = grabboxes.size(); i < max; i++) {
		grabboxes[i].render(active_object);
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		pushboxes[i].render(active_object);
	}
}

void HitboxSim::print(BattleObject* active_object) {
	if (!(active_box || active_object)) return;
	std::cout.precision(1);
	active_box->print_start(active_object);
}

void HitboxSim::print_all(BattleObject* active_object) {
	if (!active_object) return;
	std::cout.precision(1);
	std::vector<std::pair<float,std::vector<Simbox*>>> frames_start;
	std::vector<std::pair<float, std::vector<Simbox*>>> frames_end;

	//This code is a kind of gross way of organizing our script commands, but printing is slow as
	//fuck no matter what so I'm ok with it here.

	for (size_t i = 0, max = hitboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames_start.size(); i2 < max2; i2++) {
			if (hitboxes[i].frame_start == frames_start[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_start.size();
			std::vector<Simbox*> vec;
			frames_start.push_back(std::pair(hitboxes[i].frame_start, vec));
		}
		frames_start[frame_index].second.push_back(&hitboxes[i]);
		if (hitboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames_end.size(); i2 < max2; i2++) {
			if (hitboxes[i].frame_end == frames_end[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_end.size();
			std::vector<Simbox*> vec;
			frames_end.push_back(std::pair(hitboxes[i].frame_end, vec));
		}
		frames_end[frame_index].second.push_back(&hitboxes[i]);
	}
	for (size_t i = 0, max = hurtboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames_start.size(); i2 < max2; i2++) {
			if (hurtboxes[i].frame_start == frames_start[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_start.size();
			std::vector<Simbox*> vec;
			frames_start.push_back(std::pair(hurtboxes[i].frame_start, vec));
		}
		frames_start[frame_index].second.push_back(&hurtboxes[i]);
		if (hurtboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames_end.size(); i2 < max2; i2++) {
			if (hurtboxes[i].frame_end == frames_end[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_end.size();
			std::vector<Simbox*> vec;
			frames_end.push_back(std::pair(hurtboxes[i].frame_end, vec));
		}
		frames_end[frame_index].second.push_back(&hurtboxes[i]);
	}
	for (size_t i = 0, max = grabboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames_start.size(); i2 < max2; i2++) {
			if (grabboxes[i].frame_start == frames_start[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_start.size();
			std::vector<Simbox*> vec;
			frames_start.push_back(std::pair(grabboxes[i].frame_start, vec));
		}
		frames_start[frame_index].second.push_back(&grabboxes[i]);
		if (grabboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames_end.size(); i2 < max2; i2++) {
			if (grabboxes[i].frame_end == frames_end[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_end.size();
			std::vector<Simbox*> vec;
			frames_end.push_back(std::pair(grabboxes[i].frame_end, vec));
		}
		frames_end[frame_index].second.push_back(&grabboxes[i]);
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames_start.size(); i2 < max2; i2++) {
			if (pushboxes[i].frame_start == frames_start[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_start.size();
			std::vector<Simbox*> vec;
			frames_start.push_back(std::pair(pushboxes[i].frame_start, vec));
		}
		frames_start[frame_index].second.push_back(&pushboxes[i]);
		if (pushboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames_end.size(); i2 < max2; i2++) {
			if (pushboxes[i].frame_end == frames_end[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames_end.size();
			std::vector<Simbox*> vec;
			frames_end.push_back(std::pair(pushboxes[i].frame_end, vec));
		}
		frames_end[frame_index].second.push_back(&pushboxes[i]);
	}
	for (size_t i = 0, max = frames_start.size(); i < max; i++) {
		std::cout << "execute_frame(" << frames_start[i].first << ", [this]() {\n";
		for (size_t i2 = 0, max2 = frames_start[i].second.size(); i2 < max2; i2++) {
			frames_start[i].second[i2]->print_start(active_object);
		}
		for (size_t i2 = 0, max2 = frames_end[i].second.size(); i2 < max2; i2++) {
			frames_end[i].second[i2]->print_end(active_object);
		}
		std::cout << "});\n";
	}
}

Simbox::Simbox() {
	rect.init();
	anchor = glm::vec2(0, 0);
	offset = glm::vec2(0, 0);
	relative_anchor = glm::vec2(0, 0);
	relative_offset = glm::vec2(0, 0);
	prev_anchor = glm::vec2(0, 0);
	prev_offset = glm::vec2(0, 0);
	prev_relative_anchor = glm::vec2(0, 0);
	prev_relative_offset = glm::vec2(0, 0);
	frame_start = -1;
	frame_end = -1;
	id = -1;
}

void Simbox::update_rect(BattleObject* object) {
	if (prev_anchor != anchor) {
		prev_anchor = anchor;
		relative_anchor = anchor - glm::vec2(object->pos);
		relative_anchor.x *= object->facing_dir;
		if (object->anim_kind != nullptr && !object->anim_kind->flag_move) {
			relative_anchor.x += object->get_trans_offset().x;
		}
		prev_relative_anchor = relative_anchor;
	}
	if (prev_offset != offset) {
		prev_offset = offset;
		relative_offset = offset - glm::vec2(object->pos);
		relative_offset.x *= object->facing_dir;
		if (object->anim_kind != nullptr && !object->anim_kind->flag_move) {
			relative_offset.x += object->get_trans_offset().x;
		}
		prev_relative_offset = relative_offset;
	}
	if (prev_relative_anchor != relative_anchor) {
		prev_relative_anchor = relative_anchor;
		anchor = relative_anchor + glm::vec2(object->pos);
		prev_anchor = anchor;
	}
	if (prev_relative_offset != relative_offset) {
		prev_relative_offset = relative_offset;
		offset = relative_offset + glm::vec2(object->pos);
		prev_offset = offset;
	}
	rect.update_corners(anchor, offset);
}

void Simbox::render(BattleObject* active_object) {
	if (active_object->frame >= frame_start && (active_object->frame < frame_end || frame_end == -1)) {
		rect.render();
	}
}

SimHitbox::SimHitbox() {
	multihit = 0;
	damage = 0.0;
	chip_damage = 0.0;
	damage_scale = 1;
	meter_gain = 0.0;
	hit_kind = HIT_KIND_GROUND;
	attack_level = ATTACK_LEVEL_LIGHT;
	attack_height = ATTACK_HEIGHT_MID;
	hitlag = 0;
	blocklag = 0;
	hit_advantage = 0;
	block_advantage = 0;
	hit_pushback = 10.0;
	block_pushback = 10.0;
	hit_status = HIT_STATUS_NORMAL;
	counterhit_status = HIT_STATUS_NORMAL;
	counterhit_type = COUNTERHIT_TYPE_COUNTER;
	juggle_start = 0;
	juggle_increase = 0;
	juggle_max = 0;
	clank_kind = CLANK_KIND_NORMAL;
	damage_kind = DAMAGE_KIND_NORMAL;
	continue_launch = false;
	disable_hitstun_parry = false;
	launch_init_y = 0.0;
	launch_gravity_y = 0.0;
	launch_max_fall_speed = 0.0;
	launch_speed_x = 0.0;
	rect.set_rgba(glm::vec4(255.0, 0.0, 0.0, 127.0));
}

void SimHitbox::print_start(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::NEW_HITBOX, /*ID*/ ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::NEW_HITBOX, /*ID*/ ";
	}
	std::string hit_kind_text = "";
	switch (hit_kind) {
		case (HIT_KIND_GROUND): {
			hit_kind_text = ", HIT_KIND_GROUND";
		} break;
		case (HIT_KIND_AIR): {
			hit_kind_text = ", HIT_KIND_AIR";
		} break;
		case (HIT_KIND_DOWN): {
			hit_kind_text = ", HIT_KIND_DOWN";
		} break;
		case (HIT_KIND_PROJECTILE): {
			hit_kind_text = ", HIT_KIND_PROJECTILE";
		} break;
		case (HIT_KIND_SOFT_INTANGIBLE): {
			hit_kind_text = ", HIT_KIND_SOFT_INTANGIBLE";
		} break;
		case (HIT_KIND_ARMOR): {
			hit_kind_text = ", HIT_KIND_ARMOR";
		} break;
	}
	std::string attack_level_text = "";
	switch (attack_level) {
		case (ATTACK_LEVEL_LIGHT): {
			attack_level_text = ", ATTACK_LEVEL_LIGHT";
		} break;
		case (ATTACK_LEVEL_MEDIUM): {
			attack_level_text = ", ATTACK_LEVEL_MEDIUM";
		} break;
		case (ATTACK_LEVEL_HEAVY): {
			attack_level_text = ", ATTACK_LEVEL_HEAVY";
		} break;
		default: {
			attack_level_text = ", ATTACK_LEVEL_MAX";
		} break;
	}
	std::string attack_height_text = "";
	switch (attack_height) {
		case (ATTACK_HEIGHT_LOW): {
			attack_height_text = ", ATTACK_HEIGHT_LOW";
		} break;
		case (ATTACK_HEIGHT_MID): {
			attack_height_text = ", ATTACK_HEIGHT_MID";
		} break;
		case (ATTACK_HEIGHT_HIGH): {
			attack_height_text = ", ATTACK_HEIGHT_HIGH";
		} break;
		default: {
			attack_height_text = ", ATTACK_HEIGHT_MAX";
		} break;
	}

	int hitstun_frames;
	int blockstun_frames;
	switch (object->object_type) {
		case (BATTLE_OBJECT_TYPE_FIGHTER): {
			Fighter* fighter = (Fighter*)object;
			hitstun_frames = hit_advantage + fighter->get_frames_until_actionable();
			blockstun_frames = block_advantage + fighter->get_frames_until_actionable();
		} break;
		case (BATTLE_OBJECT_TYPE_PROJECTILE): {
			Projectile* projectile = (Projectile*)object;
			hitstun_frames = hit_advantage - hitlag + projectile->projectile_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
			blockstun_frames = block_advantage - blocklag + projectile->projectile_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
		} break;
		default: {
			hitstun_frames = 0;
			blockstun_frames = 0;
		} break;
	}
	if (object->object_type == BATTLE_OBJECT_TYPE_FIGHTER) {
		hitstun_frames = hit_advantage + ((Fighter*)object)->get_frames_until_actionable();
	}
	
	std::string hit_status_text = "";
	switch (hit_status) {
		case (HIT_STATUS_NORMAL): {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_NORMAL";
		} break;
		case (HIT_STATUS_KNOCKDOWN): {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_KNOCKDOWN";
		} break;
		case (HIT_STATUS_LAUNCH): {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_LAUNCH";
		} break;
		case (HIT_STATUS_FLOAT): {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_FLOAT";
		} break;
		case (HIT_STATUS_CRUMPLE): {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_CRUMPLE";
		} break;
		default: {
			hit_status_text = ", /*Hit Status*/ HIT_STATUS_MAX";
		} break;
	}
	std::string counterhit_status_text = "";
	switch (counterhit_status) {
		case (HIT_STATUS_NORMAL): {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_NORMAL";
		} break;
		case (HIT_STATUS_KNOCKDOWN): {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_KNOCKDOWN";
		} break;
		case (HIT_STATUS_LAUNCH): {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_LAUNCH";
		} break;
		case (HIT_STATUS_FLOAT): {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_FLOAT";
		} break;
		case (HIT_STATUS_CRUMPLE): {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_CRUMPLE";
		} break;
		default: {
			counterhit_status_text = ", /*Counterhit Status*/ HIT_STATUS_MAX";
		} break;
	}
	std::string counterhit_type_text = "";
	switch (counterhit_type) {
		case (COUNTERHIT_TYPE_COUNTER): {
			counterhit_type_text = ", COUNTERHIT_TYPE_COUNTER";
		} break;
		case (COUNTERHIT_TYPE_ANY): {
			counterhit_type_text = ", COUNTERHIT_TYPE_ANY";
		} break;
		case (COUNTERHIT_TYPE_PUNISH): {
			counterhit_type_text = ", COUNTERHIT_TYPE_PUNISH";
		} break;
		case (COUNTERHIT_TYPE_JUMP_COUNTER): {
			counterhit_type_text = ", COUNTERHIT_TYPE_JUMP_COUNTER";
		} break;
		case (COUNTERHIT_TYPE_NONE): {
			counterhit_type_text = ", COUNTERHIT_TYPE_NONE";
		} break;
		default: {
			counterhit_type_text = ", COUNTERHIT_TYPE_MAX";
		} break;
	}

	std::string clank_text = "";
	switch (clank_kind) {
		case (CLANK_KIND_NORMAL): {
			clank_text = ", CLANK_KIND_NORMAL";
		} break;
		case (CLANK_KIND_CLANK): {
			clank_text = ", CLANK_KIND_CLANK";
		} break;
		case (CLANK_KIND_CONTINUE): {
			clank_text = ", CLANK_KIND_CONTINUE";
		} break;
		default: {
			clank_text = ", CLANK_KIND_MAX";
		} break;
	}

	std::string damage_kind_text = "";
	switch (damage_kind) {
		case (DAMAGE_KIND_NORMAL): {
			damage_kind_text = ", DAMAGE_KIND_NORMAL";
		} break;
		case (DAMAGE_KIND_CHIP): {
			damage_kind_text = ", DAMAGE_KIND_CHIP";
		} break;
		case (DAMAGE_KIND_CHIP_KO): {
			damage_kind_text = ", DAMAGE_KIND_CHIP_KO";
		} break;
		case (DAMAGE_KIND_NO_KO): {
			damage_kind_text = ", DAMAGE_KIND_NO_KO";
		} break;
		default: {
			damage_kind_text = ", DAMAGE_KIND_MAX";
		} break;
	}
	std::string continue_launch_text = ", /*Continue Launch*/ false";
	if (continue_launch) {
		continue_launch_text = ", /*Continue Launch*/ true";
	}
	std::string disable_hitstun_parry_text = ", /*Disable Hitstun Parry*/ false";
	if (disable_hitstun_parry) {
		disable_hitstun_parry_text = ", /*Disable Hitstun Parry*/ true";
	}
	glm::ivec2 anchor = this->relative_anchor;
	glm::ivec2 offset = this->relative_offset;
	std::cout << padding << id << ", /*Multihit ID*/ " << multihit << ", /*Damage*/ " << damage
		<< ", /*Chip Damage*/ " << chip_damage << ", /*Damage Scale*/ " << damage_scale
		<< ", /*Meter Gain*/ " << meter_gain << ", glm::vec2(" << anchor.x << ", " << anchor.y
		<< "), glm::vec2(" << offset.x << ", " << offset.y << ")" << hit_kind_text
		<< attack_level_text << attack_height_text << ", /*Hitlag*/ " << hitlag << ", /*Blocklag*/ "
		<< blocklag << ", /*Hitstun*/ " << hitstun_frames << ", /*Blockstun*/ " << blockstun_frames
		<< ", /*Hit Pushback*/ " << hit_pushback << ", /*Block Pushback*/ " << block_pushback
		<< hit_status_text << counterhit_status_text << counterhit_type_text
		<< ", /*Juggle Start*/ " << juggle_start << ", /*Juggle Increase*/ " << juggle_increase
		<< ", /*Juggle Max*/ " << juggle_max << clank_text << damage_kind_text
		<< continue_launch_text << disable_hitstun_parry_text << ", /*Launch Init Y Speed*/ " 
		<< launch_init_y << ", /*Launch Gravity*/ " << launch_gravity_y 
		<< ", /*Launch Max Fall Speed*/ " << launch_max_fall_speed << ", /*Launch X Speed*/ " 
		<< launch_speed_x << ");\n";
}

void SimHitbox::print_end(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::CLEAR_HITBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::CLEAR_HITBOX, ";
	}
	std::cout << padding << id << ");\n";
}

SimHurtbox::SimHurtbox() {
	hurtbox_kind = HURTBOX_KIND_NORMAL;
	armor = false;
	intangible_kind = INTANGIBLE_KIND_NONE;
	rect.set_rgba(glm::vec4(0.0, 0.0, 255.0, 127.0));
}

void SimHurtbox::print_start(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::NEW_HURTBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::NEW_HURTBOX, ";
	}
	std::string hurtbox_text = "";
	switch (hurtbox_kind) {
		case (HURTBOX_KIND_NORMAL): {
			hurtbox_text = ", HURTBOX_KIND_NORMAL";
		} break;
		case (HURTBOX_KIND_COUNTER): {
			hurtbox_text = ", HURTBOX_KIND_COUNTER";
		} break;
		case (HURTBOX_KIND_RIGHT_OF_WAY): {
			hurtbox_text = ", HURTBOX_KIND_RIGHT_OF_WAY";
		} break;
		default: {
			hurtbox_text = ", HURTBOX_KIND_MAX";
		} break;
	}

	std::string armor_text = ", false";
	if (armor) {
		armor_text = ", true";
	}

	std::string intangible_text = "";
	switch (intangible_kind) {
		case (INTANGIBLE_KIND_NONE): {
			intangible_text = ", INTANGIBLE_KIND_NONE";
		} break;
		case (INTANGIBLE_KIND_HIGH): {
			intangible_text = ", INTANGIBLE_KIND_HIGH";
		} break;
		case (INTANGIBLE_KIND_MID): {
			intangible_text = ", INTANGIBLE_KIND_MID";
		} break;
		case (INTANGIBLE_KIND_LOW): {
			intangible_text = ", INTANGIBLE_KIND_LOW";
		} break;
		case (INTANGIBLE_KIND_AERIAL): {
			intangible_text = ", INTANGIBLE_KIND_AERIAL";
		} break;
		case (INTANGIBLE_KIND_STRIKE): {
			intangible_text = ", INTANGIBLE_KIND_STRIKE";
		} break;
		case (INTANGIBLE_KIND_THROW): {
			intangible_text = ", INTANGIBLE_KIND_THROW";
		} break;
		case (INTANGIBLE_KIND_PROJECTILE): {
			intangible_text = ", INTANGIBLE_KIND_PROJECTILE";
		} break;
		case (INTANGIBLE_KIND_INVINCIBLE): {
			intangible_text = ", INTANGIBLE_KIND_INVINCIBLE";
		} break;
		case (INTANGIBLE_KIND_SOFT): {
			intangible_text = ", INTANGIBLE_KIND_SOFT";
		} break;
		default: {
			intangible_text = ", INTANGIBLE_KIND_MAX";
		} break;
	}
	glm::ivec2 anchor = this->relative_anchor;
	glm::ivec2 offset = this->relative_offset;
	std::cout << padding << id << ", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2("
		<< offset.x << ", " << offset.y << ")" << hurtbox_text << armor_text << intangible_text 
		<< ");\n";
}

void SimHurtbox::print_end(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::CLEAR_HURTBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::CLEAR_HURTBOX, ";
	}
	std::cout << padding << id << ");\n";
}

SimGrabbox::SimGrabbox() {
	grabbox_kind = GRABBOX_KIND_NORMAL;
	hit_kind = HIT_KIND_GROUND;
	attacker_status = "FIGHTER_STATUS_THROW";
	defender_status = "FIGHTER_STATUS_GRABBED";
	rect.set_rgba(glm::vec4(0.0, 255.0, 0.0, 127.0));
}

void SimGrabbox::print_start(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::NEW_GRABBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::NEW_GRABBOX, ";
	}
	std::string grabbox_text = "";
	switch (grabbox_kind) {
		case (GRABBOX_KIND_NORMAL): {
			grabbox_text = ", GRABBOX_KIND_NORMAL";
		} break;
		case (GRABBOX_KIND_HITSTUN): {
			grabbox_text = ", GRABBOX_KIND_HITSTUN";
		} break;
		case (GRABBOX_KIND_NOTECH): {
			grabbox_text = ", GRABBOX_KIND_NOTECH";
		} break;
		case (GRABBOX_KIND_HITSTUN_NOTECH): {
			grabbox_text = ", GRABBOX_KIND_HITSTUN_NOTECH";
		} break;
		default: {
			grabbox_text = ", GRABBOX_KIND_MAX";
		} break;
	}
	std::string hit_kind_text = "";
	switch (hit_kind) {
		case (HIT_KIND_GROUND): {
			hit_kind_text = ", HIT_KIND_GROUND";
		} break;
		case (HIT_KIND_AIR): {
			hit_kind_text = ", HIT_KIND_AIR";
		} break;
		case (HIT_KIND_DOWN): {
			hit_kind_text = ", HIT_KIND_DOWN";
		} break;
		case (HIT_KIND_PROJECTILE): {
			hit_kind_text = ", HIT_KIND_PROJECTILE";
		} break;
		case (HIT_KIND_SOFT_INTANGIBLE): {
			hit_kind_text = ", HIT_KIND_SOFT_INTANGIBLE";
		} break;
		case (HIT_KIND_ARMOR): {
			hit_kind_text = ", HIT_KIND_ARMOR";
		} break;
	}
	glm::ivec2 anchor = this->relative_anchor;
	glm::ivec2 offset = this->relative_offset;
	std::cout << padding << id << ", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2("
		<< offset.x << ", " << offset.y << ")" << grabbox_text << hit_kind_text << ", " 
		<< attacker_status << ", " << defender_status << ");\n";
}

void SimGrabbox::print_end(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::CLEAR_GRABBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::CLEAR_GRABBOX, ";
	}
	std::cout << padding << id << ");\n";
}

SimPushbox::SimPushbox() {
	rect.set_rgba(glm::vec4(255.0, 255.0, 0.0, 127.0));
}

void SimPushbox::print_start(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::NEW_PUSHBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::NEW_PUSHBOX, ";
	}
	glm::ivec2 anchor = this->relative_anchor;
	glm::ivec2 offset = this->relative_offset;
	std::cout << padding << id << ", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2("
		<< offset.x << ", " << offset.y << "));\n";
}

void SimPushbox::print_end(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::CLEAR_PUSHBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::CLEAR_PUSHBOX, ";
	}
	std::cout << padding << id << ");\n";
}