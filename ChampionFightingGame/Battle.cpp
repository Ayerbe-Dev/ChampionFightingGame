#include "Battle.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <SDL/SDL.h>
#include <glew/glew.h>
#include "SDL Helpers.h"
#include "Stage.h"
#include "Menu.h"
#include "SoundManager.h"
#include "GameTexture.h"
#include "GameSettings.h"

#include "Fighters.h"
#include "FighterInterface.h"

#include "BattleObject.h"
#include "Fighter.h"
#include "BattleObjectManager.h"

#include "Projectile.h"
#include "Projectiles.h"

#include "Loader.h"

#include "Model.h"
#include "RenderManager.h"

#include "Debugger.h"
#include "ParamAccessor.h"

#include "EffectManager.h"
#include "Effect.h"
#include "Particle.h"
#include "Camera.h"

#include "ThreadManager.h"

extern bool debug;

#define DEBUG
#ifdef DEBUG
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#endif // DEBUG

void battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	PlayerInfo* player_info[2];
	for (int i = 0; i < 2; i++) {
		player_info[i] = game_manager->player_info[i];
	}
	if (player_info[0]->chara_kind == player_info[1]->chara_kind && player_info[0]->alt_color == player_info[1]->alt_color) {
		if (player_info[0]->alt_color == 0) {
			player_info[1]->alt_color++;
		}
		else {
			player_info[1]->alt_color--;
		}
	}
	Battle battle;
	battle.load_battle(game_manager);

	RenderManager *render_manager = RenderManager::get_instance();
	SDL_GetWindowSize(render_manager->window, &render_manager->s_window_width, &render_manager->s_window_height);

#ifdef DEBUG
	cotr_imgui_init();
#endif
	while (game_manager->looping[game_manager->layer]) {
		battle.frame_delay_check_performance();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		for (int i = 0; i < 2; i++) {
			if (player_info[i]->controller.check_controllers() == GAME_CONTROLLER_UPDATE_UNREGISTERED) {
				//check_controllers now returns whether or not controls were changed, and if someone's controller
				//gets unplugged midgame, we should probably do something about it
			}
		}

		battle.process_main();
		battle.render_world();
		battle.render_ui();

		battle.check_collisions();

#ifdef DEBUG
		cotr_imgui_debug_battle(&battle);
#endif

		SDL_GL_SwapWindow(render_manager->window);
	}

#ifdef DEBUG
	cotr_imgui_terminate();
#endif
	battle.unload_battle();
}

Battle::Battle() {}

Battle::~Battle() {}

