﻿#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <SDL.h>
#include "Animation.h"
#include "Battle.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "Menu.h"
#include "SoundManager.h"
#include "GameTexture.h"

#include "Object.h"
#include "Fighter.h"
#include "FighterAccessor.h"
#include "Roy.fwd.h"
#include "Roy.h"
#include "Eric.fwd.h"
#include "Eric.h"
#include "Leon.fwd.h"
#include "Leon.h"
#include "Angelica.fwd.h"
#include "Angelica.h"
#include "Nightsaber.fwd.h"
#include "Nightsaber.h"
#include "Sully.fwd.h"
#include "Sully.h"
#include "Priest.fwd.h"
#include "Priest.h"
#include "Aziel.fwd.h"
#include "Aziel.h"
#include "Bruno.fwd.h"
#include "Bruno.h"
#include "Tessa.fwd.h"
#include "Tessa.h"
#include "Alejandro.fwd.h"
#include "Alejandro.h"
#include "Norman.fwd.h"
#include "Norman.h"
#include "Atlas.fwd.h"
#include "Atlas.h"
#include "Julius.fwd.h"
#include "Julius.h"
#include "Ramona.fwd.h"
#include "Ramona.h"
#include "Zyair.fwd.h"
#include "Zyair.h"
#include "Vesuvius.fwd.h"
#include "Vesuvius.h"

#include "Projectile.h"
#include "RoyFireball.h"
#include "EricFireball.h"

#include "CharaTemplate.fwd.h"
#include "CharaTemplate.h"
#include "ProjectileTemplate.h"
#include "Loader.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern SoundManager g_soundmanager;
extern SoundInfo sounds[3][MAX_SOUNDS];

extern bool debug;

