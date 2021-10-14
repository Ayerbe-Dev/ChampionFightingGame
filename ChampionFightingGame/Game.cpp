#include <iostream>
#include <functional>
#include <vector>
#include<string>
#include "PlayerInfo.h"
#include <SDL.h>
#include "Animation.h"
#include "Game.h"

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.

void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer) {
	
	 /*
				   _.-, 
              _ .-'  / .._
           .-:'/ - - \:::::-.
         .::: '  e e  ' '-::::.
        ::::'(    ^    )_.::::::
       ::::.' '.  o   '.::::'.'/_
   .  :::.'       -  .::::'_   _.:
 .-''---' .'|      .::::'   '''::::
'. ..-:::'  |    .::::'        ::::
 '.' ::::    \ .::::'          ::::
      ::::   .::::'           ::::
       ::::.::::'._          ::::
        ::::::' /  '-      .::::
         '::::-/__    __.-::::'
           '-::::::::::::::-'
               '''::::'''
	 */

	

	//Calls the looping status function for whatever the player's current status_kind is.
	player_info->playoutStatus();

	/*
		Get the player's inputs and increment the frame.
	*/

	player_info->processInput();
	if (player_info->canStep()) {
		player_info->stepAnimation();
	}
	player_info->prev_stick_dir = player_info->get_stick_dir();
	if (player_info->chara_int[CHARA_INT_HITLAG_FRAMES] != 0) {
		player_info->chara_int[CHARA_INT_HITLAG_FRAMES] --;
	}
	else if (player_info->chara_int[CHARA_INT_HITSTUN_FRAMES] != 0) {
		player_info->chara_int[CHARA_INT_HITSTUN_FRAMES] --;
	}
	if (player_info->chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		player_info->chara_int[CHARA_INT_DASH_F_WINDOW] --;
	}
	if (player_info->chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		player_info->chara_int[CHARA_INT_DASH_B_WINDOW] --;
	}
}

void check_attack_connections(PlayerInfo *p1, PlayerInfo *p2, SDL_Renderer* renderer, bool visualize_boxes) {
	PlayerInfo* player_info[2] = { p1, p2 };
	for (int i = 0; i < 2; i++) { //Secondary loop bc otherwise P2 renders on top of P1's hitbox visuals
		int hitbox_to_use = HITBOX_COUNT_MAX;
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i]->hurtboxes[i2].id != -1) {
				SDL_Rect hurtbox;
				hurtbox = player_info[i]->hurtboxes[i2].rect;

				for (int i3 = 0; i3 < 10; i3++) {
					if (player_info[!i]->hitboxes[i3].id != -1 && !player_info[!i]->hitboxes[i3].success_hit) {
						SDL_Rect hitbox;
						hitbox = player_info[!i]->hitboxes[i3].rect;
						if (is_collide(hitbox, hurtbox)) {
							hitbox_to_use = get_event_hit_collide(player_info[!i], player_info[i], &(player_info[!i]->hitboxes[i3]), &(player_info[i]->hurtboxes[i2]));
						}
						if (hitbox_to_use != HITBOX_COUNT_MAX) {
							break;
						}
					}
				}
				if (hitbox_to_use != HITBOX_COUNT_MAX) {
					break;
				}
			}
		}
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i]->hurtboxes[i2].id != -1) {
				SDL_Rect hurtbox;
				hurtbox = player_info[i]->hurtboxes[i2].rect;

				if (visualize_boxes) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(renderer, &hurtbox);
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 127);
					SDL_RenderFillRect(renderer, &hurtbox);
				}
			}
		}
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i]->hitboxes[i2].id != -1) {
				SDL_Rect render_pos;
				render_pos = player_info[i]->hitboxes[i2].rect;

				if (visualize_boxes) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(renderer, &render_pos);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
					SDL_RenderFillRect(renderer, &render_pos);
				}
			}
		}
		player_info[i]->connected_hitbox = hitbox_to_use;
	}
	event_hit_collide(player_info[0], player_info[1], &(player_info[0]->hitboxes[player_info[0]->connected_hitbox]), &(player_info[1]->hitboxes[player_info[1]->connected_hitbox]));
}