void Battle::load_battle(GameManager* game_manager) {
	EffectManager* effect_manager = EffectManager::get_instance();

	debug_controller.add_button(BUTTON_MENU_FRAME_PAUSE, SDL_SCANCODE_LSHIFT, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button(BUTTON_MENU_ADVANCE, SDL_SCANCODE_LCTRL, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button(BUTTON_MENU_START, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_INVALID);

	SoundManager* sound_manager = SoundManager::get_instance();
	game_loader = new GameLoader(15);
	std::thread loading_thread(&GameLoader::loading_screen, game_loader);
	loading_thread.detach();

	RenderManager* render_manager = RenderManager::get_instance();
	camera = &render_manager->camera;

	thread_manager = ThreadManager::get_instance();

	visualize_boxes = false;

	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	battle_object_manager = BattleObjectManager::get_instance();

	inc_thread();

	int rng = rand() % 2;
	stage.load_stage(player_info[rng]->stage_info, battle_object_manager);

	inc_thread();


	for (int i = 0; i < 2; i++) {
		debug_rect[i].init();
		inc_thread();
		fighter[i] = create_fighter(player_info[i]->chara_kind, i, player_info[i]);
		inc_thread();
	}

	for (int i = 0; i < 2; i++) {
		fighter[i]->super_init(i);

		inc_thread();
		health_bar[i].init(fighter[i]);
	
		inc_thread();
		ex_bar[i].init(fighter[i]);
	
		inc_thread();
		player_indicator[i] = PlayerIndicator(fighter[i]);
	
		inc_thread();
	}
	
	timer.init(99);
	inc_thread();

	bool loading = true;
	int buffer_window = get_param_int("buffer_window", PARAM_FIGHTER);
	while (loading) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					game_loader->finished = true;
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
			player_info[i]->controller.poll_buttons(keyboard_state);
			if (player_info[i]->controller.is_any_inputs()) {
				loading = false;
				
				player_info[i]->controller.reset_buffer();
				player_info[!i]->controller.reset_buffer();
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		thread_manager->add_thread(i, fighter_thread, (void*)fighter[i]);
	}
	thread_manager->add_thread(THREAD_KIND_UI, ui_thread, (void*)this);
	game_loader->finished = true;
	game_manager->set_menu_info(this);

	if (getGameSetting("music_setting") == MUSIC_SETTING_STAGE) {
		sound_manager->add_sound_player(-1); //I'll find a better ID for the stage music later
		sound_manager->load_sound(stage.default_music_kind);
		sound_manager->play_sound(-1, SOUND_KIND_MUSIC, stage.default_music_kind);
	}
	else if (getGameSetting("music_setting") == MUSIC_SETTING_CHARA) {
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}

	render_manager->update_shader_lights();

	ms = std::chrono::high_resolution_clock::now();
}

void Battle::unload_battle() {
	RenderManager* render_manager = RenderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	for (int i = 0; i < 2; i++) {
		thread_manager->kill_thread(i);
		health_bar[i].destroy();
		ex_bar[i].destroy();
		delete fighter[i];
	}
	thread_manager->kill_thread(THREAD_KIND_UI);
	stage.unload_stage();
	sound_manager->stop_sound_all();
	sound_manager->unload_all_sounds();
	render_manager->unlink_all_shaders();
	effect_manager->clear_effect_all();
	effect_manager->unload_all_effects();
	effect_manager->remove_effect_casters();

	delete game_loader;
}

void Battle::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	SDL_PumpEvents();
	keyboard_state = SDL_GetKeyboardState(NULL);
	debug_controller.poll_buttons(keyboard_state);
	if (debug_controller.check_button_trigger(BUTTON_MENU_FRAME_PAUSE)) {
		if (frame_pause) {
			for (int i = 0; i < 2; i++) {
				sound_manager->resume_sound_all(i, SOUND_KIND_SE);
				sound_manager->resume_sound_all(i, SOUND_KIND_VC);
			}
		}
		frame_pause = !frame_pause;
		timer.flip_clock();
	}
	for (int i = 0; i < 2; i++) {
		if (player_info[i]->controller.check_button_trigger(BUTTON_START)) {
			game_manager->game_substate_main[GAME_SUBSTATE_PAUSE_BATTLE]();
		}
	}
//	if (debug_controller.check_button_trigger(BUTTON_MENU_START)) {
//		for (int i = 0; i < 2; i++) {
//			fighter[i]->reset();
//		}
//	}

	if (frame_pause) {
		process_frame_pause();
	}
	else {
		pre_process_fighter();
		process_fighter();
		process_ui();
		stage.process();
		post_process_fighter();
		thread_manager->wait_thread(THREAD_KIND_UI);
	}
	if (camera->following_players) {
		camera->follow_players(fighter[0]->pos, fighter[1]->pos, &stage);
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
		if (fighter[i]->situation_kind == FIGHTER_SITUATION_GROUND) {
			int pos_threshold = 0;
			if (fighter[!i]->situation_kind == FIGHTER_SITUATION_AIR) {
				pos_threshold = 5;
			}
			if (fighter[i]->pos.x > fighter[!i]->pos.x + pos_threshold) { //If you only crossed someone up by 5 pixels, don't worry about turning
				//around just yet, or else walking under a launched opponent can get weird if your x speed is close enough to the opponent's
				fighter[i]->internal_facing_right = false;
				fighter[i]->internal_facing_dir = -1.0;
			}
			else if (fighter[i]->pos.x < fighter[!i]->pos.x - pos_threshold) {
				fighter[i]->internal_facing_right = true;
				fighter[i]->internal_facing_dir = 1.0;
			}
			else { //If both players are stuck inside each other, stop that !
				if (fighter[i]->situation_kind == FIGHTER_SITUATION_GROUND && fighter[!i]->situation_kind == FIGHTER_SITUATION_GROUND) {
					fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
					fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
					if (fighter[i]->pos.x == fighter[!i]->pos.x) {
						fighter[i]->internal_facing_right = true;
						fighter[i]->internal_facing_dir = 1.0;
						fighter[!i]->internal_facing_right = false;
						fighter[!i]->internal_facing_dir = -1.0;
					}
					fighter[i]->add_pos(glm::vec3(-1.0 * fighter[i]->facing_dir, 0, 0));
					fighter[!i]->add_pos(glm::vec3(-1.0 * fighter[!i]->facing_dir, 0, 0));

					fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
					fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
				}
			}
		}
	}
}

void Battle::process_fighter() {
	//Here's some test code that forces player 2 to exclusively mash the shit out of QCF Punch and then 
	//backdash. I'll find a better place to put this once I know it works.
/*	if (fighter[1]->is_actionable()) {
		if (!fighter[1]->projectiles[0]->active) {
			if (fighter[1]->fighter_int[FIGHTER_INT_236_STEP] == 0) {
				player_info[1]->controller.set_button_on(BUTTON_DOWN, 2);
			}
			else if (fighter[1]->fighter_int[FIGHTER_INT_236_STEP] == 1) {
				player_info[1]->controller.set_button_on(BUTTON_LEFT, 3);
			}
			else if (fighter[1]->fighter_int[FIGHTER_INT_236_STEP] == 3) {
				player_info[1]->controller.set_button_on(BUTTON_LP);
			}
		}
		if (fighter[1]->status_kind == CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH) {
			player_info[1]->controller.set_button_on(BUTTON_RIGHT); //lol you can't buffer dashes
		}
	}
	else if (fighter[1]->status_kind == CHARA_ROY_STATUS_SPECIAL_FIREBALL_START) {
		if (!fighter[1]->check_button_on(BUTTON_LP)) { //do i LOOK like i know what frame it comes out on?
			player_info[1]->controller.set_button_on(BUTTON_LP);
		}
	}
	else if (fighter[1]->status_kind == CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH) {
		if (fighter[1]->frame != 0.0) { //lol you still can't buffer dashes
			if (!fighter[1]->check_button_on(BUTTON_RIGHT)) {
				player_info[1]->controller.set_button_on(BUTTON_RIGHT);
			}
		}
	}*/
	
	for (int i = 0; i < 2; i++) {
		player_info[i]->controller.poll_buttons(keyboard_state);
		thread_manager->notify_thread(i);
	}
}

void fighter_thread(void* fighter_arg) {
	Fighter* fighter = (Fighter*)fighter_arg;
	fighter->fighter_main();
}

void Battle::post_process_fighter() {
	thread_manager->wait_thread(THREAD_KIND_PLAYER_1);
	thread_manager->wait_thread(THREAD_KIND_PLAYER_2);
	for (int i = 0; i < 2; i++) {
		fighter[i]->fighter_post();
	}
	EffectManager::get_instance()->process();
}

void Battle::process_ui() {
	thread_manager->notify_thread(THREAD_KIND_UI);
}

void ui_thread(void* battle_arg) {
	Battle* battle = (Battle*)battle_arg;
	for (int i = 0; i < 2; i++) {
		battle->health_bar[i].process();
		battle->ex_bar[i].process();
	}
	battle->timer.process();
}

void Battle::process_frame_pause() {
	SoundManager* sound_manager = SoundManager::get_instance();
	if (debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE)) {
		for (int i = 0; i < 2; i++) {
			sound_manager->resume_sound_all(i, SOUND_KIND_SE);
			sound_manager->resume_sound_all(i, SOUND_KIND_VC);
		}
		pre_process_fighter();
		process_fighter();
		process_ui();
		post_process_fighter();
		thread_manager->wait_thread(THREAD_KIND_UI);
	}
	else {
		for (int i = 0; i < 2; i++) {
			sound_manager->pause_sound_all(i, SOUND_KIND_SE);
			sound_manager->pause_sound_all(i, SOUND_KIND_VC);
		}
	}
}