void battle_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	const Uint8* keyboard_state;

	BattleLoader *battle_loader = new BattleLoader;
	battle_loader->player_info[0] = player_info[0];
	battle_loader->player_info[1] = player_info[1];

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool gaming = true;
	bool loading = true;
	bool visualize_boxes = true;
	int next_state = GAME_STATE_MENU;


	Debugger debugger;
	debugger = Debugger();
	SDL_Rect debug_rect[2] = { 0, 0, 0, 0 };

	GameCoordinate debug_anchor[2] = {{0,0}};
	GameCoordinate debug_offset[2] = {{0,0}};

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);
	SDL_Texture* pGui = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pGui, SDL_BLENDMODE_BLEND);

	GameTimer timer;
	Stage stage;
	IObject* p1 = NULL;
	IObject* p2 = NULL;
	Fighter* fighter[2] = {NULL, NULL};
	HealthBar health_bar[2];
	ExBar ex_bar[2];
	PlayerIndicator player_indicator[2];
	FighterAccessor* fighter_accessor = NULL;

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadBattle, "Init.zip", (void*)battle_loader);
	SDL_DetachThread(loading_thread);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (loading) {
		frameTimeDelay();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return game_manager->update_state(GAME_STATE_CLOSE);
				}
				break;
			}
		}
		
		load_icon.move();
		SDL_LockMutex(mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		int total_items = 17;
		loadingBar.setTargetPercent(((float)battle_loader->loaded_items / total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();
		
		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(mutex);

		if (battle_loader->finished) {
			if (!battle_loader->can_ret) {
				timer = battle_loader->timer;
				stage = battle_loader->stage;

				p1 = battle_loader->p1;
				p2 = battle_loader->p2;
				fighter_accessor = battle_loader->fighter_accessor;
				for (int i = 0; i < 2; i++) {
					health_bar[i] = battle_loader->health_bar[i];
					ex_bar[i] = battle_loader->ex_bar[i];
					player_indicator[i] = battle_loader->player_indicator[i];
					fighter[i] = battle_loader->fighter[i];
					fighter[i]->player_info = player_info[i];
				}
			}
			battle_loader->can_ret = true;

			SDL_PumpEvents();
			keyboard_state = SDL_GetKeyboardState(NULL);

			for (int i = 0; i < 2; i++) {
				player_info[i]->check_controllers();
				player_info[i]->poll_buttons(keyboard_state);
				if (player_info[i]->is_any_inputs()) {
					loading = false;

					//We don't want the option the player uses to exit the loading screen to also be capable of updating the buffer

					for (int i2 = 0; i2 < BUFFER_WINDOW; i2++) {
						player_info[i]->poll_buttons(keyboard_state);
						player_info[!i]->poll_buttons(keyboard_state);
					}
				}
			}
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
	
	if (getGameSetting("music_setting") == MUSIC_SETTING_STAGE) {
		g_soundmanager.playMusic(stage.default_music_kind);
	}
	else if (getGameSetting("music_setting") == MUSIC_SETTING_CHARA) {
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}


	SDL_Rect camera; //SDL_Rect which crops the pScreenTexture

	if (fighter[0]->crash_to_debug || fighter[1]->crash_to_debug) {
		gaming = false;
		game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}

	while (gaming) {
		frameTimeDelay();
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			if (debug) {
				g_soundmanager.pauseSEAll(i);
				g_soundmanager.pauseVCAll(i);
			}
		}	

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				}
				break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

		//Check the players' buttons
		for (int i = 0; i < 2; i++) {
			if (!debug) {
				/*
				Frame advance would make it so that check_button_trigger is never true during debug mode if it got checked here, so we just check the inputs
				directly when the frame is advanced manually
				*/
				player_info[i]->poll_buttons(keyboard_state);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		/*
		Setting up facing directions must be done outside of the main fighter loop. If P1 is on one side, is determined to be facing one way, then
		P2 moves such that P1 crosses them up, it can cause both characters to face the same direction for a frame which can break stuff. Make sure the
		facing direction of each character is locked in before any movement for that frame takes place.
		*/

		for (int i = 0; i < 2; i++) {
			if (fighter[i]->situation_kind == FIGHTER_SITUATION_GROUND && fighter[i]->is_actionable()) {
				int pos_threshold = 0;
				if (fighter[!i]->situation_kind == FIGHTER_SITUATION_AIR) {
					pos_threshold = 5;
				}
				if (fighter[i]->pos.x > fighter[!i]->pos.x + pos_threshold) { //If you only crossed someone up by 5 pixels, don't worry about turning
					//around just yet, or else walking under a launched opponent can get weird if your x speed is close enough to the opponent's
					fighter[i]->facing_dir = -1.0;
					fighter[i]->facing_right = false;
				}
				else if (fighter[i]->pos.x < fighter[!i]->pos.x - pos_threshold) {
					fighter[i]->facing_dir = 1.0;
					fighter[i]->facing_right = true;
				}
				else { //If both players are stuck inside each other, stop that !
					if (fighter[i]->situation_kind == FIGHTER_SITUATION_GROUND && fighter[!i]->situation_kind == FIGHTER_SITUATION_GROUND) {
						fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
						fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
						if (fighter[i]->pos.x == fighter[!i]->pos.x) {
							fighter[i]->facing_dir = 1.0;
							fighter[i]->facing_right = true;
							fighter[!i]->facing_dir = -1.0;
							fighter[!i]->facing_right = false;
						}
						fighter[i]->add_pos(-1.0 * fighter[i]->facing_dir, 0);
						fighter[!i]->add_pos(-1.0 * fighter[i]->facing_dir, 0);

						fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
						fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
					}
				}
			}
		}

		//Main tick loop

		for (int i = 0; i < 2; i++) {
			if (debugger.check_button_trigger(BUTTON_DEBUG_ENABLE) && i == 0) {
				debug = !debug;
				if (!debug) {
					for (int i = 0; i < 2; i++) {
						g_soundmanager.resumeSEAll(i);
						g_soundmanager.resumeVCAll(i);
					}
				}
				timer.ClockMode = !timer.ClockMode;
			}
			if (!debug) {
				fighter[i]->fighter_main();
			}
			else if (i == 0) {
				if (debugger.check_button_trigger(BUTTON_DEBUG_QUERY)) {
					debugger.print_commands();
					string command;
					cin >> command;
					debugger.debug_query(command, fighter[debugger.target], fighter[!debugger.target]);
				}
				if (debugger.check_button_trigger(BUTTON_DEBUG_CHANGE_TARGET)) {
					debugger.target = !debugger.target;
				}
				if (debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)) {
					for (int i = 0; i < 2; i++) {
						g_soundmanager.resumeSEAll(i);
						g_soundmanager.resumeVCAll(i);
					}

					for (int i = 0; i < 2; i++) {
						player_info[i]->poll_buttons(keyboard_state);
						fighter[i]->fighter_main();
					}
					timer.Tick();
					if (debugger.print_frames) {
						cout << "Player " << debugger.target + 1 << " Frame: " << fighter[debugger.target]->frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Render Frame: " << fighter[debugger.target]->render_frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Pos X: " << fighter[debugger.target]->pos.x << endl;
						cout << "Player " << debugger.target + 1 << " Pos Y: " << fighter[debugger.target]->pos.y << endl;
						cout << "Player " << debugger.target + 1 << " Health: " << fighter[debugger.target]->fighter_float[FIGHTER_FLOAT_HEALTH] << endl;
					}
				}
				debugger.debug_mode(fighter[debugger.target], &debug_rect[debugger.target], &debug_anchor[debugger.target], &debug_offset[debugger.target]);
				if (debugger.check_button_trigger(BUTTON_DEBUG_RESET)) {
					debug = false;
					gaming = false;
					game_manager->update_state(GAME_STATE_DEBUG_MENU);
					displayLoadingScreen();
					goto DONE_GAMING;
				}
			}

			if (fighter[i]->crash_to_debug) {
				gaming = false;
				game_manager->update_state(GAME_STATE_DEBUG_MENU);
				displayLoadingScreen();
				goto DONE_GAMING;
			}
		}

		/*
			Rendering. The method for rendering relies on setting render targets. pScreenTexture is where all the scalable
			textures are rendered. The GUI will be applied after the render target has been set back to the window.
			After RenderTarget has been set, nothing has to be done with pScreenTexture untill it is time to SDL_Present
			the content in pScreenTexture.
		*/

		SDL_SetRenderTarget(g_renderer, pGui);
		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderClear(g_renderer);

		SDL_SetRenderTarget(g_renderer, pScreenTexture);

		SDL_RenderCopy(g_renderer, stage.pBackgroundTexture, NULL, NULL);

		int render_priority = 0;
		if (fighter[0]->requesting_priority && fighter[1]->requesting_priority) {
			render_priority = fighter_accessor->render_priority;
		}
		else if (fighter[0]->requesting_priority) {
			render_priority = 0;
		}
		else if (fighter[1]->requesting_priority) {
			render_priority = 1;
		}
		else {
			render_priority = fighter_accessor->render_priority_no_req;
		}
		SDL_Rect render_pos;
		SDL_RendererFlip flip;

		render_pos = getRenderPos(fighter[!render_priority], fighter[!render_priority]->fighter_flag[FIGHTER_FLAG_FORCE_ANIM_CENTER]);
		flip = fighter[!render_priority]->facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		SDL_RenderCopyEx(g_renderer, fighter[!render_priority]->anim_kind->spritesheet, &(fighter[!render_priority]->frame_rect), &render_pos, (const double)fighter[!render_priority]->angle, NULL, flip);

		render_pos = getRenderPos(fighter[render_priority], fighter[render_priority]->fighter_flag[FIGHTER_FLAG_FORCE_ANIM_CENTER]);
		flip = fighter[render_priority]->facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		SDL_RenderCopyEx(g_renderer, fighter[render_priority]->anim_kind->spritesheet, &(fighter[render_priority]->frame_rect), &render_pos, (const double)fighter[!render_priority]->angle, NULL, flip);

		for (int i = 0; i < 2; i++) {
			if (fighter[i]->fighter_int[FIGHTER_INT_COMBO_COUNT] > 1) {
				SDL_SetRenderTarget(g_renderer, pGui);
				float id_ope = -1;
				if (fighter[i]->id == 1) {
					id_ope *= -1;
				}
				draw_text("FiraCode-Regular.ttf", to_string(fighter[i]->fighter_int[FIGHTER_INT_COMBO_COUNT]), { id_ope * 500, WINDOW_HEIGHT * 0.75 }, 200, 255, 0, 0, 255);
				SDL_SetRenderTarget(g_renderer, pScreenTexture);
			}

			//Projectile Renders

			for (int o = 0; o < MAX_PROJECTILES; o++) {
				if (fighter[i]->projectiles[o]->id != -1) {
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					if (!fighter[i]->projectiles[o]->facing_right) {
						flip = SDL_FLIP_HORIZONTAL;
					}
					SDL_Rect render_pos;
					render_pos.x = fighter[i]->projectiles[o]->pos.getRenderCoodrinateX();
					render_pos.y = fighter[i]->projectiles[o]->pos.y;
					int width;
					int height;
					SDL_QueryTexture(fighter[i]->projectiles[o]->anim_kind->spritesheet, NULL, NULL, &width, &height);
					render_pos.w = (width / (fighter[i]->projectiles[o]->anim_kind->length + 1));
					render_pos.h = height;
					const double angle = (const double)fighter[i]->projectiles[o]->angle;
					SDL_RenderCopyEx(g_renderer, fighter[i]->projectiles[o]->anim_kind->spritesheet, &(fighter[i]->projectiles[o]->frame_rect), &render_pos, angle, NULL, flip);
				}
			}
		}

		if (debug) {
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(g_renderer, &debug_rect[debugger.target]);
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 127);
			SDL_RenderFillRect(g_renderer, &debug_rect[debugger.target]);
		}

		check_attack_connections(fighter[0], fighter[1], visualize_boxes, !debug || (debug && debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)));

		//Camera things
		camera = updateCamera(fighter[0]->pos.getRenderCoodrinateX(), fighter[0]->pos.getRenderCoodrinateY(), fighter[1]->pos.getRenderCoodrinateX(), fighter[1]->pos.getRenderCoodrinateY(), debugger.zoom);

		//The tag system doesn't play nice with the render offsets I've been working on, we can reimplement it later but for now it's easier to just not think about

/*		for (int i = 0; i < 2; i++) {
			if (fighter[i]->anim_kind->move_dir) {
				player_indicator[i].indicator_rect = SDL_Rect{
				(int)(fighter[i]->pos.getRenderCoodrinateX() - fighter[i]->get_param_float(fighter[i]->anim_kind->name + "_move_offset", fighter[i]->param_table) * fighter[i]->facing_dir + 20),
				(int)(fighter[i]->pos.getRenderCoodrinateYAnim() - 33),
				30,
				30 };
				SDL_RenderCopy(g_renderer, player_indicator[i].texture, nullptr, &(player_indicator[i].indicator_rect));
			}
			else {
				player_indicator[i].indicator_rect = SDL_Rect{
				(int)(fighter[i]->pos.getRenderCoodrinateX() + 20),
				(int)(fighter[i]->pos.getRenderCoodrinateYAnim() - 33),
				30,
				30};
				SDL_RenderCopy(g_renderer, player_indicator[i].texture, nullptr, &(player_indicator[i].indicator_rect));
			}
		}*/

		SDL_SetRenderTarget(g_renderer, pGui); //set target to gui layer
		for (int i = 0; i < 2; i++) {
			
			//The health bar and the regular EX bar target %s are pretty straightforward, and we can update them every frame...

			health_bar[i].health_texture.setTargetPercent(fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] / health_bar[i].max_health);
			ex_bar[i].ex_texture.setTargetPercent(fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] / ex_bar[i].max_ex);

			//...but the segmented EX bar has to be a diva about things because it won't move unless it can look pretty doing it

			int prev_segments = ex_bar[i].prev_segments; //How many bars were already filled
			int segments = floor(fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] / (ex_bar[i].max_ex / EX_METER_BARS)); //How many are now
			ex_bar[i].prev_segments = segments; //Update for the next frame
			if (prev_segments != segments) { //If the value changed
				if (prev_segments > segments) { //If it went down, lower the segmented bar to match the number of segments at the same rate as the
					//regular bar
					ex_bar[i].ex_segment_texture.setTargetPercent((ex_bar[i].max_ex / EX_METER_BARS) / (ex_bar[i].max_ex / segments));
				}
				else if (!(segments % 2)) { //If it went up, and the number of segments is even, increase the segmented bar really fast
					ex_bar[i].ex_segment_texture.setTargetPercent((ex_bar[i].max_ex / EX_METER_BARS) / (ex_bar[i].max_ex / segments), 0.2, 1);
				}
			}
			
			health_bar[i].health_texture.render();
			health_bar[i].bar_texture.render();
			ex_bar[i].ex_texture.render();
			ex_bar[i].ex_segment_texture.render();
			ex_bar[i].bar_texture.render();
		}

		if (!debug) timer.Tick();
		timer.Render();

		SDL_SetRenderTarget(g_renderer, NULL); //set target to the window
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
		SDL_RenderCopy(g_renderer, pScreenTexture, &camera, NULL); //render scale to window
		SDL_RenderCopy(g_renderer, pGui, NULL, NULL); //render gui to window

		SDL_RenderPresent(g_renderer); //finalize
	}

	game_manager->update_state(next_state);
	
	DONE_GAMING:

	for (int i = 0; i < 2; i++) {
		health_bar[i].bar_texture.clearTexture();
		health_bar[i].health_texture.clearTexture();
		ex_bar[i].bar_texture.clearTexture();
		ex_bar[i].ex_texture.clearTexture();
	}
	g_soundmanager.unloadSoundAll();
	cleanup(p1, p2);

	delete fighter_accessor;
	delete battle_loader;
}

