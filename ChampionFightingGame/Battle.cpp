#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <SDL.h>
#include "Animation.h"
#include "Battle.h"
#include "Stage.h"
#include "Menu.h"
#include "SoundManager.h"
#include "GameTexture.h"

#include "Fighters.h"
#include "FighterInterface.h"

#include "BattleObject.h"
#include "Fighter.h"
#include "FighterAccessor.h"

#include "Projectile.h"
#include "RoyFireball.h"
#include "EricFireball.h"

#include "CharaTemplate.h"
#include "ProjectileTemplate.h"
#include "Loader.h"

#include "Model.h"
#include "RenderManager.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern SoundManager g_soundmanager;
extern RenderManager g_rendermanager;
extern SoundInfo sounds[3][MAX_SOUNDS];

extern bool debug;

void battle_main(GameManager* game_manager) {
	PlayerInfo* player_info[2];
	for (int i = 0; i < 2; i++) {
		player_info[i] = game_manager->player_info[i];
	}
	Battle battle;
	battle.load_battle(game_manager);

	while (*game_manager->looping[game_manager->layer]) {
		battle.frame_delay_check_performance();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							int width;
							int height;
							SDL_GetWindowSize(g_window, &width, &height);
							glViewport(0, 0, width, height);
						} break;
					}
				} break;
			}
		}

		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			if (debug) {
				g_soundmanager.pauseSEAll(i);
				g_soundmanager.pauseVCAll(i);
			}
		}

		battle.process_main();

		battle.render_world();
		battle.render_ui();

		battle.check_collisions();
		SDL_GL_SwapWindow(g_window);
	}

	battle.unload_battle();
}

Battle::Battle() {}

Battle::~Battle() {}

void Battle::load_battle(GameManager* game_manager) {
	game_loader = new GameLoader(16);
	SDL_Thread* loading_thread;
	loading_thread = SDL_CreateThread(LoadingScreen, "Init.rar", (void*)game_loader);
	SDL_DetachThread(loading_thread);

	visualize_boxes = true;

	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	int rng = rand() % 2;
	Stage stage = player_info[rng]->stage;

	fighter_accessor = new FighterAccessor;
	inc_thread();

	for (int i = 0; i < 2; i++) {
		debug_rect[i].init();
		inc_thread();
		fighter[i] = create_fighter(player_info[i]->chara_kind, i, player_info[i], fighter_accessor);
		inc_thread();
	}
	for (int i = 0; i < 2; i++) {
		fighter[i]->player_info = player_info[i];
		fighter[i]->pos.x = 0;
		fighter_accessor->fighter[i] = fighter[i];
		fighter[i]->fighter_accessor = fighter_accessor;
		fighter[i]->superInit(i);
		inc_thread();
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			if (fighter[i]->projectiles[i2] != nullptr) {
				fighter[i]->projectiles[i2]->owner_id = i;
			}
		}
		health_bar[i].init(fighter[i]);
		inc_thread();
		ex_bar[i].init(fighter[i]);
		inc_thread();
		player_indicator[i] = PlayerIndicator(fighter[i]);
		inc_thread();
	}
	timer.init(99);
	inc_thread();
	g_soundmanager.fighter_accessor = fighter_accessor;
	for (int i = 0; i < 2; i++) {
		fighter[i]->loadCharaSounds();
		inc_thread();
	}

	bool loading = true;
	while (loading) {
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

		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			player_info[i]->poll_buttons(keyboard_state);
			if (player_info[i]->is_any_inputs()) {
				loading = false;

				for (int i2 = 0; i2 < BUFFER_WINDOW; i2++) {
					player_info[i]->poll_buttons(keyboard_state);
					player_info[!i]->poll_buttons(keyboard_state);
				}
			}
		}
	}
	game_loader->finished = true;
	game_manager->set_menu_info(this);

	if (getGameSetting("music_setting") == MUSIC_SETTING_STAGE) {
		g_soundmanager.loadMusic(stage.default_music_kind);
		g_soundmanager.playMusic(stage.default_music_kind);
	}
	else if (getGameSetting("music_setting") == MUSIC_SETTING_CHARA) {
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}
	ticks = SDL_GetTicks();
}
void Battle::unload_battle() {
	for (int i = 0; i < 2; i++) {
		health_bar[i].destroy();
		ex_bar[i].destroy();
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			if (fighter[i]->projectiles[i2] != nullptr) {
				for (int i3 = 0; i3 < 10; i3++) {
					fighter[i]->projectiles[i2]->hitboxes[i3].rect.destroy();
				}
				delete fighter[i]->projectiles[i2];
			}
		}
		for (int i2 = 0; i2 < 10; i2++) {
			fighter[i]->hitboxes[i2].rect.destroy();
			fighter[i]->hurtboxes[i2].rect.destroy();
			fighter[i]->grabboxes[i2].rect.destroy();
		}
		delete fighter[i];
	}
	g_soundmanager.unloadSoundAll();

	delete fighter_accessor;
	delete game_loader;
}

