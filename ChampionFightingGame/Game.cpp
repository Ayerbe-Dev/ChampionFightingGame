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
		int hitbox_to_use = -1;
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
				for (int i3 = 0; i3 < 10; i3++) {
					if (player_info[!i]->hitboxes[i3].id != -1) {
						SDL_Rect hitbox;
						hitbox = player_info[!i]->hitboxes[i3].rect;
						if (is_collide(hitbox, hurtbox)) {
							hitbox_to_use = call_event_hit_collide(player_info[!i], player_info[i], &(player_info[!i]->hitboxes[i3]), &(player_info[i]->hurtboxes[i2]));
						}
						if (hitbox_to_use != -1) {
							break;
						}
					}
				}
				if (hitbox_to_use != -1) {
					break;
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
}

int call_event_hit_collide(PlayerInfo* attacker, PlayerInfo* defender, Hitbox *hitbox, Hurtbox *hurtbox) {
	//First, check if the hit and hurtboxes are even compatible

	if (hitbox->situation_hit != SITUATION_HIT_ALL) {
		if (hitbox->situation_hit != defender->situation_kind) {
			return -1;
		}
	}
	if (hitbox->hitbox_kind == HITBOX_KIND_BLOCK) {
		defender->chara_flag[CHARA_FLAG_PROX_GUARD] = true;
		return -1;
	}
	if (hurtbox->intangible_kind == hitbox->attack_height
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_NORMAL
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return -1;
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
		return hitbox->id;
	}

	attacker->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = true;
	return hitbox->id;
}