/// <summary>
/// Compares all active hitboxes, grabboxes and hurtboxes against each other and/or renders visualizations for all boxes.
/// </summary>
/// <param name="p1">: Player 1 and any projectiles they may have.</param>
/// <param name="p2">: Player 2 and any projectiles they may have.</param>
/// <param name="visualize_boxes">: Whether or not to visualize hit/hurt/grabboxes.</param>
/// <param name="check">: Whether or not to check collisions (This arg is false during frame advance).</param>
void check_attack_connections(Fighter* p1, Fighter* p2, bool visualize_boxes, bool check) {
	Fighter* fighter[2] = { p1, p2 };
	if (check) {
		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (fighter[0]->projectiles[i]->id != -1) {
				for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
					if (fighter[1]->projectiles[i2]->id != -1) {
						for (int i3 = 0; i3 < 10; i3++) {
							if (fighter[0]->projectiles[i]->hitboxes[i3].id != -1
								&& fighter[0]->projectiles[i]->hitboxes[i3].trade) {
								for (int i4 = 0; i4 < 10; i4++) {
									if (fighter[1]->projectiles[i2]->hitboxes[i4].id != -1
										&& fighter[1]->projectiles[i2]->hitboxes[i4].trade) {
										SDL_Rect p1_hitbox, p2_hitbox;
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
		for (int i = 0; i < 2; i++) {
			int hitbox_to_use = HITBOX_COUNT_MAX;
			int projectile_hitbox_to_use = HITBOX_COUNT_MAX;
			int grabbox_to_use = HITBOX_COUNT_MAX;
			fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = false;
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hurtboxes[i2].id != -1) {
					SDL_Rect hurtbox;
					hurtbox = fighter[i]->hurtboxes[i2].rect;

					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[!i]->hitboxes[i3].id != -1) {
							SDL_Rect hitbox;
							hitbox = fighter[!i]->hitboxes[i3].rect;
							if (fighter[!i]->hitboxes[i3].hitbox_kind != HITBOX_KIND_BLOCK) {
								if (is_collide(hitbox, hurtbox)) {
									hitbox_to_use = get_event_hit_collide_player(fighter[!i], fighter[i], &(fighter[!i]->hitboxes[i3]), &(fighter[i]->hurtboxes[i2]));
								}
							}
							else {
								fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = is_collide(hitbox, hurtbox);
							}
							if (hitbox_to_use != HITBOX_COUNT_MAX) {
								break;
							}
						}
					}
					for (int i3 = 0; i3 < MAX_PROJECTILES; i3++) {
						if (fighter[!i]->projectiles[i3]->id != -1) {
							for (int i4 = 0; i4 < 10; i4++) {
								if (fighter[!i]->projectiles[i3]->hitboxes[i4].id != -1) {
									SDL_Rect hitbox;
									hitbox = fighter[!i]->projectiles[i3]->hitboxes[i4].rect;
									if (is_collide(hitbox, hurtbox)) {
										projectile_hitbox_to_use = get_event_hit_collide_projectile(fighter[!i]->projectiles[i3], fighter[i], &(fighter[!i]->projectiles[i3]->hitboxes[i4]), &(fighter[i]->hurtboxes[i2]));
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
						if (fighter[!i]->grabboxes[i3].id != -1) {
							SDL_Rect grabbox;
							grabbox = fighter[!i]->grabboxes[i3].rect;
							if (is_collide(grabbox, hurtbox)) {
								grabbox_to_use = get_event_grab_collide_player(fighter[!i], fighter[i], &(fighter[!i]->grabboxes[i3]), &(fighter[i]->hurtboxes[i2]));
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
			fighter[i]->connected_hitbox = hitbox_to_use;
			fighter[i]->connected_grabbox = grabbox_to_use;
			fighter[i]->connected_projectile_hitbox = projectile_hitbox_to_use;
		}
		if (!event_hit_collide_player(fighter[0], fighter[1], &(fighter[0]->hitboxes[fighter[1]->connected_hitbox]), &(fighter[1]->hitboxes[fighter[0]->connected_hitbox]))) {
			event_grab_collide_player(fighter[0], fighter[1], &(fighter[0]->grabboxes[fighter[1]->connected_grabbox]), &(fighter[1]->grabboxes[fighter[0]->connected_grabbox]));
		}
		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
				if (fighter[i]->projectiles[i2]->id != -1) {
					event_hit_collide_projectile(fighter[i], fighter[!i], fighter[i]->projectiles[i2], &(fighter[i]->projectiles[i2]->hitboxes[fighter[!i]->connected_projectile_hitbox]));
				}
			}
		}
	}
	if (visualize_boxes) {
		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hurtboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter[i]->hurtboxes[i2].rect;
					Vec4f hurtbox_color = { 0, 0, 255, 127 };
					if (fighter[i]->hurtboxes[i2].intangible_kind != INTANGIBLE_KIND_NONE) {
						hurtbox_color.y = 255;
					}

					SDL_SetRenderDrawColor(g_renderer, hurtbox_color.x, hurtbox_color.y, hurtbox_color.z, 255);
					SDL_RenderDrawRect(g_renderer, &render_pos);
					SDL_SetRenderDrawColor(g_renderer, hurtbox_color.x, hurtbox_color.y, hurtbox_color.z, hurtbox_color.w);
					SDL_RenderFillRect(g_renderer, &render_pos);
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hitboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter[i]->hitboxes[i2].rect;

					Vec4f hitbox_color = { 255, 0, 0, 127 };
					if (fighter[i]->hitboxes[i2].hitbox_kind == HITBOX_KIND_BLOCK) {
						hitbox_color.y = 165;
						hitbox_color.w = 50;
					}

					SDL_SetRenderDrawColor(g_renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, 255);
					SDL_RenderDrawRect(g_renderer, &render_pos);
					SDL_SetRenderDrawColor(g_renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, hitbox_color.w);
					SDL_RenderFillRect(g_renderer, &render_pos);
				}
				for (int i3 = 0; i3 < 10; i3++) {
					if (fighter[i]->projectiles[i2]->id != -1 && fighter[i]->projectiles[i2]->hitboxes[i3].id != -1) {
						SDL_Rect render_pos;
						render_pos = fighter[i]->projectiles[i2]->hitboxes[i3].rect;

						Vec4f hitbox_color = { 255, 0, 0, 127 };

						SDL_SetRenderDrawColor(g_renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, 255);
						SDL_RenderDrawRect(g_renderer, &render_pos);
						SDL_SetRenderDrawColor(g_renderer, hitbox_color.x, hitbox_color.y, hitbox_color.z, hitbox_color.w);
						SDL_RenderFillRect(g_renderer, &render_pos);
					}
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->grabboxes[i2].id != -1) {
					SDL_Rect render_pos;
					render_pos = fighter[i]->grabboxes[i2].rect;

					Vec4f grabbox_color = { 0, 255, 0, 127 };
					if (fighter[i]->grabboxes[i2].grabbox_kind & GRABBOX_KIND_NOTECH) {
						grabbox_color.x = 128;
						grabbox_color.y = 0;
						grabbox_color.z = 128;
					}

					SDL_SetRenderDrawColor(g_renderer, grabbox_color.x, grabbox_color.y, grabbox_color.z, 255);
					SDL_RenderDrawRect(g_renderer, &render_pos);
					SDL_SetRenderDrawColor(g_renderer, grabbox_color.x, grabbox_color.y, grabbox_color.z, grabbox_color.w);
					SDL_RenderFillRect(g_renderer, &render_pos);
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

/// <summary>
/// Checks if any of the attacker's hitboxes have connected with any of the defender's hurtboxes, and sets up values based on what type of event the
/// connection should trigger.
/// </summary>
/// <param name="attacker">: The attacker.</param>
/// <param name="defender">: The defender.</param>
/// <param name="hitbox">: The hitbox that connected.</param>
/// <param name="hurtbox">: The hurtbox that connected.</param>
/// <returns>Either the first hitbox ID to successfully trigger a collision event, or HITBOX_KIND_MAX if a different hitbox ID must be checked.</returns>
int get_event_hit_collide_player(Fighter* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
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
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->max_juggle < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
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
	if (defender->is_actionable() || defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
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
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 14;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 14 + PARRY_ADVANTAGE_FRAMES - hitbox->blockstun;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
		return hitbox->id;
	}
	if (blocking && !hitbox->unblockable) {
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		defender->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	attacker->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = true;
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
int get_event_grab_collide_player(Fighter* attacker, Fighter* defender, Grabbox* grabbox, Hurtbox* hurtbox) {
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
int get_event_hit_collide_projectile(Projectile* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox) {
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
	if (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->max_juggle < defender->fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
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
	if (defender->is_actionable() || defender->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
		if (defender->get_stick_dir() == 4 || defender->get_stick_dir() == 1) {
			blocking = true;
		}
	}
	if (defender->fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID || defender->fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			parrying = true;
		}
	}
	if (parrying) {
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 6;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 6 + PARRY_ADVANTAGE_FRAMES - hitbox->blockstun;
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = true;
		return hitbox->id;
	}
	if (blocking && !hitbox->unblockable) {
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
		attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
		defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
		defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
		defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		defender->fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
		defender->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = true;
		return hitbox->id;
	}

	attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	attacker->projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	defender->fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = defender->fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	defender->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(defender->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
	attacker->projectile_flag[PROJECTILE_FLAG_HIT] = true;
	return hitbox->id;
}

/// <summary>
/// Handle any potential hitbox collision events between two Fighters on this frame and change statuses/decrease health accordingly.
/// </summary>
/// <param name="p1">: Player 1</param>
/// <param name="p2">: Player 2</param>
/// <param name="p1_hitbox">: Which of P1's hitboxes (if any) first connected with P2.</param>
/// <param name="p2_hitbox">: Which of P2's hitboxes (if any) first connected with P1.</param>
/// <returns>Whether or not any kind of collision event occured.</returns>
bool event_hit_collide_player(Fighter* p1, Fighter* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
	bool p1_hit = p2_hitbox->id != -1;
	bool p2_hit = p1_hitbox->id != -1;
	u32 p1_status_post_hit = p1->status_kind;
	u32 p2_status_post_hit = p2->status_kind;
	if (p1_hit && p2_hit) { //Both players got hit
		if (p1->situation_kind != p2->situation_kind) {
			if (p1->situation_kind == FIGHTER_SITUATION_GROUND) {
				/*
					Grounded opponents will always win trades against aerial opponents. This is important for balancing because holy shit jumping is
					gonna be busted without stuff like this
					Oh also, no need to check OTG because there won't be any attacks you can use outside of FIGHTER_SITUATION_GROUND or
					FIGHTER_SITUATION_AIR
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
					p2->change_status(FIGHTER_STATUS_CLANK);
					return false;
				}
				else if (p1_hitbox->clank_kind != CLANK_KIND_CONTINUE) {
					p1->change_status(FIGHTER_STATUS_CLANK);
					return false;
				}
				else { //If both people have trample, it's better to make them both clank than it is to make them both ignore having just been hit
					p1->change_status(FIGHTER_STATUS_CLANK);
					p2->change_status(FIGHTER_STATUS_CLANK);
					return false;
				}
			}
			else if (p1_hitbox->clank_kind == CLANK_KIND_CLANK || p2_hitbox->clank_kind == CLANK_KIND_CLANK) {
				p1->change_status(FIGHTER_STATUS_CLANK);
				p2->change_status(FIGHTER_STATUS_CLANK);
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
		if (p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block / 2, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2->get_param_float("meter_gain_on_parry"), EX_METER_SIZE);
			p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = FIGHTER_STATUS_PARRY;
		}
		else if (p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(!p1_hitbox->can_chip_ko, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
			p1->fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS] = true;
			p2_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
		}
		else if (!p1->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
			//If Attack Connected is false but this still got set off, that means that the opponent hit you while you had armor. In this situation,
			//the attacker gets the same meter gain as if they got blocked, and the defender takes half the usual amount of chip damage. However, this
			//chip damage is incapable of KOing the defender no matter what they got hit by.

			//Note: This is also what will happen to Leon if he gets hit while he has Right of Way armor, so if we ever want to remove the chip damage
			//for when he gets hit with RoW, we'll need to find a way to account for that. I don't think it'll be that big of a deal though.
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - (p1_hitbox->chip_damage / 2), p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
		}
		else {
			/*
				If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
				is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
			*/
			p2->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
			if (p2->get_status_group() == STATUS_GROUP_HITSTUN) {
				if (!p1->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
					p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += p1_hitbox->scale;
				}
			}
			else {
				p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
			}
			p2->fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED] = p1_hitbox->launch_init_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x;
			/*
			If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise,
			increase it by one so that the opponent's juggle value is always going up
			*/
			if (p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] >= p1_hitbox->juggle_set) {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE]++;
			}
			else {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_set;
			}
			float prev_x = p2->pos.x;
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			if (can_counterhit(p2, p1_hitbox)) {
				if (p1_hitbox->scale == -5) {
					p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -5;
				}
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_counterhit, EX_METER_SIZE);
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage * p1_hitbox->counterhit_damage_mul, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
				p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
				p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
				if (p2->status_kind == FIGHTER_STATUS_LAUNCH && p1_hitbox->continue_launch) {
					p2_status_post_hit = FIGHTER_STATUS_LAUNCH;
				}
			}
			else {
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_hit, EX_METER_SIZE);
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage * ((clampf(1, 10 - p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10), p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
				if (p2->status_kind == FIGHTER_STATUS_LAUNCH && p1_hitbox->continue_launch) {
					p2_status_post_hit = FIGHTER_STATUS_LAUNCH;
				}
			}
			p1->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = false;
			p1->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
		}
	}

	if (p1_hit) { //P1 got hit
		p2->update_hitbox_connect(p2_hitbox->multihit);
		if (p1->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2_hitbox->meter_gain_on_block / 2, EX_METER_SIZE);
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1->get_param_float("meter_gain_on_parry"), EX_METER_SIZE);
			p1->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p1_status_post_hit = FIGHTER_STATUS_PARRY;
		}
		else if (p1->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2_hitbox->meter_gain_on_block, EX_METER_SIZE);
			p1->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(!p2_hitbox->can_chip_ko, p1->fighter_float[FIGHTER_FLOAT_HEALTH] - p2_hitbox->chip_damage, p1->fighter_float[FIGHTER_FLOAT_HEALTH]);
			float prev_x = p1->pos.x;
			p1->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->block_pushback / p1->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p1->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
			p2->fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS] = true;
			p1_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
		}
		else if (!p2->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2_hitbox->meter_gain_on_block, EX_METER_SIZE);
			p1->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1, p1->fighter_float[FIGHTER_FLOAT_HEALTH] - (p2_hitbox->chip_damage / 2), p1->fighter_float[FIGHTER_FLOAT_HEALTH]);
		}
		else {
			p1->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
			if (p1->get_status_group() == STATUS_GROUP_HITSTUN) {
				if (!p2->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
					p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += p2_hitbox->scale;
				}
			}
			else {
				p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
			}
			p1->fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED] = p2_hitbox->launch_init_y;
			p1->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = p2_hitbox->launch_gravity_y;
			p1->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = p2_hitbox->launch_max_fall_speed;
			p1->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = p2_hitbox->launch_speed_x;
			if (p1->fighter_int[FIGHTER_INT_JUGGLE_VALUE] >= p2_hitbox->juggle_set) {
				p1->fighter_int[FIGHTER_INT_JUGGLE_VALUE]++;
			}
			else {
				p1->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p2_hitbox->juggle_set;
			}
			p1->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p2_hitbox->hit_pushback / p1->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			if (can_counterhit(p1, p2_hitbox)) {
				if (p2_hitbox->scale == -5) {
					p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -5;
				}
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2_hitbox->meter_gain_on_counterhit, EX_METER_SIZE);
				p1->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_HEALTH] - p2_hitbox->damage * p2_hitbox->counterhit_damage_mul, p1->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p1->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
				p1->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
				p1->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p1_status_post_hit = get_damage_status(p2_hitbox->counterhit_status, p1->situation_kind);
				if (p1->status_kind == FIGHTER_STATUS_LAUNCH && p2_hitbox->continue_launch) {
					p1_status_post_hit = FIGHTER_STATUS_LAUNCH;
				}
			}
			else {
				p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2_hitbox->meter_gain_on_hit, EX_METER_SIZE);
				p1->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_HEALTH] - p2_hitbox->damage * ((clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10), p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p1->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = p2_hitbox->attack_level;
				p1_status_post_hit = get_damage_status(p2_hitbox->hit_status, p1->situation_kind);
				if (p1->status_kind == FIGHTER_STATUS_LAUNCH && p2_hitbox->continue_launch) {
					p1_status_post_hit = FIGHTER_STATUS_LAUNCH;
				}
			}
			p2->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = false;
			p2->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
		}
	}
	if (p1_hit) { //Rerunning move scripts to put up the first frame of the defender's hurtbox on the frame they get hit
		p1->change_status(p1_status_post_hit, true, false);
		p1->move_script.move_script();
		p1->update_hurtbox_pos();
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
		p2->move_script.move_script();
		p2->update_hurtbox_pos();
	}
	return (p1_hit || p2_hit);
}

/// <summary>
/// Handle any potential grabbox collision events between two Fighters on this frame and change statuses accordingly.
/// </summary>
/// <param name="p1">: Player 1</param>
/// <param name="p2">: Player 2</param>
/// <param name="p1_hitbox">: Which of P1's grabboxes (if any) first connected with P2.</param>
/// <param name="p2_hitbox">: Which of P2's grabboxes (if any) first connected with P1.</param>
/// <returns>Whether or not any kind of collision event occured.</returns>
void event_grab_collide_player(Fighter* p1, Fighter* p2, Grabbox* p1_grabbox, Grabbox* p2_grabbox) {
	bool p1_hit = p2_grabbox->id != -1;
	bool p1_tech = p2_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH;
	bool p2_hit = p1_grabbox->id != -1;
	bool p2_tech = p1_grabbox->grabbox_kind & GRABBOX_KIND_NOTECH;
	if (p1_hit && p2_hit) {
		if (p1_tech == p2_tech) {
			p1->change_status(FIGHTER_STATUS_THROW_TECH);
			p2->change_status(FIGHTER_STATUS_THROW_TECH);
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
		if (p1->fighter_flag[FIGHTER_FLAG_THROW_TECH] && p1_tech) {
			p1->change_status(FIGHTER_STATUS_THROW_TECH);
			p2->change_status(FIGHTER_STATUS_THROW_TECH);
		}
		else {
			p2->change_status(p2_grabbox->attacker_status_if_hit);
			p1->change_status(p2_grabbox->defender_status_if_hit);
		}
		p1->move_script.move_script();
		p1->update_hurtbox_pos();
	}
	if (p2_hit) {
		if (p2->fighter_flag[FIGHTER_FLAG_THROW_TECH] && p2_tech) {
			p1->change_status(FIGHTER_STATUS_THROW_TECH);
			p2->change_status(FIGHTER_STATUS_THROW_TECH);
		}
		else {
			p1->change_status(p1_grabbox->attacker_status_if_hit);
			p2->change_status(p1_grabbox->defender_status_if_hit);
		}
		p2->move_script.move_script();
		p2->update_hurtbox_pos();
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
void event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox) {
	bool p2_hit = p1_hitbox->id != -1;
	u32 p2_status_post_hit = p2->status_kind;
	if (p2_hit) {
		p1_projectile->update_hitbox_connect(p1_hitbox->multihit);
		p1_projectile->projectile_int[PROJECTILE_INT_HEALTH] --;
		if (p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block / 2, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2->get_param_float("meter_gain_on_parry"), EX_METER_SIZE);
			p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = FIGHTER_STATUS_PARRY;
		}
		else if (p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(!p1_hitbox->can_chip_ko, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
			p2_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
		}
		else if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block / 2, EX_METER_SIZE);
			p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage / 2, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
		}
		else {
			/*
				If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
				is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
			*/
			p2->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
			if (p2->get_status_group() == STATUS_GROUP_HITSTUN) {
				if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS]) {
					p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += p1_hitbox->scale;
				}
			}
			else {
				p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
			}
			p2->fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED] = p1_hitbox->launch_init_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = p1_hitbox->launch_gravity_y;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = p1_hitbox->launch_max_fall_speed;
			p2->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = p1_hitbox->launch_speed_x;
			/*
			If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise,
			increase it by one so that the opponent's juggle value is always going up
			*/
			if (p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] >= p1_hitbox->juggle_set) {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE]++;
			}
			else {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_set;
			}
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			if (can_counterhit(p2, p1_hitbox)) {
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_counterhit, EX_METER_SIZE);
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage * p1_hitbox->counterhit_damage_mul, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
				p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
				p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
			}
			else {
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_hit, EX_METER_SIZE);
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage * ((clampf(1, 10 - p1->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10), p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = p1_hitbox->attack_level;
				p2_status_post_hit = get_damage_status(p1_hitbox->hit_status, p2->situation_kind);
			}
			if (p2->status_kind == FIGHTER_STATUS_LAUNCH && p1_hitbox->continue_launch) {
				p2_status_post_hit = FIGHTER_STATUS_LAUNCH;
			}
		}
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT] = false;
		p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = true;
		p1->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS] = true;
	}
	if (p2_hit) {
		p2->change_status(p2_status_post_hit, true, false);
		p2->move_script.move_script();
		p2->update_hurtbox_pos();
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="defender"></param>
/// <param name="hitbox"></param>
/// <returns></returns>
bool can_counterhit(Fighter* defender, Hitbox* hitbox) {
	if (defender->anim_kind->name == "hitstun_parry" || defender->anim_kind->name == "hitstun_parry_air") {
		return true;
	}
	return defender->fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] && (hitbox->counterhit_type == COUNTERHIT_TYPE_NORMAL
	|| (defender->situation_kind == FIGHTER_SITUATION_AIR && hitbox->counterhit_type == COUNTERHIT_TYPE_AERIAL));
}

