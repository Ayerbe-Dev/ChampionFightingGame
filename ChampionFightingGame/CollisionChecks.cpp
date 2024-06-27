#include "Battle.h"
#include "ParamAccessor.h"
#include "utils.h"

void Battle::process_collisions() {
	if (frame_pause && !(fighter[0]->check_button_trigger(BUTTON_MENU_FRAME_ADVANCE) 
		|| fighter[1]->check_button_trigger(BUTTON_MENU_FRAME_ADVANCE))) {
		return;
	}
	std::vector<Blockbox*> blockboxes[2];
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
		if (fighter[i]->blockbox.active) {
			blockboxes[i].push_back(&fighter[i]->blockbox);
		}
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
			if (fighter[i]->projectiles[i2]->blockbox.active) {
				blockboxes[i].push_back(&fighter[i]->projectiles[i2]->blockbox);
			}
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
		fighter[i]->check_incoming_blockbox_collisions(blockboxes[!i]);
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
			if (objects[i][i2]->definite_hitbox.active) {
				DefiniteHitbox& hitbox = objects[i][i2]->definite_hitbox;
				Fighter* attacker = hitbox.attacker;
				Fighter* defender = hitbox.defender;
				defender->process_definite_hitbox_activated(&hitbox, attacker);
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < objects[i].size(); i2++) {
			if (objects[i][i2]->post_collision_status != BATTLE_OBJECT_STATUS_NONE) {
				objects[i][i2]->change_status(objects[i][i2]->post_collision_status, true);
				objects[i][i2]->post_collision_status = BATTLE_OBJECT_STATUS_NONE;

				if (objects[i][i2]->object_type != BATTLE_OBJECT_TYPE_FIGHTER
				|| (fighter[i]->connected_grabbox == nullptr 
					&& fighter[i]->connected_hitbox == nullptr
					&& !fighter[i]->object_flag[FIGHTER_FLAG_APPLIED_DEFINITE_HITBOX])) continue;
				fighter[i]->object_flag[FIGHTER_FLAG_APPLIED_DEFINITE_HITBOX] = false;
				if (fighter[!i]->object_int[FIGHTER_INT_COMBO_COUNT] > 1) {
					if (combo_counter[!i] != nullptr) {
						combo_counter[!i]->update_text(std::to_string(fighter[!i]->object_int[FIGHTER_INT_COMBO_COUNT]));
						combo_counter[!i]->texture.scale_all_percent(1.1f);
						combo_hits[!i]->texture.scale_all_percent(1.1f);
					}
					else {
						add_message("combo", std::to_string(fighter[!i]->object_int[FIGHTER_INT_COMBO_COUNT]), VBP(fighter[i]->object_flag[FIGHTER_FLAG_ENDED_HITSTUN]), 5, glm::vec2(275.0, 500.0), glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						messages_active.back().texture.scale_all_percent(1.1f);
						combo_counter[!i] = &messages_active.back();
						add_message("message", "hits", VBP(fighter[i]->object_flag[FIGHTER_FLAG_ENDED_HITSTUN]), 5, glm::vec2(275.0, 800.0), glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						messages_active.back().texture.scale_all_percent(1.1f);
						combo_hits[!i] = &messages_active.back();
						if (i) {
							combo_counter[!i]->texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
							combo_hits[!i]->texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
						}
						else {
							combo_counter[!i]->texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
							combo_hits[!i]->texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
						}
					}
				}
				if (game_context == GAME_CONTEXT_TRAINING) {
					training_info[!i].fields[TRAINING_FIELD_STARTUP].update_text(
						get_font("info"),
						"Startup: " + std::to_string(
							fighter[!i]->object_int[FIGHTER_INT_EXTERNAL_FRAME] + 1
						), glm::vec4(255.0), 
						glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
					training_info[!i].fields[TRAINING_FIELD_DAMAGE].update_text(
						get_font("info"),
						"Damage: " 
						+ float_to_string(fighter[i]->object_float[FIGHTER_FLOAT_DAMAGE_UI_TRAINING], 3) 
						+ "(" + float_to_string(fighter[i]->object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI_TRAINING], 3) 
						+ "%)",
						glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
					training_info[!i].fields[TRAINING_FIELD_COMBO_DAMAGE].update_text(
						get_font("info"),
						"Total: " + float_to_string(
							fighter[!i]->object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING], 3
						), 
						glm::vec4(255.0), 
						glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
					int stun_frames = 0;
					int airtime = 0;
					std::string stun_frame_string = "Stun Frames: ";
					std::string airtime_string = "Airtime: ";
					switch (fighter[i]->status_kind) {
						case (FIGHTER_STATUS_HITSTUN_AIR): {
							airtime = fighter[i]->calc_launch_frames();
							[[fallthrough]];
						} break;
						default: {
							stun_frames = fighter[i]->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES];
						} break;
						case(FIGHTER_STATUS_PARRY): {

						} break;
						case (FIGHTER_STATUS_LAUNCH_START): {
							stun_frames = fighter[i]->get_anim_length("launch_start");
							[[fallthrough]];
						}
						case (FIGHTER_STATUS_LAUNCH): {
							airtime = fighter[i]->calc_launch_frames();
							stun_frames += airtime + fighter[i]->get_anim_length("knockdown_up");
							if (fighter[i]->object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
								stun_frames += fighter[i]->get_anim_length("wakeup_slow_up");
							}
							else {
								stun_frames += fighter[i]->get_anim_length("wakeup_up");
							}
						} break;
						case (FIGHTER_STATUS_KNOCKDOWN_START): {
							stun_frames = fighter[i]->get_anim_length("knockdown_start_up") + fighter[i]->get_anim_length("knockdown_up");
							if (fighter[i]->object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
								stun_frames += fighter[i]->get_anim_length("wakeup_slow_up");
							}
							else {
								stun_frames += fighter[i]->get_anim_length("wakeup_up");
							}
						} break;
					}
					stun_frame_string += std::to_string(stun_frames);
					airtime_string += std::to_string(airtime);
					int frame_advantage = stun_frames - fighter[!i]->get_frames_until_actionable();
					int air_frame_advantage = airtime - fighter[!i]->get_frames_until_actionable();
					fighter[!i]->object_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
					fighter[i]->object_int[FIGHTER_INT_FRAME_ADVANTAGE] = -frame_advantage;
					std::string advantage = std::to_string(frame_advantage);
					std::string air_advantage = std::to_string(air_frame_advantage);
					if (frame_advantage >= 0) {
						stun_frame_string += " (+" + advantage + ")";
					}
					else {
						stun_frame_string += " (" + advantage + ")";
					}
					if (air_frame_advantage >= 0) {
						airtime_string += " (+" + air_advantage + ")";
					}
					else {
						airtime_string += " (" + air_advantage + ")";
					}
					training_info[!i].fields[TRAINING_FIELD_STUN_FRAMES].update_text(
						get_font("info"),
						stun_frame_string,
						glm::vec4(255.0),
						glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
					training_info[!i].fields[TRAINING_FIELD_AIRTIME].update_text(
						get_font("info"),
						airtime_string,
						glm::vec4(255.0),
						glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
				}
			}
			objects[i][i2]->connected_hitbox = nullptr;
			objects[i][i2]->connected_grabbox = nullptr;
		}
	}



	if (game_context != GAME_CONTEXT_TRAINING && internal_state == BATTLE_STATE_BATTLE 
		&& (fighter[0]->object_float[FIGHTER_FLOAT_HEALTH] == 0.0
		|| fighter[1]->object_float[FIGHTER_FLOAT_HEALTH] == 0.0)) {
		internal_state = BATTLE_STATE_KO;
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