void Battle::render_world() {
	RenderManager* render_manager = RenderManager::get_instance();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	
	//Enabling face culling causes all 2D objects that were flipped to not render at all. This could technically be fixed by sorting all 2D objects 
	//into flipped and non-flipped, turning culling off, rendering the flipped ones, turning culling back on and rendering the non-flipped ones, 
	//but either way we have to keep culling off while rendering certain textures. Rather than managing flipped and not-flipped data every frame,
	//it's easier to just enable culling only for the part of the rendering process that actually needs it (Since enabling culling on 2D textures
	//doesn't make much of a difference either way)

	render_manager->update_shader_lights();

	///SHADOW PASS
	
	//Setup for the render
	glViewport(0, 0, render_manager->shadow_map.SHADOW_WIDTH, render_manager->shadow_map.SHADOW_WIDTH);
	glBindFramebuffer(GL_FRAMEBUFFER, render_manager->shadow_map.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, render_manager->shadow_map.shadow_texture);
	glCullFace(GL_FRONT);

	//The actual render
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_shadow(!fighter[i]->facing_right);
		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_shadow(!fighter[i]->facing_right);
			}
		}
	}

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//COLOR PASS

	glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, render_manager->shadow_map.shadow_texture);	

	for (int i = 0; i < 2; i++) {
		fighter[i]->render(!fighter[i]->facing_right);
		//player tags will go here

		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render(!fighter[i]->facing_right);
			}
		}
	}

	stage.render();
	glDisable(GL_CULL_FACE);

	//	EffectManager::get_instance()->render_prepared();
	EffectManager::get_instance()->render();

	if (visualize_boxes) {
		glBindFramebuffer(GL_FRAMEBUFFER, render_manager->box_layer.FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_manager->box_layer.texture);

		for (int i = 0; i < 2; i++) {
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
				if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->active) {
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
			fighter[i]->jostle_box.render();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_manager->box_layer.render(255.0);
	}
}