/// <summary>
/// 
/// </summary>
/// <param name="hit_status"></param>
/// <param name="situation_kind"></param>
/// <returns></returns>
int get_damage_status(int hit_status, int situation_kind) {
	if (hit_status == HIT_STATUS_CRUMPLE) {
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			return FIGHTER_STATUS_LAUNCH;
		}
		else {
			return FIGHTER_STATUS_CRUMPLE;
		}
	}
	else if (hit_status == HIT_STATUS_LAUNCH) {
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			return FIGHTER_STATUS_LAUNCH;
		}
		else {
			return FIGHTER_STATUS_LAUNCH_START;
		}
	}
	else if (hit_status == HIT_STATUS_KNOCKDOWN) {
		return FIGHTER_STATUS_KNOCKDOWN_START;
	}
	else {
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			return FIGHTER_STATUS_HITSTUN_AIR;
		}
		else {
			return FIGHTER_STATUS_HITSTUN;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="fighter"></param>
void decrease_common_fighter_variables(Fighter* fighter) {
	if (fighter->fighter_int[FIGHTER_INT_236_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_236_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_236_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_214_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_214_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_214_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_623_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_623_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_623_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_41236_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_41236_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_41236_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_63214_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_63214_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_63214_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_236236_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_236236_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_236236_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_214214_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_214214_TIMER] --;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_214214_STEP] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER]--;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
	}
	if (fighter->fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		fighter->fighter_int[FIGHTER_INT_HITLAG_FRAMES]--;
	}
	else { 
		if (fighter->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] != 0) {
			fighter->fighter_int[FIGHTER_INT_HITSTUN_FRAMES]--;
		}
		if (fighter->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] != 0) {
			fighter->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES]--;
		}
	}
	if (fighter->fighter_int[FIGHTER_INT_DASH_F_WINDOW] != 0) {
		fighter->fighter_int[FIGHTER_INT_DASH_F_WINDOW]--;
	}
	if (fighter->fighter_int[FIGHTER_INT_DASH_B_WINDOW] != 0) {
		fighter->fighter_int[FIGHTER_INT_DASH_B_WINDOW]--;
	}
	if (fighter->fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] != 0) {
		fighter->fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW]--;
	}
	if (fighter->fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] != 0) {
		fighter->fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER]--;
	}
	else {
		fighter->fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="projectile"></param>
void decrease_common_projectile_variables(Projectile* projectile) {
	if (projectile->projectile_int[PROJECTILE_INT_ACTIVE_TIME] > 0) {
		projectile->projectile_int[PROJECTILE_INT_ACTIVE_TIME] --;
	}
	if (projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0) {
		projectile->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] --;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
void cleanup(IObject* p1, IObject* p2) {
	Fighter* fighter[2];
	fighter[0] = p1->get_fighter();
	fighter[1] = p2->get_fighter();
	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			if (fighter[i]->projectile_objects[i2] != NULL) {
				delete fighter[i]->projectile_objects[i2];
			}
			delete fighter[i]->projectiles[i2];
		}
		delete fighter[i];
	}
	delete p1;
	delete p2;
}

