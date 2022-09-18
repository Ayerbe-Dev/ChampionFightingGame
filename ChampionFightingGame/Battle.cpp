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
#include "SaveManager.h"
#include "FontManager.h"

extern bool debug;

//#define DEBUG
#ifdef DEBUG
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#endif // DEBUG

void battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();

	font_manager->load_face("Fiend-Oblique");
	font_manager->load_face("FiraCode");

	Player* player[2];
	for (int i = 0; i < 2; i++) {
		player[i] = game_manager->player[i];
	}

	if (player[0]->chara_kind == player[1]->chara_kind && player[0]->alt_color == player[1]->alt_color) {
		if (player[0]->alt_color == 0) {
			player[1]->alt_color++;
		}
		else {
			player[1]->alt_color--;
		}
	}

	Battle *battle = new Battle;
#ifdef DEBUG
	cotr_imgui_init();
#endif
	while (*battle->looping) {
		battle->frame_delay_check_fps();

		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);

		#ifdef DEBUG
			game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);
		#else
			game_manager->handle_window_events();
		#endif

		for (int i = 0; i < 2; i++) {
			if (player[i]->controller.check_controllers() == GAME_CONTROLLER_UPDATE_UNREGISTERED) {
				//check_controllers now returns whether or not controls were changed, and if someone's controller
				//gets unplugged midgame, we should probably do something about it
			}
		}

		battle->process_main();
		render_manager->execute_buffered_events();
		battle->render_world();
		battle->render_ui();
		battle->fps_counter.render();
		battle->fps_texture.render();

		battle->check_collisions();

#ifdef DEBUG
		cotr_imgui_debug_battle(battle);
#endif

		SDL_GL_SwapWindow(render_manager->window);
	}

#ifdef DEBUG
	cotr_imgui_terminate();
#endif
	battle->fps_font.unload_font();
	font_manager->unload_face("Fiend-Oblique");
	delete battle;
}

Battle::Battle() {
	GameManager* game_manager = GameManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();

	game_manager->set_menu_info(this);

	debug_controller.add_button_mapping(BUTTON_MENU_FRAME_PAUSE, SDL_SCANCODE_LSHIFT, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button_mapping(BUTTON_MENU_ADVANCE, SDL_SCANCODE_LCTRL, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button_mapping(BUTTON_MENU_START, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_INVALID);

	game_loader = new GameLoader(13);
	std::thread loading_thread(&GameLoader::loading_screen, game_loader);
	loading_thread.detach();

	thread_manager = ThreadManager::get_instance();

	visualize_boxes = true;

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	battle_object_manager = BattleObjectManager::get_instance();

	inc_thread();

	combo_font = font_manager->load_font("Fiend-Oblique", 64);
	message_font = font_manager->load_font("Fiend-Oblique", 20);

	inc_thread();

	int rng = rand() % 2;
	stage.load_stage(player[rng]->stage_info, battle_object_manager);

	inc_thread();

	debug_boxes.init();
	active_debug_box = &debug_boxes.boxes[0][0];
	debug_anchor = &debug_boxes.anchor[0][0];
	debug_offset = &debug_boxes.offset[0][0];
	active_debug_box->set_alpha(180);
	
	inc_thread();

	for (int i = 0; i < 2; i++) {
		fighter[i] = create_fighter(player[i]->chara_kind, i, player[i]);
		inc_thread();
	}

	camera = &render_manager->camera;
	for (int i = 0; i < 2; i++) {
		camera->fighter[i] = fighter[i];
	}
	camera->stage = &stage;
	camera->set_fov(45.0);

	for (int i = 0; i < 2; i++) {
		meters[i].init(fighter[i]);
	
		inc_thread();
		player_indicator[i] = PlayerIndicator(fighter[i]);
	
		inc_thread();

		texts[i].push_back(BattleText());
		texts[i].back().init(&message_font, "Frame Advantage: ", -1, fighter[i], glm::vec2(380.0, 350.0));
		frame_advantage[i] = &texts[i].back();

		inc_thread();
	}

	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();
	
	timer.init(99);
	inc_thread();

	bool loading = true;
	int buffer_window = get_param_int("buffer_window", PARAM_FIGHTER);
	while (loading && *looping) {
		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_buttons(keyboard_state);
			if (player[i]->controller.is_any_inputs()) {
				loading = false;
				
				player[i]->controller.reset_buffer();
				player[!i]->controller.reset_buffer();
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		thread_manager->add_thread(i, fighter_thread, (void*)fighter[i]);
	}
	thread_manager->add_thread(THREAD_KIND_UI, ui_thread, (void*)this);
	game_loader->finished = true;

	if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_STAGE) {
		sound_manager->add_sound_player(-1); //I'll find a better ID for the stage music later
		sound_manager->load_sound(stage.default_music_kind);
		sound_manager->play_sound(-1, SOUND_KIND_MUSIC, stage.default_music_kind);
	}
	else if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_CHARA) {
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}

	ms = std::chrono::high_resolution_clock::now();
}

Battle::~Battle() {
	RenderManager* render_manager = RenderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	for (int i = 0; i < 2; i++) {
		thread_manager->kill_thread(i);
		meters[i].destroy();
		delete fighter[i];
		camera->fighter[i] = nullptr;
	}
	debug_boxes.destroy();
	camera->stage = nullptr;
	camera->unload_camera_anims();
	thread_manager->kill_thread(THREAD_KIND_UI);
	stage.unload_stage();
	render_manager->remove_light();
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
	keyboard_state = SDL_GetKeyboardState(NULL);
	mouse.poll_buttons();
	process_debug_boxes();

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
		if (player[i]->controller.check_button_trigger(BUTTON_START)) {
			game_manager->game_substate_main[GAME_SUBSTATE_PAUSE_BATTLE]();
		}
	}
	if (frame_pause) {
		process_frame_pause();
	}
	else {
		battle_object_manager->world_frame += battle_object_manager->world_rate;
		pre_process_fighter();
		process_fighter();
		process_ui();
		stage.process();
		post_process_fighter();
		thread_manager->wait_thread(THREAD_KIND_UI);
		camera->camera_main();
	}
//	if (*game_manager->game_context == GAME_CONTEXT_TRAINING) {
		process_training();
//	}
	if (battle_object_manager->world_frame >= 0.97) {
		battle_object_manager->world_frame = 0.0;
	}
	if (game_manager->is_crash()) {
		game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}
}

