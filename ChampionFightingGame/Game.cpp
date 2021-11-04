#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <SDL.h>
#include "Animation.h"
#include "Game.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "Menu.h"
#include "SoundManager.h"

#include "ObjectInstance.h"
#include "FighterInstance.h"
#include "ProjectileInstance.h"
#include "FighterInstanceAccessor.h"
#include "Roy.fwd.h"
#include "Roy.h"
#include "RoyFireball.h"
#include "Eric.fwd.h"
#include "Eric.h"
#include "EricFireball.h"
#include "Atlas.fwd.h"
#include "Atlas.h"

#include "CharaTemplate.fwd.h"
#include "CharaTemplate.h"
#include "ProjectileTemplate.h"

extern bool debug;
int game_main(SDL_Renderer* pRenderer, SDL_Window* window, PlayerInfo player_info[2]) {
	Uint32 tick=0,tok=0;
	bool gaming = true;
	bool visualize_boxes = true;
	int next_state = GAME_STATE_MENU;

	Debugger debugger;
	debugger = Debugger();
	SDL_Rect debug_rect[2] = { 0, 0, 0, 0 };

	GameCoordinate debug_anchor[2];
	GameCoordinate debug_offset[2];

	SDL_Rect camera; //SDL_Rect which crops the pScreenTexture

	//init stage
	int rng = rand() % 2;
	Stage stage = Stage(pRenderer, player_info[rng].stage_kind);

	//init players
	FighterInstance* fighter_instance[2];
	FighterInstanceAccessor* fighter_instance_accessor = new FighterInstanceAccessor;

	IObject* p1 = new IObject(OBJECT_TYPE_FIGHTER, (&player_info[0])->chara_kind, pRenderer, 0, &player_info[0], fighter_instance_accessor);
	IObject* p2 = new IObject(OBJECT_TYPE_FIGHTER, (&player_info[1])->chara_kind, pRenderer, 1, &player_info[1], fighter_instance_accessor);

	fighter_instance[0] = p1->get_fighter();
	fighter_instance[1] = p2->get_fighter();

	for (int i = 0; i < 2; i++) {
		fighter_instance[i]->player_info = &player_info[i]; //I was wrong, we actually need this
		fighter_instance[i]->pos.x = 0;
		fighter_instance_accessor->fighter_instance[i] = fighter_instance[i];
		fighter_instance[i]->fighter_instance_accessor = fighter_instance_accessor;
	}
	for (int i = 0; i < 2; i++) {
		fighter_instance[i]->superInit(i, pRenderer);
		/*
			Requires fighter instance accessor to be fully initialized since it makes a call that involves checking the other character's x pos, so we'll
			execute this part after the first loop has finished
		*/
	}

	if (fighter_instance[0]->crash_to_debug || fighter_instance[1]->crash_to_debug) {
		gaming = false;
		next_state = GAME_STATE_DEBUG_MENU;
	}

	//init ui
	GameTimer timer = GameTimer(pRenderer, 99);

	HealthBar health_bar[2];
	health_bar[0] = HealthBar(pRenderer, fighter_instance[0]);
	health_bar[1] = HealthBar(pRenderer, fighter_instance[1]);

	PlayerIndicator player_indicator[2];
	player_indicator[0] = PlayerIndicator(pRenderer, fighter_instance[0]);
	player_indicator[1] = PlayerIndicator(pRenderer, fighter_instance[1]);

	const Uint8* keyboard_state;

	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			fighter_instance[i]->projectile_objects[i2]->owner_id = i;
		}
	}

	while (gaming) {

		frameTimeDelay(&tick,&tok);

		SDL_Texture* pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
		SDL_Texture* pGui = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
		SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(pGui, SDL_BLENDMODE_BLEND);
		
		//yeah

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					next_state = GAME_STATE_CLOSE;
					gaming = false;
				}
				break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(window, 0);
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

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
		
		SDL_SetRenderTarget(pRenderer, pScreenTexture);
		SDL_RenderCopy(pRenderer, stage.pBackgroundTexture, nullptr, nullptr);

		/*
		Setting up facing directions must be done outside of the main fighter loop. If P1 is on one side, is determined to be facing one way, then
		P2 moves such that P1 crosses them up, it can cause both characters to face the same direction for a frame which can break stuff. Make sure the
		facing direction of each character is locked in before any movement for that frame takes place.
		*/

		for (int i = 0; i < 2; i++) {
			if (fighter_instance[i]->situation_kind == CHARA_SITUATION_GROUND && fighter_instance[i]->is_actionable()) {
				int pos_threshold = 0;
				if (fighter_instance[!i]->situation_kind == CHARA_SITUATION_AIR) {
					pos_threshold = 5;
				}
				if (fighter_instance[i]->pos.x > fighter_instance[!i]->pos.x + pos_threshold) { //If you only crossed someone up by 5 pixels, don't worry about turning
					//around just yet, or else walking under a launched opponent can get weird if your x speed is close enough to the opponent's
					fighter_instance[i]->facing_dir = -1.0;
					fighter_instance[i]->facing_right = false;
				}
				else if (fighter_instance[i]->pos.x < fighter_instance[!i]->pos.x - pos_threshold) {
					fighter_instance[i]->facing_dir = 1.0;
					fighter_instance[i]->facing_right = true;
				}
				else { //If both players are stuck inside each other, stop that !
					if (fighter_instance[i]->situation_kind == CHARA_SITUATION_GROUND && fighter_instance[!i]->situation_kind == CHARA_SITUATION_GROUND) {
						fighter_instance[i]->chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
						fighter_instance[!i]->chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
						fighter_instance[i]->add_pos(-1.0, 0);
						fighter_instance[i]->facing_dir = 1.0;
						fighter_instance[i]->facing_right = true;
						fighter_instance[!i]->add_pos(1.0, 0);
						fighter_instance[!i]->facing_dir = -1.0;
						fighter_instance[!i]->facing_right = false;
						fighter_instance[i]->chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
						fighter_instance[!i]->chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
					}
				}
			}
		}

		for (int i = 0; i < 2; i++) {
			if (debugger.check_button_trigger(BUTTON_DEBUG_ENABLE) && i == 0) {
				debug = !debug;
				timer.ClockMode = !timer.ClockMode;
			}
			if (!debug) {
				//this is the line that gets called every frame for the fighters to do their stuff
				fighter_instance[i]->fighter_main();
			}
			else if (i == 0) {
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_1)) {
					debugger.target = 0;
				}
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_2)) {
					debugger.target = 1;
				}
				if (debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)) {
					player_info[0].update_buttons(keyboard_state);
					player_info[1].update_buttons(keyboard_state);
					fighter_instance[0]->fighter_main();
					fighter_instance[1]->fighter_main();
					timer.Tick();
					if (debugger.print_frames) {
						cout << "Player " << debugger.target + 1 << " Frame: " << fighter_instance[debugger.target]->frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Render Frame: " << fighter_instance[debugger.target]->render_frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Pos X: " << fighter_instance[debugger.target]->pos.x << endl;
						cout << "Player " << debugger.target + 1 << " Pos Y: " << fighter_instance[debugger.target]->pos.y << endl;
					}
				}
				debug_mode(&debugger, fighter_instance[debugger.target], pRenderer, &debug_rect[debugger.target], &debug_anchor[debugger.target], &debug_offset[debugger.target]);
				if (debugger.check_button_trigger(BUTTON_DEBUG_RESET)) {
					debug = false;
					gaming = false;
					next_state = GAME_STATE_DEBUG_MENU;
				}
			}

			if (fighter_instance[i]->crash_to_debug) {
				gaming = false;
				next_state = GAME_STATE_DEBUG_MENU;
			}

			SDL_Rect render_pos = getRenderPos(fighter_instance[i], fighter_instance[i]->chara_flag[CHARA_FLAG_FORCE_ANIM_CENTER]);
			SDL_RendererFlip flip = fighter_instance[i]->facing_right ? SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL;
			const double angle = (const double)fighter_instance[i]->angle;
			int error_render = SDL_RenderCopyEx(pRenderer, fighter_instance[i]->anim_kind->SPRITESHEET, &(fighter_instance[i]->frame_rect), &render_pos, angle, NULL, flip);
			if (error_render != 0) {
				cout << "\n" << SDL_GetError();
			}
			if (fighter_instance[i]->chara_int[CHARA_INT_COMBO_COUNT] > 1) {
				SDL_SetRenderTarget(pRenderer, pGui);
				float id_ope = -1;
				if (fighter_instance[i]->id == 1) {
					id_ope *= -1;
				}
				draw_text(pRenderer, "FiraCode-Regular.ttf", to_string(fighter_instance[i]->chara_int[CHARA_INT_COMBO_COUNT]), {id_ope * 500, WINDOW_HEIGHT * 0.75}, 200,  255, 0, 0, 255);
				SDL_SetRenderTarget(pRenderer, pScreenTexture);
			}

			//Projectile Renders

			for (int o = 0; o < MAX_PROJECTILES; o++) {
				if (fighter_instance[i]->projectile_objects[o]->id != -1) {
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					if (!fighter_instance[i]->projectile_objects[o]->facing_right) {
						flip = SDL_FLIP_HORIZONTAL;
					}
					SDL_Rect render_pos;
					render_pos.x = fighter_instance[i]->projectile_objects[o]->pos.getRenderCoodrinateX();
					render_pos.y = fighter_instance[i]->projectile_objects[o]->pos.y;
					int width;
					int height;
					SDL_QueryTexture(fighter_instance[i]->projectile_objects[o]->anim_kind->SPRITESHEET, NULL, NULL, &width, &height);
					render_pos.w = (width / (fighter_instance[i]->projectile_objects[o]->anim_kind->length + 1));
					render_pos.h = height;
					const double angle = (const double)fighter_instance[i]->projectile_objects[o]->angle;
					int error_render = SDL_RenderCopyEx(pRenderer, fighter_instance[i]->projectile_objects[o]->anim_kind->SPRITESHEET, &(fighter_instance[i]->projectile_objects[o]->frame_rect), &render_pos, angle, NULL, flip);
					if (error_render != 0) {
						cout << SDL_GetError() << endl;
					}
				}
			}
		}

		if (debug) {
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(pRenderer, debug_rect);
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 127);
			SDL_RenderFillRect(pRenderer, debug_rect);
		}

		check_attack_connections(fighter_instance[0], fighter_instance[1], pRenderer, visualize_boxes, !debug || (debug && debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)));

		//Camera things
		camera = updateCamera(fighter_instance[0]->pos.getRenderCoodrinateX(), fighter_instance[0]->pos.getRenderCoodrinateY(), fighter_instance[1]->pos.getRenderCoodrinateX(), fighter_instance[1]->pos.getRenderCoodrinateY(), debugger.zoom);

		//The tag system doesn't play nice with the render offsets I've been working on, we can reimplement it later but for now it's easier to just not think about

