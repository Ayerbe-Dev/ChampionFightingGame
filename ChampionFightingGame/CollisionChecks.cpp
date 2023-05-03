#include "Battle.h"
#include "ParamAccessor.h"
#include "utils.h"

void Battle::check_collisions() {
	if (frame_pause && !debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE)) {
		return;
	}
	check_projectile_collisions();
	check_fighter_collisions();
}

void Battle::check_projectile_collisions() {
	for (int i = 0; i < fighter[0]->num_projectiles; i++) {
		if (fighter[0]->projectiles[i]->active) {
			for (int i2 = 0; i2 < fighter[1]->num_projectiles; i2++) {
				if (fighter[1]->projectiles[i2]->active) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[0]->projectiles[i]->hitboxes[i3].active && fighter[0]->projectiles[i]->hitboxes[i3].trade) {
							for (int i4 = 0; i4 < 10; i4++) {
								if (fighter[1]->projectiles[i2]->hitboxes[i4].active && fighter[1]->projectiles[i2]->hitboxes[i4].trade) {
									GameRect p1_hitbox, p2_hitbox;
									p1_hitbox = fighter[0]->projectiles[i]->hitboxes[i3].rect;
									p2_hitbox = fighter[1]->projectiles[i2]->hitboxes[i4].rect;
									if (is_collide(p1_hitbox, p2_hitbox)) {
										fighter[0]->projectiles[i]->clear_hitbox(i3);
										fighter[1]->projectiles[i2]->clear_hitbox(i4);
										fighter[0]->projectiles[i]->projectile_int[PROJECTILE_INT_HEALTH] --;
										fighter[1]->projectiles[i2]->projectile_int[PROJECTILE_INT_HEALTH] --;
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
		int hitbox_to_use = HITBOX_COUNT_MAX;
		int projectile_hitbox_to_use = HITBOX_COUNT_MAX;
		int grabbox_to_use = HITBOX_COUNT_MAX;
		fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = false;
		for (Hurtbox& hurtbox : fighter[i]->hurtboxes) {
			if (hurtbox.active) {
				Blockbox& blockbox = fighter[!i]->blockbox;
				if (blockbox.active) {
					fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = is_collide(blockbox.rect, hurtbox.rect);
				}
				if (hitbox_to_use == HITBOX_COUNT_MAX) {
					for (Hitbox& hitbox : fighter[!i]->hitboxes) {
						if (hitbox.active) {
							if (is_collide(hitbox.rect, hurtbox.rect)) {
								hitbox_to_use = get_event_hit_collide_player(fighter[!i], fighter[i], &hitbox, &hurtbox);
							}
							if (hitbox_to_use != HITBOX_COUNT_MAX) {
								break;
							}
						}
					}
				}
				if (projectile_hitbox_to_use == HITBOX_COUNT_MAX) {
					for (Projectile* projectile : fighter[!i]->projectiles) {
						if (projectile != nullptr && projectile->active) {
							for (Hitbox& hitbox : projectile->hitboxes) {
								if (hitbox.active) {
									if (is_collide(hitbox.rect, hurtbox.rect)) {
										projectile_hitbox_to_use = get_event_hit_collide_projectile(projectile, fighter[i], &hitbox, &hurtbox);
									}
									if (projectile_hitbox_to_use != HITBOX_COUNT_MAX) {
										break;
									}
								}
							}
						}
					}
				}
				if (grabbox_to_use == HITBOX_COUNT_MAX) {
					for (Grabbox& grabbox : fighter[!i]->grabboxes) {
						if (grabbox.active) {
							if (is_collide(grabbox.rect, hurtbox.rect)) {
								grabbox_to_use = get_event_grab_collide_player(fighter[!i], fighter[i], &grabbox, &hurtbox);
							}
							if (grabbox_to_use != HITBOX_COUNT_MAX) {
								break;
							}
						}
					}
				}
			}
		}
		fighter[i]->connected_hitbox = hitbox_to_use;
		fighter[i]->connected_grabbox = grabbox_to_use;
		fighter[i]->connected_projectile_hitbox = projectile_hitbox_to_use;
	}
	if (!event_hit_collide_player()) {
		event_grab_collide_player();
	}
	for (int i = 0; i < 2; i++) {
		for (Projectile* projectile : fighter[i]->projectiles) {
			if (projectile != nullptr && projectile->active) {
				event_hit_collide_projectile(fighter[i], fighter[!i], projectile, &projectile->hitboxes[fighter[!i]->connected_projectile_hitbox]);
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
	//First, check if the hit and hurtboxes are even compatible
	if (attacker->multihit_connected[hitbox->multihit]) {
		return HITBOX_COUNT_MAX;
	}

	if (hitbox->situation_hit != SITUATION_HIT_ALL) {
		if (hitbox->situation_hit != SITUATION_HIT_GROUND_AIR) {
			if (hitbox->situation_hit != defender->situation_kind) {
				return HITBOX_COUNT_MAX;
			}
		}
		else if (defender->situation_kind == FIGHTER_SITUATION_DOWN) {
			return HITBOX_COUNT_MAX;
		}
	}
	if (hurtbox->intangible_kind == hitbox->attack_height || hurtbox->intangible_kind == INTANGIBLE_KIND_NORMAL || hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->juggle_max < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		return HITBOX_COUNT_MAX;
	}

	//Then, check if the hurtbox is invincible. If it is, the attacker's entire attack has failed. This will be pretty rare tbh.

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

	bool blocking = false;
	bool parrying = false;
	if ((defender->is_actionable() || defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) && defender->situation_kind == FIGHTER_SITUATION_GROUND) {
		if (defender->get_stick_dir() == 4 && hitbox->attack_height != ATTACK_HEIGHT_LOW) {
			blocking = true;
		}
		if (defender->get_stick_dir() == 1 && hitbox->attack_height != ATTACK_HEIGHT_HIGH) {
			blocking = true;
		}

	}
	if (defender->fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height || defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			parrying = true;
		}
	}
	if (parrying) {

		//This type of parry factors in blockstun

/*
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 12;
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 12 + get_param_int("parry_advantage_frames", PARAM_FIGHTER) - hitbox->blockstun;
		int diff = 0 - attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		if (diff > 0) {
			defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] += diff;
			attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] += diff;
		}
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] += 4;
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] += 4;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
		int frame_advantage = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] - (attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + attacker->get_frames_until_actionable());
		attacker->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;		
*/

		//This type of parry doesn't

		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
		defender->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = defender->fighter_float[FIGHTER_FLOAT_HEALTH];
		attacker->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = -attacker->get_frames_until_actionable();

		return hitbox->id;
	}
	if (blocking && hitbox->chip_damage != -1.0) {
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		defender->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = true;
		defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;
		defender->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;

		int frame_advantage = defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - attacker->get_frames_until_actionable();
		attacker->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
		return hitbox->id;
	}

	attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	attacker->fighter_flag[FIGHTER_FLAG_ATTACK_SUCCEEDED] = true;
	defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;
	defender->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;

	int frame_advantage = defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - attacker->get_frames_until_actionable();
	attacker->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
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
	if (grabbox->situation_hit != SITUATION_HIT_ALL) {
		if (grabbox->situation_hit != SITUATION_HIT_GROUND_AIR) {
			if (grabbox->situation_hit != defender->situation_kind) {
				return HITBOX_COUNT_MAX;
			}
		}
		else if (defender->situation_kind == FIGHTER_SITUATION_DOWN) {
			return HITBOX_COUNT_MAX;
		}
	}
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_THROW || hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->get_status_group() == STATUS_GROUP_HITSTUN && (grabbox->grabbox_kind & GRABBOX_KIND_HITSTUN) == 0) {
		return HITBOX_COUNT_MAX;
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
	if (attacker->multihit_connected[hitbox->multihit]) {
		return HITBOX_COUNT_MAX;
	}

	if (hitbox->situation_hit != SITUATION_HIT_ALL) {
		if (hitbox->situation_hit != SITUATION_HIT_GROUND_AIR) {
			if (hitbox->situation_hit != defender->situation_kind) {
				return HITBOX_COUNT_MAX;
			}
		}
		else if (defender->situation_kind == FIGHTER_SITUATION_DOWN) {
			return HITBOX_COUNT_MAX;
		}
	}
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_PROJECTILE || hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->juggle_max < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		return HITBOX_COUNT_MAX;
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

	bool blocking = false;
	bool parrying = false;
	if ((defender->is_actionable() || defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) && defender->situation_kind == FIGHTER_SITUATION_GROUND) {
		if (defender->get_stick_dir() == 4 && hitbox->attack_height != ATTACK_HEIGHT_LOW) {
			blocking = true;
		}
		if (defender->get_stick_dir() == 1 && hitbox->attack_height != ATTACK_HEIGHT_HIGH) {
			blocking = true;
		}

	}
	if (defender->fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height || defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			parrying = true;
		}
	}

	if (parrying) {
		//Factors in blockstun

/*
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16 + get_param_int("parry_advantage_frames", PARAM_FIGHTER) - hitbox->blockstun;
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
		int frame_advantage = attacker->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + 16 - attacker->projectile_int[PROJECTILE_INT_OWNER_ENDLAG];
*/

		//Doesn't factor in blockstun

		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = defender->fighter_float[FIGHTER_FLOAT_HEALTH];
		int frame_advantage = attacker->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + 16 - attacker->projectile_int[PROJECTILE_INT_OWNER_ENDLAG];
		attacker->owner->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;

		return hitbox->id;
	}
	if (blocking && hitbox->chip_damage != -1.0) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		defender->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = true;
		defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;
		defender->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
		int frame_advantage = attacker->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - attacker->projectile_int[PROJECTILE_INT_OWNER_ENDLAG];
		attacker->owner->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
		return hitbox->id;
	}

 	attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
 	attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
	attacker->projectile_flag[PROJECTILE_FLAG_HIT] = true;
	defender->fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;
	defender->fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	int frame_advantage = attacker->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] + defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] - attacker->projectile_int[PROJECTILE_INT_OWNER_ENDLAG];
	attacker->owner->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
	return hitbox->id;
}

/// <summary>
/// Handle any potential hitbox collision events between two Fighters on this frame and change statuses/decrease health accordingly.
/// </summary>
/// <returns>Whether or not any kind of collision event occured.</returns>
bool Battle::event_hit_collide_player() {
	Hitbox* hitboxes[2] = { &(fighter[0]->hitboxes[fighter[1]->connected_hitbox]), &(fighter[1]->hitboxes[fighter[0]->connected_hitbox]) };
	bool players_hit[2] = { hitboxes[1]->id != -1, hitboxes[0]->id != -1 };
	unsigned int post_hit_status[2] = { fighter[0]->status_kind, fighter[1]->status_kind };
	if (players_hit[0] && players_hit[1]) { 
		//First, we determine what to do if both people hit each other
		if (fighter[0]->situation_kind != fighter[1]->situation_kind) {
			//We prioritize grounded opponents over airborne opponents. I'm tempted to remove this part.
			if (fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND) {
				players_hit[0] = false;
			}
			else {
				players_hit[1] = false;
			}
		}
		else if (hitboxes[0]->attack_level == hitboxes[1]->attack_level) {
			if (hitboxes[0]->clank_kind == CLANK_KIND_CONTINUE || hitboxes[1]->clank_kind == CLANK_KIND_CONTINUE) {
				const int clank_kinds[2] = { hitboxes[0]->clank_kind, hitboxes[1]->clank_kind }; 
				//If player 1's clank_kind is CLANK_KIND_CONTINUE, player 2's status will change. 
				//This will cause player 2's hitbox to be destroyed before it's checked against player
				//1, so in order to compensate for port priority, we back up the clank_kind as a 
				//constant 
				for (int i = 0; i < 2; i++) {
					if (clank_kinds[i] == CLANK_KIND_CONTINUE) {
						fighter[!i]->change_status(FIGHTER_STATUS_CLANK);
					}
				}
				return false;
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
				fighter[i]->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
				post_hit_status[i] = FIGHTER_STATUS_PARRY;
				training_info[!i].stun_frames.update_text(info_font, "Stun Frames: 0", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			}
			else if (fighter[i]->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				if (hitboxes[!i]->damage_kind == DAMAGE_KIND_CHIP) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
				}
				else if (hitboxes[!i]->damage_kind == DAMAGE_KIND_CHIP_KO) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
				}
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->block_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				fighter[i]->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = true;
				post_hit_status[i] = FIGHTER_STATUS_BLOCKSTUN;
				training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(hitboxes[!i]->blockstun), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			}
			else if (!fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_SUCCEEDED]) {
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
				fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitboxes[!i]->juggle_increase;
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->hit_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				float damage;
				float scale;
				int counterhit_val = can_counterhit(fighter[i], hitboxes[!i]);
				if (counterhit_val >= 1) {
					scale = clampf(1.2, (clampf(1, 10 - fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10, 1.5);
					damage = hitboxes[!i]->damage * scale;
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					float accum_damage = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					damage += accum_damage;
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitboxes[!i]->juggle_increase; //Reset the opponent's juggle value on counterhit :)
					fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
					if (counterhit_val == 2) {
						post_hit_status[i] = get_damage_status(hitboxes[!i]->counterhit_status, fighter[i]->situation_kind);
					}
					else {
						post_hit_status[i] = get_damage_status(hitboxes[!i]->hit_status, fighter[i]->situation_kind);
					}
					texts[!i].push_back(BattleText());
					texts[!i].back().init(&message_font, "Counter", 40, fighter[!i], glm::vec2(275.0, 450.0));
				}
				else {
					scale = (clampf(1, 10 - fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10;
					damage = hitboxes[!i]->damage * scale;
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
					float accum_damage = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitboxes[!i]->damage_kind == DAMAGE_KIND_NO_KO, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
					damage += accum_damage;
					post_hit_status[i] = get_damage_status(hitboxes[!i]->hit_status, fighter[i]->situation_kind);
				}
				if (!fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitboxes[!i]->damage_scale;
				}
				if (fighter[i]->status_kind == FIGHTER_STATUS_LAUNCH && hitboxes[!i]->continue_launch) {
					post_hit_status[i] = FIGHTER_STATUS_LAUNCH;
				}
				fighter[!i]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
				fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitboxes[!i]->attack_level;
				if (post_hit_status[i] == FIGHTER_STATUS_HITSTUN_AIR) {
					fighter[i]->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = hitboxes[!i]->hit_pushback * fighter[!i]->facing_dir * 0.1;
					fighter[i]->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 5.0;
					fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] *= 1.2;
				}

				if (fighter[i]->fighter_flag[FIGHTER_FLAG_IN_ENDLAG]) {
					texts[!i].push_back(BattleText());
					texts[!i].back().init(&message_font, "Punish", 40, fighter[!i], glm::vec2(275.0, 1280.0));
				}
				if (fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT] != 1) {
					if (combo_counter[!i] != nullptr) {
						combo_counter[!i]->update(std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT]), fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
						combo_hit[!i]->update("hits", fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
					}
					else {
						texts[!i].push_back(BattleText());
						texts[!i].back().init(&combo_font, std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT]), fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES], fighter[!i], glm::vec2(275.0, 500.0));
						combo_counter[!i] = &texts[!i].back();
						texts[!i].push_back(BattleText());
						texts[!i].back().init(&message_font, "hits", fighter[i]->fighter_int[FIGHTER_INT_HITLAG_FRAMES] + fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES], fighter[!i], glm::vec2(275.0, 800.0));
						combo_hit[!i] = &texts[!i].back();
					}
				}
				training_info[!i].hit_frame.update_text(info_font, "Hit Frame: " + std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_EXTERNAL_FRAME]), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				training_info[!i].damage.update_text(info_font,
					"Damage: " + float_to_string(damage, 3) + "(" + float_to_string(scale * 100.0, 3) + "%)",
					glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0)
				);
				training_info[!i].combo_damage.update_text(info_font, "Total: " + float_to_string(fighter[!i]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE], 3), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));

				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_SUCCEEDED] = false;
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = true;
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			fighter[i]->change_status(post_hit_status[i], true, false);
			fighter[i]->update_hurtbox_pos();
		}
	}
	return (players_hit[0] || players_hit[1]);
}

