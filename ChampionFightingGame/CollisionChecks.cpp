#include "Battle.h"
#include "ParamAccessor.h"
#include "utils.h"

void Battle::check_collisions() {
	if (frame_pause && !debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE)) {
		return;
	}
	std::vector<Hitbox*> hitboxes[2];
	std::vector<Grabbox*> grabboxes[2];
	std::vector<Pushbox> fighter_pushboxes[2];
	std::vector<Pushbox> projectile_pushboxes[2];
	std::vector<BattleObject*> objects[2];

	//Get a list of every active hit, grab, and pushbox. Specifically for pushboxes, we should
	//also sort by whether or not we're working with a projectile, as the fighter pushbox code
	//only runs for P1, while the projectile pushbox code runs for both.

	for (int i = 0; i < 2; i++) {
		objects[i].push_back(fighter[i]);
		for (int i2 = 0; i2 < 10; i2++) {
			if (fighter[i]->hitboxes[i2].active) {
				hitboxes[i].push_back(&fighter[i]->hitboxes[i2]);
			}
			if (fighter[i]->grabboxes[i2].active) {
				grabboxes[i].push_back(&fighter[i]->grabboxes[i2]);
			}
			if (fighter[i]->pushboxes[i2].active) {
				fighter_pushboxes[i].push_back(fighter[i]->pushboxes[i2]);
			}
		}
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (!fighter[i]->projectiles[i2]->active) continue;
			objects[i].push_back(fighter[i]->projectiles[i2]);
			for (int i3 = 0; i3 < 10; i3++) {
				if (fighter[i]->projectiles[i2]->hitboxes[i3].active) {
					hitboxes[i].push_back(&fighter[i]->projectiles[i2]->hitboxes[i3]);
				}
				if (fighter[i]->projectiles[i2]->grabboxes[i3].active) {
					grabboxes[i].push_back(&fighter[i]->projectiles[i2]->grabboxes[i3]);
				}
				if (fighter[i]->projectiles[i2]->pushboxes[i3].active) {
					projectile_pushboxes[i].push_back(fighter[i]->projectiles[i2]->pushboxes[i3]);
				}
			}
		}
	}

	//First we check any pushbox collisions between P1 and P2. Then we check for collisions between
	//their projectiles.

	fighter[0]->process_fighter_pushbox_collisions(fighter_pushboxes[0], fighter_pushboxes[1]);
	for (int i = 0; i < 2; i++) {
		fighter[i]->process_projectile_pushbox_collisions(fighter_pushboxes[i], projectile_pushboxes[!i]);
		fighter[i]->update_hitbox_pos();
		fighter[i]->update_hurtbox_pos();
		fighter[i]->update_grabbox_pos();
		fighter[i]->update_pushbox_pos();
		fighter[i]->update_blockbox_pos();
	}

	for (int i = 0; i < 2; i++) {
		fighter[i]->check_incoming_grabbox_collisions(grabboxes[!i]);
		for (int i2 = 0; i2 < objects[i].size(); i2++) {
			objects[i][i2]->check_incoming_hitbox_collisions(hitboxes[!i]);
		}
	}
	//If both fighters get grabbed on the same frame, we need to resolve that in some way:
	// - If one is techable and the other one isn't, the untechable one wins. 
	// - If one came from a projectile and one didn't, the projectile wins. 
	// - If all other factors are equal, both grabboxes are deactivated, and if a throw tech is
	//	possible, both fighters change their status to it.
	if (fighter[0]->connected_grabbox != nullptr && fighter[1]->connected_grabbox != nullptr) {
		if ((fighter[0]->connected_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH) == 
			(fighter[1]->connected_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) {
			if (fighter[0]->connected_grabbox->object->object_type > 
				fighter[1]->connected_grabbox->object->object_type) {
				fighter[1]->connected_grabbox = nullptr;
			}
			else if (fighter[0]->connected_grabbox->object->object_type < 
				fighter[1]->connected_grabbox->object->object_type) {
				fighter[0]->connected_grabbox = nullptr;
			}
			else {
				fighter[0]->connected_grabbox->object->clear_grabbox_all();
				fighter[1]->connected_grabbox->object->clear_grabbox_all();
				if (!(fighter[0]->connected_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) {
					fighter[0]->post_collision_status = FIGHTER_STATUS_THROW_TECH;
					fighter[1]->post_collision_status = FIGHTER_STATUS_THROW_TECH;
				}
				fighter[0]->connected_grabbox = nullptr;
				fighter[1]->connected_grabbox = nullptr;
			}
		}
		else if (fighter[0]->connected_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH) {
			fighter[1]->connected_grabbox = nullptr;
		}
		else {
			fighter[0]->connected_grabbox = nullptr;
		}
	}
	//If both fighters hit each other on the same frame, we resolve this based on the attack levels
	//and then, if they match, the clank kinds.
	else if (fighter[0]->connected_hitbox != nullptr && fighter[0]->connected_hitbox->object == fighter[1]
		&& fighter[1]->connected_hitbox != nullptr && fighter[1]->connected_hitbox->object == fighter[0]) {		
		if (fighter[0]->connected_hitbox->attack_level == fighter[1]->connected_hitbox->attack_level) {
			if (fighter[0]->connected_hitbox->clank_kind == CLANK_KIND_CONTINUE ||
				fighter[1]->connected_hitbox->clank_kind == CLANK_KIND_CONTINUE) {
				if (fighter[0]->connected_hitbox->clank_kind == fighter[1]->connected_hitbox->clank_kind) {
					fighter[0]->update_hitbox_connect(fighter[1]->connected_hitbox->multihit);
					fighter[1]->update_hitbox_connect(fighter[0]->connected_hitbox->multihit);
					fighter[0]->connected_hitbox = nullptr;
					fighter[1]->connected_hitbox = nullptr;
				}
				else if (fighter[0]->connected_hitbox->clank_kind == CLANK_KIND_CONTINUE) {
					fighter[1]->update_hitbox_connect(fighter[0]->connected_hitbox->multihit);
					fighter[0]->post_collision_status = FIGHTER_STATUS_CLANK;
					fighter[0]->connected_hitbox = nullptr;
					fighter[1]->connected_hitbox = nullptr;
				}
				else {
					fighter[0]->update_hitbox_connect(fighter[1]->connected_hitbox->multihit);
					fighter[1]->post_collision_status = FIGHTER_STATUS_CLANK;
					fighter[0]->connected_hitbox = nullptr;
					fighter[1]->connected_hitbox = nullptr;
				}
			}
			else if (fighter[0]->connected_hitbox->clank_kind == CLANK_KIND_CLANK ||
					fighter[1]->connected_hitbox->clank_kind == CLANK_KIND_CLANK) {
				fighter[0]->change_status(FIGHTER_STATUS_CLANK);
				fighter[1]->change_status(FIGHTER_STATUS_CLANK);
				fighter[0]->connected_hitbox = nullptr;
				fighter[1]->connected_hitbox = nullptr;
			}
		}
		else if (fighter[0]->connected_hitbox->attack_level > fighter[1]->connected_hitbox->attack_level) {
			fighter[1]->connected_hitbox = nullptr;
		}
		else {
			fighter[0]->connected_hitbox = nullptr;
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < objects[i].size(); i2++) {
			if (objects[i][i2]->connected_grabbox == nullptr) {
				if (objects[i][i2]->connected_hitbox != nullptr) {
					objects[i][i2]->process_incoming_hitbox_collision(
						objects[i][i2]->connected_hitbox,
						objects[i][i2]->connected_hitbox->object
					);
				}
			}
			else {
				//This is kind of a degenerate way of handling grabbox collisions. Essentially,
				//only fighters can get grabbed, which means that:
				// 
				//	A. The incoming grabbox collision doesn't even exist for things that aren't 
				//	fighters
				//	
				//	B. If an object has a connected grabbox, we know for a fact that it is a fighter
				//	so we can access the fighter directly rather than using our object vector.

				//you may not like it, but this is what peak performance looks like
				fighter[i]->process_incoming_grabbox_collision(
					fighter[i]->connected_grabbox, 
					fighter[i]->connected_grabbox->object
				);
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < objects[i].size(); i2++) {
			if (objects[i][i2]->post_collision_status != FIGHTER_STATUS_NONE) {
				objects[i][i2]->change_status(objects[i][i2]->post_collision_status, true, false);
				objects[i][i2]->post_collision_status = FIGHTER_STATUS_NONE;
			}
			objects[i][i2]->connected_hitbox = nullptr;
			objects[i][i2]->connected_grabbox = nullptr;
		}
	}

	if (GameManager::get_instance()->game_context != GAME_CONTEXT_TRAINING &&
		(fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0
		|| fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0)) {
		ko_timer = 300;
		state = BATTLE_STATE_KO;
	}
}

void Battle::check_projectile_collisions() {
	for (int i = 0; i < fighter[0]->projectiles.size(); i++) {
		if (fighter[0]->projectiles[i]->active) {
			for (int i2 = 0; i2 < fighter[1]->projectiles.size(); i2++) {
				if (fighter[1]->projectiles[i2]->active) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[0]->projectiles[i]->hitboxes[i3].active) {
							for (int i4 = 0; i4 < 10; i4++) {
								if (fighter[1]->projectiles[i2]->hitboxes[i4].active) {
									GameRect p1_hitbox, p2_hitbox;
									p1_hitbox = fighter[0]->projectiles[i]->hitboxes[i3].rect;
									p2_hitbox = fighter[1]->projectiles[i2]->hitboxes[i4].rect;
									if (is_collide(p1_hitbox, p2_hitbox)) {
										fighter[0]->projectiles[i]->clear_hitbox(i3);
										fighter[1]->projectiles[i2]->clear_hitbox(i4);
										fighter[0]->projectiles[i]->process_hit();
										fighter[1]->projectiles[i2]->process_hit();
										int common_hitlag = std::min(fighter[0]->projectiles[i]->hitboxes[i3].hitlag, fighter[1]->projectiles[i2]->hitboxes[i4].hitlag);
										fighter[0]->projectiles[i]->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = common_hitlag;
										fighter[0]->projectiles[i]->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = common_hitlag;
										fighter[1]->projectiles[i2]->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = common_hitlag;
										fighter[1]->projectiles[i2]->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = common_hitlag;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Battle::check_fighter_collisions() {
	for (int i = 0; i < 2; i++) {
		int hitbox_to_use = -1;
		int projectile_hitbox_to_use = -1;
		int grabbox_to_use = -1;
		fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = false;
		for (Hurtbox& hurtbox : fighter[i]->hurtboxes) {
			if (hurtbox.active) {
				Blockbox& blockbox = fighter[!i]->blockbox;
				if (blockbox.active) {
					fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = is_collide(blockbox.rect, hurtbox.rect);
				}
				if (hitbox_to_use == -1) {
					for (Hitbox& hitbox : fighter[!i]->hitboxes) {
						if (hitbox.active) {
							if (is_collide(hitbox.rect, hurtbox.rect)) {
								hitbox_to_use = get_event_hit_collide_player(fighter[!i], fighter[i], &hitbox, &hurtbox);
							}
							if (hitbox_to_use != -1) {
								break;
							}
						}
					}
				}
				if (projectile_hitbox_to_use == -1) {
					for (Projectile* projectile : fighter[!i]->projectiles) {
						if (projectile != nullptr && projectile->active) {
							for (Hitbox& hitbox : projectile->hitboxes) {
								if (hitbox.active) {
									if (is_collide(hitbox.rect, hurtbox.rect)) {
										projectile_hitbox_to_use = get_event_hit_collide_projectile(projectile, fighter[i], &hitbox, &hurtbox);
									}
									if (projectile_hitbox_to_use != -1) {
										break;
									}
								}
							}
						}
					}
				}
				if (grabbox_to_use == -1) {
					for (Grabbox& grabbox : fighter[!i]->grabboxes) {
						if (grabbox.active) {
							if (is_collide(grabbox.rect, hurtbox.rect)) {
								grabbox_to_use = get_event_grab_collide_player(fighter[!i], fighter[i], &grabbox, &hurtbox);
							}
							if (grabbox_to_use != -1) {
								break;
							}
						}
					}
				}
			}
		}
		if (hitbox_to_use != -1) {
			fighter[i]->connected_hitbox = &fighter[!i]->hitboxes[hitbox_to_use];
		}
		else {
			fighter[i]->connected_hitbox = nullptr;
		}
		if (grabbox_to_use != -1) {
			fighter[i]->connected_grabbox = &fighter[!i]->grabboxes[grabbox_to_use];
		}
		else {
			fighter[i]->connected_grabbox = nullptr;
		}
//		fighter[i]->connected_projectile_hitbox = projectile_hitbox_to_use;
	}
	if (!event_grab_collide_player()) {
		event_hit_collide_player();
	}
	for (int i = 0; i < 2; i++) {
		for (Projectile* projectile : fighter[i]->projectiles) {
			if (projectile != nullptr && projectile->active) {
//				event_hit_collide_projectile(fighter[i], fighter[!i], projectile, &projectile->hitboxes[fighter[!i]->connected_projectile_hitbox]);
			}
		}
	}
}

/*
░░░░░░░░░░░░░░░░██████████████████
░░░░░░░░░░░░████░░░░░░░░░░░░░░░░░░████
░░░░░░░░░░██░░░░░░░░░░░░░░░░░░░░░░░░░░██
░░░░░░░░░░██░░░░░░░░░░░░░░░░░░░░░░░░░░██
░░░░░░░░██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██
░░░░░░░░██░░░░░░░░░░░░░░░░░░░░██████░░░░██
░░░░░░░░██░░░░░░░░░░░░░░░░░░░░██████░░░░██
░░░░░░░░██░░░░██████░░░░██░░░░██████░░░░██
░░░░░░░░░░██░░░░░░░░░░██████░░░░░░░░░░██
░░░░░░░░████░░██░░░░░░░░░░░░░░░░░░██░░████
░░░░░░░░██░░░░██████████████████████░░░░██
░░░░░░░░██░░░░░░██░░██░░██░░██░░██░░░░░░██
░░░░░░░░░░████░░░░██████████████░░░░████
░░░░░░░░██████████░░░░░░░░░░░░░░██████████
░░░░░░██░░██████████████████████████████░░██
░░░░████░░██░░░░██░░░░░░██░░░░░░██░░░░██░░████
░░░░██░░░░░░██░░░░██████░░██████░░░░██░░░░░░██
░░██░░░░████░░██████░░░░██░░░░██████░░████░░░░██
░░██░░░░░░░░██░░░░██░░░░░░░░░░██░░░░██░░░░░░░░██
░░██░░░░░░░░░░██░░██░░░░░░░░░░██░░██░░░░░░░░░░██
░░░░██░░░░░░██░░░░████░░░░░░████░░░░██░░░░░░██
░░░░░░████░░██░░░░██░░░░░░░░░░██░░░░██░░████
░░░░░░░░██████░░░░██████████████░░░░██████
░░░░░░░░░░████░░░░██████████████░░░░████
░░░░░░░░██████████████████████████████████
░░░░░░░░████████████████░░████████████████
░░░░░░░░░░████████████░░░░░░████████████
░░░░░░██████░░░░░░░░██░░░░░░██░░░░░░░░██████
░░░░░░██░░░░░░░░░░████░░░░░░████░░░░░░░░░░██
░░░░░░░░██████████░░░░░░░░░░░░░░██████████
*/

/// <summary>
/// Checks if any of the attacker's hitboxes have connected with any of the defender's hurtboxes, and sets up values based on what type of event the
/// connection should trigger.
/// </summary>
/// <param name="attacker">: The attacker.</param>
/// <param name="defender">: The defender.</param>
/// <param name="hitbox">: The hitbox that connected.</param>
/// <param name="hurtbox">: The hurtbox that connected.</param>
/// <returns>Either the first hitbox ID to successfully trigger a collision event, or HITBOX_KIND_MAX if a different hitbox ID must be checked.</returns>
int Battle::get_event_hit_collide_player(Fighter* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
	//Verify we didn't already hit with a hitbox on this multihit index and that we're not already 
	//in hitlag
	if (attacker->multihit_connected[hitbox->multihit] 
	|| attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) { 
		return -1;
	}

	//Verify that the hitbox is designed to hit the opponent in their current situation
	if (defender->situation_kind == FIGHTER_SITUATION_GROUND && !(hitbox->hit_kind & HIT_KIND_GROUND)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && !(hitbox->hit_kind & HIT_KIND_AIR)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_DOWN && !(hitbox->hit_kind & HIT_KIND_DOWN)) {
		return -1;
	}


	//Make sure that the hurtbox's intangibility (if any) doesn't protect them from us
	if (hurtbox->intangible_kind == hitbox->attack_height 
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_STRIKE 
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT
		&& !(hitbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_AERIAL 
		&& attacker->situation_kind == FIGHTER_SITUATION_AIR)) {
		return -1;
	}

	//Make sure that we can juggle the opponent
	if (defender->situation_kind == FIGHTER_SITUATION_AIR 
		&& hitbox->juggle_max < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		return -1;
	}

	//Then, check if the hurtbox is invincible. If it is, the attacker's entire attack has failed. 
	//This will be pretty rare tbh.
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	//If the hurtbox has armor, both parties get SOME hitlag to acknowledge the move hitting

	if (hurtbox->is_armor) {
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_RIGHT_OF_WAY) {
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	if (defender->fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height
			|| defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
			return hitbox->id;
		}
	}

	defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;
	if (defender->situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			//While in blockstun, forward, down, and neutral inputs are still valid block directions
			//as long as we're still blocking the correct height. This allows reversal DP motions
			//and such
			switch (hitbox->attack_height) {
				case (ATTACK_HEIGHT_HIGH): {
					if (defender->get_stick_dir() >= 4) {
						return hitbox->id;
					}
				} break;
				case (ATTACK_HEIGHT_LOW): {
					if (defender->get_stick_dir() < 4) {
						case (ATTACK_HEIGHT_MID): {

							return hitbox->id;
						}
					}
				} break;
			}
		}
		else if (defender->is_actionable()) { 
			//While not in blockstun, we need to specifically block using back or down back
			if ((defender->get_stick_dir() == 4 && hitbox->attack_height != ATTACK_HEIGHT_LOW)
				|| (defender->get_stick_dir() == 1 && hitbox->attack_height != ATTACK_HEIGHT_HIGH)) {
				return hitbox->id;
			}
		}
	}

	//If we haven't found a reason to return yet, that means our hitbox has connected

	return hitbox->id;
}

/// <summary>
/// Checks if any of the attacker's grabboxes have connected with any of the defender's hurtboxes, and sets up values based on what type of event the
/// connection should trigger.
/// </summary>
/// <param name="attacker">: The attacker.</param>
/// <param name="defender">: The defender.</param>
/// <param name="grabbox">: The grabbox that connected.</param>
/// <param name="hurtbox">: The hurtbox that connected.</param>
/// <returns>Either the first grabbox ID to successfully trigger a collision event, or HITBOX_KIND_MAX if a different grabbox ID must be checked.</returns>
int Battle::get_event_grab_collide_player(Fighter* attacker, Fighter* defender, Grabbox* grabbox, Hurtbox* hurtbox) {
	if (defender->situation_kind == FIGHTER_SITUATION_GROUND && !(grabbox->hit_kind & HIT_KIND_GROUND)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && !(grabbox->hit_kind & HIT_KIND_AIR)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_DOWN && !(grabbox->hit_kind & HIT_KIND_DOWN)) {
		return -1;
	}

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_THROW 
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT
		&& !(grabbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))) {
		return -1;
	}
	if (defender->get_status_group() == STATUS_GROUP_HITSTUN && (grabbox->grabbox_kind & GRABBOX_KIND_HITSTUN) == 0) {
		return -1;
	}

	return grabbox->id;
}

/// <summary>
/// Checks if any of the projectile's hitboxes have connected with any of the defender's hurtboxes, and sets up values based on what type of event the
/// connection should trigger.
/// </summary>
/// <param name="attacker">: The projectile.</param>
/// <param name="defender">: The defender.</param>
/// <param name="hitbox">: The hitbox that connected.</param>
/// <param name="hurtbox">: The hurtbox that connected.</param>
/// <returns>Either the first hitbox ID to successfully trigger a collision event, or HITBOX_KIND_MAX if a different hitbox ID must be checked.</returns>
int Battle::get_event_hit_collide_projectile(Projectile* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
	if (attacker->multihit_connected[hitbox->multihit] || attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0) {
		return -1;
	}

	if (defender->situation_kind == FIGHTER_SITUATION_GROUND && !(hitbox->hit_kind & HIT_KIND_GROUND)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && !(hitbox->hit_kind & HIT_KIND_AIR)) {
		return -1;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_DOWN && !(hitbox->hit_kind & HIT_KIND_DOWN)) {
		return -1;
	}


	//Make sure that the hurtbox's intangibility (if any) doesn't protect them from us
	if (hurtbox->intangible_kind == hitbox->attack_height
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_PROJECTILE
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT
		&& !(hitbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))) {
		return -1;
	}

	if (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->juggle_max < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		return -1;
	}

	//Then, check if the hurtbox is invincible. If it is, the attacker's entire attack has failed. This will be pretty rare tbh.

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		return hitbox->id;
	}

	//If the hurtbox has armor, both parties get SOME hitlag to acknowledge the move hitting

	if (hurtbox->is_armor) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	if (defender->fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height || defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
			return hitbox->id;
		}
	}
	defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;
	if (defender->situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			//While in blockstun, forward, down, and neutral inputs are still valid block directions
			//as long as we're still blocking the correct height. This allows reversal DP motions
			//and such
			switch (hitbox->attack_height) {
			case (ATTACK_HEIGHT_HIGH): {
				if (defender->get_stick_dir() >= 4) {
					return hitbox->id;
				}
			} break;
			case (ATTACK_HEIGHT_LOW): {
				if (defender->get_stick_dir() < 4) {
			case (ATTACK_HEIGHT_MID): {
				return hitbox->id;
			}
				}
			} break;
			}
		}
		else if (defender->is_actionable()) {
			//While not in blockstun, we need to specifically block using back or down back
			if ((defender->get_stick_dir() == 4 && hitbox->attack_height != ATTACK_HEIGHT_LOW)
				|| (defender->get_stick_dir() == 1 && hitbox->attack_height != ATTACK_HEIGHT_HIGH)) {
				return hitbox->id;
			}
		}
	}
 
	attacker->projectile_flag[PROJECTILE_FLAG_HIT] = true;
	return hitbox->id;
}

/// <summary>
/// Handle any potential hitbox collision events between two Fighters on this frame and change statuses/decrease health accordingly.
/// </summary>
/// <returns>Nothing.</returns>
void Battle::event_hit_collide_player() {
	Hitbox* hitboxes[2] = { fighter[1]->connected_hitbox, fighter[0]->connected_hitbox };
	bool players_hit[2] = { hitboxes[1] != nullptr, hitboxes[0] != nullptr };
	unsigned int post_hit_status[2] = { fighter[0]->status_kind, fighter[1]->status_kind };
	if (players_hit[0] && players_hit[1]) { 
		//First, we determine what to do if both people hit each other
		if (hitboxes[0]->attack_level == hitboxes[1]->attack_level) {
			if (hitboxes[0]->clank_kind == CLANK_KIND_CONTINUE || hitboxes[1]->clank_kind == CLANK_KIND_CONTINUE) {
				const int clank_kinds[2] = { hitboxes[0]->clank_kind, hitboxes[1]->clank_kind }; 
				//If player 1's clank_kind is CLANK_KIND_CONTINUE, player 2's status will change. 
				//This will cause player 2's hitbox to be destroyed before it's checked against player
				//1, so in order to compensate for port priority, we back up the clank_kind as a 
				//constant 
				for (int i = 0; i < 2; i++) {
					if (clank_kinds[i] != CLANK_KIND_CONTINUE) {
						fighter[i]->change_status(FIGHTER_STATUS_CLANK);
					}
				}
			}
		}
		else if (hitboxes[0]->attack_level > hitboxes[1]->attack_level) {
			players_hit[0] = false;
		}
		else {
			players_hit[1] = false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			fighter[!i]->update_hitbox_connect(hitboxes[!i]->multihit);
			if (fighter[i]->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + fighter[i]->get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
				fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
				fighter[i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[!i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH];
				fighter[!i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -fighter[!i]->get_frames_until_actionable();
				fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = fighter[!i]->get_frames_until_actionable();
				fighter[i]->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
				post_hit_status[i] = FIGHTER_STATUS_PARRY;
				training_info[!i].stun_frames.update_text(info_font, "Stun Frames: 0", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			}
			else if (true) { //BLOCK
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitboxes[!i]->blocklag;
				fighter[!i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitboxes[!i]->blocklag;
				fighter[i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitboxes[!i]->blockstun;
				fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
				fighter[i]->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitboxes[!i]->attack_height;
				fighter[i]->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
				int frame_advantage = fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - fighter[!i]->get_frames_until_actionable();
				fighter[!i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
				fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
				fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				if (hitboxes[!i]->damage_kind == DAMAGE_KIND_CHIP) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
				}
				else if (hitboxes[!i]->damage_kind == DAMAGE_KIND_CHIP_KO) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
				}
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->block_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = true;
				post_hit_status[i] = FIGHTER_STATUS_BLOCKSTUN;
				training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(hitboxes[!i]->blockstun), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			}
			else if (!false) { //ARMOR
				//If Attack Connected is false but this still got set off, that means that the opponent hit you while you had armor. In this situation,
				//the attacker gets the same meter gain as if they got blocked, and the defender takes half the usual amount of chip damage. However, this
				//chip damage is incapable of KOing the defender no matter what they got hit by.

				//Note: This is also what will happen to Leon if he gets hit while he has Right of Way armor, so if we ever want to remove the chip damage
				//for when he gets hit with RoW, we'll need to find a way to account for that. I don't think it'll be that big of a deal though.
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0.0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitboxes[!i]->damage / 2), fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			}
			else {
				/*
					If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
					is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
				*/
				fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitboxes[!i]->hitlag;
				fighter[!i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[!i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitboxes[!i]->hitlag;
				fighter[i]->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitboxes[!i]->hitstun;
				fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
				fighter[i]->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
				fighter[i]->fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX] = hitboxes[!i]->disable_hitstun_parry;
				fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT]++;
				if (fighter[i]->get_status_group() != STATUS_GROUP_HITSTUN) {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
				}
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = hitboxes[!i]->launch_init_y;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = hitboxes[!i]->launch_gravity_y;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = hitboxes[!i]->launch_max_fall_speed;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = hitboxes[!i]->launch_speed_x * fighter[!i]->facing_dir;
				/*
				If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise,
				increase it by one so that the opponent's juggle value is always going up
				*/
				if (fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] < hitboxes[!i]->juggle_start) {
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitboxes[!i]->juggle_start;
				}
				else {
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitboxes[!i]->juggle_increase;
				}
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->hit_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				float damage;
				float scale;
				int counterhit_val = can_counterhit(fighter[i], hitboxes[!i]);
				if (counterhit_val >= 1) {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -counterhit_val;
					scale = (clampf(1, 10 - fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
					damage = hitboxes[!i]->damage * scale;
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					float accum_damage = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					damage += accum_damage;
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitboxes[!i]->juggle_start; //Reset the opponent's juggle value on counterhit :)
					fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
					texts[!i].push_back(BattleText());
					texts[!i].back().init(&message_font, "Counter", 40, fighter[!i], glm::vec2(275.0, 450.0));
				}
				else {
					scale = (clampf(1, 10 - fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
					damage = hitboxes[!i]->damage * scale;
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					float accum_damage = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					damage += accum_damage;
				}
				post_hit_status[i] = get_damage_status(fighter[i], hitboxes[!i], counterhit_val);
				if (!fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitboxes[!i]->damage_scale;
				}
				fighter[!i]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
				fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitboxes[!i]->attack_level;
				if (post_hit_status[i] == FIGHTER_STATUS_HITSTUN_AIR) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * fighter[!i]->facing_dir * 0.3;
					fighter[i]->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 25.0;
				}
				fighter[i]->fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
				if (fighter[i]->fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
					texts[!i].push_back(BattleText());
					texts[!i].back().init(&message_font, "Punish", 40, fighter[!i], glm::vec2(275.0, 300.0));
				}
				if (fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT] != 1) {
					if (combo_counter[!i] != nullptr) {
						combo_counter[!i]->update(std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT]), fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
						combo_hit[!i]->update("hits", fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
					}
					else {
						texts[!i].push_back(BattleText());
						texts[!i].back().init(&combo_font, std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT]), fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] + 30, fighter[!i], glm::vec2(275.0, 500.0));
						combo_counter[!i] = &texts[!i].back();
						texts[!i].push_back(BattleText());
						texts[!i].back().init(&message_font, "hits", fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] + 30, fighter[!i], glm::vec2(275.0, 800.0));
						combo_hit[!i] = &texts[!i].back();
					}
				}
				training_info[!i].hit_frame.update_text(info_font, "Hit Frame: " + std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_EXTERNAL_FRAME] + 1), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				int stun_frames = 0;
				switch (post_hit_status[i]) {
					default: {
						stun_frames = fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES];
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					case (FIGHTER_STATUS_LAUNCH_START): {
						stun_frames = fighter[i]->get_anim_length("launch_start");
					} 
					case (FIGHTER_STATUS_LAUNCH): {
						stun_frames += fighter[i]->calc_launch_frames();
						int stun_frames_slow = stun_frames + fighter[i]->get_anim_length("wakeup_up") + fighter[i]->get_anim_length("knockdown_up");
						stun_frames += fighter[i]->get_anim_length("wakeup_fast");
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					case (FIGHTER_STATUS_KNOCKDOWN_START): {
						stun_frames = fighter[i]->get_anim_length("knockdown_start");
						int stun_frames_slow = stun_frames + fighter[i]->get_anim_length("wakeup_up") + fighter[i]->get_anim_length("knockdown_up");
						stun_frames += fighter[i]->get_anim_length("wakeup_fast");
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
				}
				int frame_advantage = stun_frames - fighter[!i]->get_frames_until_actionable();
				fighter[!i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
				fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
				training_info[!i].damage.update_text(info_font,
					"Damage: " + float_to_string(damage, 3) + "(" + float_to_string(scale * 100.0, 3) + "%)",
					glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0)
				);
				training_info[!i].combo_damage.update_text(info_font, "Total: " + float_to_string(fighter[!i]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE], 3), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));

				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_HIT] = true;
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			fighter[i]->change_status(post_hit_status[i], true, false);
			fighter[i]->update_hurtbox_pos();
		}
	}
}

/// <summary>
/// Handle any potential grabbox collision events between two Fighters on this frame and change statuses accordingly.
/// </summary>
bool Battle::event_grab_collide_player() {
	Grabbox* grabboxes[2] = { fighter[1]->connected_grabbox, fighter[0]->connected_grabbox };
	bool players_hit[2] = { grabboxes[1] != nullptr, grabboxes[0] != nullptr };
	bool players_tech[2] = { (players_hit[0] && !grabboxes[1]->grabbox_kind & GRABBOX_KIND_NOTECH), (players_hit[1] && !grabboxes[0]->grabbox_kind & GRABBOX_KIND_NOTECH) };
	if (players_hit[0] && players_hit[1]) {
		if (players_tech[0] == players_tech[1]) {
			for (int i = 0; i < 2; i++) {
				fighter[i]->change_status(FIGHTER_STATUS_THROW_TECH);
			}
			return true;
		}
		else if (players_tech[0]) {
			players_hit[0] = false;
		}
		else {
			players_hit[1] = false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			if (fighter[i]->fighter_flag[FIGHTER_FLAG_THROW_TECH] && players_tech[i]) {
				fighter[i]->change_status(FIGHTER_STATUS_THROW_TECH);
				fighter[!i]->change_status(FIGHTER_STATUS_THROW_TECH);
			}
			else {
				if (fighter[i]->get_status_group() != STATUS_GROUP_HITSTUN) {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
				}
				training_info[!i].hit_frame.update_text(info_font, "Hit Frame: " + std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_EXTERNAL_FRAME] + 1), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				fighter[!i]->change_status(grabboxes[!i]->attacker_status_if_hit);
				fighter[i]->change_status(grabboxes[!i]->defender_status_if_hit);
			}
			fighter[i]->update_hurtbox_pos();
			fighter[!i]->update_hurtbox_pos();
			return true;
		}
	}
	return false;
}

