#include "Fighter.h"
#include "Projectile.h"
#include "utils.h"
#include "GameManager.h"

void Fighter::process_fighter_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes) {
	Fighter* that = object_manager->fighter[1];
	float x_diff = object_float[BATTLE_OBJECT_FLOAT_X_SPEED];
	float that_x_diff = that->object_float[BATTLE_OBJECT_FLOAT_X_SPEED];
	bool allow_corner_crossup = object_flag[FIGHTER_FLAG_ALLOW_CORNER_CROSSUP] 
		|| that->object_flag[FIGHTER_FLAG_ALLOW_CORNER_CROSSUP];
	bool strict_wall_pushboxes = true;
	float inter_mul = (get_scale_vec().x + that->get_scale_vec().x) / 2.0f;
	if (get_pos().x == that->get_pos().x && prev_pos.x != that->prev_pos.x) {
		if (get_pos().x <= -stage->stage_bound) {
			if ((prev_pos.x == get_pos().x) == allow_corner_crossup) {
				add_pos(glm::vec3(1, 0, 0));
			}
			else if ((that->prev_pos.x == that->get_pos().x) == allow_corner_crossup) {
				that->add_pos(glm::vec3(1, 0, 0));
			}
		}
		if (get_pos().x >= stage->stage_bound) {
			if ((prev_pos.x == get_pos().x) == allow_corner_crossup) {
				add_pos(glm::vec3(-1, 0, 0));
			}
			else if ((that->prev_pos.x == that->get_pos().x) == allow_corner_crossup) {
				that->add_pos(glm::vec3(-1, 0, 0));
			}
		}
		update_pushbox_pos();
		that->update_pushbox_pos();
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		for (size_t i2 = 0, max2 = that_pushboxes.size(); i2 < max2; i2++) {
			float intersection = get_rect_intersection(pushboxes[i], that->pushboxes[i2]);
			if (intersection == -1.0f || get_scaled_pos().x == that->get_scaled_pos().x) continue;
			
			float dir = get_scaled_pos().x > that->get_scaled_pos().x ? 1.0f : -1.0f;
			bool p1_pos_change = add_pos_validate(glm::vec3(intersection * inter_mul / 2.0f * dir, 0.0f, 0.0f));
			bool p2_pos_change = that->add_pos_validate(glm::vec3(intersection * inter_mul / -2.0f * dir, 0.0f, 0.0f));
			
			if ((!p1_pos_change || !p2_pos_change) && strict_wall_pushboxes) {
				update_pushbox_pos();
				that->update_pushbox_pos();
				bool facing_wall = facing_dir * get_scaled_pos().x > 0.0f;
				bool that_facing_wall = that->facing_dir * that->get_scaled_pos().x > 0.0f;
				float pushbox_front = get_pos().x + (facing_wall
					? get_pushbox_back(i) : get_pushbox_front(i));
				float that_pushbox_front = that->get_pos().x + (that_facing_wall
					? that->get_pushbox_back(i2) : that->get_pushbox_front(i2));
				if (!p1_pos_change) {
					if (that_facing_wall) {
						that->set_pos_validate(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->get_pos().y, that->get_pos().z), true);
					}
					else {
						that->set_pos_validate(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->get_pos().y, that->get_pos().z), true);
					}
				}
				if (!p2_pos_change) {
					if (facing_wall) {
						set_pos_validate(glm::vec3(that_pushbox_front - get_pushbox_front(i), get_pos().y, get_pos().z), true);
					}
					else {
						set_pos_validate(glm::vec3(that_pushbox_front - get_pushbox_back(i), get_pos().y, get_pos().z), true);
					}
				}
				if (!p1_pos_change && !p2_pos_change) std::cout 
					<< "Both players pushed into invalid pos...?\n";
			}
		}
	}
}

