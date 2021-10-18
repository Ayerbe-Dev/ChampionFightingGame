#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include "FighterInstance.h"
#include <SDL.h>
#include "Animation.h"
#include "Game.h"
#include "Roy.fwd.h"
#include "Roy.h"
#include "Eric.fwd.h"
#include "Eric.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "Menu.h"
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern u32 tick;
extern u32 tok;
extern int error_render;

int game_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]) {
	bool gaming = true;
	bool visualize_boxes = true;

	Debugger debugger;
	debugger = Debugger();
	SDL_Rect debug_rect[2] = { 0, 0, 0, 0 };

	GameCoordinate debug_anchor[2];
	GameCoordinate debug_offset[2];

	//init stage
	Stage stage = Stage(pRenderer, "training_room");

	SDL_Texture* pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Rect camera;

	//init players
	FighterInstance* fighter_instance[2];

	IFighter* p1 = new IFighter((&player_info[0])->chara_kind, pRenderer, 0);
	IFighter* p2 = new IFighter((&player_info[1])->chara_kind, pRenderer, 1);

	fighter_instance[0] = p1->get_fighter();
	fighter_instance[1] = p2->get_fighter();

	//init ui
	Timer timer = Timer(pRenderer, -1);

	HealthBar health_bar[2];
	health_bar[0] = HealthBar(pRenderer, fighter_instance[0]);
	health_bar[1] = HealthBar(pRenderer, fighter_instance[1]);

	PlayerIndicator player_indicator[2];
	player_indicator[0] = PlayerIndicator(pRenderer, fighter_instance[0]);
	player_indicator[1] = PlayerIndicator(pRenderer, fighter_instance[1]);

	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	while (gaming) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return GAME_STATE_CLOSE;
				} break;
			}
		}

		//Frame delay

		tok = SDL_GetTicks() - tick;
		if (tok < TICK_RATE_MS) {
			SDL_Delay(TICK_RATE_MS - tok);
		}
		tick = SDL_GetTicks();

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		//Check the players' buttons
		for (int i = 0; i < 2; i++) {
			if (!debug) {
				/*
				Frame advance would make it so that check_button_trigger is never true during debug mode if it got checked here, so we just check the inputs
				directly when the frame is advanced manually
				*/
				(&player_info[i])->update_buttons(keyboard_state);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		/*
			Rendering. The method for rendering relies on setting render targets. pScreenTexture is where all the scalable
			textures are rendered. The GUI will be applied after the render target has been set back to the window.
			After RenderTarget has been set, nothing has to be done with pScreenTexture untill it is time to SDL_Present
			the content in pScreenTexture.
		*/

		SDL_RenderClear(pRenderer);
		SDL_SetRenderTarget(pRenderer, pScreenTexture);
		SDL_RenderCopy(pRenderer, stage.pBackgroundTexture, nullptr, nullptr);

		//All of the player code runs here

		/*
		Start by flipping the characters.It's important that both characters get flipped before anything else happens because facing direction will affect the players'
		inputs
		*/
		for (int i = 0; i < 2; i++) {
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (fighter_instance[i]->situation_kind == CHARA_SITUATION_GROUND && fighter_instance[i]->is_actionable()) {
				if (fighter_instance[i]->pos.x > fighter_instance[!i]->pos.x) {
					fighter_instance[i]->facing_dir = -1.0;
					fighter_instance[i]->facing_right = false;
					flip = SDL_FLIP_HORIZONTAL;
				}
				else {
					fighter_instance[i]->facing_dir = 1.0;
					fighter_instance[i]->facing_right = true;
				}
			}
			else if (!fighter_instance[i]->facing_right) {
				flip = SDL_FLIP_HORIZONTAL;
			}

			//Debug mode
			if (debugger.check_button_trigger(BUTTON_DEBUG_ENABLE) && i == 0) {
				debug = !debug;
			}
			if (!debug) {
				tickOnce(fighter_instance[i], pRenderer);
				frame_advance_entry_ms = SDL_GetTicks();
			}
			else if (i == 0) {
				frame_advance_ms = SDL_GetTicks() - frame_advance_entry_ms;
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_1)) {
					debugger.target = 0;
				}
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_2)) {
					debugger.target = 1;
				}
				if (debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)) {
					for (int o = 0; o < BUTTON_MAX; o++) {
						(&player_info[i])->update_buttons(keyboard_state);
					}
					frame_advance_entry_ms = SDL_GetTicks();
					tickOnce(fighter_instance[0], pRenderer);
					tickOnce(fighter_instance[1], pRenderer);
					if (debugger.print_frames) {
						cout << "Player " << debugger.target + 1 << " Frame: " << fighter_instance[debugger.target]->frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Render Frame: " << fighter_instance[debugger.target]->render_frame - 1 << endl;
					}
				}
				debug_mode(&debugger, fighter_instance[debugger.target], pRenderer, &debug_rect[debugger.target], &debug_anchor[debugger.target], &debug_offset[debugger.target]);
				if (debugger.check_button_trigger(BUTTON_DEBUG_RESET)) {
					debug = false;
					gaming = false;
				}
			}

			SDL_Rect render_pos;
			render_pos.x = fighter_instance[i]->pos.getRenderCoodrinateXFacingDir(fighter_instance[i]->facing_dir);
			render_pos.y = fighter_instance[i]->pos.getRenderCoodrinateY();
			int width;
			int height;
			SDL_QueryTexture(fighter_instance[i]->anim_kind->SPRITESHEET, NULL, NULL, &width, &height);
			render_pos.w = (width / (fighter_instance[i]->anim_kind->length + 1));
			render_pos.h = height;
			const double angle = 0;
			error_render = SDL_RenderCopyEx(pRenderer, fighter_instance[i]->anim_kind->SPRITESHEET, &(fighter_instance[i]->frame_rect), &render_pos, angle, NULL, flip);
			if (error_render != 0) {
				cout << "\n"
					<< SDL_GetError();
			}
		}
		// Main character function and textures have now been set up

		//Only do this part once everything else is done
		check_attack_connections(fighter_instance[0], fighter_instance[1], pRenderer, visualize_boxes);

		//Camera things
		camera = updateCamera(fighter_instance[0]->pos.getRenderCoodrinateX(), fighter_instance[0]->pos.getRenderCoodrinateY(), fighter_instance[1]->pos.getRenderCoodrinateX(), fighter_instance[1]->pos.getRenderCoodrinateY());

		for (int i = 0; i < 2; i++) {
			player_indicator[i].indicator_rect = SDL_Rect{
				(int)(fighter_instance[i]->pos.getRenderCoodrinateX() + 20),
				(int)(fighter_instance[i]->pos.getRenderCoodrinateY() - 33),
				30,
				30
			};
			SDL_RenderCopy(pRenderer, player_indicator[i].texture, nullptr, &(player_indicator[i].indicator_rect));
		}

		SDL_SetRenderTarget(pRenderer, nullptr);

		SDL_RenderCopy(pRenderer, pScreenTexture, &camera, nullptr);

		SDL_RenderCopy(pRenderer, timer.texture, nullptr, &(timer.timer_rect));

		//if the static cast is so bad, just... don't do it?
		for (int i = 0; i < 2; i++) {
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (i == 1) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			const double angle = 0;

			SDL_RenderCopyEx(pRenderer, health_bar[i].bar_texture, nullptr, &(health_bar[i].bar_rect), angle, NULL, flip);
			health_bar[i].health_rect.w = 400 * (fighter_instance[i]->chara_float[CHARA_FLOAT_HEALTH] / health_bar[i].max_health);
			SDL_RenderCopyEx(pRenderer, health_bar[i].health_texture, nullptr, &(health_bar[i].health_rect), angle, NULL, flip);
		}

		SDL_RenderPresent(pRenderer);
	}

	return GAME_STATE_MENU;
}