/// <summary>
/// Handle any potential projectile collision events between two Fighters on this frame and change statuses/decrease health accordingly.
/// </summary>
/// <param name="p1">: Player 1</param>
/// <param name="p2">: Player 2</param>
/// <param name="p1_projectile">: Which of P1's projectiles (if any) first connected with P2.</param>
/// <param name="p1_hitbox">: Which of P1's projectile's hitboxes (if any) first connected with P1.</param>
/// <returns>Whether or not any kind of collision event occured.</returns>
void Battle::event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox) {
	bool p2_hit = p1_hitbox->id != -1;
	unsigned int p2_status_post_hit = p2->status_kind;
	if (p2_hit) {
		p1_projectile->update_hitbox_connect(p1_hitbox->multihit);
		p1_projectile->process_hit();
		if (p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2->get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
			p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
			p2->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
			p1_projectile->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
			p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = p2->fighter_float[FIGHTER_FLOAT_HEALTH];
			int frame_advantage = p1_projectile->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + 16 - p1_projectile->projectile_int[PROJECTILE_INT_OWNER_ENDLAG];
			p1->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
			p2->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
			p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = FIGHTER_STATUS_PARRY;
			training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: 0", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		else if (true) { //BLOCK
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = p1_hitbox->blocklag;
			p1_projectile->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
			p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = p1_hitbox->blocklag;
			p2->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
			p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = p1_hitbox->blockstun;
			p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
			p2->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = p1_hitbox->attack_height;
			p2->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
			int frame_advantage = p1_projectile->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - p1_projectile->projectile_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
			p1->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
			p2->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
			p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			if (p1_hitbox->damage_kind == DAMAGE_KIND_CHIP) {
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			}
			else if (p1_hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			}
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p2_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
			training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(p1_hitbox->blockstun), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		else if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0.0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - p1_hitbox->damage / 2, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		}
		else {
			p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = p1_hitbox->hitlag;
			p1_projectile->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = p1_projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
			p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = p1_hitbox->hitlag;
			p2->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
			p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = p1_hitbox->hitstun;
			p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
			p2->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
			p2->fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX] = p1_hitbox->disable_hitstun_parry;
			p1->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
			if (p2->get_status_group() != STATUS_GROUP_HITSTUN) {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
			}
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = p1_hitbox->launch_init_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x * p1_projectile->facing_dir;
			if (p2->situation_kind == FIGHTER_SITUATION_AIR) {
				if (p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] < p1_hitbox->juggle_start) {
					p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_start;
				}
				else {
					p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] += p1_hitbox->juggle_increase;
				}
			}
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			float damage;
			float scale;
			int counterhit_val = can_counterhit(p2, p1_hitbox);
			if (counterhit_val >= 1) {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -counterhit_val;
				scale = (clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
				damage = p1_hitbox->damage * scale;
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				float accum_damage = p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				damage += accum_damage;
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_increase;
				p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
				texts[p1->id].push_back(BattleText());
				texts[p1->id].back().init(&message_font, "Counter", 40, p1, glm::vec2(275.0, 450.0));
			}
			else {
				scale = (clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
				damage = p1_hitbox->damage * scale;
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				float accum_damage = p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				damage += accum_damage;
			}
			p2_status_post_hit = get_damage_status(p2, p1_hitbox, counterhit_val);
			if (!p1->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS]) {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += p1_hitbox->damage_scale;
			}
			p1->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
			p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;			
			if (p2_status_post_hit == FIGHTER_STATUS_HITSTUN_AIR) {
				p2->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * p1->facing_dir * 0.3;
				p2->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 25.0;
			}
			p2->fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
			if (p2->fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
				texts[p1->id].push_back(BattleText());
				texts[p1->id].back().init(&message_font, "Punish", 40, p2, glm::vec2(275.0, 300.0));
			}

			if (p1->fighter_int[FIGHTER_INT_COMBO_COUNT] != 1) {
				if (combo_counter[p1->id] != nullptr) {
					combo_counter[p1->id]->update(std::to_string(p1->fighter_int[FIGHTER_INT_COMBO_COUNT]), p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
					combo_hit[p1->id]->update("hits", p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
				}
				else {
					texts[p1->id].push_back(BattleText());
					texts[p1->id].back().init(&combo_font, std::to_string(p1->fighter_int[FIGHTER_INT_COMBO_COUNT]), p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES], p1, glm::vec2(275.0, 500.0));
					combo_counter[p1->id] = &texts[p1->id].back();
					texts[p1->id].push_back(BattleText());
					texts[p1->id].back().init(&message_font, "hits", p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES], p1, glm::vec2(275.0, 800.0));
					combo_hit[p1->id] = &texts[p1->id].back();
				}
			}
			training_info[p1->id].hit_frame.update_text(info_font, "Hit Frame: " + std::to_string(p1_projectile->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES]), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			int stun_frames = 0;
			switch (p2_status_post_hit) {
			default: {
				stun_frames =p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES];
				training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			} break;
			case (FIGHTER_STATUS_LAUNCH_START): {
				stun_frames = p2->get_anim_length("launch_start");
			}
			case (FIGHTER_STATUS_LAUNCH): {
				stun_frames += p2->calc_launch_frames();
				int stun_frames_slow = stun_frames + p2->get_anim_length("wakeup_up") + p2->get_anim_length("knockdown_up");
				stun_frames += p2->get_anim_length("wakeup_fast");
				training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			} break;
			case (FIGHTER_STATUS_KNOCKDOWN_START): {
				stun_frames = p2->get_anim_length("knockdown_start");
				int stun_frames_slow = stun_frames + p2->get_anim_length("wakeup_up") + p2->get_anim_length("knockdown_up");
				stun_frames += p2->get_anim_length("wakeup_fast");
				training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			} break;
			}
			int frame_advantage = p1_projectile->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] 
				+ p2->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + stun_frames 
				- p1_projectile->projectile_int[PROJECTILE_INT_OWNER_ENDLAG] - 1;
			p1->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
			p2->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
			training_info[p1->id].damage.update_text(info_font,
				"Damage: " + float_to_string(damage, 3) + "(" + float_to_string(scale * 100.0, 3) + "%)",
				glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0)
			);
			training_info[p1->id].combo_damage.update_text(info_font, "Total: " + float_to_string(fighter[p1->id]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE], 3), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT] = false;
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = true;
		p1->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS] = true;
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
		p2->update_hurtbox_pos();
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="defender"></param>
/// <param name="hitbox"></param>
/// <returns>0 if the hitbox isn't a counterhit at all, 1 if it is but we aren't going to use the counterhit status, 2 
/// if it is and we're using the counterhit status.</returns>
int Battle::can_counterhit(Fighter* defender, Hitbox* hitbox) {
	if (defender->status_kind == FIGHTER_STATUS_HITSTUN_PARRY_START
		|| defender->status_kind == FIGHTER_STATUS_LAUNCH_PARRY_START) {
		return 2;
	}
	if (defender->anim_kind == nullptr) {
		return 0;
	}
	switch (hitbox->counterhit_type) {
		case (COUNTERHIT_TYPE_COUNTER):
		default: { 
			//If the counterhit flag is on, apply the counterhit multipliers and counterhit status
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) * 2;
		} break;
		case (COUNTERHIT_TYPE_ANY): { 
			//Apply the counterhit multipliers and status if either the counterhit flag or the endlag flag is on
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] 
				|| defender->fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) * 2;
		} break;
		case (COUNTERHIT_TYPE_PUNISH): {
			//Apply the counterhit multipliers and status if the endlag flag is on, apply only the
			//mutlipliers if the counterhit flag is on
			if (defender->fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
				return 2;
			}
			else {
				return (int)defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT];
			}
		} break;
		case (COUNTERHIT_TYPE_JUMP_COUNTER): {
			//Apply the counterhit multipliers as long as the counterhit flag is on, but only apply the status if the opponent
			//is in the air
			if (defender->fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT]) {
				return 2;
			}
			else {
				return (int)defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT];
			}
		} break;
		case (COUNTERHIT_TYPE_NONE): {
			//Never apply the status, but apply the multipliers as long as the counterhit flag is on
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]);
		} break;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="hit_status"></param>