void Battle::render_ui() {
	for (int i = 0; i < 2; i++) {
		if (fighter[i]->fighter_int[FIGHTER_INT_COMBO_COUNT] > 1) {
			//Combo Counter text here
		}
		health_bar[i].render();
		ex_bar[i].render();
	}
	timer.render();
	/*if (debug) {
		debug_rect[debugger.target].render();
	}*/

}

void Battle::process_background() {
	render_world();
	render_ui();
}

void Battle::check_collisions() {
	if (frame_pause && !debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE)) {
		return;
	}
	check_projectile_collisions();
	check_fighter_collisions();
}

void Battle::check_projectile_collisions() {
	for (int i = 0; i < fighter[0]->num_projectiles; i++) {
		if (fighter[0]->projectiles[i]->active) {
			for (int i2 = 0; i2 < fighter[1]->num_projectiles; i2++) {
				if (fighter[1]->projectiles[i2]->active) {
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
		for (Hurtbox& hurtbox : fighter[i]->hurtboxes) {
			if (hurtbox.id != -1) {
				for (Hitbox& hitbox : fighter[!i]->hitboxes) {
					if (hitbox.id != -1) {
						if (hitbox.hitbox_kind != HITBOX_KIND_BLOCK) {
							if (is_collide(hitbox.rect, hurtbox.rect)) {
								hitbox_to_use = get_event_hit_collide_player(fighter[!i], fighter[i], &hitbox, &hurtbox);
							}
						}
						else {
							fighter[i]->fighter_flag[FIGHTER_FLAG_PROX_GUARD] = is_collide(hitbox.rect, hurtbox.rect);
						}
						if (hitbox_to_use != HITBOX_COUNT_MAX) {
							break;
						}
					}
				}
				for (Projectile* projectile : fighter[!i]->projectiles) {
					if (projectile != nullptr && projectile->active) {
						for (Hitbox& hitbox : projectile->hitboxes) {
							if (hitbox.id != -1) {
								if (is_collide(hitbox.rect, hurtbox.rect)) {
									projectile_hitbox_to_use = get_event_hit_collide_projectile(projectile, fighter[i], &hitbox, &hurtbox);
								}
								if (projectile_hitbox_to_use != HITBOX_COUNT_MAX) {
									break;
								}
							}
						}
					}
				}
				for (Grabbox& grabbox : fighter[!i]->grabboxes) {
					if (grabbox.id != -1) {
						if (is_collide(grabbox.rect, hurtbox.rect)) {
							grabbox_to_use = get_event_grab_collide_player(fighter[!i], fighter[i], &grabbox, &hurtbox);
						}
						if (grabbox_to_use != HITBOX_COUNT_MAX) {
							break;
						}
					}
				}
			}
		}
		fighter[i]->connected_hitbox = hitbox_to_use;
		fighter[i]->connected_grabbox = grabbox_to_use;
		fighter[i]->connected_projectile_hitbox = projectile_hitbox_to_use;
	}
	if (!event_hit_collide_player()) {
		event_grab_collide_player();
	}
	for (int i = 0; i < 2; i++) {
		for (Projectile* projectile : fighter[i]->projectiles) {
			if (projectile != nullptr && projectile->active) {
				event_hit_collide_projectile(fighter[i], fighter[!i], projectile, &projectile->hitboxes[fighter[!i]->connected_projectile_hitbox]);
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
		attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 14 + get_param_int("parry_advantage_frames", PARAM_FIGHTER) - hitbox->blockstun;
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
		attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 6 + get_param_int("parry_advantage_frames", PARAM_FIGHTER) - hitbox->blockstun;
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
/// <returns>Whether or not any kind of collision event occured.</returns>
bool Battle::event_hit_collide_player() {
	Hitbox* hitboxes[2] = { &(fighter[0]->hitboxes[fighter[1]->connected_hitbox]), &(fighter[1]->hitboxes[fighter[0]->connected_hitbox]) };
	bool players_hit[2] = { hitboxes[1]->id != -1, hitboxes[0]->id != -1 };
	unsigned int post_hit_status[2] = { fighter[0]->status_kind, fighter[1]->status_kind };
	if (players_hit[0] && players_hit[1]) {
		if (fighter[0]->situation_kind != fighter[1]->situation_kind) {
			if (fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND) {
				players_hit[0] = false;
			}
			else {
				players_hit[1] = false;
			}
		}
		else if (hitboxes[0]->attack_level == hitboxes[1]->attack_level) {
			if (hitboxes[0]->clank_kind == CLANK_KIND_CONTINUE || hitboxes[1]->clank_kind == CLANK_KIND_CONTINUE) {
				const int clank_kinds[2] = { hitboxes[0]->clank_kind, hitboxes[1]->clank_kind }; //If player 1's clank_kind is CLANK_KIND_CONTINUE, 
				//player 2's status will change. This will cause player 2's hitbox to be destroyed before it's checked against player 1, so in order
				//to compensate for port priority, we back up the clank_kind as a constant 
				for (int i = 0; i < 2; i++) {
					if (clank_kinds[i] == CLANK_KIND_CONTINUE) {
						fighter[!i]->change_status(FIGHTER_STATUS_CLANK);
					}
				}
				return false;
			}
		}
		else if (hitboxes[0]->attack_level > hitboxes[1]->attack_level) {
			players_hit[0] = false;
		}
		else {
			players_hit[1] = false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			fighter[!i]->update_hitbox_connect(hitboxes[!i]->multihit);
			if (fighter[i]->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain_on_block / 2, get_param_int("ex_meter_size", PARAM_FIGHTER));
				fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + fighter[i]->get_local_param_float("meter_gain_on_parry"), get_param_int("ex_meter_size", PARAM_FIGHTER));
				fighter[i]->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
				post_hit_status[i] = FIGHTER_STATUS_PARRY;
			}
			else if (fighter[i]->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain_on_block, get_param_int("ex_meter_size", PARAM_FIGHTER));
				fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(!hitboxes[!i]->can_chip_ko, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->chip_damage, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->block_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				fighter[i]->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS] = true;
				post_hit_status[i] = FIGHTER_STATUS_BLOCKSTUN;
			}
			else if (!fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
				//If Attack Connected is false but this still got set off, that means that the opponent hit you while you had armor. In this situation,
				//the attacker gets the same meter gain as if they got blocked, and the defender takes half the usual amount of chip damage. However, this
				//chip damage is incapable of KOing the defender no matter what they got hit by.

				//Note: This is also what will happen to Leon if he gets hit while he has Right of Way armor, so if we ever want to remove the chip damage
				//for when he gets hit with RoW, we'll need to find a way to account for that. I don't think it'll be that big of a deal though.
				fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain_on_block, get_param_int("ex_meter_size", PARAM_FIGHTER));
				fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - (hitboxes[!i]->chip_damage / 2), fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
			}
			else {
				/*
					If the opponent was in hitstun the first time you connected with a move during this status, increase the damage scaling by however much
					is specified by the hitbox. Otherwise, reset the attacker's damage scaling.
				*/
				fighter[i]->fighter_int[FIGHTER_INT_COMBO_COUNT] ++;
				if (fighter[i]->get_status_group() == STATUS_GROUP_HITSTUN) {
					if (!fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
						fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitboxes[!i]->scale;
					}
				}
				else {
					fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
				}
				fighter[i]->fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED] = hitboxes[!i]->launch_init_y;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = hitboxes[!i]->launch_gravity_y;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = hitboxes[!i]->launch_max_fall_speed;
				fighter[i]->fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = hitboxes[!i]->launch_speed_x;
				/*
				If the opponent's juggle value >= whatever the hitbox says to set it to, increase it directly to the hitbox's juggle value. Otherwise,
				increase it by one so that the opponent's juggle value is always going up
				*/
				if (fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] >= hitboxes[!i]->juggle_start) {
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitboxes[!i]->juggle_increase;
				}
				else {
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitboxes[!i]->juggle_start;
				}
				float prev_x = fighter[i]->pos.x;
				fighter[i]->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitboxes[!i]->hit_pushback / fighter[i]->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
				if (can_counterhit(fighter[i], hitboxes[!i])) {
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain_on_counterhit, get_param_int("ex_meter_size", PARAM_FIGHTER));
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->damage * hitboxes[!i]->counterhit_damage_mul, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
					fighter[i]->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
					fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
					fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
					post_hit_status[i] = get_damage_status(hitboxes[!i]->counterhit_status, fighter[i]->situation_kind);
					if (fighter[i]->status_kind == FIGHTER_STATUS_LAUNCH && hitboxes[!i]->continue_launch) {
						post_hit_status[i] = FIGHTER_STATUS_LAUNCH;
					}
				}
				else {
					fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, fighter[!i]->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitboxes[!i]->meter_gain_on_hit, get_param_int("ex_meter_size", PARAM_FIGHTER));
					fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] - hitboxes[!i]->damage * ((clampf(1, 10 - fighter[i]->fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10), fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH]);
					fighter[i]->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitboxes[!i]->attack_level;
					post_hit_status[i] = get_damage_status(hitboxes[!i]->hit_status, fighter[i]->situation_kind);
					if (fighter[i]->status_kind == FIGHTER_STATUS_LAUNCH && hitboxes[!i]->continue_launch) {
						post_hit_status[i] = FIGHTER_STATUS_LAUNCH;
					}
				}
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = false;
				fighter[!i]->fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] = true;
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			fighter[i]->change_status(post_hit_status[i], true, false);
			fighter[i]->move_script.move_script();
			fighter[i]->update_hurtbox_pos();
		}
	}
	return (players_hit[0] || players_hit[1]);
}