void Battle::process_debug_boxes() {
	if (mouse.check_button_on(MOUSE_BUTTON_M1)) {
		glm::vec2 game_rect_coords = mouse_pos_to_rect_coord(mouse.get_pos());
		*debug_offset = game_rect_coords;
		if (mouse.check_button_trigger(MOUSE_BUTTON_M1)) {
			*debug_anchor = game_rect_coords;
		}
		active_debug_box->update_corners(*debug_anchor, *debug_offset);
	}
	if (mouse.check_button_trigger(MOUSE_BUTTON_M2)) {
		debug_boxes.print(fighter[0]);
	}
	if (mouse.check_button_trigger(MOUSE_BUTTON_M4)) {
		active_debug_box->set_alpha(127);
		if (debug_boxes.active_cat == 2) {
			debug_boxes.active_cat = 0;
		}
		else {
			debug_boxes.active_cat++;
		}
		active_debug_box = &debug_boxes.boxes[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		debug_anchor = &debug_boxes.anchor[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		debug_offset = &debug_boxes.offset[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		active_debug_box->set_alpha(186);
	}
	if (mouse.check_button_trigger(MOUSE_BUTTON_M5)) {
		active_debug_box->set_alpha(127);
		if (debug_boxes.active_box[debug_boxes.active_cat] == 9) {
			debug_boxes.active_box[debug_boxes.active_cat] = 0;
		}
		else {
			debug_boxes.active_box[debug_boxes.active_cat]++;
		}
		active_debug_box = &debug_boxes.boxes[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		debug_anchor = &debug_boxes.anchor[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		debug_offset = &debug_boxes.offset[debug_boxes.active_cat][debug_boxes.active_box[debug_boxes.active_cat]];
		active_debug_box->set_alpha(186);
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
	for (int i = 0; i < 2; i++) {
		player[i]->controller.poll_buttons(keyboard_state);
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

void Battle::process_training() {
	for (int i = 0; i < 2; i++) {
		std::string frames = std::to_string(fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE]);
		if (fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] >= 0) {
			frames = "+" + frames;
		}
		std::string advantage = "Frame Advantage: " + frames;
		if (frame_advantage[i]->get_text() != advantage) {
			frame_advantage[i]->update(advantage, -1);
		}
	}
}

void ui_thread(void* battle_arg) {
	Battle* battle = (Battle*)battle_arg;
	for (int i = 0; i < 2; i++) {
		battle->meters[i].process();
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
		battle_object_manager->world_frame += battle_object_manager->world_rate;
		pre_process_fighter();
		process_fighter();
		process_ui();
		post_process_fighter();
		thread_manager->wait_thread(THREAD_KIND_UI);
		camera->camera_main();
	}
	else {
		for (int i = 0; i < 2; i++) {
			sound_manager->pause_sound_all(i, SOUND_KIND_SE);
			sound_manager->pause_sound_all(i, SOUND_KIND_VC);
		}
		camera->update_view();
	}
}

void Battle::render_world() {
	RenderManager* render_manager = RenderManager::get_instance();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE); //Face culling should be off for UI, which means we have to toggle it every frame

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0x00);

	//SHADOW PASS

	render_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_shadow(!fighter[i]->facing_right);
		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_shadow(!fighter[i]->facing_right);
			}
		}
	}
	glCullFace(GL_BACK);

	//COLOR PASS

	render_manager->g_buffer.use();
	glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_manager->shadow_map.bind_textures();

	for (int i = 0; i < 2; i++) {
		fighter[i]->render(!fighter[i]->facing_right);
		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render(!fighter[i]->facing_right);
			}
		}
	}
	stage.render();

	//EFFECT RENDERING

	glDisable(GL_CULL_FACE);
	EffectManager::get_instance()->render();

	//OUTLINE PASS

	render_manager->outline.use();

	glStencilMask(0xFF);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render(!fighter[i]->facing_right);
		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render(!fighter[i]->facing_right);
			}
		}
	}
	glStencilMask(0x00);
	stage.render();
	glClear(GL_COLOR_BUFFER_BIT);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	glDepthMask(GL_FALSE);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_outline(!fighter[i]->facing_right);
		for (int i2 = 0; i2 < fighter[i]->num_projectiles; i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_outline(!fighter[i]->facing_right);
			}
		}
	}

	glDepthMask(GL_TRUE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	//SSAO PASS

	render_manager->SSAO.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_manager->SSAO.render();
	render_manager->SSAO_blur.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_manager->SSAO_blur.render();

	//LIGHTING PASS

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	render_manager->g_buffer.render();
	render_manager->outline.render_passthrough();

	//HITBOX PASS

	if (visualize_boxes) {
		render_manager->box_layer.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hurtboxes[i2].active) {
					fighter[i]->hurtboxes[i2].rect.render();
				}
			}
			if (fighter[i]->blockbox.active) {
				fighter[i]->blockbox.rect.render();
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->grabboxes[i2].active) {
					fighter[i]->grabboxes[i2].rect.render();
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->hitboxes[i2].active) {
					fighter[i]->hitboxes[i2].rect.render();
				}
			}
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->active) {
					if (fighter[i]->projectiles[i2]->blockbox.active) {
						fighter[i]->projectiles[i2]->blockbox.rect.render();
					}
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->hitboxes[i3].active) {
							fighter[i]->projectiles[i2]->hitboxes[i3].rect.render();
						}
					}
				}
			}
			fighter[i]->jostle_box.render();
		}
		debug_boxes.render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_manager->box_layer.render();
	}
}

void Battle::render_ui() {
	for (int i = 0; i < 2; i++) {
		meters[i].render();
		std::list<BattleText>::iterator it = texts[i].begin();
		while (it != texts[i].end()) {
			if (it->duration == 0) {
				if (&*it == combo_counter[i]) {
					combo_counter[i] = nullptr;
					combo_hit[i] = nullptr;
				}
				if (it->alpha == 0) {
					it->destroy();
					if (texts[i].size() != 1) {
						it = texts[i].erase(it);
						continue;
					}
					else {
						texts[i].erase(it);
					}
				}
				else {
					it->add_alpha(-51);
				}
			}
			else if (it->duration != -1) {
				it->duration--;
			}
			it++;
		}
		for (BattleText &text : texts[i]) {
			text.render();
			text.scale_all_percent(1.0, false);
		}
	}
	timer.render();

}

void Battle::process_background() {
	render_world();
	render_ui();
}