int get_event_hit_collide(PlayerInfo* attacker, PlayerInfo* defender, Hitbox *hitbox, Hurtbox *hurtbox) {
	//First, check if the hit and hurtboxes are even compatible

	if (hitbox->situation_hit != SITUATION_HIT_ALL) {
		if (hitbox->situation_hit != defender->situation_kind) {
			return HITBOX_COUNT_MAX;
		}
	}
	if (hitbox->hitbox_kind == HITBOX_KIND_BLOCK) {
		defender->chara_flag[CHARA_FLAG_PROX_GUARD] = true;
		return HITBOX_COUNT_MAX;
	}
	if (hurtbox->intangible_kind == hitbox->attack_height
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_NORMAL
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}

	//Then, check if the hurtbox is invincible. If it is, the attacker's entire attack has failed. This will be pretty rare tbh.

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		return hitbox->id;
	}

	//If the hurtbox has armor, both parties get SOME hitlag to acknowledge the move hitting

	if (hurtbox->is_armor) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		return hitbox->id;
	}

	bool blocking = false;
	bool parrying = false;
	if (defender->is_actionable() || defender->status_kind == CHARA_STATUS_BLOCKSTUN) {
		if (defender->get_stick_dir() == 4 && hitbox->attack_height != ATTACK_HEIGHT_LOW) {
			blocking = true;
		}
		if (defender->get_stick_dir() == 1 && hitbox->attack_height != ATTACK_HEIGHT_HIGH) {
			blocking = true;
		}
	}
	if (defender->status_kind == CHARA_STATUS_PARRY) {
		if (defender->chara_int[CHARA_INT_PARRY_HEIGHT] == hitbox->attack_height) {
			parrying = true;
		}
	}
	if (parrying) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag + 12;
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag + 8;
		defender->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = true;
		return hitbox->id;
	}
	if (blocking && !hitbox->unblockable) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	attacker->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = true;
	attacker->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
	return hitbox->id;
}

/*
	If this function is triggered and a player didn't hit the opponent, the hitbox arg for that player should be HITBOX_COUNT_MAX. Since this hitbox
	will never actually be used, its ID will always be 0.
*/
void event_hit_collide(PlayerInfo* p1, PlayerInfo* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
	if (p1_hitbox->id != -1 && p2_hitbox->id != -1) { //Both players got hit

	}
	else if (p1_hitbox->id != -1) { //P2 got hit
		p1_hitbox->success_hit = true; //This hitbox may not connect again
		if (p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2->stats.meter_gain_on_parry;
			p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p2->change_status(CHARA_STATUS_PARRY);
		}
		else if (p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->chip_damage;
			p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p2->change_status(CHARA_STATUS_BLOCKSTUN);
		}
		else if (!p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED]) {
			//If the opponent didn't block but this flag is still false, that means the opponent armored through it
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage / 2;
		}
		else {
			p2->chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED] = p1_hitbox->launch_init_y;
			p2->chara_float[CHARA_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->chara_float[CHARA_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->chara_float[CHARA_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x;
			if (can_counterhit(p2, p1_hitbox)) {
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_counterhit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * p1_hitbox->counterhit_damage_mul;
				p2->change_status(get_damage_status(p1_hitbox->counterhit_status));
			}
			else {
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_hit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage;
				p2->change_status(get_damage_status(p1_hitbox->hit_status));
			}
			p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
		}
	}
	else if (p2_hitbox->id != -1) { //P1 got hit
		p2_hitbox->success_hit = true; //This hitbox may not connect again
		if (p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1->stats.meter_gain_on_parry;
			p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p1->change_status(CHARA_STATUS_PARRY);
		}
		else if (p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->chip_damage;
			p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p1->change_status(CHARA_STATUS_BLOCKSTUN);
		}
		else if (!p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED]) {
			//If the opponent didn't block but this flag is still false, that means the opponent armored through it
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage / 2;
		}
		else {
			p1->chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED] = p2_hitbox->launch_init_y;
			p1->chara_float[CHARA_FLOAT_LAUNCH_GRAVITY] = p2_hitbox->launch_gravity_y;
			p1->chara_float[CHARA_FLOAT_LAUNCH_FALL_SPEED_MAX] = p2_hitbox->launch_max_fall_speed;
			p1->chara_float[CHARA_FLOAT_LAUNCH_SPEED_X] = p2_hitbox->launch_speed_x;
			if (can_counterhit(p1, p2_hitbox)) {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_counterhit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage * p2_hitbox->counterhit_damage_mul;
				p1->change_status(get_damage_status(p2_hitbox->counterhit_status));
			}
			else {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_hit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage;
				p1->change_status(get_damage_status(p2_hitbox->hit_status));
			}
			p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
		}
	}
	else { //No one got hit
		return;
	}
}

bool can_counterhit(PlayerInfo* defender, Hitbox* hitbox) {
	return false;
}

int get_damage_status(int hit_status) {
	if (hit_status == HIT_STATUS_CRUMPLE) {
		return CHARA_STATUS_CRUMPLE;
	}
	else if (hit_status == HIT_STATUS_LAUNCH) {
		return CHARA_STATUS_LAUNCH_START;
	}
	else {
		return CHARA_STATUS_HITSTUN;
	}
}