/// <summary>
/// Handle any potential grabbox collision events between two Fighters on this frame and change statuses accordingly.
/// </summary>
void Battle::event_grab_collide_player() {
	Grabbox* grabboxes[2] = { &(fighter[0]->grabboxes[fighter[1]->connected_grabbox]), &(fighter[1]->grabboxes[fighter[0]->connected_grabbox]) };
	bool players_hit[2] = { grabboxes[1]->id != -1, grabboxes[0]->id != -1 };
	bool players_tech[2] = { grabboxes[1]->grabbox_kind & GRABBOX_KIND_NOTECH, grabboxes[0]->grabbox_kind & GRABBOX_KIND_NOTECH };
	if (players_hit[0] && players_hit[1]) {
		if (players_tech[0] == players_tech[1]) {
			for (int i = 0; i < 2; i++) {
				fighter[i]->change_status(FIGHTER_STATUS_THROW_TECH);
			}
			return;
		}
		else if (players_tech[0]) {
			players_hit[0] = false;
		}
		else {
			players_hit[1] = false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (players_hit[i]) {
			if (fighter[i]->fighter_flag[FIGHTER_FLAG_THROW_TECH] && players_tech[i]) {
				fighter[i]->change_status(FIGHTER_STATUS_THROW_TECH);
				fighter[!i]->change_status(FIGHTER_STATUS_THROW_TECH);
			}
			else {
				fighter[!i]->change_status(grabboxes[!i]->attacker_status_if_hit);
				fighter[i]->change_status(grabboxes[!i]->defender_status_if_hit);
			}
			fighter[i]->move_script.move_script();
			fighter[!i]->move_script.move_script();
			fighter[i]->update_hurtbox_pos();
			fighter[!i]->update_hurtbox_pos();
			return;
		}
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
	unsigned int p2_status_post_hit = p2->status_kind;
	if (p2_hit) {
		p1_projectile->update_hitbox_connect(p1_hitbox->multihit);
		p1_projectile->projectile_int[PROJECTILE_INT_HEALTH] --;
		if (p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block / 2, get_param_int("ex_meter_size", PARAM_FIGHTER));
			p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p2->get_local_param_float("meter_gain_on_parry"), get_param_int("ex_meter_size", PARAM_FIGHTER));
			p2->fighter_flag[FIGHTER_FLAG_SUCCESSFUL_PARRY] = false;
			p2_status_post_hit = FIGHTER_STATUS_PARRY;
		}
		else if (p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block, get_param_int("ex_meter_size", PARAM_FIGHTER));
			p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(!p1_hitbox->can_chip_ko, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->chip_damage, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->block_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			p2->fighter_flag[FIGHTER_FLAG_ENTER_BLOCKSTUN] = false;
			p2_status_post_hit = FIGHTER_STATUS_BLOCKSTUN;
		}
		else if (!p1_projectile->projectile_flag[PROJECTILE_FLAG_HIT]) {
			p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_block / 2, get_param_int("ex_meter_size", PARAM_FIGHTER));
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
			if (p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] >= p1_hitbox->juggle_start) {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE]++;
			}
			else {
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = p1_hitbox->juggle_start;
			}
			p2->fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = p1_hitbox->hit_pushback / p2->fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			if (can_counterhit(p2, p1_hitbox)) {
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_counterhit, get_param_int("ex_meter_size", PARAM_FIGHTER));
				p2->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, p2->fighter_float[FIGHTER_FLOAT_HEALTH] - p1_hitbox->damage * p1_hitbox->counterhit_damage_mul, p2->fighter_float[FIGHTER_FLOAT_HEALTH]);
				p2->fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0; //Reset the opponent's juggle value on counterhit :)
				p2->fighter_int[FIGHTER_INT_HITSTUN_FRAMES] *= 1.2;
				p2->fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = ATTACK_LEVEL_HEAVY;
				p2_status_post_hit = get_damage_status(p1_hitbox->counterhit_status, p2->situation_kind);
			}
			else {
				p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, p1->fighter_float[FIGHTER_FLOAT_SUPER_METER] + p1_hitbox->meter_gain_on_hit, get_param_int("ex_meter_size", PARAM_FIGHTER));
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