void Battle::process_main() {
	SDL_PumpEvents();
	keyboard_state = SDL_GetKeyboardState(NULL);

	debugger.poll_inputs(keyboard_state);

	if (debugger.check_button_trigger(BUTTON_DEBUG_ENABLE)) {
		debug = !debug;
		timer.ClockMode = !timer.ClockMode;
		if (!debug) {
			for (int i = 0; i < 2; i++) {
				g_soundmanager.resumeSEAll(i);
				g_soundmanager.resumeVCAll(i);
			}
		}
	}
	if (debug) {
		process_debug();
	}
	else {
		timer.Tick();
		pre_process_fighter();
		process_fighter();
		post_process_fighter();
		process_ui();
	}
	for (int i = 0; i < 2; i++) {
		if (fighter[i]->crash_to_debug) {
			*looping = false;
			*game_state = GAME_STATE_DEBUG_MENU;
		}
	}
}

void Battle::pre_process_fighter() {
	for (int i = 0; i < 2; i++) {
		if (fighter[i]->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION]) {
			return;
		}
	}
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
					fighter[i]->add_pos(vec3(-1.0 * fighter[i]->facing_dir, 0, 0));
					fighter[!i]->add_pos(vec3(-1.0 * fighter[i]->facing_dir, 0, 0));

					fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
					fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
				}
			}
		}
	}
}

void Battle::process_fighter() {
	for (int i = 0; i < 2; i++) {
		player_info[i]->poll_buttons(keyboard_state);
		fighter[i]->fighter_main();
	}
}

void Battle::post_process_fighter() {
	for (int i = 0; i < 2; i++) {
		fighter[i]->update_hitbox_pos();
		fighter[i]->update_grabbox_pos();
		fighter[i]->update_hurtbox_pos();
		fighter[i]->rot.z += radians(90.0 * fighter[i]->facing_dir);
		fighter[i]->rot += fighter[i]->extra_rot;
		fighter[i]->create_jostle_rect(vec2{ -15, 25 }, vec2{ 15, 0 });
	}
}

void Battle::process_ui() {
	for (int i = 0; i < 2; i++) {
		health_bar[i].process();
		ex_bar[i].process();
	}
}

void Battle::process_debug() {
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
		timer.Tick();
		for (int i = 0; i < 2; i++) {
			g_soundmanager.resumeSEAll(i);
			g_soundmanager.resumeVCAll(i);
		}
		pre_process_fighter();
		process_fighter();
		post_process_fighter();
		process_ui();
		if (debugger.print_frames) {
			cout << "Player " << debugger.target + 1 << " Frame: " << fighter[debugger.target]->frame << endl;
			cout << "Player " << debugger.target + 1 << " Pos X: " << fighter[debugger.target]->pos.x << endl;
			cout << "Player " << debugger.target + 1 << " Pos Y: " << fighter[debugger.target]->pos.y << endl;
			cout << "Player " << debugger.target + 1 << " Health: " << fighter[debugger.target]->fighter_float[FIGHTER_FLOAT_HEALTH] << endl;
		}
	}
	debugger.debug_mode(fighter[debugger.target], &debug_rect[debugger.target], &debug_anchor[debugger.target], &debug_offset[debugger.target]);
	if (debugger.check_button_trigger(BUTTON_DEBUG_RESET)) {
		debug = false;
		*looping = false;
		*game_state = GAME_STATE_DEBUG_MENU;
	}
}

void Battle::render_world() {
	//update camera pos

	//render the stage

	for (int i = 0; i < 2; i++) {
		fighter[i]->render();
		//player tags will go here

		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->id != -1 && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render();
			}
		}
	}
}

void Battle::render_ui() {
	for (int i = 0; i < 2; i++) {
		if (visualize_boxes) {
			fighter[i]->jostle_box.render();
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hurtboxes[i2].id != -1) {
					fighter[i]->hurtboxes[i2].rect.render();
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hitboxes[i2].id != -1) {
					fighter[i]->hitboxes[i2].rect.render();
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->id != -1) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->hitboxes[i3].id != -1) {
							fighter[i]->projectiles[i2]->hitboxes[i3].rect.render();
						}
					}
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->grabboxes[i2].id != -1) {
					fighter[i]->grabboxes[i2].rect.render();
				}
			}
			if (fighter[i]->fighter_int[FIGHTER_INT_COMBO_COUNT] > 1) {
				//Combo Counter text here
			}
		}
		health_bar[i].render();
		ex_bar[i].render();
	}
	timer.Render();

	if (debug) {
		debug_rect[debugger.target].render();
	}
}