/*		for (int i = 0; i < 2; i++) {
			if (fighter_instance[i]->anim_kind->move_dir) {
				player_indicator[i].indicator_rect = SDL_Rect{
				(int)(fighter_instance[i]->pos.getRenderCoodrinateX() - fighter_instance[i]->get_param_float(fighter_instance[i]->anim_kind->name + "_move_offset", fighter_instance[i]->param_table) * fighter_instance[i]->facing_dir + 20),
				(int)(fighter_instance[i]->pos.getRenderCoodrinateYAnim() - 33),
				30,
				30 };
				SDL_RenderCopy(pRenderer, player_indicator[i].texture, nullptr, &(player_indicator[i].indicator_rect));
			}
			else {
				player_indicator[i].indicator_rect = SDL_Rect{
				(int)(fighter_instance[i]->pos.getRenderCoodrinateX() + 20),
				(int)(fighter_instance[i]->pos.getRenderCoodrinateYAnim() - 33),
				30,
				30};
				SDL_RenderCopy(pRenderer, player_indicator[i].texture, nullptr, &(player_indicator[i].indicator_rect));
			}
		}*/

		SDL_SetRenderTarget(pRenderer, nullptr); //set target to the window
		SDL_RenderCopy(pRenderer, pScreenTexture, &camera, nullptr); //render scale to window

		SDL_SetRenderTarget(pRenderer, pGui); //set target to gui layer
		for (int i = 0; i < 2; i++) {
			switch (i) {
				case 0:
					health_bar[i].RenderAsP1();
					break;

				case 1:
					health_bar[i].RenderAsP2();
					break;

				default:
					health_bar[i].RenderAsP1();
					break;
			}

		}

		if (!debug) timer.Tick();
		timer.Render();


		SDL_SetRenderTarget(pRenderer, nullptr); //set target to the window
		SDL_RenderCopy(pRenderer, pGui, nullptr, nullptr); //render gui to window

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255); //lmao help
		SDL_RenderPresent(pRenderer); //finalize
		
		SDL_DestroyTexture(pGui); //ok it's on screen, now get that shit outta here
		SDL_DestroyTexture(pScreenTexture);

	}

	cleanup(p1, p2);

	delete fighter_instance_accessor;

	return next_state;
}