HealthBar::HealthBar() {
	health = nullptr;
	max_health = 0.0;
}

void HealthBar::init(Fighter* fighter) {
	health_texture.init("resource/ui/battle/hp/health.png");
	bar_texture.init("resource/ui/battle/hp/bar.png");
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
	health_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	bar_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	health = &fighter->fighter_float[FIGHTER_FLOAT_HEALTH];
	max_health = fighter->get_local_param_float("health");
}

void HealthBar::destroy() {
	health_texture.destroy();
	bar_texture.destroy();
}

void HealthBar::process() {
	health_texture.scale_left_percent(*health / max_health);
	health_texture.prepare_render();
	bar_texture.prepare_render();
}

void HealthBar::render() {
	health_texture.render_prepared();
	bar_texture.render_prepared();
}

ExBar::ExBar() {
	ex = nullptr;
	max_ex = 0.0;
	fighter = nullptr;
	num_bars = 0;
}

void ExBar::init(Fighter* fighter) {
	this->fighter = fighter;
	ex = &fighter->fighter_float[FIGHTER_FLOAT_SUPER_METER];
	max_ex = get_param_int("ex_meter_size", PARAM_FIGHTER);
	ex_texture.init("resource/ui/battle/ex/ex.png");
	ex_segment_texture.init("resource/ui/battle/ex/ex_segment.png");
	bar_texture.init("resource/ui/battle/ex/bar.png");
	ex_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));
	ex_segment_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));
	num_bars = get_param_int("ex_meter_bars", PARAM_FIGHTER);

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

	int segments = floor(*ex / (max_ex / num_bars));
	if (prev_segments != segments) {
		if (prev_segments > segments) {
			ex_segment_texture.set_right_target(clampf(0.0, (float)segments / num_bars, 1.0), 1);
		}
		else if (!(segments % 2)) {
			ex_segment_texture.set_right_target(clampf(0.0, (float)segments / num_bars, 1.0), 1);
		}
	}
	prev_segments = segments;
	ex_texture.process();
	ex_segment_texture.process();
	ex_texture.prepare_render();
	ex_segment_texture.prepare_render();
	bar_texture.prepare_render();
}

