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
			if (objects[i][i2]->post_collision_status != FIGHTER_STATUS_NONE) {
				//Since change_status resets the punish flag, we have to put this code here. It's
				//kinda gross because of the double fighter check, but ideally once the UIText 
				//system is implemented this will be handled directly from the fighter collision
				//so I'll allow it for now.
				if (objects[i][i2]->object_type == BATTLE_OBJECT_TYPE_FIGHTER && fighter[i]->fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
					texts[!i].push_back(BattleText());
					texts[!i].back().init(&message_font, "Punish", 40, fighter[!i], glm::vec2(275.0, 300.0));
				}

				objects[i][i2]->change_status(objects[i][i2]->post_collision_status, true);
				objects[i][i2]->post_collision_status = FIGHTER_STATUS_NONE;

				if (objects[i][i2]->object_type != BATTLE_OBJECT_TYPE_FIGHTER
				|| (fighter[i]->connected_grabbox == nullptr 
					&& fighter[i]->connected_hitbox == nullptr
					&& !fighter[i]->fighter_flag[FIGHTER_FLAG_APPLIED_DEFINITE_HITBOX])) continue;
				fighter[i]->fighter_flag[FIGHTER_FLAG_APPLIED_DEFINITE_HITBOX] = false;
				if (fighter[!i]->fighter_int[FIGHTER_INT_COMBO_COUNT] > 1) {
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
				if (game_context == GAME_CONTEXT_TRAINING) {
					training_info[!i].hit_frame.update_text(info_font, "Hit Frame: " + std::to_string(fighter[!i]->fighter_int[FIGHTER_INT_EXTERNAL_FRAME] + 1), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					training_info[!i].damage.update_text(info_font,
						"Damage: " + float_to_string(fighter[i]->fighter_float[FIGHTER_FLOAT_LAST_DAMAGE], 3) + "(" + float_to_string(fighter[i]->fighter_float[FIGHTER_FLOAT_LAST_DAMAGE_SCALE] * 100.0, 3) + "%)",
						glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0)
					);
					training_info[!i].combo_damage.update_text(info_font, "Total: " + float_to_string(fighter[!i]->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE], 3), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					int stun_frames = 0;
					switch (fighter[i]->status_kind) {
					default: {
						stun_frames = fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES];
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					case(FIGHTER_STATUS_PARRY): {
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: 0", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					case (FIGHTER_STATUS_LAUNCH_START): {
						stun_frames = fighter[i]->get_anim_length("launch_start");
						[[fallthrough]];
					}
					case (FIGHTER_STATUS_LAUNCH): {
						stun_frames += fighter[i]->calc_launch_frames();
						int stun_frames_slow = stun_frames + fighter[i]->get_anim_length("wakeup_up") + fighter[i]->get_anim_length("knockdown_up");
						stun_frames += fighter[i]->get_anim_length("wakeup_fast_up");
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					case (FIGHTER_STATUS_KNOCKDOWN_START): {
						stun_frames = fighter[i]->get_anim_length("knockdown_start_up");
						int stun_frames_slow = stun_frames + fighter[i]->get_anim_length("wakeup_up") + fighter[i]->get_anim_length("knockdown_up");
						stun_frames += fighter[i]->get_anim_length("wakeup_fast_up");
						training_info[!i].stun_frames.update_text(info_font, "Stun Frames: " + std::to_string(stun_frames) + "/" + std::to_string(stun_frames_slow), glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					} break;
					}
					int frame_advantage = stun_frames - fighter[!i]->get_frames_until_actionable();
					fighter[!i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] = frame_advantage;
				}
			}
			objects[i][i2]->connected_hitbox = nullptr;
			objects[i][i2]->connected_grabbox = nullptr;
		}
	}



	if (game_context != GAME_CONTEXT_TRAINING && internal_state == BATTLE_STATE_BATTLE 
		&& (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0
		|| fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0)) {
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