void check_attack_connections(FighterInstance* p1, FighterInstance* p2, SDL_Renderer* renderer, bool visualize_boxes, bool check) {
	FighterInstance* fighter_instance[2] = { p1, p2 };
	if (check) {
		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (fighter_instance[0]->projectile_objects[i]->id != -1) {
				for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
					if (fighter_instance[1]->projectile_objects[i2]->id != -1) {
						for (int i3 = 0; i3 < 10; i3++) {
							if (fighter_instance[0]->projectile_objects[i]->hitboxes[i3].id != -1
								&& !fighter_instance[0]->projectile_objects[i]->hitboxes[i3].success_hit
								&& fighter_instance[0]->projectile_objects[i]->hitboxes[i3].trade) {
								for (int i4 = 0; i4 < 10; i4++) {
									if (fighter_instance[1]->projectile_objects[i2]->hitboxes[i4].id != -1
										&& !fighter_instance[1]->projectile_objects[i2]->hitboxes[i4].success_hit
										&& fighter_instance[1]->projectile_objects[i2]->hitboxes[i4].trade) {
										SDL_Rect p1_hitbox, p2_hitbox;
										p1_hitbox = fighter_instance[0]->projectile_objects[i]->hitboxes[i3].rect;
										p2_hitbox = fighter_instance[1]->projectile_objects[i2]->hitboxes[i4].rect;
										if (is_collide(p1_hitbox, p2_hitbox)) {
											fighter_instance[0]->projectile_objects[i]->clear_hitbox(i3);
											fighter_instance[1]->projectile_objects[i2]->clear_hitbox(i4);
											fighter_instance[0]->projectile_objects[i]->projectile_int[PROJECTILE_INT_HEALTH] --;
											fighter_instance[1]->projectile_objects[i2]->projectile_int[PROJECTILE_INT_HEALTH] --;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			int hitbox_to_use = HITBOX_COUNT_MAX;
			int projectile_hitbox_to_use = HITBOX_COUNT_MAX;
			int grabbox_to_use = HITBOX_COUNT_MAX;
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
					for (int i3 = 0; i3 < MAX_PROJECTILES; i3++) {
						if (fighter_instance[!i]->projectile_objects[i3]->id != -1) {
							for (int i4 = 0; i4 < 10; i4++) {
								if (fighter_instance[!i]->projectile_objects[i3]->hitboxes[i4].id != -1
									&& !fighter_instance[!i]->projectile_objects[i3]->hitboxes[i4].success_hit) {
									SDL_Rect hitbox;
									hitbox = fighter_instance[!i]->projectile_objects[i3]->hitboxes[i4].rect;
									if (is_collide(hitbox, hurtbox)) {
										projectile_hitbox_to_use = get_event_hit_collide_projectile(fighter_instance[!i]->projectile_objects[i3], fighter_instance[i], &(fighter_instance[!i]->projectile_objects[i3]->hitboxes[i4]), &(fighter_instance[i]->hurtboxes[i2]));
									}
									if (projectile_hitbox_to_use != HITBOX_COUNT_MAX) {
										break;
									}
								}
							}
						}
					}
					if (hitbox_to_use != HITBOX_COUNT_MAX) {
						break;
					}
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter_instance[!i]->grabboxes[i3].id != -1) {
							SDL_Rect grabbox;
							grabbox = fighter_instance[!i]->grabboxes[i3].rect;
							if (is_collide(grabbox, hurtbox)) {
								grabbox_to_use = get_event_grab_collide_player(fighter_instance[!i], fighter_instance[i], &(fighter_instance[!i]->grabboxes[i3]), &(fighter_instance[i]->hurtboxes[i2]));
							}
							if (grabbox_to_use != HITBOX_COUNT_MAX) {
								break;
							}
						}
					}
					if (grabbox_to_use != HITBOX_COUNT_MAX) {
						break;
					}
				}
			}
			fighter_instance[i]->connected_hitbox = hitbox_to_use;
			fighter_instance[i]->connected_grabbox = grabbox_to_use;
			fighter_instance[i]->connected_projectile_hitbox = projectile_hitbox_to_use;
		}
		if (!event_hit_collide_player(fighter_instance[0], fighter_instance[1], &(fighter_instance[0]->hitboxes[fighter_instance[1]->connected_hitbox]), &(fighter_instance[1]->hitboxes[fighter_instance[0]->connected_hitbox]))) {
			event_grab_collide_player(fighter_instance[0], fighter_instance[1], &(fighter_instance[0]->grabboxes[fighter_instance[1]->connected_grabbox]), &(fighter_instance[1]->grabboxes[fighter_instance[0]->connected_grabbox]));
		}
		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
				if (fighter_instance[i]->projectile_objects[i2]->id != -1) {
					event_hit_collide_projectile(fighter_instance[i], fighter_instance[!i], fighter_instance[i]->projectile_objects[i2], &(fighter_instance[i]->projectile_objects[i2]->hitboxes[fighter_instance[!i]->connected_projectile_hitbox]));
				}
			}
		}
	}
	if (visualize_boxes) {
		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter_instance[i]->hurtboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter_instance[i]->hurtboxes[i2].rect;
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
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter_instance[i]->hitboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter_instance[i]->hitboxes[i2].rect;

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
				for (int i3 = 0; i3 < 10; i3++) {
					if (fighter_instance[i]->projectile_objects[i2]->id != -1 && fighter_instance[i]->projectile_objects[i2]->hitboxes[i3].id != -1) {
						SDL_Rect render_pos;
						render_pos = fighter_instance[i]->projectile_objects[i2]->hitboxes[i3].rect;

						Vec4f hitbox_color = { 255, 0, 0, 127 };

						SDL_SetRenderDrawColor(renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, 255);
						SDL_RenderDrawRect(renderer, &render_pos);
						SDL_SetRenderDrawColor(renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, hitbox_color.w);
						SDL_RenderFillRect(renderer, &render_pos);
					}
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter_instance[i]->grabboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter_instance[i]->grabboxes[i2].rect;

					Vec4f grabbox_color = { 0, 255, 0, 127 };
					if (fighter_instance[i]->grabboxes[i2].grabbox_kind & GRABBOX_KIND_NOTECH) {
						grabbox_color.x = 128;
						grabbox_color.y = 0;
						grabbox_color.z = 128;
					}

					SDL_SetRenderDrawColor(renderer, grabbox_color.x, grabbox_color.y, grabbox_color.z, 255);
					SDL_RenderDrawRect(renderer, &render_pos);
					SDL_SetRenderDrawColor(renderer, grabbox_color.x, grabbox_color.y, grabbox_color.z, grabbox_color.w);
					SDL_RenderFillRect(renderer, &render_pos);
				}
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

int get_event_hit_collide_player(FighterInstance* attacker, FighterInstance* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
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
	if (hurtbox->intangible_kind == hitbox->attack_height || hurtbox->intangible_kind == INTANGIBLE_KIND_NORMAL || hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->situation_kind == CHARA_SITUATION_AIR && hitbox->max_juggle < defender->chara_int[CHARA_INT_JUGGLE_VALUE]) {
		return HITBOX_COUNT_MAX;
	}

	//Then, check if the hurtbox is invincible. If it is, the attacker's entire attack has failed. This will be pretty rare tbh.

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		attacker->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = attacker->chara_int[CHARA_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	//If the hurtbox has armor, both parties get SOME hitlag to acknowledge the move hitting

	if (hurtbox->is_armor) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		attacker->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = attacker->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
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
	if (defender->chara_flag[CHARA_FLAG_PARRY_ACTIVE]) {
		if (defender->chara_int[CHARA_INT_PARRY_HEIGHT] == hitbox->attack_height || defender->chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			parrying = true;
		}
	}
	if (parrying) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag + 12;
		attacker->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = attacker->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag + 8;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = true;
		return hitbox->id;
	}
	if (blocking && !hitbox->unblockable) {
		attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = attacker->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = attacker->chara_int[CHARA_INT_HITLAG_FRAMES];
	defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
	defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	attacker->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = true;
	return hitbox->id;
}

int get_event_grab_collide_player(FighterInstance* attacker, FighterInstance* defender, Grabbox* grabbox, Hurtbox* hurtbox) {
	if (grabbox->situation_hit != SITUATION_HIT_ALL) {
		if (grabbox->situation_hit != SITUATION_HIT_GROUND_AIR) {
			if (grabbox->situation_hit != defender->situation_kind) {
				return HITBOX_COUNT_MAX;
			}
		}
		else if (defender->situation_kind == CHARA_SITUATION_DOWN) {
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

int get_event_hit_collide_projectile(ProjectileInstance* attacker, FighterInstance* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
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
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_PROJECTILE || hurtbox->intangible_kind == INTANGIBLE_KIND_ALL) {
		return HITBOX_COUNT_MAX;
	}
	if (defender->situation_kind == CHARA_SITUATION_AIR && hitbox->max_juggle < defender->chara_int[CHARA_INT_JUGGLE_VALUE]) {
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
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
		return hitbox->id;
	}

	bool blocking = false;
	bool parrying = false;
	if (defender->is_actionable() || defender->status_kind == CHARA_STATUS_BLOCKSTUN) {
		if (defender->get_stick_dir() == 4 || defender->get_stick_dir() == 1) {
			blocking = true;
		}
	}
	if (defender->chara_flag[CHARA_FLAG_PARRY_ACTIVE]) {
		if (defender->chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID || defender->chara_int[CHARA_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			parrying = true;
		}
	}
	if (parrying) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag + 12;
		attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag + 8;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = true;
		return hitbox->id;
	}
	if (blocking && !hitbox->unblockable) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
		defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->chara_int[CHARA_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	defender->chara_int[CHARA_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->chara_int[CHARA_INT_INIT_HITLAG_FRAMES] = defender->chara_int[CHARA_INT_HITLAG_FRAMES];
	defender->chara_int[CHARA_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	attacker->projectile_flag[PROJECTILE_FLAG_HIT] = true;
	return hitbox->id;
}

bool event_hit_collide_player(FighterInstance* p1, FighterInstance* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
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
					return false;
				}
				else if (p1_hitbox->clank_kind != CLANK_KIND_CONTINUE) {
					p1->change_status(CHARA_STATUS_CLANK);
					return false;
				}
				else { //If both people have trample, it's better to make them both clank than it is to make them both ignore having just been hit
					p1->change_status(CHARA_STATUS_CLANK);
					p2->change_status(CHARA_STATUS_CLANK);
					return false;
				}
			}
			else if (p1_hitbox->clank_kind == CLANK_KIND_CLANK || p2_hitbox->clank_kind == CLANK_KIND_CLANK) {
				p1->change_status(CHARA_STATUS_CLANK);
				p2->change_status(CHARA_STATUS_CLANK);
				return false;
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
		p1->update_hitbox_connect(p1_hitbox->multihit);
		if (p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2->get_param_float("meter_gain_on_parry");
			p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = CHARA_STATUS_PARRY;
		}
		else if (p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->chip_damage;
			p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
			p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p1->chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS] = true;
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
			p2->chara_int[CHARA_INT_COMBO_COUNT] ++;
			if (p2->get_status_group() == STATUS_GROUP_HITSTUN) {
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
				p2->chara_int[CHARA_INT_JUGGLE_VALUE]++;
			}
			else {
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] = p1_hitbox->juggle_set;
			}
			float prev_x = p2->pos.x;
			p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
			if (can_counterhit(p2, p1_hitbox)) {
				if (p1_hitbox->scale == -5) {
					p1->chara_int[CHARA_INT_DAMAGE_SCALE] = -5;
				}
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_counterhit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * p1_hitbox->counterhit_damage_mul;
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
				p2->chara_int[CHARA_INT_HITSTUN_FRAMES] *= 1.2;
				p2->chara_int[CHARA_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
				if (p2->status_kind == CHARA_STATUS_LAUNCH && p1_hitbox->continue_launch) {
					p2_status_post_hit = CHARA_STATUS_LAUNCH;
				}
			}
			else {
				p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_hit;
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * ((float)(clamp(1, 10 - p1->chara_int[CHARA_INT_DAMAGE_SCALE], 15)) / 10);
				p2->chara_int[CHARA_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
				if (p2->status_kind == CHARA_STATUS_LAUNCH && p1_hitbox->continue_launch) {
					p2_status_post_hit = CHARA_STATUS_LAUNCH;
				}
			}
		}
		p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
		p1->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
	}

	if (p1_hit) { //P1 got hit
		p2->update_hitbox_connect(p2_hitbox->multihit);
		if (p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1->get_param_float("meter_gain_on_parry");
			p1->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p1_status_post_hit = CHARA_STATUS_PARRY;
		}
		else if (p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->chip_damage;
			float prev_x = p1->pos.x;
			p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->block_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES];
			p1->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p2->chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS] = true;
			p1_status_post_hit = CHARA_STATUS_BLOCKSTUN;
		}
		else if (!p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED]) {
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_block / 2;
			p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage / 2;
		}
		else {
			p1->chara_int[CHARA_INT_COMBO_COUNT] ++;
			if (p1->get_status_group() == STATUS_GROUP_HITSTUN) {
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
				p1->chara_int[CHARA_INT_JUGGLE_VALUE]++;
			}
			else {
				p1->chara_int[CHARA_INT_JUGGLE_VALUE] = p2_hitbox->juggle_set;
			}
			p1->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->hit_pushback / p1->chara_int[CHARA_INT_HITLAG_FRAMES];
			if (can_counterhit(p1, p2_hitbox)) {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_counterhit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage * p2_hitbox->counterhit_damage_mul;
				p1->chara_int[CHARA_INT_JUGGLE_VALUE] = 0;
				p1->chara_int[CHARA_INT_HITSTUN_FRAMES] *= 1.2;
				p1->chara_int[CHARA_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p1_status_post_hit = get_damage_status(p2_hitbox->counterhit_status, p1->situation_kind);
				if (p1->status_kind == CHARA_STATUS_LAUNCH && p2_hitbox->continue_launch) {
					p1_status_post_hit = CHARA_STATUS_LAUNCH;
				}
			}
			else {
				p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2_hitbox->meter_gain_on_hit;
				p1->chara_float[CHARA_FLOAT_HEALTH] -= p2_hitbox->damage * ((float)(clamp(1, 10 - p2->chara_int[CHARA_INT_DAMAGE_SCALE], 15)) / 10.0);
				p1->chara_int[CHARA_INT_HITSTUN_LEVEL] = p2_hitbox->attack_level;
				p1_status_post_hit = get_damage_status(p2_hitbox->hit_status, p1->situation_kind);
				if (p1->status_kind == CHARA_STATUS_LAUNCH && p2_hitbox->continue_launch) {
					p1_status_post_hit = CHARA_STATUS_LAUNCH;
				}
			}
		}
		p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED] = false;
		p2->chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
	}
	if (p1_hit) {
		p1->change_status(p1_status_post_hit, true, false);
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
	}
	return (p1_hit || p2_hit);
}

void event_grab_collide_player(FighterInstance* p1, FighterInstance* p2, Grabbox* p1_grabbox, Grabbox* p2_grabbox) {
	bool p1_hit = p2_grabbox->id != -1;
	bool p1_tech = p2_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH;
	bool p2_hit = p1_grabbox->id != -1;
	bool p2_tech = p1_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH;
	if (p1_hit && p2_hit) {
		if (p1_tech == p2_tech) {
			p1->change_status(CHARA_STATUS_THROW_TECH);
			p2->change_status(CHARA_STATUS_THROW_TECH);
			return;
		}
		else if (p1_tech) {
			p1_hit = false;
		}
		else {
			p2_hit = false;
		}
	}
	if (p1_hit) {
		if (p1->chara_flag[CHARA_FLAG_THROW_TECH] && p1_tech) {
			p1->change_status(CHARA_STATUS_THROW_TECH);
			p2->change_status(CHARA_STATUS_THROW_TECH);
		}
		else {
			p2->change_status(p2_grabbox->attacker_status_if_hit);
			p1->change_status(p2_grabbox->defender_status_if_hit);
		}
	}
	if (p2_hit) {
		if (p2->chara_flag[CHARA_FLAG_THROW_TECH] && p2_tech) {
			p1->change_status(CHARA_STATUS_THROW_TECH);
			p2->change_status(CHARA_STATUS_THROW_TECH);
		}
		else {
			p1->change_status(p1_grabbox->attacker_status_if_hit);
			p2->change_status(p1_grabbox->defender_status_if_hit);
		}
	}
}

void event_hit_collide_projectile(FighterInstance* p1, FighterInstance* p2, ProjectileInstance* p1_projectile, Hitbox* p1_hitbox) {
	bool p2_hit = p1_hitbox->id != -1;
	u32 p2_status_post_hit = p2->status_kind;
	if (p2_hit) {
		p2->chara_int[CHARA_INT_COMBO_COUNT] ++;
		p1_projectile->update_hitbox_connect(p1_hitbox->multihit);
		p1_projectile->projectile_int[PROJECTILE_INT_HEALTH] --;
		if (p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_SUPER_METER] += p2->get_param_float("meter_gain_on_parry");
			p2->chara_flag[CHARA_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = CHARA_STATUS_PARRY;
		}
		else if (p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->chip_damage;
			p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
			p2->chara_flag[CHARA_FLAG_ENTER_BLOCKSTUN] = false;
			p2_status_post_hit = CHARA_STATUS_BLOCKSTUN;
		}
		else if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT]) {
			p1->chara_float[CHARA_FLOAT_SUPER_METER] += p1_hitbox->meter_gain_on_block / 2;
			p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage / 2;
		}
		else {
			/*
				If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
				is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
			*/
			if (p2->get_status_group() == STATUS_GROUP_HITSTUN) {
				if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS]) {
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
				p2->chara_int[CHARA_INT_JUGGLE_VALUE]++;
			}
			else {
				p2->chara_int[CHARA_INT_JUGGLE_VALUE] = p1_hitbox->juggle_set;
			}
			p2->chara_float[CHARA_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->chara_int[CHARA_INT_HITLAG_FRAMES];
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
				p2->chara_float[CHARA_FLOAT_HEALTH] -= p1_hitbox->damage * ((float)(clamp(1, 10 - p1->chara_int[CHARA_INT_DAMAGE_SCALE], 15)) / 10);
				p2->chara_int[CHARA_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
			}
			if (p2->status_kind == CHARA_STATUS_LAUNCH && p1_hitbox->continue_launch) {
				p2_status_post_hit = CHARA_STATUS_LAUNCH;
			}
		}
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT] = false;
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = true;
		p1->chara_flag[CHARA_FLAG_PROJECTILE_CONNECTED_DURING_STATUS] = true;
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
	}
}