void ExBar::render() {
	ex_texture.render_prepared();
	ex_segment_texture.render_prepared();
	bar_texture.render_prepared();
}

PlayerIndicator::PlayerIndicator() {
	fighter = nullptr;
}

PlayerIndicator::PlayerIndicator(Fighter* fighter, std::string nametag) {
	this->fighter = fighter;
	this->nametag = nametag;
	std::string resource_dir = "resource/ui/battle/tag/";
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
	this->texture.init(resource_dir);
}

GameTimer::GameTimer() {
	clock_mode = 0;
	deca_frames = 0;
	frames = 0;
	deca_seconds = 0;
	seconds = 0;
	time_up = false;
}

GameTimer::GameTimer(int time) {
	init(time);
};

void GameTimer::init(int time) {
	deca_seconds = time / 10;
	seconds = time % 10;
	deca_frames = 5;
	frames = 9;
	clock_mode = 1;

	clock.init("resource/ui/battle/timer/clockface.png");
	second_texture.init("resource/ui/battle/timer/bigtypeface.png");
	deca_second_texture.init("resource/ui/battle/timer/bigtypeface.png");
	frame_texture.init("resource/ui/battle/timer/smalltypeface.png");
	deca_frame_texture.init("resource/ui/battle/timer/smalltypeface.png");

	clock.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	second_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	deca_second_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	frame_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	deca_frame_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);

	clock.load_spritesheet("resource/ui/battle/timer/clockface.yml");
	second_texture.load_spritesheet("resource/ui/battle/timer/bigtypeface.yml");
	deca_second_texture.load_spritesheet("resource/ui/battle/timer/bigtypeface.yml");
	frame_texture.load_spritesheet("resource/ui/battle/timer/smalltypeface.yml");
	deca_frame_texture.load_spritesheet("resource/ui/battle/timer/smalltypeface.yml");

	clock.set_scale(1.5);
	deca_second_texture.set_scale(1.5);
	second_texture.set_scale(1.5);
	deca_frame_texture.set_scale(1.5);
	frame_texture.set_scale(1.5);

	clock.set_pos(glm::vec3(0.0, 20.0, 0.0));
	deca_second_texture.set_pos(glm::vec3(-76.0, 38.5, 0.0));
	second_texture.set_pos(glm::vec3(4.0, 38.5, 0.0));
	deca_frame_texture.set_pos(glm::vec3(68.0, 155.0, 0.0));
	frame_texture.set_pos(glm::vec3(100.0, 155.0, 0.0));

	clock.set_sprite(1);
}

void GameTimer::process() {
	if (frames == 0) {
		if (deca_frames == 0) {
			if (seconds == 0 && deca_seconds == 0) {
				time_up = true;
			}
			else {
				deca_frames = 5;
				frames = 9;
				if (seconds == 0) {
					deca_seconds--;
					seconds = 9;
				}
				else {
					seconds--;
				}
			}
		}
		else {
			deca_frames--;
			frames = 9;
		}
	}
	else {
		frames--;
	}
	second_texture.set_sprite(seconds);
	deca_second_texture.set_sprite(deca_seconds);
	frame_texture.set_sprite(frames);
	deca_frame_texture.set_sprite(deca_frames);

	clock.prepare_render();
	second_texture.prepare_render();
	deca_second_texture.prepare_render();
	frame_texture.prepare_render();
	deca_frame_texture.prepare_render();
}

void GameTimer::flip_clock() {
	clock_mode = !clock_mode;
	clock.set_sprite(clock_mode);
}

void GameTimer::render() {
	clock.render_prepared();
	second_texture.render_prepared();
	deca_second_texture.render_prepared();
	frame_texture.render_prepared();
	deca_frame_texture.render_prepared();
}