void tickOnce(FighterInstance* fighter_instance, SDL_Renderer* renderer) {
	
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

	fighter_instance->prevpos = fighter_instance->pos;

	//Calls the looping status function for whatever the player's current status_kind is.
	fighter_instance->playoutStatus();

	/*
		Get the player's inputs and increment the frame.
	*/

	if (fighter_instance->check_button_on(BUTTON_START)) {
		fighter_instance->superInit(fighter_instance->id, renderer); //Debugging
	}
	fighter_instance->processInput();
	if (fighter_instance->canStep()) {
		fighter_instance->stepAnimation();
	}
	fighter_instance->prev_stick_dir = fighter_instance->get_stick_dir();
	if (fighter_instance->chara_int[CHARA_INT_HITLAG_FRAMES] != 0) {
		fighter_instance->chara_int[CHARA_INT_HITLAG_FRAMES] --;
	}
	else if (fighter_instance->chara_int[CHARA_INT_HITSTUN_FRAMES] != 0) {
		fighter_instance->chara_int[CHARA_INT_HITSTUN_FRAMES] --;
	}
	if (fighter_instance->chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		fighter_instance->chara_int[CHARA_INT_DASH_F_WINDOW] --;
	}
	if (fighter_instance->chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		fighter_instance->chara_int[CHARA_INT_DASH_B_WINDOW] --;
	}
}