bool can_counterhit(FighterInstance* defender, Hitbox* hitbox) {
	if (defender->anim_kind->name == "hitstun_parry" || defender->anim_kind->name == "hitstun_parry_air") {
		return true;
	}
	return defender->chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] && (hitbox->counterhit_type == COUNTERHIT_TYPE_NORMAL
	|| (defender->situation_kind == CHARA_SITUATION_AIR && hitbox->counterhit_type == COUNTERHIT_TYPE_AERIAL));
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

void decrease_common_fighter_variables(FighterInstance* fighter_instance) {
	if (fighter_instance->chara_int[CHARA_INT_236_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_236_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_236_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_214_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_214_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_214_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_623_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_623_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_623_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_41236_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_41236_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_41236_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_63214_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_63214_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_63214_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_236236_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_236236_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_236236_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_214214_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_214214_TIMER] --;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_214214_STEP] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_DOWN_CHARGE_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_DOWN_CHARGE_TIMER]--;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (fighter_instance->chara_int[CHARA_INT_HITLAG_FRAMES] != 0) {
		fighter_instance->chara_int[CHARA_INT_HITLAG_FRAMES]--;
	}
	else if (fighter_instance->chara_int[CHARA_INT_HITSTUN_FRAMES] != 0) {
		fighter_instance->chara_int[CHARA_INT_HITSTUN_FRAMES]--;
	}
	if (fighter_instance->chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		fighter_instance->chara_int[CHARA_INT_DASH_F_WINDOW]--;
	}
	if (fighter_instance->chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		fighter_instance->chara_int[CHARA_INT_DASH_B_WINDOW]--;
	}
	if (fighter_instance->chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW] != 0) {
		fighter_instance->chara_int[CHARA_INT_KNOCKDOWN_TECH_WINDOW]--;
	}
	if (fighter_instance->chara_int[CHARA_INT_BACK_CHARGE_TIMER] != 0) {
		fighter_instance->chara_int[CHARA_INT_BACK_CHARGE_TIMER]--;
	}
	else {
		fighter_instance->chara_int[CHARA_INT_BACK_CHARGE_FRAMES] = 0;
	}
}