void Fighter::process_projectile_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes) {
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		for (size_t i2 = 0, max2 = that_pushboxes.size(); i2 < max2; i2++) {
			if (is_rect_collide(pushboxes[i], that_pushboxes[i2])) {
				Projectile* that = (Projectile*)that_pushboxes[i2].object;
				//Only they are moving.
				float that_x_diff = that->object_float[BATTLE_OBJECT_FLOAT_X_SPEED];
				float pushbox_front = get_scaled_pos().x + get_pushbox_front(i);
				float pushbox_back = get_scaled_pos().x + get_pushbox_back(i);
				float that_pushbox_front = that->get_scaled_pos().x + that->get_pushbox_front(i2);
				float that_pushbox_back = that->get_scaled_pos().x + that->get_pushbox_back(i2);

				if (that_x_diff > 0.0 == that->get_scaled_pos().x < get_scaled_pos().x) {
					bool facing_that = that_x_diff > 0.0 == that->facing_dir > 0.0;
					bool facing_same = facing_dir == that->facing_dir;
					if (facing_that) {
						if (facing_same) {
							set_pos_validate(glm::vec3(that_pushbox_front - get_pushbox_back(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
						else {
							set_pos_validate(glm::vec3(that_pushbox_front - get_pushbox_front(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
					}
					else {
						if (facing_same) {
							set_pos_validate(glm::vec3(that_pushbox_back - get_pushbox_front(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
						else {
							set_pos_validate(glm::vec3(that_pushbox_back - get_pushbox_back(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
					}
				}
				else {  //They're moving away from us
					bool facing_that = that_x_diff > 0.0 != that->facing_dir > 0.0;
					bool facing_same = facing_dir == that->facing_dir;
					if (facing_that) {
						if (facing_same) {
							set_pos_validate(glm::vec3(that_pushbox_back - get_pushbox_front(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
						else {
							set_pos_validate(glm::vec3(that_pushbox_front - get_pushbox_front(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
					}
					else {
						if (facing_same) {
							set_pos_validate(glm::vec3(that_pushbox_back - get_pushbox_front(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
						else {
							set_pos_validate(glm::vec3(that_pushbox_back - get_pushbox_back(i), get_scaled_pos().y, get_scaled_pos().z), true);
						}
					}
				}
			}
		}
	}
}

bool Fighter::is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) {
	if (attacker->object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0
	|| attacker->multihit_connected[hitbox->multihit]) {
		return false;
	}

	switch (hitbox->hit_height) {
		case (HIT_HEIGHT_HIGH): {
			if ((hurtbox->intangible_kind & INTANGIBLE_KIND_HIGH) 
			&& attacker->fighter_context == FIGHTER_CONTEXT_GROUND) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_MID): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_MID) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_LOW): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_LOW) {
				return false;
			}
		} break;
		default: {
			return false;
		}
	}

	if (((hurtbox->intangible_kind & INTANGIBLE_KIND_GROUND) && attacker->fighter_context == FIGHTER_CONTEXT_GROUND)
	||	((hurtbox->intangible_kind & INTANGIBLE_KIND_AERIAL) && attacker->fighter_context == FIGHTER_CONTEXT_AIR)) {
		return false;
	}

	switch (fighter_context) {
		case FIGHTER_CONTEXT_GROUND: {
			if (!(hitbox->collision_kind & COLLISION_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_CONTEXT_AIR: {
			if ((!(hitbox->collision_kind & COLLISION_KIND_AIR))
			|| (hitbox->hit_result.juggle_max != -1 &&
				hitbox->hit_result.juggle_max < object_int[FIGHTER_INT_JUGGLE_VALUE])) {
				return false;
			}
		} break;
		case FIGHTER_CONTEXT_DOWN: {
			if (!(hitbox->collision_kind & COLLISION_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (object_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (object_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->hit_height
			|| object_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			if (status_kind == FIGHTER_STATUS_PARRY_START) {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_PARRIED;
			}
			else {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_HITSTUN_PARRIED;
			}
			return true;
		}
	}


	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_RIGHT_OF_WAY) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED;
		return true;
	}

	if (hurtbox->armor_hits) {
		if ((hitbox->collision_kind & COLLISION_KIND_ARMOR) || object_int[FIGHTER_INT_ARMOR_HITS] == hurtbox->armor_hits) {
			object_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			Scene* battle = GameManager::get_instance()->get_scene();
			SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
			msg.string_var("next_msg", "Armor Break").execute_event("on_hit");
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
		}
		else {
			object_int[FIGHTER_INT_ARMOR_HITS]++;
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
		}
		return true;
	}

	object_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;
	if (fighter_context == FIGHTER_CONTEXT_GROUND && hitbox->hit_result.chip_percent != -1.0) {
		bool reverse_block = ((hitbox->anchor.x > get_scaled_pos().x && hitbox->offset.x > get_scaled_pos().x)
			!= internal_facing_right) && ((attacker->get_scaled_pos().x > get_scaled_pos().x) != internal_facing_right);
		if (object_flag[FIGHTER_FLAG_AUTO_GUARD]) {
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
					object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
				} break;
				case (HIT_HEIGHT_MID): {

				} break;
				case (HIT_HEIGHT_LOW): {
					object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
				} break;
			}
			if (reverse_block) {
				facing_right = !internal_facing_right;
				facing_dir = !internal_facing_dir;
			}
			else {
				facing_right = internal_facing_right;
				facing_dir = internal_facing_dir;
			}
			incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
			return true;
		}
		else if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			if (get_stick_dir() < 4) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
			}
			else {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
			}
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
					if (reverse_block) {
						if (get_stick_dir() == 6) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() >= 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				default:
				case (HIT_HEIGHT_MID): {
					if (reverse_block) {
						if (!get_stick_dir() % 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
					return true;
				} break;
				case (HIT_HEIGHT_LOW): {
					if (reverse_block) {
						if (get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() < 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
			}
		}
		else if (is_actionable()) {
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
					if (reverse_block) {
						if (get_stick_dir() == 6) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				default:
				case (HIT_HEIGHT_MID): {
					if (reverse_block) {
						if (get_stick_dir() == 6 || get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4 || get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				case (HIT_HEIGHT_LOW): {
					if (reverse_block) {
						if (get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}

					}
				} break;
			}
		}
	}

	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

bool Fighter::is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) {
	if (attacker->object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0
		|| attacker->multihit_connected[hitbox->multihit]) {
		return false;
	}

	switch (hitbox->hit_height) {
		case (HIT_HEIGHT_HIGH): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_HIGH) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_MID): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_MID) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_LOW): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_LOW) {
				return false;
			}
		} break;
		default: {
			return false;
		}
	}

	if (hurtbox->intangible_kind & INTANGIBLE_KIND_PROJECTILE) {
		return false;
	}

	switch (fighter_context) {
		case FIGHTER_CONTEXT_GROUND: {
			if (!(hitbox->collision_kind & COLLISION_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_CONTEXT_AIR: {
			if ((!(hitbox->collision_kind & COLLISION_KIND_AIR))
				|| (hitbox->hit_result.juggle_max != -1 &&
					hitbox->hit_result.juggle_max < object_int[FIGHTER_INT_JUGGLE_VALUE])) {
				return false;
			}
		} break;
		case FIGHTER_CONTEXT_DOWN: {
			if (!(hitbox->collision_kind & COLLISION_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (object_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (object_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->hit_height
			|| object_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			if (status_kind == FIGHTER_STATUS_PARRY_START) {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_PARRIED;
			}
			else {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_HITSTUN_PARRIED;
			}
			return true;
		}
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_RIGHT_OF_WAY) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED;
		return true;
	}

	if (hurtbox->armor_hits) {
		if ((hitbox->collision_kind & COLLISION_KIND_ARMOR) || object_int[FIGHTER_INT_ARMOR_HITS] == hurtbox->armor_hits) {
			object_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			Scene* battle = GameManager::get_instance()->get_scene();
			SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->owner_id + 1) + " Message");
			msg.string_var("next_msg", "Armor Break").execute_event("on_hit");
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
		}
		else {
			object_int[FIGHTER_INT_ARMOR_HITS]++;
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
		}
		return true;
	}

	object_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;

	if (fighter_context == FIGHTER_CONTEXT_GROUND && hitbox->hit_result.chip_percent != -1.0) {
		bool reverse_block = ((hitbox->anchor.x > get_scaled_pos().x && hitbox->offset.x > get_scaled_pos().x)
			!= internal_facing_right) && ((attacker->get_scaled_pos().x > get_scaled_pos().x) != internal_facing_right);
		if (object_flag[FIGHTER_FLAG_AUTO_GUARD]) {
			switch (hitbox->hit_height) {
			case (HIT_HEIGHT_HIGH): {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
			} break;
			case (HIT_HEIGHT_MID): {

			} break;
			case (HIT_HEIGHT_LOW): {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
			} break;
			}
			if (reverse_block) {
				facing_right = !internal_facing_right;
				facing_dir = !internal_facing_dir;
			}
			else {
				facing_right = internal_facing_right;
				facing_dir = internal_facing_dir;
			}
			incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
			return true;
		}
		else if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			if (get_stick_dir() < 4) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
			}
			else {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
			}
			switch (hitbox->hit_height) {
			case (HIT_HEIGHT_HIGH): {
				if (reverse_block) {
					if (get_stick_dir() == 6) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() >= 4) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			default:
			case (HIT_HEIGHT_MID): {
				if (reverse_block) {
					if (!get_stick_dir() % 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
					facing_right = internal_facing_right;
					facing_dir = internal_facing_dir;
					return true;
				}
				return true;
			} break;
			case (HIT_HEIGHT_LOW): {
				if (reverse_block) {
					if (get_stick_dir() == 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() < 4) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			}
		}
		else if (is_actionable()) {
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
					if (reverse_block) {
						if (get_stick_dir() == 6) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				default:
				case (HIT_HEIGHT_MID): {
					if (reverse_block) {
						if (get_stick_dir() == 6 || get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4 || get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				case (HIT_HEIGHT_LOW): {
					if (reverse_block) {
						if (get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !facing_right;
							facing_dir = !facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
			}
		}
	}

	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

void Fighter::process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) {
	object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	object_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = hitbox->hit_flags & HIT_FLAG_DISABLE_HITSTUN_PARRY;
	if (object_int[FIGHTER_INT_JUGGLE_VALUE] < hitbox->hit_result.juggle_min) {
		object_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->hit_result.juggle_min;
	}
	else if (!object_int[FIGHTER_INT_JUGGLE_VALUE]) {
		object_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->hit_result.juggle_start;
	}
	else {
		object_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->hit_result.juggle_increase;
	}
	float damage;
	int counterhit_val = COUNTERHIT_VAL_NONE;
	if (attacker->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	switch (counterhit_val) {
		case (COUNTERHIT_VAL_CRIT_STATUS): {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.2f;
			damage = hitbox->hit_result.base_damage * 1.2f;
			attacker->gain_ex(hitbox->hit_result.meter_gain * 1.5f);
			gain_ex(hitbox->hit_result.meter_gain * 0.9f);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.crit_hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.crit_hitstun;
		} break;
		case (COUNTERHIT_VAL_PUNISH): 
		case (COUNTERHIT_VAL_COUNTER): {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.1f;
			damage = hitbox->hit_result.base_damage * object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
			attacker->gain_ex(hitbox->hit_result.meter_gain * 1.2f);
			gain_ex(hitbox->hit_result.meter_gain * 0.72f);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.hitstun + (3 * counterhit_val);
		} break;
		default:
		case (COUNTERHIT_VAL_NONE): {
			if (attacker->object_flag[FIGHTER_FLAG_ATTACK_HIT]) {
				damage = hitbox->hit_result.base_damage * object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE];
			}
			else {
				damage = hitbox->hit_result.base_damage * object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
			}
			attacker->gain_ex(hitbox->hit_result.meter_gain);
			gain_ex(hitbox->hit_result.meter_gain * 0.6);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.hitstun;
		} break;
	}

	if (!attacker->object_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		if (!(object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN)
		&& counterhit_val == COUNTERHIT_VAL_NONE) {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = hitbox->hit_result.damage_scale_combo_start;
		}
		else {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] -= hitbox->hit_result.damage_scale_add;
		}
	}
	object_float[FIGHTER_FLOAT_DAMAGE_UI_TRAINING] = damage;
	object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI_TRAINING] = object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] * 100.0f;
	attacker->object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING] += damage;
	set_post_collision_status(hitbox, counterhit_val);

	object_flag[FIGHTER_FLAG_HIT_BY_EX_SUPER] = attacker->object_flag[BATTLE_OBJECT_FLAG_ACTIVE_EX_SUPER];
	object_flag[FIGHTER_FLAG_HIT_BY_CHAMPION_SUPER] = attacker->object_flag[BATTLE_OBJECT_FLAG_ACTIVE_CHAMPION_SUPER];
	object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = -1;

	unique_process_incoming_fighter_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hit(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {
	object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	object_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = hitbox->hit_flags & HIT_FLAG_DISABLE_HITSTUN_PARRY;
	if (object_int[FIGHTER_INT_JUGGLE_VALUE] < hitbox->hit_result.juggle_min) {
		object_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->hit_result.juggle_min;
	}
	else if (!object_int[FIGHTER_INT_JUGGLE_VALUE]) {
		object_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->hit_result.juggle_start;
	}
	else {
		object_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->hit_result.juggle_increase;
	}
	float damage;
	int counterhit_val = COUNTERHIT_VAL_NONE;
	if (attacker->owner->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->owner->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->owner->object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	switch (counterhit_val) {
		case (COUNTERHIT_VAL_CRIT_STATUS): {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.2f;
			damage = hitbox->hit_result.base_damage * 1.2f;
			attacker->owner->gain_ex(hitbox->hit_result.meter_gain * 1.5f);
			gain_ex(hitbox->hit_result.meter_gain * 0.9f);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.crit_hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.crit_hitstun;
		} break;
		case (COUNTERHIT_VAL_PUNISH):
		case (COUNTERHIT_VAL_COUNTER): {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.1f;
			damage = hitbox->hit_result.base_damage * 1.1f;
			attacker->owner->gain_ex(hitbox->hit_result.meter_gain * 1.2f);
			gain_ex(hitbox->hit_result.meter_gain * 0.72f);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.hitstun + (3 * counterhit_val);
		} break;
		default:
		case (COUNTERHIT_VAL_NONE): {
			if (attacker->object_flag[PROJECTILE_FLAG_ATTACK_HIT]) {
				damage = hitbox->hit_result.base_damage * object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE];
			}
			else {
				damage = hitbox->hit_result.base_damage * object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
			}
			attacker->owner->gain_ex(hitbox->hit_result.meter_gain);
			gain_ex(hitbox->hit_result.meter_gain * 0.6);
			float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
			object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.hitstun;
		} break;
	}

	if (!attacker->object_flag[PROJECTILE_FLAG_ATTACK_HIT]) {
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		if (!(object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN)
			&& counterhit_val == COUNTERHIT_VAL_NONE) {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = hitbox->hit_result.damage_scale_combo_start;
		}
		else {
			object_float[FIGHTER_FLOAT_DAMAGE_SCALE] -= hitbox->hit_result.damage_scale_add;
		}
	}

	object_float[FIGHTER_FLOAT_DAMAGE_UI_TRAINING] = damage;
	object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI_TRAINING] = object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] * 100.0f;
	attacker->owner->object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING] += damage;
	set_post_collision_status(hitbox, counterhit_val);

	object_flag[FIGHTER_FLAG_HIT_BY_EX_SUPER] = attacker->object_flag[BATTLE_OBJECT_FLAG_ACTIVE_EX_SUPER];
	object_flag[FIGHTER_FLAG_HIT_BY_CHAMPION_SUPER] = attacker->object_flag[BATTLE_OBJECT_FLAG_ACTIVE_CHAMPION_SUPER];
	object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = -1;

	unique_process_incoming_projectile_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hit(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = defender->object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_int[FIGHTER_INT_COMBO_COUNT]++;
	object_flag[FIGHTER_FLAG_ATTACK_HIT] = true;
	play_sound(hitbox->hit_sound, 0.0f);
	unique_process_outgoing_fighter_hitbox_collision_hit(hitbox, defender);
}

void Fighter::process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	unique_process_outgoing_projectile_hitbox_collision_hit(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker) {
	gain_ex(hitbox->hit_result.meter_gain * 0.3);
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.blockstun;
	if (hitbox->hit_move.pushback_frames) {
		object_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->hit_move.pushback_frames;
		object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->hit_move.pushback_block / object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	}
	object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	if (hitbox->damage_kind == DAMAGE_KIND_CHIP) {
		object_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, object_float[FIGHTER_FLOAT_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_HEALTH]);
	}
	else if (hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
		object_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_HEALTH]);
	}
	post_collision_status = FIGHTER_STATUS_BLOCKSTUN;
	switch (object_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
		case (HITSTUN_HEIGHT_STAND): {
			object_string[FIGHTER_STRING_HITSTUN_ANIM] = hitbox->hit_result.stand_block_anim;
		} break;
		case (HITSTUN_HEIGHT_CROUCH): {
			object_string[FIGHTER_STRING_HITSTUN_ANIM] = hitbox->hit_result.crouch_block_anim;
		} break;
	}
	object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	unique_process_incoming_fighter_hitbox_collision_blocked(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_blocked(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(hitbox->hit_result.meter_gain * 0.3);
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hit_result.blockstun;
	if (hitbox->hit_move.pushback_frames) {
		object_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->hit_move.pushback_frames;
		object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->hit_move.pushback_block / object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	}
	object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	if (hitbox->damage_kind == DAMAGE_KIND_CHIP) {
		object_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, object_float[FIGHTER_FLOAT_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_HEALTH]);
	}
	else if (hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
		object_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_HEALTH] - hitbox->hit_result.base_damage * hitbox->hit_result.chip_percent, object_float[FIGHTER_FLOAT_HEALTH]);
	}

	post_collision_status = FIGHTER_STATUS_BLOCKSTUN;	
	switch (object_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
		case (HITSTUN_HEIGHT_STAND): {
			object_string[FIGHTER_STRING_HITSTUN_ANIM] = hitbox->hit_result.stand_block_anim;
		} break;
		case (HITSTUN_HEIGHT_CROUCH): {
			object_string[FIGHTER_STRING_HITSTUN_ANIM] = hitbox->hit_result.crouch_block_anim;
		} break;
	}
	object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	unique_process_incoming_projectile_hitbox_collision_blocked(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_blocked(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(hitbox->hit_result.meter_gain * 0.5);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = true;
//	play_sound("common_attack_blocked", 0.0f); TODO: Add this sound
	unique_process_outgoing_fighter_hitbox_collision_blocked(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker) {
	gain_ex(get_param_float("meter_gain_on_parry"));
	if (object_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY]) {
		start_cinematic_sequence("hitstun_parry", 1.0, 0.0, 0.6, false, 0.0067);
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 58;
		object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 58;
//		play_reserved_sound("hitstun_counter_parry", 0.0f);
	}
	else {
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 16;
		object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 16;
	}
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = object_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_parried(hitbox, attacker);
//	play_sound("common_attack_parried", 0.0f);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(get_param_float("meter_gain_on_parry"));
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 16;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 16;
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = object_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_projectile_hitbox_collision_parried(hitbox, attacker);
//	play_sound("common_attack_parried", 0.0f);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(get_param_float("meter_gain_on_parry") * 0.5);
	if (defender->object_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY]) {
		object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 1;
		object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 1;
	}
	else {
		object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 16;
		object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 16;
	}
	unique_process_outgoing_fighter_hitbox_collision_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker) {
	gain_ex(get_param_float("meter_gain_on_parry"));
	start_cinematic_sequence("hitstun_parry", 1.0, 0.0, 0.6, false, 0.0067);
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 58;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 58;
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = object_float[FIGHTER_FLOAT_HEALTH];
	object_flag[FIGHTER_FLAG_HITSTUN_PARRY] = true;
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_hitstun_parried(hitbox, attacker);
//	play_reserved_sound("hitstun_parry", 0.0f);
//	play_sound("common_attack_parried", 0.0f);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(get_param_float("meter_gain_on_parry"));
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 16;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 16;
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = object_float[FIGHTER_FLOAT_HEALTH];
//	play_sound("common_attack_parried", 0.0f);
	object_flag[FIGHTER_FLAG_HITSTUN_PARRY] = true;
	post_collision_status = FIGHTER_STATUS_PARRY;
	
	unique_process_incoming_projectile_hitbox_collision_hitstun_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(get_param_float("meter_gain_on_parry") * 0.5);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 1;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 1;
	object_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED] = true;
	enable_cancel("parry_start", CANCEL_KIND_ANY);
	unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker) {
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag / 2;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->hit_result.base_damage / 2), object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	unique_process_incoming_fighter_hitbox_collision_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_armored(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_armored(hitbox, attacker);
}

void Fighter::process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag / 2;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	gain_ex(hitbox->hit_result.meter_gain * 0.3);
//	play_sound("common_attack_armored", 0.0f);
	unique_process_outgoing_fighter_hitbox_collision_armored(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker) {
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->hit_result.base_damage / 2), object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	//Set a timer to however long the RoW lockout window is
	//Set a definite hitbox based on this hitbox
	unique_process_incoming_fighter_hitbox_collision_right_of_way_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker) {
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->hit_result.base_damage / 2), object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	unique_process_incoming_projectile_hitbox_collision_right_of_way_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker) {
	unique_process_incoming_fighter_hitbox_collision_invincibility(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_invincibility(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_invincibility(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_invincibility(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag / 2;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_invincibility(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	post_collision_status = object_int[FIGHTER_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_fighter_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_counter(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	post_collision_status = object_int[FIGHTER_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_projectile_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_counter(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	post_collision_status = defender->object_int[FIGHTER_INT_COUNTER_ATTACKER_STATUS];
	unique_process_outgoing_fighter_hitbox_collision_counter(hitbox, defender);
}

void Fighter::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->multihit);
	object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME] = frame;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
	post_collision_status = defender->object_int[PROJECTILE_INT_COUNTER_ATTACKER_STATUS];
	unique_process_outgoing_projectile_hitbox_collision_counter(hitbox, defender);
}

void Fighter::check_incoming_blockbox_collisions(std::vector<Blockbox*> blockboxes) {
	object_flag[FIGHTER_FLAG_PROX_GUARD] = false;
	for (size_t i = 0; i < 10; i++) {
		if (!hurtboxes[i].active) continue;
		for (size_t i2 = 0, max2 = blockboxes.size(); i2 < max2; i2++) {
			if (is_rect_collide(hurtboxes[i], *blockboxes[i2])) {
				object_flag[FIGHTER_FLAG_PROX_GUARD] = true;
				return;
			}
		}
	}
}

void Fighter::check_incoming_grabbox_collisions(std::vector<Grabbox*> grabboxes) {
	for (size_t i = 0; i < 10; i++) {
		if (!hurtboxes[i].active) continue;
		for (size_t i2 = 0, max2 = grabboxes.size(); i2 < max2; i2++) {
			if (is_valid_incoming_grabbox_collision(&hurtboxes[i], grabboxes[i2])) {
				connected_grabbox = grabboxes[i2];
				return;
			}
		}
	}
}

bool Fighter::is_valid_incoming_grabbox_collision(Hurtbox* hurtbox, Grabbox* grabbox) {
	if (!is_rect_collide(*hurtbox, *grabbox)) return false;
	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		grabbox->clear();
		return false;
	}
	if (hurtbox->intangible_kind & INTANGIBLE_KIND_THROW) {
		return false;
	}
	if ((object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN) && !(grabbox->grabbox_kind & GRABBOX_KIND_HITSTUN)) {
		return false;
	}
	switch (fighter_context) {
		case FIGHTER_CONTEXT_GROUND: {
			return grabbox->collision_kind & COLLISION_KIND_GROUND;
		} break;
		case FIGHTER_CONTEXT_AIR: {
			return grabbox->collision_kind & COLLISION_KIND_AIR;
		} break;
		case FIGHTER_CONTEXT_DOWN: {
			return grabbox->collision_kind & COLLISION_KIND_DOWN;
		} break;
		default: {
			return false;
		} break;
	}
}

void Fighter::process_incoming_grabbox_collision(Grabbox* grabbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_grabbox_collision(grabbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_grabbox_collision(grabbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void Fighter::process_incoming_fighter_grabbox_collision(Grabbox* grabbox, Fighter* attacker) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && object_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		object_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = (grabbox->grabbox_kind & GRABBOX_KIND_NOTECH);
		post_collision_status = grabbox->defender_status_if_hit;
		object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	}
	unique_process_incoming_fighter_grabbox_collision(grabbox, attacker);
	attacker->process_outgoing_fighter_grabbox_collision(grabbox, this);
}

void Fighter::process_incoming_projectile_grabbox_collision(Grabbox* grabbox, Projectile* attacker) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && object_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		object_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = (grabbox->grabbox_kind & GRABBOX_KIND_NOTECH);
		post_collision_status = grabbox->defender_status_if_hit;
		object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	}
	unique_process_incoming_projectile_grabbox_collision(grabbox, attacker);
	attacker->process_outgoing_fighter_grabbox_collision(grabbox, this);
}

void Fighter::process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && defender->object_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		post_collision_status = grabbox->attacker_status_if_hit;
	}
	unique_process_outgoing_fighter_grabbox_collision(grabbox, defender);
}

void Fighter::process_definite_hitbox_activated(DefiniteHitbox* hitbox, Fighter* attacker) {
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = hitbox->hitlag;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = hitbox->hitstun;
	object_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	object_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = hitbox->hit_flags & HIT_FLAG_DISABLE_HITSTUN_PARRY;
	if (!object_int[FIGHTER_INT_JUGGLE_VALUE]) {
		object_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
	}
	else {
		object_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->juggle_increase;
	}
	float damage;
	if (attacker->object_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		damage = hitbox->damage * object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE];
	}
	else {
		damage = hitbox->damage * object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
	}
	attacker->gain_ex(hitbox->meter_gain);
	gain_ex(hitbox->meter_gain * 0.6);
	float accum_damage = object_float[FIGHTER_FLOAT_HEALTH] - object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
	object_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	damage += accum_damage;

	if (!attacker->object_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] = object_float[FIGHTER_FLOAT_DAMAGE_SCALE];
		object_float[FIGHTER_FLOAT_DAMAGE_SCALE] -= hitbox->damage_scale;
	}
	object_float[FIGHTER_FLOAT_DAMAGE_UI_TRAINING] = damage;
	object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI_TRAINING] = object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] * 100.0f;
	attacker->object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING] += damage;

	HitFlag hit_flags = hitbox->hit_flags;
	HitMove hit_move = hitbox->hit_move;
	if ((hit_flags & HIT_FLAG_FORCE_AERIAL) == HIT_FLAG_FORCE_AERIAL) {
		change_context(FIGHTER_CONTEXT_AIR);
	}
	object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0f;
	if (hit_move.pushback_frames) {
		object_int[FIGHTER_INT_PUSHBACK_FRAMES] = hit_move.pushback_frames;
		object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hit_move.pushback_hit / object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	}
	switch (hitbox->hit_status) {
		case (FIGHTER_STATUS_HITSTUN): {
			if (hit_flags & HIT_FLAG_FORCE_STAND) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
			}
			else if (hit_flags & HIT_FLAG_FORCE_CROUCH) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
			}
			object_string[FIGHTER_STRING_HITSTUN_ANIM] = hitbox->hit_anim;
			object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		} break;
		case (FIGHTER_STATUS_THROWN):
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH): {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = hit_move.air_x * attacker->facing_dir;
			object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = hit_move.air_init_y;
			object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = hit_move.air_gravity;
			object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = hit_move.air_max_fall;
		} [[fallthrough]];
		case (FIGHTER_STATUS_HITSTUN_FLOAT):
		case (FIGHTER_STATUS_KNOCKDOWN_START):
		case (FIGHTER_STATUS_KNOCKDOWN):
		case (FIGHTER_STATUS_CRUMPLE): {
			object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = hit_flags & HIT_FLAG_HARD_KNOCKDOWN;
			object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = hit_flags & HIT_FLAG_KNOCKDOWN_FACE_DOWN;
		} break;
		default: {

		} break;
	}
	if (hit_move.target_frames) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = (hit_move.target_x - get_scaled_pos().x) / hit_move.target_frames;
		object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = (hit_move.target_y - get_scaled_pos().y) / hit_move.target_frames;
	}

	object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;

	if (object_float[FIGHTER_FLOAT_HEALTH] == 0.0f 
		&& GameManager::get_instance()->get_scene()->context != SCENE_CONTEXT_TRAINING) {
		if (hitbox->hit_flags & HIT_FLAG_CRUMPLE_ON_KO) {
			post_collision_status = FIGHTER_STATUS_CRUMPLE;
		}
		else {
			switch (fighter_context) {
			case (FIGHTER_CONTEXT_GROUND): {
				if (hitbox->hit_status == HIT_STATUS_LAUNCH) {
					post_collision_status = FIGHTER_STATUS_LAUNCH_START;
				}
				else {
					post_collision_status = FIGHTER_STATUS_KNOCKDOWN_START;
				}
			} break;
			case (FIGHTER_CONTEXT_AIR): {
				if (hitbox->hit_move.air_gravity == 0.0f
					&& hitbox->hit_move.air_init_y == 0.0f
					&& hitbox->hit_move.air_max_fall == 0.0f
					&& hitbox->hit_move.air_x == 0.0f) {
					object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 10.0f * attacker->facing_dir;
					object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 15.0f;
					object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = 1.5f;
					object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 15.0f;
				}
				post_collision_status = FIGHTER_STATUS_LAUNCH;
			} break;
			case (FIGHTER_CONTEXT_DOWN): {
				post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
			} break;
			}
		}
	}
	else {
		post_collision_status = hitbox->hit_status;
	}
	hitbox->clear();
	object_flag[FIGHTER_FLAG_APPLIED_DEFINITE_HITBOX] = true;
}

int Fighter::get_counterhit_val(Hitbox* hitbox) {
	Scene* battle = GameManager::get_instance()->get_scene();
	Fighter* attacker = nullptr;
	if (hitbox->object->object_type == BATTLE_OBJECT_TYPE_FIGHTER) {
		attacker = (Fighter*)hitbox->object;
	}
	else {
		attacker = (Fighter*)((Projectile*)hitbox->object)->owner;
	}
	if (object_flag[FIGHTER_FLAG_FORCE_CRITICAL]) {
		SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
		msg.string_var("next_msg", "Critical").execute_event("on_hit");
		return COUNTERHIT_VAL_CRIT_STATUS;
	}
	if (object_flag[FIGHTER_FLAG_ENABLE_DODGE_COUNTER]) { 
		if (hitbox->critical_condition & CRITICAL_CONDITION_DODGE_COUNTER) {
			SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
			msg.string_var("next_msg", "Critical").execute_event("on_hit");
			return COUNTERHIT_VAL_CRIT_STATUS;
		}
		//NOTE: If a move doesn't crit on dodge dounter, getting hit while dodge counter is enabled will
		//still result in a regular counterhit, but if a move is designed to crit on regular counter, it
		//will NOT crit. This is a design choice, not an oversight: Moves that crit on counterhit are
		//meant to frame trap people mashing offensive options out of otherwise fake pressure, while
		//moves that crit on dodge counter are meant to punish people for picking the wrong defensive
		//option during real pressure sequences/wakeup
		SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
		msg.string_var("next_msg", "Counter").execute_event("on_hit");
		return COUNTERHIT_VAL_COUNTER;
	}
	if (object_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
		if (hitbox->critical_condition & CRITICAL_CONDITION_PUNISH) {
			SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
			msg.string_var("next_msg", "Critical Punish").execute_event("on_hit");
			return COUNTERHIT_VAL_CRIT_STATUS;
		}
		SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
		msg.string_var("next_msg", "Punish").execute_event("on_hit");
		return COUNTERHIT_VAL_PUNISH;
	}
	if (object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
		if (hitbox->critical_condition & CRITICAL_CONDITION_COUNTERHIT) {
			SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
			msg.string_var("next_msg", "Critical Counter").execute_event("on_hit");
			return COUNTERHIT_VAL_CRIT_STATUS;
		}
		SceneElement& msg = battle->get_element("root/Battle UI/P" + std::to_string(attacker->id + 1) + " Message");
		msg.string_var("next_msg", "Counter").execute_event("on_hit");
		return COUNTERHIT_VAL_COUNTER;
	}

	return COUNTERHIT_VAL_NONE;
}

void Fighter::set_post_collision_status(Hitbox* hitbox, int counterhit_val) {
	BattleObject* attacker = nullptr;
	if (hitbox->object->object_type == BATTLE_OBJECT_TYPE_FIGHTER) {
		attacker = (Fighter*)hitbox->object;
	}
	else {
		attacker = (Projectile*)hitbox->object;
	}
	int hit_status = hitbox->hit_status;
	HitMove hit_move = hitbox->hit_move;
	HitFlag hit_flags = hitbox->hit_flags;
	std::string stand_anim = hitbox->hit_result.stand_hit_anim;
	std::string crouch_anim = hitbox->hit_result.crouch_hit_anim;
	if (counterhit_val == COUNTERHIT_VAL_CRIT_STATUS) {
		hit_status = hitbox->critical_status;
		hit_move = hitbox->critical_hit_move;
		hit_flags = hitbox->critical_hit_flags;
		stand_anim = "hitstun_stand_critical";
		crouch_anim = "hitstun_crouch_critical";
	}
	if (object_float[FIGHTER_FLOAT_HEALTH] == 0.0f &&
		GameManager::get_instance()->get_scene()->context != SCENE_CONTEXT_TRAINING) {
		if (hit_flags & HIT_FLAG_CRUMPLE_ON_KO) {
			post_collision_status = FIGHTER_STATUS_CRUMPLE;
		}
		else {
			switch (fighter_context) {
				case (FIGHTER_CONTEXT_GROUND): {
					if (hit_status == HIT_STATUS_LAUNCH) {
						post_collision_status = FIGHTER_STATUS_LAUNCH_START;
					}
					else {
						post_collision_status = FIGHTER_STATUS_KNOCKDOWN_START;
					}
				} break;
				case (FIGHTER_CONTEXT_AIR): {
					post_collision_status = FIGHTER_STATUS_LAUNCH;
				} break;
				case (FIGHTER_CONTEXT_DOWN): {
					post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
				} break;
			}
		}
	}
	else if (hit_flags & HIT_FLAG_CONTINUE_LAUNCH && (status_kind == FIGHTER_STATUS_LAUNCH
		|| status_kind == FIGHTER_STATUS_HITSTUN_FLOAT)) {
		post_collision_status = FIGHTER_STATUS_LAUNCH;
	}
	else {
		switch (hit_status) {
			case (HIT_STATUS_NORMAL): {
				if ((hit_flags & HIT_FLAG_FORCE_AERIAL) == HIT_FLAG_FORCE_AERIAL) {
					change_context(FIGHTER_CONTEXT_AIR);
				}
				switch (fighter_context) {
					case (FIGHTER_CONTEXT_AIR): {
						post_collision_status = FIGHTER_STATUS_HITSTUN_AIR;
					} break;
					case (FIGHTER_CONTEXT_GROUND): {
						post_collision_status = FIGHTER_STATUS_HITSTUN;
					} break;
					case (FIGHTER_CONTEXT_DOWN): {
						post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
					} break;
				}
			} break;
			case (HIT_STATUS_CRUMPLE): {
				switch (fighter_context) {
					case (FIGHTER_CONTEXT_AIR): {
						post_collision_status = FIGHTER_STATUS_LAUNCH;
					} break;
					case (FIGHTER_CONTEXT_GROUND): {
						post_collision_status = FIGHTER_STATUS_CRUMPLE;
					} break;
					case (FIGHTER_CONTEXT_DOWN): {
						post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
					} break;
				}
			} break;
			case (HIT_STATUS_FLOAT): {
				if (fighter_context == FIGHTER_CONTEXT_AIR) {
					post_collision_status = FIGHTER_STATUS_LAUNCH;
					object_flag[FIGHTER_FLAG_LAUNCH_FLOAT] = true;
					object_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = true;
				}
				else {
					post_collision_status = FIGHTER_STATUS_HITSTUN_FLOAT;
				}
			} break;
			case (HIT_STATUS_LAUNCH): {
				if (fighter_context == FIGHTER_CONTEXT_AIR) {
					post_collision_status = FIGHTER_STATUS_LAUNCH;
				}
				else {
					post_collision_status = FIGHTER_STATUS_LAUNCH_START;
				}
			} break;
			case (HIT_STATUS_KNOCKDOWN): {
				if (fighter_context == FIGHTER_CONTEXT_DOWN) {
					post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
				}
				else {
					post_collision_status = FIGHTER_STATUS_KNOCKDOWN_START;
				}
			} break;
			case (HIT_STATUS_CUSTOM): {
				if (counterhit_val == COUNTERHIT_VAL_CRIT_STATUS) {
					post_collision_status = hitbox->custom_critical_status;
				}
				else {
					post_collision_status = hitbox->custom_hit_status;
				}
			} break;
			default:
			case (HIT_STATUS_NONE): {

			} break;
		}
	}
	object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = 0.0f;
	if (hit_move.pushback_frames) {
		object_int[FIGHTER_INT_PUSHBACK_FRAMES] = hit_move.pushback_frames;
		object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hit_move.pushback_hit / object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	}
	switch (post_collision_status) {
		case (FIGHTER_STATUS_HITSTUN): {
			if (hit_flags & HIT_FLAG_FORCE_STAND) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
			}
			else if (hit_flags & HIT_FLAG_FORCE_CROUCH) {
				object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
			}
			switch (object_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
				case (HITSTUN_HEIGHT_STAND): {
					object_string[FIGHTER_STRING_HITSTUN_ANIM] = stand_anim;
				} break;
				case (HITSTUN_HEIGHT_CROUCH): {
					object_string[FIGHTER_STRING_HITSTUN_ANIM] = crouch_anim;
				} break;
			}

			object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		} break;
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH): {
			if (hit_move.air_gravity == 0.0f
			&& hit_move.air_init_y == 0.0f
			&& hit_move.air_max_fall == 0.0f
			&& hit_move.air_x == 0.0f) {
				object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 10.0f * attacker->facing_dir;
				object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 15.0f;
				object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = 1.5f;
				object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 15.0f;
			}
			else {
				object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = hit_move.air_x * attacker->facing_dir;
				object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = hit_move.air_init_y;
				object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = hit_move.air_gravity;
				object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = hit_move.air_max_fall;
			}
		} [[fallthrough]];
		case (FIGHTER_STATUS_HITSTUN_FLOAT):
		case (FIGHTER_STATUS_KNOCKDOWN_START):
		case (FIGHTER_STATUS_KNOCKDOWN):
		case (FIGHTER_STATUS_CRUMPLE): {
			object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = hit_flags & HIT_FLAG_HARD_KNOCKDOWN;
			object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = hit_flags & HIT_FLAG_KNOCKDOWN_FACE_DOWN;
		} break;
		default: {

		} break;
	}
	if (hit_move.target_frames) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = (hit_move.target_x - get_scaled_pos().x) / hit_move.target_frames;
		object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = (hit_move.target_y - get_scaled_pos().y) / hit_move.target_frames;
	}
}