/// <param name="situation_kind"></param>
/// <returns></returns>
int Battle::get_damage_status(Fighter* defender, Hitbox* hitbox, int counterhit_val) {
	int situation_kind = defender->situation_kind;
	int hit_status;
	if (counterhit_val == 2) {
		hit_status = hitbox->counterhit_status;
	}
	else {
		hit_status = hitbox->hit_status;
	}
	if (defender->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0f &&
		GameManager::get_instance()->game_context != GAME_CONTEXT_TRAINING) {
		if (hitbox->attack_level == ATTACK_LEVEL_LIGHT && situation_kind == FIGHTER_SITUATION_GROUND) {
			return FIGHTER_STATUS_CRUMPLE;
		}
		else {
			switch (situation_kind) {
				case (FIGHTER_SITUATION_GROUND): {
					if (hit_status == HIT_STATUS_LAUNCH) {
						return FIGHTER_STATUS_LAUNCH_START;
					}
					else {
						return FIGHTER_STATUS_KNOCKDOWN_START;
					}
				} break;
				case (FIGHTER_SITUATION_AIR): {
					return FIGHTER_STATUS_LAUNCH;
				} break;
				case (FIGHTER_SITUATION_DOWN): {
					return FIGHTER_STATUS_KNOCKDOWN;
				} break;
			}

		}
	}
	if (hitbox->continue_launch && (defender->status_kind == FIGHTER_STATUS_LAUNCH 
		|| defender->status_kind == FIGHTER_STATUS_HITSTUN_FLOAT)) {
		return FIGHTER_STATUS_LAUNCH;
	}
	switch (hit_status) {
		case (HIT_STATUS_NORMAL):
		default: {
			switch (situation_kind) {
				case (FIGHTER_SITUATION_AIR): {
					return FIGHTER_STATUS_HITSTUN_AIR;
				} break;
				case (FIGHTER_SITUATION_GROUND): {
					return FIGHTER_STATUS_HITSTUN;
				} break;
				case (FIGHTER_SITUATION_DOWN): {
					return FIGHTER_STATUS_KNOCKDOWN;
				} break;
			}
		} break;
		case (HIT_STATUS_CRUMPLE): {
			switch (situation_kind) {
				case (FIGHTER_SITUATION_AIR): {
					return FIGHTER_STATUS_LAUNCH;
				} break;
				case (FIGHTER_SITUATION_GROUND): {
					return FIGHTER_STATUS_CRUMPLE;
				} break;
				case (FIGHTER_SITUATION_DOWN): {
					return FIGHTER_STATUS_KNOCKDOWN;
				} break;
			}
		} break;
		case (HIT_STATUS_FLOAT): {
			if (situation_kind == FIGHTER_SITUATION_AIR) {
				return FIGHTER_STATUS_HITSTUN_AIR;
			}
			else {
				return FIGHTER_STATUS_HITSTUN_FLOAT;
			}
		} break;
		case (HIT_STATUS_LAUNCH): {
			if (situation_kind == FIGHTER_SITUATION_AIR) {
				return FIGHTER_STATUS_LAUNCH;
			}
			else {
				return FIGHTER_STATUS_LAUNCH_START;
			}
		} break;
		case (HIT_STATUS_KNOCKDOWN): {
			if (situation_kind == FIGHTER_SITUATION_DOWN) {
				return FIGHTER_STATUS_KNOCKDOWN;
			}
			else {
				return FIGHTER_STATUS_KNOCKDOWN_START;
			}
		} break;
	}
}