void decrease_common_projectile_variables(ProjectileInstance* projectile_instance) {
	if (projectile_instance->projectile_int[PROJECTILE_INT_ACTIVE_TIME] > 0) {
		projectile_instance->projectile_int[PROJECTILE_INT_ACTIVE_TIME] --;
	}
	if (projectile_instance->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0) {
		projectile_instance->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] --;
	}
}

void cleanup(IObject* p1, IObject* p2) {
	FighterInstance* fighter_instance[2];
	fighter_instance[0] = p1->get_fighter();
	fighter_instance[1] = p2->get_fighter();
	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			if (fighter_instance[i]->projectile_instances[i2] != NULL) {
				delete fighter_instance[i]->projectile_instances[i2];
			}
			delete fighter_instance[i]->projectile_objects[i2];
		}
		delete fighter_instance[i];
	}
	delete p1;
	delete p2;
}

IObject::IObject(int object_type, int object_kind, SDL_Renderer* renderer, int id, PlayerInfo *player_info, FighterInstanceAccessor* fighter_instance_accessor) {
	if (object_type == OBJECT_TYPE_FIGHTER) {
		switch (object_kind) {
			case (CHARA_KIND_ROY):
			{
				fighter_instance = new Roy(renderer, id, player_info, fighter_instance_accessor);
			}
			break;
			case (CHARA_KIND_ERIC):
			{
				fighter_instance = new Eric(renderer, id, player_info, fighter_instance_accessor);
			}
			break;
			case (CHARA_KIND_ATLAS):
			{
				fighter_instance = new Atlas(renderer, id, player_info, fighter_instance_accessor);
			}
			break;
			case (CHARA_KIND_CHARA_TEMPLATE):
			{
				fighter_instance = new CharaTemplate(renderer, id, player_info, fighter_instance_accessor);
			} break;
			case (CHARA_KIND_MAX):
			{
				fighter_instance = NULL;
			}
			break;
		}
	}
	else if (object_type == OBJECT_TYPE_PROJECTILE) {
		switch (object_kind) {
			case (PROJECTILE_KIND_ROY_FIREBALL):
			{
				projectile_instance = new RoyFireball(renderer, id, player_info, fighter_instance_accessor);
			}
			break;
			case (PROJECTILE_KIND_ERIC_FIREBALL):
			{
				projectile_instance = new EricFireball(renderer, id, player_info, fighter_instance_accessor);
			}
			break;
			case (PROJECTILE_KIND_PROJECTILE_TEMPLATE):
			{
				projectile_instance = new ProjectileTemplate(renderer, id, player_info, fighter_instance_accessor);
			} break;
			case (PROJECTILE_KIND_MAX):
			{
				projectile_instance = NULL;
			}
			break;
		}
	}

}

IObject::~IObject() {}

FighterInstance* IObject::get_fighter() {
	return fighter_instance;
}

ProjectileInstance* IObject::get_projectile() {
	return projectile_instance;
}

SDL_Rect getRenderPos(FighterInstance* fighter_instance, bool force_center) {
	SDL_Rect render_pos;
	if (force_center) {
		render_pos.x = fighter_instance->pos.getRenderCoodrinateX();
	}
	else {
		render_pos.x = fighter_instance->pos.getRenderCoodrinateXAnim();
	}
	render_pos.y = fighter_instance->pos.getRenderCoodrinateYAnim();
	int width;
	int sprite_width = 0;
	int height;
	SDL_QueryTexture(fighter_instance->anim_kind->SPRITESHEET, NULL, NULL, &width, &height);
	render_pos.w = (width / (fighter_instance->anim_kind->length + 1));
	if (!fighter_instance->facing_right && force_center) {
		SDL_QueryTexture(fighter_instance->base_texture, NULL, NULL, &sprite_width, NULL);
		render_pos.x -= (render_pos.w - sprite_width);
	}
	render_pos.h = height;

	return render_pos;
}