IObject::IObject() {}

IObject::IObject(int object_type, int object_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	if (object_type == OBJECT_TYPE_FIGHTER) {
		switch (object_kind) {
			case (CHARA_KIND_ROY):
			default: {
				fighter = new Roy(id, player_info, fighter_accessor);
			}
			break;
			case (CHARA_KIND_ERIC):
			{
				fighter = new Eric(id, player_info, fighter_accessor);
			}
			break;
			case (CHARA_KIND_LEON):
			case (CHARA_KIND_CHAMELEON): //Leon and Chameleon are the same character, but the specifics on where to get resources are in the constructor
			{
				fighter = new Leon(id, player_info, fighter_accessor);
			}
			break;
			case (CHARA_KIND_ANGELICA):
			{
				fighter = new Angelica(id, player_info, fighter_accessor);
			} 
			break;
			case (CHARA_KIND_NIGHTSABER):
			{
				fighter = new Nightsaber(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_SULLY):
			{
				fighter = new Sully(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_PRIEST):
			{
				fighter = new Priest(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_AZIEL):
			{
				fighter = new Aziel(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_BRUNO):
			{
				fighter = new Bruno(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_TESSA):
			{
				fighter = new Tessa(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_ALEJANDRO):
			{
				fighter = new Alejandro(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_NORMAN):
			{
				fighter = new Norman(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_ATLAS):
			{
				fighter = new Atlas(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_JULIUS):
			{
				fighter = new Julius(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_RAMONA):
			{
				fighter = new Ramona(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_ZYAIR):
			{
				fighter = new Zyair(id, player_info, fighter_accessor);
			} break;
			case (CHARA_KIND_VESUVIUS):
			{
				fighter = new Vesuvius(id, player_info, fighter_accessor);
			} break;
		}
	}
	else if (object_type == OBJECT_TYPE_PROJECTILE) {
		switch (object_kind) {
			case (PROJECTILE_KIND_ROY_FIREBALL):
			{
				projectile = new RoyFireball(id, player_info, fighter_accessor);
			}
			break;
			case (PROJECTILE_KIND_ERIC_FIREBALL):
			{
				projectile = new EricFireball(id, player_info, fighter_accessor);
			}
			break;
			case (PROJECTILE_KIND_PROJECTILE_TEMPLATE):
			{
				projectile = new ProjectileTemplate(id, player_info, fighter_accessor);
			} break;
			case (PROJECTILE_KIND_MAX):
			{
				projectile = NULL;
			}
			break;
		}
	}

}

IObject::~IObject() {}

Fighter* IObject::get_fighter() {
	return fighter;
}

Projectile* IObject::get_projectile() {
	return projectile;
}

SDL_Rect getRenderPos(Fighter* fighter, bool force_center) {
	SDL_Rect render_pos = fighter->anim_kind->anim_map[fighter->render_frame];
	if (force_center) {
		render_pos.x = fighter->pos.getRenderCoodrinateX();
	}
	else {
		render_pos.x = fighter->pos.getRenderCoodrinateXAnim();
	}
	render_pos.y = fighter->pos.getRenderCoodrinateYAnim();
	if (render_pos.w == 0 && render_pos.h == 0) {
		int width;
		int sprite_width = 0;
		int height;
		SDL_QueryTexture(fighter->anim_kind->spritesheet, NULL, NULL, &width, &height);
		render_pos.w = (width / (fighter->anim_kind->length + 1));
		if (!fighter->facing_right && force_center) {
			SDL_QueryTexture(fighter->base_texture, NULL, NULL, &sprite_width, NULL);
			render_pos.x -= (render_pos.w - sprite_width);
		}
		render_pos.h = height;
	}

	return render_pos;
}