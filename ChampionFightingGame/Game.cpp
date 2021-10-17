#include <iostream>
#include <functional>
#include <vector>
#include<string>
#include "PlayerInfo.h"
#include <SDL.h>
#include "Animation.h"
#include "Game.h"
#include "Roy.fwd.h"
#include "Roy.h"
#include "Eric.fwd.h"
#include "Eric.h"

IFighter::IFighter(int chara_id, SDL_Renderer *renderer, int id) {
	switch (chara_id) {
		case(CHARA_KIND_ROY): {
			player_info = new Roy(renderer, id);
		} break;
		case(CHARA_KIND_ERIC): {
			player_info = new Eric(renderer, id);
		} break;
		case (CHARA_KIND_MAX): {
			player_info = NULL;
		} break;
	}
}

IFighter::~IFighter() {
	if (player_info) {
		delete[] player_info;
		player_info = NULL;
	}
}

PlayerInfo *IFighter::get_fighter() {
	return player_info;
}

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

	player_info->prevpos = player_info->pos;

	//Calls the looping status function for whatever the player's current status_kind is.
	player_info->playoutStatus();

	/*
		Get the player's inputs and increment the frame.
	*/

	if (player_info->check_button_on(BUTTON_START)) {
		player_info->superInit(player_info->id, renderer); //Debugging
	}
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
		player_info[i]->chara_flag[CHARA_FLAG_PROX_GUARD] = false;
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i]->hurtboxes[i2].id != -1) {
				SDL_Rect hurtbox;
				hurtbox = player_info[i]->hurtboxes[i2].rect;

				for (int i3 = 0; i3 < 10; i3++) {
					if (player_info[!i]->hitboxes[i3].id != -1 && !player_info[!i]->hitboxes[i3].success_hit) {
						SDL_Rect hitbox;
						hitbox = player_info[!i]->hitboxes[i3].rect;
						if (player_info[!i]->hitboxes[i3].hitbox_kind != HITBOX_KIND_BLOCK) {
							if (is_collide(hitbox, hurtbox)) {
								hitbox_to_use = get_event_hit_collide_player(player_info[!i], player_info[i], &(player_info[!i]->hitboxes[i3]), &(player_info[i]->hurtboxes[i2]));
							}
						}
						else {
							player_info[i]->chara_flag[CHARA_FLAG_PROX_GUARD] = is_collide(hitbox, hurtbox);
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
				SDL_Rect render_pos;
				render_pos = player_info[i]->hurtboxes[i2].rect;

				if (visualize_boxes) {
					Vec4f hurtbox_color = { 0, 0, 255, 127 };
					if (player_info[i]->hurtboxes[i2].intangible_kind != INTANGIBLE_KIND_NONE) {
						hurtbox_color.y = 255;
					}

					SDL_SetRenderDrawColor(renderer, hurtbox_color.x, hurtbox_color.y, hurtbox_color.z, 255);
					SDL_RenderDrawRect(renderer, &render_pos);
					SDL_SetRenderDrawColor(renderer, hurtbox_color.x, hurtbox_color.y, hurtbox_color.z, hurtbox_color.w);
					SDL_RenderFillRect(renderer, &render_pos);
				}
			}
		}
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i]->hitboxes[i2].id != -1) {
				SDL_Rect render_pos;
				render_pos = player_info[i]->hitboxes[i2].rect;

				if (visualize_boxes) {
					Vec4f hitbox_color = { 255, 0, 0, 127 };
					if (player_info[i]->hitboxes[i2].hitbox_kind == HITBOX_KIND_BLOCK) {
						hitbox_color.y = 165;
						hitbox_color.w = 50;
					}

					SDL_SetRenderDrawColor(renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, 255);
					SDL_RenderDrawRect(renderer, &render_pos);
					SDL_SetRenderDrawColor(renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, hitbox_color.w);
					SDL_RenderFillRect(renderer, &render_pos);
				}
			}
		}
		player_info[i]->connected_hitbox = hitbox_to_use;
	}
	event_hit_collide_player(player_info[0], player_info[1], &(player_info[0]->hitboxes[player_info[1]->connected_hitbox]), &(player_info[1]->hitboxes[player_info[0]->connected_hitbox]));
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

