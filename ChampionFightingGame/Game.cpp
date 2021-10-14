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
	(*player_info).prev_stick_dir = (*player_info).get_stick_dir();
	if ((*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] --;
	}
	if ((*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] --;
	}
}

void check_attack_connections(PlayerInfo player_info[2], SDL_Renderer* renderer, bool visualize_boxes) {
	for (int i = 0; i < 2; i++) { //Secondary loop bc otherwise P2 renders on top of P1's hitbox visuals
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i].hurtboxes[i2].id != -1) {
				SDL_Rect hurtbox;
				hurtbox = player_info[i].hurtboxes[i2].rect;

				if (visualize_boxes) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(renderer, &hurtbox);
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 127);
					SDL_RenderFillRect(renderer, &hurtbox);
				}
				for (int i3 = 0; i3 < 10; i3++) {
					if (player_info[!i].hitboxes[i3].id != -1) {
						SDL_Rect hitbox;
						hitbox = player_info[!i].hitboxes[i3].rect;
						if (is_collide(hitbox, hurtbox)) {
							(&player_info)[!i]->clear_hitbox_all();
							break;
						}
					}
				}
			}
		}
		for (int i2 = 0; i2 < 10; i2++) {
			if (player_info[i].hitboxes[i2].id != -1) {
				SDL_Rect render_pos;
				render_pos = player_info[i].hitboxes[i2].rect;

				if (visualize_boxes) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(renderer, &render_pos);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
					SDL_RenderFillRect(renderer, &render_pos);
				}
			}
		}
	}
}