void check_attack_connections(FighterInstance *p1, FighterInstance *p2, SDL_Renderer* renderer, bool visualize_boxes) {
	FighterInstance* fighter_instance[2] = { p1, p2 };
	for (int i = 0; i < 2; i++) { //Secondary loop bc otherwise P2 renders on top of P1's hitbox visuals
		int hitbox_to_use = HITBOX_COUNT_MAX;
		fighter_instance[i]->chara_flag[CHARA_FLAG_PROX_GUARD] = false;
		for (int i2 = 0; i2 < 10; i2++) {
			if (fighter_instance[i]->hurtboxes[i2].id != -1) {
				SDL_Rect hurtbox;
				hurtbox = fighter_instance[i]->hurtboxes[i2].rect;

				for (int i3 = 0; i3 < 10; i3++) {
					if (fighter_instance[!i]->hitboxes[i3].id != -1 && !fighter_instance[!i]->hitboxes[i3].success_hit) {
						SDL_Rect hitbox;
						hitbox = fighter_instance[!i]->hitboxes[i3].rect;
						if (fighter_instance[!i]->hitboxes[i3].hitbox_kind != HITBOX_KIND_BLOCK) {
							if (is_collide(hitbox, hurtbox)) {
								hitbox_to_use = get_event_hit_collide_player(fighter_instance[!i], fighter_instance[i], &(fighter_instance[!i]->hitboxes[i3]), &(fighter_instance[i]->hurtboxes[i2]));
							}
						}
						else {
							fighter_instance[i]->chara_flag[CHARA_FLAG_PROX_GUARD] = is_collide(hitbox, hurtbox);
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
			if (fighter_instance[i]->hurtboxes[i2].id != -1) {
				SDL_Rect render_pos;
				render_pos = fighter_instance[i]->hurtboxes[i2].rect;

				if (visualize_boxes) {
					Vec4f hurtbox_color = { 0, 0, 255, 127 };
					if (fighter_instance[i]->hurtboxes[i2].intangible_kind != INTANGIBLE_KIND_NONE) {
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
			if (fighter_instance[i]->hitboxes[i2].id != -1) {
				SDL_Rect render_pos;
				render_pos = fighter_instance[i]->hitboxes[i2].rect;

				if (visualize_boxes) {
					Vec4f hitbox_color = { 255, 0, 0, 127 };
					if (fighter_instance[i]->hitboxes[i2].hitbox_kind == HITBOX_KIND_BLOCK) {
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
		fighter_instance[i]->connected_hitbox = hitbox_to_use;
	}
	event_hit_collide_player(fighter_instance[0], fighter_instance[1], &(fighter_instance[0]->hitboxes[fighter_instance[1]->connected_hitbox]), &(fighter_instance[1]->hitboxes[fighter_instance[0]->connected_hitbox]));
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

int get_event_hit_collide_player(FighterInstance* attacker, FighterInstance* defender, Hitbox *hitbox, Hurtbox *hurtbox) {
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

void event_hit_collide_player(FighterInstance* p1, FighterInstance* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
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

bool can_counterhit(FighterInstance* defender, Hitbox* hitbox) {
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

IFighter::IFighter(int chara_id, SDL_Renderer* renderer, int id) {
	switch (chara_id) {
	case(CHARA_KIND_ROY): {
		fighter_instance = new Roy(renderer, id);
	} break;
	case(CHARA_KIND_ERIC): {
		fighter_instance = new Eric(renderer, id);
	} break;
	case (CHARA_KIND_MAX): {
		fighter_instance = NULL;
	} break;
	}
}

IFighter::~IFighter() {
	if (fighter_instance) {
		delete[] fighter_instance;
		fighter_instance = NULL;
	}
}

FighterInstance* IFighter::get_fighter() {
	return fighter_instance;
}