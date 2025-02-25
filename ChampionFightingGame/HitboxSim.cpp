#include "Battle.h"
#include "glm/gtx/string_cast.hpp"

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
	std::vector<std::pair<float, std::pair<std::vector<Simbox*>, std::vector<Simbox*>>>> frames;

	//This code is a kind of gross way of organizing our script commands, but printing is slow as
	//fuck no matter what so I'm ok with it here.

	for (size_t i = 0, max = hitboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (hitboxes[i].frame_start == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(hitboxes[i].frame_start, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.first.push_back(&hitboxes[i]);
		if (hitboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (hitboxes[i].frame_end == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec;
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(hitboxes[i].frame_end, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.second.push_back(&hitboxes[i]);
	}
	for (size_t i = 0, max = hurtboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (hurtboxes[i].frame_start == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(hurtboxes[i].frame_start, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.first.push_back(&hurtboxes[i]);
		if (hurtboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (hurtboxes[i].frame_end == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec;
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(hurtboxes[i].frame_end, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.second.push_back(&hurtboxes[i]);
	}
	for (size_t i = 0, max = grabboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (grabboxes[i].frame_start == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(grabboxes[i].frame_start, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.first.push_back(&grabboxes[i]);
		if (grabboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (hitboxes[i].frame_end == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec;
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(grabboxes[i].frame_end, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.second.push_back(&grabboxes[i]);
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		int frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (pushboxes[i].frame_start == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(pushboxes[i].frame_start, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.first.push_back(&pushboxes[i]);
		if (pushboxes[i].frame_end == -1) continue;
		frame_index = -1;
		for (size_t i2 = 0, max2 = frames.size(); i2 < max2; i2++) {
			if (pushboxes[i].frame_end == frames[i2].first) {
				frame_index = i2;
				break;
			}
		}
		if (frame_index == -1) {
			frame_index = frames.size();
			std::vector<Simbox*> vec;
			std::vector<Simbox*> vec_start;
			std::vector<Simbox*> vec_end;
			frames.push_back(std::pair(pushboxes[i].frame_end, std::pair(vec_start, vec_end)));
		}
		frames[frame_index].second.second.push_back(&pushboxes[i]);
	}
	for (size_t i = 0, max = frames.size(); i < max; i++) {
		std::cout << "execute_frame(" << frames[i].first << ", [this]() {\n";
		for (size_t i2 = 0, max2 = frames[i].second.first.size(); i2 < max2; i2++) {
			frames[i].second.first[i2]->print_start(active_object);
		}
		for (size_t i2 = 0, max2 = frames[i].second.second.size(); i2 < max2; i2++) {
			frames[i].second.second[i2]->print_end(active_object);
		}
		std::cout << "});\n";
	}
}

Simbox::Simbox() {
	rect.init("resource/misc/black.png", 0).set_orientation(TEXTURE_BOTTOM_LEFT);
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
		relative_anchor = anchor - glm::vec2(object->get_scaled_pos());
		relative_anchor.x *= object->facing_dir;
		if (object->anim_kind != nullptr && !object->anim_kind->flag_move) {
			relative_anchor.x += object->get_trans_offset().x;
		}
		prev_relative_anchor = relative_anchor;
	}
	if (prev_offset != offset) {
		prev_offset = offset;
		relative_offset = offset - glm::vec2(object->get_scaled_pos());
		relative_offset.x *= object->facing_dir;
		if (object->anim_kind != nullptr && !object->anim_kind->flag_move) {
			relative_offset.x += object->get_trans_offset().x;
		}
		prev_relative_offset = relative_offset;
	}
	if (prev_relative_anchor != relative_anchor) {
		prev_relative_anchor = relative_anchor;
		anchor = relative_anchor + glm::vec2(object->get_scaled_pos());
		prev_anchor = anchor;
	}
	if (prev_relative_offset != relative_offset) {
		prev_relative_offset = relative_offset;
		offset = relative_offset + glm::vec2(object->get_scaled_pos());
		prev_offset = offset;
	}
	if (object->prev_pos != object->get_scaled_pos()) {
		anchor = relative_anchor + glm::vec2(object->get_scaled_pos());
		prev_anchor = anchor;
		offset = relative_offset + glm::vec2(object->get_scaled_pos());
		prev_offset = offset;
	}
	rect.set_pos(glm::vec3(anchor, 0.0f)).set_width(offset.x).set_height(offset.y);
}

void Simbox::render(BattleObject* active_object) {
	if (active_object->frame >= frame_start && (active_object->frame < frame_end || frame_end == -1)) {
		rect.render();
	}
}

SimHitbox::SimHitbox() {
	multihit = 0;
	collision_kind_ground = false;
	collision_kind_air = false;
	collision_kind_down = false;
	collision_kind_projectile = false;
	collision_kind_soft_intangible = false;
	collision_kind_armor = false;
	critical_condition = CRITICAL_CONDITION_COUNTERHIT;
	hit_status = HIT_STATUS_NORMAL;
	custom_hit_status = "";
	critical_status = HIT_STATUS_NORMAL;
	custom_critical_status = "";
	down_face_down = false;
	continue_launch = false;
	juggle_start = 0;
	juggle_increase = 0;
	juggle_max = 0;
	hit_height = HIT_HEIGHT_MID;
	damage = 0.0;
	chip_damage = 0.0;
	damage_scale = 1;
	meter_gain = 0.0;
	hitlag = 0;
	blocklag = 0;
	hit_advantage = 0;
	block_advantage = 0;
	disable_hitstun_parry = false;
	pushback_ground_hit = 10.0;
	pushback_ground_block = 10.0;
	pushback_air_x = 3.0;
	pushback_air_y = 25.0;
	pushback_frames = 10;
	has_launch_target_pos = false;
	launch_target_pos = glm::vec3(0.0);
	launch_init_y = 0.0;
	launch_gravity_y = 0.0;
	launch_max_fall_speed = 0.0;
	launch_speed_x = 0.0;
	damage_kind = DAMAGE_KIND_NORMAL;
	hit_effect = -1;
	hit_sound = -1;
	rect.set_colormod(glm::vec3(255.0, 0.0, 0.0)).set_alpha(127);
}

void SimHitbox::print_start(BattleObject* object) {
	std::string output = "	push_function(&Fighter::NEW_HITBOX, /*ID*/ ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		output = "	push_function(&Projectile::NEW_HITBOX, /*ID*/ ";
	}
	output += std::to_string(id) + ", /*Multihit ID*/ " + std::to_string(multihit)
		+ ", " + glm::to_string(relative_anchor) + ", " + glm::to_string(relative_offset);
	std::string sub_output = ", ";
	if (collision_kind_ground) {
		sub_output += "COLLISION_KIND_GROUND";
	}
	if (collision_kind_air) {
		if (sub_output == ", ") {
			sub_output += "COLLISION_KIND_AIR";
		}
		else {
			sub_output += " | COLLISION_KIND_AIR";
		}
	}
	if (collision_kind_down) {
		if (sub_output == ", ") {
			sub_output += "COLLISION_KIND_DOWN";
		}
		else {
			sub_output += " | COLLISION_KIND_DOWN";
		}
	}
	if (collision_kind_projectile) {
		if (sub_output == ", ") {
			sub_output += "COLLISION_KIND_PROJECTILE";
		}
		else {
			sub_output += " | COLLISION_KIND_PROJECTILE";
		}
	}
	if (collision_kind_soft_intangible) {
		if (sub_output == ", ") {
			sub_output += "COLLISION_KIND_SOFT_INTANGIBLE";
		}
		else {
			sub_output += " | COLLISION_KIND_SOFT_INTANGIBLE";
		}
	}
	if (collision_kind_armor) {
		if (sub_output == ", ") {
			sub_output += "COLLISION_KIND_ARMOR";
		}
		else {
			sub_output += " | COLLISION_KIND_ARMOR";
		}
	}
	output += sub_output;
	switch (hit_status) {
		case (HIT_STATUS_NORMAL): {
			output += ", /*Hit Status*/ HIT_STATUS_NORMAL";
		} break;
		case (HIT_STATUS_KNOCKDOWN): {
			output += ", /*Hit Status*/ HIT_STATUS_KNOCKDOWN";
		} break;
		case (HIT_STATUS_LAUNCH): {
			output += ", /*Hit Status*/ HIT_STATUS_LAUNCH";
		} break;
		case (HIT_STATUS_FLOAT): {
			output += ", /*Hit Status*/ HIT_STATUS_FLOAT";
		} break;
		case (HIT_STATUS_CRUMPLE): {
			output += ", /*Hit Status*/ HIT_STATUS_CRUMPLE";
		} break;
		case (HIT_STATUS_CUSTOM): {
			output += ", /*Custom Hit Status*/ " + custom_hit_status;
		} break;
		case (HIT_STATUS_NONE): {
			output += ", /*Hit Status*/ HIT_STATUS_NONE";
		} break;
		default: {
			output = ", /*Hit Status*/ HIT_STATUS_MAX";
		} break;
	}
	sub_output = ", ";
	if (force_stand) {
		sub_output += "HIT_FLAG_FORCE_STAND";
	}
	else if (force_crouch) {
		sub_output += "HIT_FLAG_FORCE_CROUCH";
	}
	else {
		if (force_aerial) {
			sub_output += "HIT_FLAG_FORCE_AERIAL";
		}
		if (down_face_down) {
			if (sub_output == ", ") {
				sub_output += "HIT_FLAG_DOWN_FACE_DOWN";
			}
			else {
				sub_output += " | HIT_FLAG_DOWN_FACE_DOWN";
			}
		}
		if (hard_knockdown) {
			if (sub_output == ", ") {
				sub_output += "HIT_FLAG_HARD_KNOCKDOWN";
			}
			else {
				sub_output += " | HIT_FLAG_HARD_KNOCKDOWN";
			}
		}
		if (continue_launch) {
			if (sub_output == ", ") {
				sub_output += "HIT_FLAG_CONTINUE_LAUNCH";
			}
			else {
				sub_output += " | HIT_FLAG_CONTINUE_LAUNCH";
			}
		}
		if (disable_hitstun_parry) {
			if (sub_output == ", ") {
				sub_output += "HIT_FLAG_DISABLE_HITSTUN_PARRY";
			}
			else {
				sub_output += " | HIT_FLAG_DISABLE_HITSTUN_PARRY";
			}
		}
	}
	if (sub_output == ", ") {
		sub_output += "HIT_FLAG_NONE";
	}
	output += sub_output;
	switch (critical_condition) {
		case (CRITICAL_CONDITION_COUNTERHIT): {
			output += ", CRITICAL_CONDITION_COUNTERHIT";
		} break;
		case (CRITICAL_CONDITION_PUNISH): {
			output += ", CRITICAL_CONDITION_PUNISH";
		} break;
		case (CRITICAL_CONDITION_NONE): {
			output += ", CRITICAL_CONDITION_NONE";
		} break;
		default: {
			output += ", CRITICAL_CONDITION_MAX";
		} break;
	}
	if (critical_condition != CRITICAL_CONDITION_NONE) {
		switch (critical_status) {
			case (HIT_STATUS_NORMAL): {
				output += ", /*Critical Status*/ HIT_STATUS_NORMAL";
			} break;
			case (HIT_STATUS_KNOCKDOWN): {
				output += ", /*Critical Status*/ HIT_STATUS_KNOCKDOWN";
			} break;
			case (HIT_STATUS_LAUNCH): {
				output += ", /*Critical Status*/ HIT_STATUS_LAUNCH";
			} break;
			case (HIT_STATUS_FLOAT): {
				output += ", /*Critical Status*/ HIT_STATUS_FLOAT";
			} break;
			case (HIT_STATUS_CRUMPLE): {
				output += ", /*Critical Status*/ HIT_STATUS_CRUMPLE";
			} break;
			case (HIT_STATUS_CUSTOM): {
				output += ", /*Custom Critical Status*/ " + custom_critical_status;
			} break;
			case (HIT_STATUS_NONE): {
				output += ", /*Critical Status*/ HIT_STATUS_NONE";
			} break;
			default: {
				output = ", /*Critical Status*/ HIT_STATUS_MAX";
			} break;
		}

		sub_output = ", ";
		if (critical_force_stand) {
			sub_output += "HIT_FLAG_FORCE_STAND";
		}
		else if (critical_force_crouch) {
			sub_output += "HIT_FLAG_FORCE_CROUCH";
		}
		else {
			if (critical_force_aerial) {
				sub_output += "HIT_FLAG_FORCE_AERIAL";
			}
			if (critical_down_face_down) {
				if (sub_output == ", ") {
					sub_output += "HIT_FLAG_DOWN_FACE_DOWN";
				}
				else {
					sub_output += " | HIT_FLAG_DOWN_FACE_DOWN";
				}
			}
			if (critical_hard_knockdown) {
				if (sub_output == ", ") {
					sub_output += "HIT_FLAG_HARD_KNOCKDOWN";
				}
				else {
					sub_output += " | HIT_FLAG_HARD_KNOCKDOWN";
				}
			}
			if (critical_continue_launch) {
				if (sub_output == ", ") {
					sub_output += "HIT_FLAG_CONTINUE_LAUNCH";
				}
				else {
					sub_output += " | HIT_FLAG_CONTINUE_LAUNCH";
				}
			}
			if (critical_disable_hitstun_parry) {
				if (sub_output == ", ") {
					sub_output += "HIT_FLAG_DISABLE_HITSTUN_PARRY";
				}
				else {
					sub_output += " | HIT_FLAG_DISABLE_HITSTUN_PARRY";
				}
			}
		}
		if (sub_output != ", ") {
			sub_output += "HIT_FLAG_NONE";
		}
		output += sub_output;
	}
	if (collision_kind_air || hit_status == HIT_STATUS_LAUNCH || critical_status == HIT_STATUS_LAUNCH) {
		output += ", /*Juggle Start*/ " + std::to_string(juggle_start);
		if (collision_kind_air) {
			output += ", /*Juggle Increase*/ " + std::to_string(juggle_increase) + ", /*Juggle Max*/ "
				+ std::to_string(juggle_max);
		}
	}
	output += ", HIT_HEIGHT_";
	switch (hit_height) {
		case (HIT_HEIGHT_LOW): {
			output += "LOW";
		} break;
		case (HIT_HEIGHT_MID): {
			output += "MID";
		} break;
		case (HIT_HEIGHT_HIGH): {
			output += "HIGH";
		} break;
		default: {
			output += "MAX";
		} break;
	}
	output += ", /*Damage*/ " + std::to_string(damage);
	if (collision_kind_ground) {
		output += ", /*Chip Damage*/ " + std::to_string(chip_damage);
	}
	output += ", /*Damage Scale*/ " + std::to_string(damage_scale) + ", /*Meter Gain*/ "
		+ std::to_string(meter_gain) + ", /*Hitlag*/ " + std::to_string(hitlag);
	if (collision_kind_ground) {
		output += ", /*Blocklag*/ " + std::to_string(blocklag);
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
			hitstun_frames = hit_advantage - hitlag + projectile->object_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
			blockstun_frames = block_advantage - blocklag + projectile->object_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
		} break;
		default: {
			hitstun_frames = 0;
			blockstun_frames = 0;
		} break;
	}
	output += ", /*Hitstun*/ " + std::to_string(hitstun_frames);
	if (collision_kind_ground) {
		output += ", /*Blockstun*/ " + std::to_string(blockstun_frames);
	}
	if (hit_status == HIT_STATUS_NORMAL || (critical_condition != CRITICAL_CONDITION_NONE
		&& critical_status == HIT_STATUS_NORMAL)) {
		if (collision_kind_ground) {
			output += ", /*Pushback Ground Hit*/ " + std::to_string(pushback_ground_hit)
				+ ", /*Pushback Ground Block*/ " + std::to_string(pushback_ground_block);
		}
		if (collision_kind_air) {
			output += ", /*Pushback Air X*/ " + std::to_string(pushback_air_x)
				+ ", /*Pushback Air Y*/ " + std::to_string(pushback_air_y);
		}
		output += ", /*Pushback Frames*/ " + std::to_string(pushback_frames);
	}
	else if (collision_kind_ground) {
		output += ", /*Pushback Ground Block*/ " + std::to_string(pushback_ground_block)
			+ ", /*Pushback Frames*/ " + std::to_string(pushback_frames);
	}
	if (hit_status == HIT_STATUS_LAUNCH || hit_status == HIT_STATUS_FLOAT
		|| (critical_condition != CRITICAL_CONDITION_NONE && (critical_status == HIT_STATUS_LAUNCH 
		|| critical_status == HIT_STATUS_FLOAT)) || continue_launch) {
		if (has_launch_target_pos) {
			output += ", /*Launch Target Pos*/ " + glm::to_string(launch_target_pos);
		}
		else {
			output += ", /*Launch Init Y*/ " + std::to_string(launch_init_y) 
				+ ", /*Launch Gravity*/ " + std::to_string(launch_gravity_y) 
				+ ", /*Launch Max Fall Speed*/ " + std::to_string(launch_max_fall_speed) 
				+ ", /*Launch Speed X*/ " + std::to_string(launch_speed_x);
		}
	}
	output += ", DAMAGE_KIND_";
	switch (damage_kind) {
		case (DAMAGE_KIND_NORMAL): {
			output += "NORMAL";
		} break;
		case (DAMAGE_KIND_CHIP): {
			output += "CHIP";
		} break;
		case (DAMAGE_KIND_CHIP_KO): {
			output += "CHIP_KO";
		} break;
		case (DAMAGE_KIND_NO_KO): {
			output += "NO_KO";
		} break;
		default: {
			output += "MAX";
		} break;
	}
	output += ", /*Hit Effect*/ \"" + hit_effect
		+ "\", /*Hit Sound*/ \"" + hit_sound + "\");";

	std::cout << output << "\n";
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
	armor_hits = 0;
	intangible_kind_high = false;
	intangible_kind_mid = false;
	intangible_kind_low = false;
	intangible_kind_aerial = false;
	intangible_kind_throw = false;
	intangible_kind_projectile = false;
	intangible_kind_invincible = false;
	intangible_kind_soft = false;
	rect.set_colormod(glm::vec3(0.0, 0.0, 255.0)).set_alpha(127);
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

	std::string armor_text = ", " + armor_hits;

	std::string intangible_text = ", INTANGIBLE_KIND_NONE";
	if (intangible_kind_high) {
		intangible_text = ", INTANGIBLE_KIND_HIGH";
	}
	if (intangible_kind_mid) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_MID";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_MID";
		}
	}
	if (intangible_kind_low) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_LOW";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_LOW";
		}
	}
	if (intangible_kind_aerial) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_AERIAL";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_AERIAL";
		}
	}
	if (intangible_kind_throw) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_THROW";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_THROW";
		}
	}
	if (intangible_kind_projectile) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_PROJECTILE";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_PROJECTILE";
		}
	}
	if (intangible_kind_invincible) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_INVINCIBLE";
		}
		else {

		}
	}
	if (intangible_kind_soft) {
		if (intangible_text == ", INTANGIBLE_KIND_NONE") {
			intangible_text = ", INTANGIBLE_KIND_SOFT";
		}
		else {
			intangible_text += " | INTANGIBLE_KIND_SOFT";
		}
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
	grabbox_kind_hitstun = false;
	grabbox_kind_notech = false;
	collision_kind_ground = false;
	collision_kind_air = false;
	collision_kind_down = false;
	collision_kind_projectile = false;
	collision_kind_soft_intangible = false;
	collision_kind_armor = false;
	attacker_status = "FIGHTER_STATUS_THROW";
	defender_status = "FIGHTER_STATUS_GRABBED";
	rect.set_colormod(glm::vec3(0.0, 255.0, 0.0)).set_alpha(127);
}

void SimGrabbox::print_start(BattleObject* object) {
	std::string padding = "	push_function(&Fighter::NEW_GRABBOX, ";
	if (object->object_type == BATTLE_OBJECT_TYPE_PROJECTILE) {
		padding = "	push_function(&Projectile::NEW_GRABBOX, ";
	}
	std::string grabbox_text = "GRABBOX_KIND_NORMAL";
	if (grabbox_kind_notech) {
		grabbox_text = "GRABBOX_KIND_NOTECH";
	}
	if (grabbox_kind_hitstun) {
		if (grabbox_text == "GRABBOX_KIND_NORMAL") {
			grabbox_text = "GRABBOX_KIND_HITSTUN";
		}
		else {
			grabbox_text += "| GRABBOX_KIND_HITSTUN";
		}
	}
	std::string collision_kind_text = ", ";
	if (collision_kind_ground) {
		collision_kind_text += "COLLISION_KIND_GROUND";
	}
	if (collision_kind_air) {
		if (collision_kind_text == ", ") {
			collision_kind_text += "COLLISION_KIND_AIR";
		}
		else {
			collision_kind_text += " | COLLISION_KIND_AIR";
		}
	}
	if (collision_kind_down) {
		if (collision_kind_text == ", ") {
			collision_kind_text += "COLLISION_KIND_DOWN";
		}
		else {
			collision_kind_text += " | COLLISION_KIND_DOWN";
		}
	}
	if (collision_kind_projectile) {
		if (collision_kind_text == ", ") {
			collision_kind_text += "COLLISION_KIND_PROJECTILE";
		}
		else {
			collision_kind_text += " | COLLISION_KIND_PROJECTILE";
		}
	}
	if (collision_kind_soft_intangible) {
		if (collision_kind_text == ", ") {
			collision_kind_text += "COLLISION_KIND_SOFT_INTANGIBLE";
		}
		else {
			collision_kind_text += " | COLLISION_KIND_SOFT_INTANGIBLE";
		}
	}
	if (collision_kind_armor) {
		if (collision_kind_text == ", ") {
			collision_kind_text += "COLLISION_KIND_ARMOR";
		}
		else {
			collision_kind_text += " | COLLISION_KIND_ARMOR";
		}
	}
	glm::ivec2 anchor = this->relative_anchor;
	glm::ivec2 offset = this->relative_offset;
	std::cout << padding << id << ", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2("
		<< offset.x << ", " << offset.y << ")" << grabbox_text << collision_kind_text << ", " 
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
	rect.set_colormod(glm::vec3(255.0, 255.0, 0.0)).set_alpha(127);
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