int get_event_hit_collide_player(PlayerInfo* attacker, PlayerInfo* defender, Hitbox *hitbox, Hurtbox *hurtbox) {
	//First, check if the hit and hurtboxes are even compatible
	if (hitbox->success_hit) {
		return HITBOX_COUNT_MAX;
	}

	if (hitbox->situation_hit != SITUATION_HIT_ALL) {
		if (hitbox->situation_hit != SITUATION_HIT_GROUND_AIR) {
			if (hitbox->situation_hit != defender->situation_kind) {
				return HITBOX_COUNT_MAX;
			}
		}
		else if (defender->situation_kind == CHARA_SITUATION_DOWN) {
			return HITBOX_COUNT_MAX;
		}
	}
	if (hurtbox->intangible_kind == hitbox->attack_height
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_NORMAL
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->situation_kind == CHARA_SITUATION_AIR && hitbox->max_juggle < defender->chara_int[CHARA_INT_JUGGLE_VALUE]) {
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
	if (defender->status_kind == CHARA_STATUS_PARRY_START && defender->chara_flag[CHARA_FLAG_PARRY_ACTIVE]) {
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
		defender->chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	attacker->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = true;
	return hitbox->id;
}

void event_hit_collide_player(PlayerInfo* p1, PlayerInfo* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
	bool p1_hit = p2_hitbox->id != -1;
	bool p2_hit = p1_hitbox->id != -1;
	u32 p1_status_post_hit = p1->status_kind;
	u32 p2_status_post_hit = p2->status_kind;
	if (p1_hit && p2_hit) { //Both players got hit
		if (p1->situation_kind != p2->situation_kind) {
			if (p1->situation_kind == CHARA_SITUATION_GROUND) { 
				/*
					Grounded opponents will always win trades against aerial opponents. This is important for balancing because holy shit jumping is
					gonna be busted without stuff like this

					Oh also, no need to check OTG because there won't be any attacks you can use outside of CHARA_SITUATION_GROUND or 
					CHARA_SITUATION_AIR
				*/
				p1_hit = false;
			}
			else {
				p2_hit = false;
			}
		}
		else if (p1_hitbox->attack_level == p2_hitbox->attack_level) {
			if (p1_hitbox->clank_kind == CLANK_KIND_CONTINUE || p2_hitbox->clank_kind == CLANK_KIND_CONTINUE) {
				if (p2_hitbox->clank_kind != CLANK_KIND_CONTINUE) {
					p2->change_status(CHARA_STATUS_CLANK);
					return;
				}
				else if (p1_hitbox->clank_kind != CLANK_KIND_CONTINUE) {
					p1->change_status(CHARA_STATUS_CLANK);
					return;
				}
				else { //If both people have trample, it's better to make them both clank than it is to make them both ignore having just been hit
					p1->change_status(CHARA_STATUS_CLANK);
					p2->change_status(CHARA_STATUS_CLANK);
					return;
				}
			}
			else if (p1_hitbox->clank_kind == CLANK_KIND_CLANK || p2_hitbox->clank_kind == CLANK_KIND_CLANK) {
				p1->change_status(CHARA_STATUS_CLANK);
				p2->change_status(CHARA_STATUS_CLANK);
				return;
			}
			//if both players have CLANK_KIND_NORMAL as well as using the same attack level, they can just trade and both take damage
		}
		else if (p1_hitbox->attack_level > p2_hitbox->attack_level) {
			p1_hit = false;
		}
		else {
			p2_hit = false;
		}
	}
	if (p2_hit) {
		p1->stepAnimation();
		p1->frame++;
		p1->update_hitbox_connect();
		if (p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2->stats.meter_gain_on_parry;
			p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = CHARA_STATUS_PARRY;
		}
		else if (p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->chip_damage;
			if (p2->invalid_x(p2->pos.x - p1_hitbox->block_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES] * p2->facing_dir)) {
				p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = (p1_hitbox->block_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES]) / 2.0;
			}
			else {
				p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
			}
			p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p2_status_post_hit = CHARA_STATUS_BLOCKSTUN;
		}
		else if (!p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage / 2;
		}
		else {
			/*
				If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
				is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
			*/
			if (p2->get_status_group(p2->status_kind) == STATUS_GROUP_HITSTUN) { 
				if (!p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
					p1->chara_int[CHARA_INT_DAMAGE_SCALE] += p1_hitbox->scale;
				}
			}
			else {
				p1->chara_int[CHARA_INT_DAMAGE_SCALE] = 0;
			}
			p2->chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED] = p1_hitbox->launch_init_y;
			p2->chara_float[CHARA_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->chara_float[CHARA_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->chara_float[CHARA_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x;
			/*
			If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise, 
			increase it by one so that the opponent's juggle value is always going up
			*/
			if (p2->chara_int[CHARA_INT_JUGGLE_VALUE] >= p1_hitbox->juggle_set) {
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] ++;
			}
			else {
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] = p1_hitbox->juggle_set;
			}
			if (p2->invalid_x(p2->pos.x - p1_hitbox->hit_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES] * p2->facing_dir)) {
				p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = (p1_hitbox->hit_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES]) / 2.0;
			}
			else {
				p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
			}
			if (can_counterhit(p2, p1_hitbox)) {
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_counterhit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * p1_hitbox->counterhit_damage_mul;
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
				p2->chara_int[CHARA_INT_HITSTUN_FRAMES] *= 1.2;
				p2->chara_int[CHARA_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
			}
			else {
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_hit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * ((f32)(clamp(1, 10 - p1->chara_int[CHARA_INT_DAMAGE_SCALE], 15)) / 10);
				p2->chara_int[CHARA_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
			}
			p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
			p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
		}
	}

	if (p1_hit) { //P1 got hit
		p2->stepAnimation();
		p2->frame++;
		p2->update_hitbox_connect();
		if (p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1->stats.meter_gain_on_parry;
			p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p1_status_post_hit = CHARA_STATUS_PARRY;
		}
		else if (p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->chip_damage;
			if (p1->invalid_x(p1->pos.x - p2_hitbox->block_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES] * p1->facing_dir)) {
				p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = (p2_hitbox->block_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES]) / 2.0;
			}
			else {
				p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->block_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES];
			}
			p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p1_status_post_hit = CHARA_STATUS_BLOCKSTUN;
		}
		else if (!p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage / 2;
		}
		else {
			if (p1->get_status_group(p2->status_kind) == STATUS_GROUP_HITSTUN) {
				if (!p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
					p2->chara_int[CHARA_INT_DAMAGE_SCALE] += p2_hitbox->scale;
				}
			}
			else {
				p2->chara_int[CHARA_INT_DAMAGE_SCALE] = 0;
			}
			p1->chara_float[CHARA_FLOAT_INIT_LAUNCH_SPEED] = p2_hitbox->launch_init_y;
			p1->chara_float[CHARA_FLOAT_LAUNCH_GRAVITY] = p2_hitbox->launch_gravity_y;
			p1->chara_float[CHARA_FLOAT_LAUNCH_FALL_SPEED_MAX] = p2_hitbox->launch_max_fall_speed;
			p1->chara_float[CHARA_FLOAT_LAUNCH_SPEED_X] = p2_hitbox->launch_speed_x;
			if (p1->chara_int[CHARA_INT_JUGGLE_VALUE] >= p2_hitbox->juggle_set) {
				p1->chara_int[CHARA_INT_JUGGLE_VALUE] ++;
			}
			else {
				p1->chara_int[CHARA_INT_JUGGLE_VALUE] = p2_hitbox->juggle_set;
			}
			if (p1->invalid_x(p1->pos.x - p2_hitbox->hit_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES] * p1->facing_dir)) {
				p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = (p2_hitbox->hit_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES]) / 2.0;
			}
			else {
				p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->hit_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES];
			}
			if (can_counterhit(p1, p2_hitbox)) {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_counterhit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage * p2_hitbox->counterhit_damage_mul;
				p1->chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
				p1->chara_int[CHARA_INT_HITSTUN_FRAMES] *= 1.2;
				p1->chara_int[CHARA_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p1_status_post_hit = get_damage_status(p2_hitbox->counterhit_status, p1->situation_kind);
			}
			else {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_hit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage * ((f32)(clamp(1, 10 - p2->chara_int[CHARA_INT_DAMAGE_SCALE], 15)) / 10.0);
				p1->chara_int[CHARA_INT_HITSTUN_LEVEL] = p2_hitbox->attack_level;
				p1_status_post_hit = get_damage_status(p2_hitbox->hit_status, p1->situation_kind);
			}
			p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
			p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
		}
	}
	if (p1_hit) {
		p1->change_status(p1_status_post_hit, true, false);
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
	}
}

bool can_counterhit(PlayerInfo* defender, Hitbox* hitbox) {
	if (defender->status_kind == CHARA_STATUS_HITSTUN_PARRY) {
		hitbox->scale = -5;
	}
	return defender->chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT];
}

int get_damage_status(int hit_status, int situation_kind) {
	if (hit_status == HIT_STATUS_CRUMPLE) {
		if (situation_kind == CHARA_SITUATION_AIR) {
			return CHARA_STATUS_LAUNCH;
		}
		else {
			return CHARA_STATUS_CRUMPLE;
		}
	}
	else if (hit_status == HIT_STATUS_LAUNCH) {
		if (situation_kind == CHARA_SITUATION_AIR) {
			return CHARA_STATUS_LAUNCH;
		}
		else {
			return CHARA_STATUS_LAUNCH_START;
		}
	}
	else if (hit_status == HIT_STATUS_KNOCKDOWN) {
		return CHARA_STATUS_KNOCKDOWN_START;
	}
	else {
		if (situation_kind == CHARA_SITUATION_AIR) {
			return CHARA_STATUS_HITSTUN_AIR;
		}
		else {
			return CHARA_STATUS_HITSTUN;
		}
	}
}