/// <summary>
/// Handle any potential grabbox collision events between two Fighters on this frame and change statuses accordingly.
/// </summary>
void Battle::event_grab_collide_player() {
	Grabbox* grabboxes[2] = { &(fighter[0]->grabboxes[fighter[1]->connected_grabbox]), &(fighter[1]->grabboxes[fighter[0]->connected_grabbox]) };
	bool players_hit[2] = { grabboxes[1]->id != -1, grabboxes[0]->id != -1 };
	bool players_tech[2] = { !grabboxes[1]->grabbox_kind & GRABBOX_KIND_NOTECH, !grabboxes[0]->grabbox_kind & GRABBOX_KIND_NOTECH };
	if (players_hit[0] && players_hit[1]) {
		if (players_tech[0] == players_tech[1]) {
			for (int i = 0; i < 2; i++) {
				fighter[i]->change_status(FIGHTER_STATUS_THROW_TECH);
			}
			return;
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
				fighter[!i]->change_status(grabboxes[!i]->attacker_status_if_hit);
				fighter[i]->change_status(grabboxes[!i]->defender_status_if_hit);
			}
			fighter[i]->update_hurtbox_pos();
			fighter[!i]->update_hurtbox_pos();
			return;
		}
	}
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
		p1_projectile->projectile_int[PROJECTILE_INT_HEALTH] --;
		if (p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2->get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = FIGHTER_STATUS_PARRY;
			training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: 0", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		else if (p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			if (p1_hitbox->damage_kind == DAMAGE_KIND_CHIP) {
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			}
			else if (p1_hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			}
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
			p2_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
			training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(p1_hitbox->blockstun), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		else if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0.0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
			p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - p1_hitbox->damage / 2, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		}
		else {
			p2->fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX] = p1_hitbox->disable_hitstun_parry;
			p1->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
			if (p2->get_status_group() != STATUS_GROUP_HITSTUN) {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
			}
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = p1_hitbox->launch_init_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x * p1_projectile->facing_dir;
			/*
			If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise,
			increase it by one so that the opponent's juggle value is always going up
			*/
			if (p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] < p1_hitbox->juggle_start) {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_start;
			}
			p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] += p1_hitbox->juggle_increase;
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			float damage;
			float scale;
			int counterhit_val = can_counterhit(p2, p1_hitbox);
			if (counterhit_val >= 1) {
				scale = clampf(1.2, (clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10, 1.5);
				damage = p1_hitbox->damage * scale;
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				float accum_damage = p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				damage += accum_damage;
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_increase;
				p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
				if (counterhit_val == 2) {
					p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
				}
				else {
					p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
				}
				texts[p1->id].push_back(BattleText());
				texts[p1->id].back().init(&message_font, "Counter", 40, p1, glm::vec2(275.0, 450.0));
			}
			else {
				scale = (clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10;
				damage = p1_hitbox->damage * scale;
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
				float accum_damage = p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(p1_hitbox->damage_kind == DAMAGE_KIND_NO_KO, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, p2->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
				damage += accum_damage;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
			}
			if (!p1->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS]) {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += p1_hitbox->damage_scale;
			}
			if (p2->status_kind == FIGHTER_STATUS_LAUNCH && p1_hitbox->continue_launch) {
				p2_status_post_hit = FIGHTER_STATUS_LAUNCH;
			}
			p1->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
			p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;			
			if (p2_status_post_hit == FIGHTER_STATUS_HITSTUN_AIR) {
				p2->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = p1_hitbox->hit_pushback * p1->facing_dir * 0.1;
				p2->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 5.0;
				p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] *= 1.2;
			}
			if (p2->fighter_flag[FIGHTER_FLAG_IN_ENDLAG]) {
				texts[p1->id].push_back(BattleText());
				texts[p1->id].back().init(&message_font, "Punish", 40, p2, glm::vec2(275.0, 1280.0));
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
			training_info[p1->id].hit_frame.update_text(info_font, "Hit Frame: " + float_to_string((float)p1_projectile->projectile_int[PROJECTILE_INT_ELAPSED_FRAMES], 2), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			training_info[p1->id].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
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
	if (defender->anim_kind == nullptr) {
		return 0;
	}
	switch (hitbox->counterhit_type) {
		case (COUNTERHIT_TYPE_NORMAL):
		default: { 
			//If the counterhit flag is on, apply the counterhit multipliers and counterhit status
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) * 2;
		} break;
		case (COUNTERHIT_TYPE_PUNISH): { 
			//Apply the counterhit multipliers and status if either the counterhit flag or the endlag flag is on
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] 
				|| defender->fighter_flag[FIGHTER_FLAG_IN_ENDLAG]) * 2;
		} break;
		case (COUNTERHIT_TYPE_COUNTER_ONLY): {
			//Apply the counterhit multipliers as long as the counterhit flag is on, but only apply the status if the endlag
			//flag is OFF
			return (int)(defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) * 2
				- (int)(defender->fighter_flag[FIGHTER_FLAG_IN_ENDLAG]);
		} break;
		case (COUNTERHIT_TYPE_PUNISH_ONLY): {
			//Apply the counterhit multipliers as long as the counterhit flag is on, but only apply the status if the endlag
			//flag is ON
			if (!defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
				return 0;
			}
			return 1 + (int)(defender->fighter_flag[FIGHTER_FLAG_IN_ENDLAG]);
		} break;
		case (COUNTERHIT_TYPE_AERIAL_ONLY): {
			//Apply the counterhit multipliers as long as the counterhit flag is on, but only apply the status if the opponent
			//is in the air
			if (!defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
				return 0;
			}
			return 1 + (int)(defender->situation_kind == FIGHTER_SITUATION_AIR);
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
int Battle::get_damage_status(int hit_status, int situation_kind) {
	switch (hit_status) {
		case (HIT_STATUS_NORMAL):
		default: {
			if (situation_kind == FIGHTER_SITUATION_AIR) {
				return FIGHTER_STATUS_HITSTUN_AIR;
			}
			else {
				return FIGHTER_STATUS_HITSTUN;
			}
		} break;
		case (HIT_STATUS_CRUMPLE): {
			if (situation_kind == FIGHTER_SITUATION_AIR) {
				return FIGHTER_STATUS_LAUNCH;
			}
			else {
				return FIGHTER_STATUS_CRUMPLE;
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
			return FIGHTER_STATUS_KNOCKDOWN_START;
		} break;
	}
}