void Battle::check_collisions() {
	if (debug && !debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)) {
		return;
	}
	check_projectile_collisions();
	check_fighter_collisions();
}

void Battle::check_projectile_collisions() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (fighter[0]->projectiles[i] != nullptr && fighter[0]->projectiles[i]->id != -1) {
			for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
				if (fighter[1]->projectiles[i2] != nullptr && fighter[1]->projectiles[i2]->id != -1) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[0]->projectiles[i]->hitboxes[i3].id != -1 && fighter[0]->projectiles[i]->hitboxes[i3].trade) {
							for (int i4 = 0; i4 < 10; i4++) {
								if (fighter[1]->projectiles[i2]->hitboxes[i4].id != -1 && fighter[1]->projectiles[i2]->hitboxes[i4].trade) {
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
		for (int i2 = 0; i2 < 10; i2++) {
			if (fighter[i]->hurtboxes[i2].id != -1) {
				GameRect hurtbox = fighter[i]->hurtboxes[i2].rect;

				for (int i3 = 0; i3 < 10; i3++) {
					if (fighter[!i]->hitboxes[i3].id != -1) {
						GameRect hitbox = fighter[!i]->hitboxes[i3].rect;
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
					if (fighter[!i]->projectiles[i3] != nullptr && fighter[!i]->projectiles[i3]->id != -1) {
						for (int i4 = 0; i4 < 10; i4++) {
							if (fighter[!i]->projectiles[i3]->hitboxes[i4].id != -1) {
								GameRect hitbox = fighter[!i]->projectiles[i3]->hitboxes[i4].rect;
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
						GameRect grabbox = fighter[!i]->grabboxes[i3].rect;
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
			if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->id != -1) {
				event_hit_collide_projectile(fighter[i], fighter[!i], fighter[i]->projectiles[i2], &(fighter[i]->projectiles[i2]->hitboxes[fighter[!i]->connected_projectile_hitbox]));
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
bool Battle::event_hit_collide_player(Fighter* p1, Fighter* p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox) {
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
				p1->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_HEALTH] - p2_hitbox->damage * ((clampf(1, 10 - p2->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10), p1->fighter_float[FIGHTER_FLOAT_HEALTH]);
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
void Battle::event_grab_collide_player(Fighter* p1, Fighter* p2, Grabbox* p1_grabbox, Grabbox* p2_grabbox) {
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
void Battle::event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox) {
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
bool Battle::can_counterhit(Fighter* defender, Hitbox* hitbox) {
	if (defender->anim_kind == nullptr) {
		return false;
	}
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
int Battle::get_damage_status(int hit_status, int situation_kind) {
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

HealthBar::HealthBar() {}

void HealthBar::init(Fighter* fighter) {
	health_texture.init("resource/ui/game/hp/health.png");
	bar_texture.init("resource/ui/game/hp/bar.png");
	if (fighter->id) {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		health_texture.flip_h();
		bar_texture.flip_h();
	}
	else {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	}
	health = &fighter->fighter_float[FIGHTER_FLOAT_HEALTH];
	max_health = fighter->get_param_float("health");
}

void HealthBar::destroy() {
	health_texture.destroy();
	bar_texture.destroy();
}

void HealthBar::process() {
	health_texture.scale_left_percent(*health / max_health);
}

void HealthBar::render() {
	health_texture.render();
	bar_texture.render();
}

ExBar::ExBar() {}

void ExBar::init(Fighter* fighter) {
	this->fighter = fighter;
	ex = &fighter->fighter_float[FIGHTER_FLOAT_SUPER_METER];
	max_ex = EX_METER_SIZE;
	ex_texture.init("resource/ui/game/ex/ex.png");
	ex_segment_texture.init("resource/ui/game/ex/ex_segment.png");
	bar_texture.init("resource/ui/game/ex/bar.png");
	ex_texture.set_pos(vec3(119.0, 60.0, 0.0));
	ex_segment_texture.set_pos(vec3(119.0, 60.0, 0.0));

	if (fighter->id == 0) {
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	else {
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_texture.flip_h();
		ex_segment_texture.flip_h();
		bar_texture.flip_h();
	}
	ex_texture.scale_right_percent(0);
	ex_segment_texture.scale_right_percent(0);
}

void ExBar::destroy() {
	ex_texture.destroy();
	ex_segment_texture.destroy();
	bar_texture.destroy();
}

void ExBar::process() {
	ex_texture.set_right_target(*ex / max_ex, 6);

	int segments = floor(*ex / (max_ex / EX_METER_BARS));
	if (prev_segments != segments) {
		if (prev_segments > segments) {
			ex_segment_texture.set_right_target((*ex / EX_METER_BARS) / (max_ex / segments), 1);
		}
		else if (!(segments % 2)) {
			ex_segment_texture.set_right_target((max_ex / EX_METER_BARS) / (max_ex / segments), 1);
		}
	}
	prev_segments = segments;
	ex_texture.process();
	ex_segment_texture.process();
}

void ExBar::render() {
	ex_texture.render();
	ex_segment_texture.render();
	bar_texture.render();
}

PlayerIndicator::PlayerIndicator() {}
PlayerIndicator::PlayerIndicator(Fighter* fighter, string nametag) {
	this->fighter = fighter;
	this->nametag = nametag;
	string resource_dir = "resource/ui/game/tag/";
	if (fighter->id == 0) {
		resource_dir += "p1_tag";
	}
	else {
		resource_dir += "p2_tag";
	}
	if (nametag == "") {
		resource_dir += "_no_nametag";
	}
	resource_dir += ".png";
	const char* file_dir = resource_dir.c_str();
	this->texture = loadSDLTexture(file_dir);
	this->indicator_rect.x = 0;
	this->indicator_rect.y = 0;
	this->indicator_rect.w = 92;
	this->indicator_rect.h = 92;
}
GameTimer::GameTimer() {};
GameTimer::GameTimer(int time) {
	init(time);
};

void GameTimer::init(int time) {
	uiDecaseconds = time / 10;
	uiSeconds = time % 10;
	uiDecaframes = 6;
	uiFrames = 0;
	ClockMode = 1;
	pBigTypeface = loadSDLTexture("resource/ui/game/timer/bigtypeface.png");
	pSmallTypeface = loadSDLTexture("resource/ui/game/timer/smalltypeface.png");
	pClockFace = loadSDLTexture("resource/ui/game/timer/clockface.png");
}

void GameTimer::Tick() {
	//printf("%d%d:%d%d\n\n",uiDecaseconds,uiSeconds,uiDecaframes,uiFrames);
	if (uiFrames == 0 && uiDecaframes == 0 && uiSeconds == 0 && uiDecaseconds == 0) {
		//end
		uiDecaseconds = 9;
		uiSeconds = 9;
		uiDecaframes = 5;
		uiFrames = 9;
		//this just resets it for now. later it will have to returns something to indicate round over
	}
	if (uiFrames == 0 && uiDecaframes == 0) {
		//reset frame counter
		uiDecaframes = 5;
		uiFrames = 9;
		//count down seconds
		if (uiSeconds == 0) {
			uiSeconds = 9;
			uiDecaseconds--;
		}
		else {
			uiSeconds--;
		}
	}
	else {
		if (uiFrames == 0) {
			uiFrames = 9;
			uiDecaframes--;
		}
		else {
			uiFrames--;
		}
	}
};

void GameTimer::Render() {
	SDL_Rect cClockFace{ (WINDOW_WIDTH / 2) - 63,15,126,130 };
	SDL_Rect cClockFaceSrc{ 84 * ClockMode,0,84,87 };
	SDL_RenderCopy(g_renderer, pClockFace, &cClockFaceSrc, &cClockFace);

	SDL_Rect cDecaDestRect{ 901,22,37,88 };
	SDL_Rect cDecaSourceRect{ uiDecaseconds * 25,0,25,59 };
	SDL_RenderCopy(g_renderer, pBigTypeface, &cDecaSourceRect, &cDecaDestRect);

	SDL_Rect cDestRect{ 942,22,37,88 };
	SDL_Rect cSourceRect{ uiSeconds * 25,0,25,59 };
	SDL_RenderCopy(g_renderer, pBigTypeface, &cSourceRect, &cDestRect);

	SDL_Rect cDecaFrameDestRect{ 984,85,16,28 };
	SDL_Rect cFrameSourceRect{ uiDecaframes * 11,0,11,19 };
	SDL_RenderCopy(g_renderer, pSmallTypeface, &cFrameSourceRect, &cDecaFrameDestRect);

	SDL_Rect cFrameDestRect{ 1003,85,16,28 };
	SDL_Rect cFrameSourceRectOof{ uiFrames * 11,0,11,19 };
	SDL_RenderCopy(g_renderer, pSmallTypeface, &cFrameSourceRectOof, &cFrameDestRect);
}