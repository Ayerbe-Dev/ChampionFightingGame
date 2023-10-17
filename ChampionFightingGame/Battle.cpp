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

#include "cotr_imgui_debugger.h"
#include "ParamAccessor.h"

#include "EffectManager.h"
#include "Effect.h"
#include "Particle.h"
#include "Camera.h"

#include "ThreadManager.h"
#include "SaveManager.h"
#include "FontManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "utils.h"

void battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	font_manager->load_face("Fiend-Oblique");

	Player* player[2];
	for (int i = 0; i < 2; i++) {
		player[i] = game_manager->player[i];
	}

	if (player[0]->chara_kind == player[1]->chara_kind 
		&& player[0]->alt_costume == player[1]->alt_costume
		&& player[0]->alt_color == player[1]->alt_color) {
		if (player[0]->alt_color == 0) {
//			player[1]->alt_color++;
		}
		else {
//			player[1]->alt_color--;
		}
	}

	Battle *battle = new Battle;

#ifdef DEBUG
	cotr_imgui_init();
#endif

	while (battle->looping) {
		game_manager->frame_delay_check_fps();

		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);

		#ifdef DEBUG
			render_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);
		#else
			render_manager->handle_window_events();
		#endif

		for (int i = 0; i < 2; i++) {
			int controller_update = player[i]->controller.check_controllers();
			if (controller_update == GAME_CONTROLLER_UPDATE_UNREGISTERED) {
				//check_controllers returns whether or not controls were changed, and if someone's controller
				//gets unplugged midgame, we should probably do something about it
			}
		}

		battle->process_game_state();
		if (battle->state == BATTLE_STATE_BATTLE) {
			battle->check_collisions();
		}
		battle->render_game_state();

#ifdef DEBUG
		cotr_imgui_debug_battle(battle);
#endif

		SDL_GL_SwapWindow(render_manager->window);
	}
#ifdef DEBUG
	cotr_imgui_terminate();
#endif

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

	num_rounds = 2;
	sudden_death = false;
	ko_timer = 0;

	switch (game_context) {
		case (GAME_CONTEXT_STORY):
		case (GAME_CONTEXT_TRAINING): {
			state = BATTLE_STATE_BATTLE;
		} break;
		default: {
			state = BATTLE_STATE_PRE_INTRO;
		} break;
	}
	
	debug_controller.add_button_mapping(BUTTON_MENU_FRAME_PAUSE, SDL_SCANCODE_LSHIFT, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button_mapping(BUTTON_MENU_ADVANCE, SDL_SCANCODE_LCTRL, SDL_CONTROLLER_BUTTON_INVALID);
	debug_controller.add_button_mapping(BUTTON_MENU_START, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_INVALID);

	game_loader = new GameLoader(12);
	std::thread loading_thread(&GameLoader::loading_screen, game_loader);
	loading_thread.detach();

	thread_manager = ThreadManager::get_instance();

	visualize_boxes = true;

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	battle_object_manager = BattleObjectManager::get_instance();

	inc_thread();

	combo_font = font_manager->load_font("Fiend-Oblique", 64);
	message_font = font_manager->load_font("Fiend-Oblique", 24);
	info_font = font_manager->load_font("FiraCode", 16);

	inc_thread();

	int rng = rand() % 2;
	stage.load_stage(player[rng]->stage_info, battle_object_manager);
	
	inc_thread();

	for (int i = 0; i < 2; i++) {
		fighter[i] = create_fighter(player[i]);
		inc_thread();
	}

	active_hitbox_object = fighter[0];
	active_hitbox_object_index = 0;

	camera = &render_manager->camera;
	camera->reset_camera();
	for (int i = 0; i < 2; i++) {
		camera->fighter[i] = fighter[i];
	}
	camera->stage = &stage;
	camera->set_fov(45.0);

	for (int i = 0; i < 2; i++) {
		meters[i].init(fighter[i], 0);
	
		inc_thread();
		player_indicator[i] = PlayerIndicator(fighter[i]);

		inc_thread();

		training_info[i].init(fighter[i], &info_font);

		inc_thread();
	}

	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();
	
	if (game_context == GAME_CONTEXT_TRAINING) {
		timer.init(-1);
	}
	else {
		timer.init(99);
	}
	inc_thread();

	bool loading = true;
	while (loading && looping) {
		render_manager->handle_window_events();

		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_buttons();
			if (player[i]->controller.is_any_inputs()) {
				loading = false;
				
				player[i]->controller.reset_buffer();
				player[!i]->controller.reset_buffer();
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		thread_manager->add_thread(i, gamestate_battle_fighter_thread, (void*)fighter[i]);
	}
	thread_manager->add_thread(THREAD_KIND_UI, gamestate_battle_ui_thread, (void*)this);
	game_loader->finished = true;

	if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_STAGE) {
		sound_manager->load_music("Battle Music", stage.default_music_kind, 0.0);
	}
	else if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_CHARA) {
//		sound_manager->load_sound("Battle Music", [a random directory for one of the characters here]);
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//sound_manager->load_sound("Battle Music", player[rng(0, 1)]->default_music_kind);
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}
	sound_manager->play_music("Battle Music");
}

Battle::~Battle() {
	RenderManager* render_manager = RenderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	camera->reset_camera();

	//Unload each fighter and related info

	effect_manager->clear_effect_all();
	for (int i = 0; i < 2; i++) {
		thread_manager->kill_thread(i);
		training_info[i].destroy();
		meters[i].destroy();
		camera->fighter[i] = nullptr;
		delete fighter[i];
	}

	//Sounds and effects were already removed, now fully unload them

	sound_manager->stop_music("Battle Music");
	sound_manager->unload_all_sounds();
	sound_manager->unload_music("Battle Music");
	sound_manager->clear_game_objects();
	effect_manager->unload_all_effects();
	effect_manager->remove_effect_casters();
	render_manager->remove_light();

	thread_manager->kill_thread(THREAD_KIND_UI);
	camera->stage = nullptr;
	camera->unload_camera_anims();

	stage.unload_stage();
	combo_font.unload_font();
	message_font.unload_font();
	info_font.unload_font();

	delete game_loader;
}

void Battle::process_main() {
	switch (state) {
	case(BATTLE_STATE_PRE_INTRO): {
		process_pre_intro();
	} break;
	case (BATTLE_STATE_INTRO): {
		process_intro();
	} break;
	default:
	case (BATTLE_STATE_BATTLE): {
		process_battle();
	} break;
	case (BATTLE_STATE_KO): {
		process_ko();
	} break;
	case (BATTLE_STATE_OUTRO): {
		process_outro();
	} break;
	}
}

void Battle::process_pre_intro() {
	state = BATTLE_STATE_INTRO;
	for (int i = 0; i < 2; i++) {
		fighter[i]->change_anim("intro");
	}
}

void Battle::process_intro() {
	SoundManager* sound_manager = SoundManager::get_instance();
	for (int i = 0; i < 2; i++) {
		player[i]->controller.poll_buttons();
		fighter[i]->rot.z = glm::radians(90.0);
		if (player[i]->controller.check_button_trigger(BUTTON_START)) {
			player[i]->controller.poll_buttons();
			goto POST_INTRO;
		}
	}
	if (!fighter[0]->is_anim_end) {
		fighter[0]->process_animate();
		fighter[0]->active_move_script.execute(fighter[0], fighter[0]->frame);
		fighter[0]->process_post_animate();
		sound_manager->process_sounds();
	}
	else if (!fighter[1]->is_anim_end) {
		fighter[1]->process_animate();
		fighter[1]->active_move_script.execute(fighter[1], fighter[1]->frame);
		fighter[1]->process_post_animate();
		sound_manager->process_sounds();
	}
	else {
		POST_INTRO:
		for (int i = 0; i < 2; i++) {
			fighter[i]->change_status(FIGHTER_STATUS_WAIT, false, false);
		}
		process_battle();
		state = BATTLE_STATE_BATTLE;
	}
}

void Battle::process_battle() {
	GameManager* game_manager = GameManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	process_debug_boxes();

	debug_controller.poll_buttons();
	if (debug_controller.check_button_trigger(BUTTON_MENU_FRAME_PAUSE)) {
		if (frame_pause) {
			sound_manager->resume_se_all();
			sound_manager->resume_vc_all();
		}
		frame_pause = !frame_pause;
		timer.flip_clock();
	}
	for (int i = 0; i < 2; i++) {
		if (player[i]->controller.check_button_trigger(BUTTON_START)) {
			game_manager->game_main[GAME_STATE_PAUSE_BATTLE]();
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
	sound_manager->process_sounds();
	if (game_context == GAME_CONTEXT_TRAINING) {
		process_training();
	}
	if (battle_object_manager->frame_elapsed()) {
		battle_object_manager->world_frame = 0.0;
	}
	if (timer.time_up) {
		state = BATTLE_STATE_KO;
		ko_timer = 300;
	}
	if (game_manager->is_crash()) {
		game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}
}

void Battle::process_ko() {
	SoundManager* sound_manager = SoundManager::get_instance();

	battle_object_manager->world_frame += battle_object_manager->world_rate;
	pre_process_fighter();
	for (int i = 0; i < 2; i++) {
		thread_manager->notify_thread(i);
	}
	process_ui();
	stage.process();
	post_process_fighter();
	thread_manager->wait_thread(THREAD_KIND_UI);
	camera->camera_main();
	sound_manager->process_sounds();

	if (battle_object_manager->frame_elapsed()) {
		battle_object_manager->world_frame = 0.0;
	}

	int winner = 2;
	if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
		winner = 0;
	}
	if (fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
		winner--; //If player 1's health is at 0, this will drop our winner from 2 to 1, indicating
		//player 2 winning. If not, it will drop our winner from 0 to -1, indicating a timeout
	}
	if (winner != 2 && winner != -1 && fighter[!winner]->status_kind == FIGHTER_STATUS_KNOCKDOWN) {
		fighter[!winner]->change_status(FIGHTER_STATUS_ROUND_END);
	}

	if (ko_timer > 180) {
		ko_timer--;
	}
	else {
		if (winner != 2 && winner != -1) {
			//Single KO
			if (fighter[winner]->is_actionable() && fighter[winner]->situation_kind == FIGHTER_SITUATION_GROUND) {
				if (meters[winner].wins == num_rounds) {
					state = BATTLE_STATE_OUTRO;
				}
				else {
					fighter[winner]->change_status(FIGHTER_STATUS_ROUND_END);
					fighter[!winner]->change_status(FIGHTER_STATUS_ROUND_END, false);
					if (fighter[winner]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[winner]->get_local_param_float("health")) {
						fighter[winner]->change_anim("round_win_perfect");
					}
					else {
						fighter[winner]->change_anim("round_win");
					}
				}
			}
			else {
				if (fighter[winner]->status_kind != FIGHTER_STATUS_ROUND_END) {
					ko_timer--;
					if (!ko_timer) { //If you remain inactionable for 5 seconds after KOing the
						//opponent for whatever reason, we just skip to the next round without you
						if (meters[winner].wins == num_rounds) {
							state = BATTLE_STATE_OUTRO;
						}
						else {
							meters[winner].wins++;
							for (int i = 0; i < 2; i++) {
								fighter[i]->reset();
							}
							state = BATTLE_STATE_BATTLE;
						}
					}
				}
				else {
					if (fighter[winner]->is_anim_end) {
						meters[winner].wins++;
						for (int i = 0; i < 2; i++) {
							fighter[i]->reset();
						}
						state = BATTLE_STATE_BATTLE;
					}
				}
			}
		}
		else {
			//Double KO or Timeout Draw
			if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]) {
				if (meters[0].wins == num_rounds - 1 && meters[1].wins == num_rounds - 1) {
					for (int i = 0; i < 2; i++) {
						fighter[i]->reset();
					}
					sudden_death = true;
					state = BATTLE_STATE_BATTLE;
				}
				else {
					bool game_over = false;
					for (int i = 0; i < 2; i++) {
						fighter[i]->reset();
						meters[i].wins++;
						if (meters[i].wins == num_rounds) {
							game_over = true;
						}
					}
					if (game_over) {
						state = BATTLE_STATE_OUTRO;
					}
					else {
						state = BATTLE_STATE_BATTLE;
					}
				}
			}
			else {
				//Timeout with a winner
				if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] > fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]) {
					winner = 0;
				}
				else {
					winner = 1;
				}
				if (fighter[0]->is_actionable() && fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND
					&& fighter[1]->is_actionable() && fighter[1]->situation_kind == FIGHTER_SITUATION_GROUND) {
					if (meters[winner].wins == num_rounds) {
						state = BATTLE_STATE_OUTRO;
					}
					else {
						fighter[winner]->change_status(FIGHTER_STATUS_ROUND_END);
						fighter[winner]->change_anim("round_win_timeout");
						fighter[!winner]->change_status(FIGHTER_STATUS_ROUND_END);
						fighter[!winner]->change_anim("round_lose_timeout");
					}
				}
				else {
					ko_timer--;
					if ((fighter[winner]->is_anim_end && fighter[!winner]->is_anim_end) || !ko_timer) {
						meters[winner].wins++;
						for (int i = 0; i < 2; i++) {
							fighter[i]->reset();
						}
						timer.reset();
						state = BATTLE_STATE_BATTLE;
					}
				}
			}
		}
	}	
}

void Battle::process_outro() {

}

void Battle::process_debug_boxes() {
	if (mouse.check_button_on(MOUSE_BUTTON_M2) && active_hitbox_object != nullptr &&
		hitbox_sim[active_hitbox_object->get_anim()].active_box != nullptr) {
		glm::vec2 game_rect_coords = mouse_pos_to_rect_coord(mouse.get_pos());
		hitbox_sim[active_hitbox_object->get_anim()].active_box->offset = game_rect_coords;
		if (mouse.check_button_trigger(MOUSE_BUTTON_M2)) {
			hitbox_sim[active_hitbox_object->get_anim()].active_box->anchor = game_rect_coords;
		}
	}
}

void Battle::pre_process_fighter() {
	for (int i = 0; i < 2; i++) {
		switch (fighter[i]->fighter_int[FIGHTER_INT_UI_TEXT_TYPE]) {
			case UI_TEXT_TYPE_NONE:
			default: {

			} break;
			case UI_TEXT_TYPE_ARMOR_BREAK: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Armor Break", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
			case UI_TEXT_TYPE_COUNTER: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Counter", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
			case UI_TEXT_TYPE_COUNTER_PUNISH: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Punish Counter", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
			case UI_TEXT_TYPE_COUNTER_HITSTUN_PARRY: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Hitstun Parry Counter", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
			case UI_TEXT_TYPE_COUNTER_JUMP: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Jump Counter", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
			case UI_TEXT_TYPE_REVERSAL: {
				texts[i].push_back(BattleText());
				texts[i].back().init(&message_font, "Reversal", 40, fighter[i], glm::vec2(275.0, 450.0));
			} break;
		}
		fighter[i]->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_NONE;
	}
	if (fighter[0]->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] 
		|| fighter[1]->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION]) {
		return;
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
				if (fighter[!i]->situation_kind == FIGHTER_SITUATION_GROUND) {
					fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
					fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
					if (fighter[i]->pos.x == fighter[!i]->pos.x 
						&& fighter[i]->internal_facing_dir == fighter[!i]->internal_facing_dir) {
						fighter[i]->internal_facing_right = true;
						fighter[i]->internal_facing_dir = 1.0;
						fighter[!i]->internal_facing_right = false;
						fighter[!i]->internal_facing_dir = -1.0;
					}
					fighter[i]->add_pos(glm::vec3(-10.0 * fighter[i]->internal_facing_dir, 0, 0));
					fighter[!i]->add_pos(glm::vec3(-10.0 * fighter[!i]->internal_facing_dir, 0, 0));

					fighter[i]->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = false;
					fighter[!i]->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = false;
				}
			}
		}
	}
}

void Battle::process_fighter() {	
	for (int i = 0; i < 2; i++) {
		player[i]->controller.poll_buttons();
		thread_manager->notify_thread(i);
	}
}

void gamestate_battle_fighter_thread(void* fighter_arg) {
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
		glm::vec4 color;
		std::string advantage = std::to_string(fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE]);
		if (fighter[i]->fighter_int[FIGHTER_INT_FRAME_ADVANTAGE] >= 0) {
			advantage = "Frame Advantage: +" + advantage;
			color = glm::vec4(0.0, 0.0, 255.0, 255.0);
		}
		else {
			advantage = "Frame Advantage: " + advantage;
			color = glm::vec4(255.0, 0.0, 0.0, 255.0);
		}
		if (training_info[i].frame_advantage.get_text() != advantage) {
			training_info[i].frame_advantage.update_text(info_font, advantage, color, glm::vec4(0.0, 0.0, 0.0, 2.0));
		}
		if (fighter[i]->fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] == 0) {
			fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH], fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] + 10.0, fighter[i]->get_local_param_float("health"));
			fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH];
		}
		if (fighter[i]->fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] == 0) {
			fighter[i]->fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(fighter[i]->fighter_float[FIGHTER_FLOAT_EX_METER], fighter[i]->fighter_float[FIGHTER_FLOAT_EX_METER] + 10.0, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		}

		short new_input_code = fighter[i]->get_stick_dir_no_lr() << 7;
		for (int i2 = 0; i2 < 6; i2++) {
			if (fighter[i]->check_button_on(BUTTON_LP + i2)) {
				new_input_code |= 1 << i2;
			}
		}
		if (new_input_code != training_info[i].mini_visualizers.front().input_code) {
			training_info[i].mini_visualizers.front().keep_frames = false;
			training_info[i].mini_visualizers.pop_back();
			for (std::list<InputVisualizer>::iterator it = training_info[i].mini_visualizers.begin(), 
				max = training_info[i].mini_visualizers.end(); it != max; it++) {
				for (int i2 = 0; i2 < 6; i2++) {
					it->buttons[i2].add_pos(glm::vec3(0.0, 90.0, 0.0));
				}
				
				it->stick[(it->input_code >> 7) - 1].add_pos(glm::vec3(0.0, 90.0, 0.0));
				it->num_frames.add_pos(glm::vec3(0.0, 90.0, 0.0));
				it->background.add_pos(glm::vec3(0.0, 90.0, 0.0));
			}
			training_info[i].mini_visualizers.push_front(InputVisualizer());
			training_info[i].mini_visualizers.front().init(fighter[i], &info_font, true);
			training_info[i].mini_visualizers.front().input_code = new_input_code;
		}
		else if ((!frame_pause) || (frame_pause && debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE))) {
			training_info[i].mini_visualizers.front().frame_timer = clamp(
				training_info[i].mini_visualizers.front().frame_timer, 
				training_info[i].mini_visualizers.front().frame_timer + 1, 99
			);
			training_info[i].mini_visualizers.front().num_frames.update_text(info_font, 
				std::to_string(training_info[i].mini_visualizers.front().frame_timer), 
				glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 1.0)
			);
		}
	}
}

void gamestate_battle_ui_thread(void* battle_arg) {
	Battle* battle = (Battle*)battle_arg;
	battle->timer.process();
	for (int i = 0; i < 2; i++) {
		battle->meters[i].process();
	}
}

void Battle::process_frame_pause() {
	SoundManager* sound_manager = SoundManager::get_instance();
	if (debug_controller.check_button_trigger(BUTTON_MENU_ADVANCE)) {
		sound_manager->resume_se_all();
		sound_manager->resume_vc_all();
		battle_object_manager->world_frame += battle_object_manager->world_rate;
		pre_process_fighter();
		process_fighter();
		process_ui();
		post_process_fighter();
		thread_manager->wait_thread(THREAD_KIND_UI);
		camera->camera_main();
	}
	else {
		sound_manager->pause_se_all();
		sound_manager->pause_vc_all();
		camera->update_view();
	}
}

void Battle::render_main() {
	render_world();
	if (state == BATTLE_STATE_BATTLE) {
		render_ui();
	}
}

void Battle::render_world() {
	RenderManager* render_manager = RenderManager::get_instance();
	render_manager->execute_buffered_events();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0x00);

	//SHADOW PASS

	render_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_shadow();
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_shadow();
			}
		}
	}
	glCullFace(GL_BACK);

	//COLOR PASS

	render_manager->g_buffer.use();
	glViewport(0, 0, render_manager->res_width, render_manager->res_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_manager->shadow_map.bind_textures();

	for (int i = 0; i < 2; i++) {
		fighter[i]->render();
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render();
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
		fighter[i]->render();
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render();
			}
		}
	}
	glStencilMask(0x00);
	stage.render();
	glClear(GL_COLOR_BUFFER_BIT);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	glDepthMask(GL_FALSE);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_outline();
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->active && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_outline();
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
	glViewport(0, 0, render_manager->window_width, render_manager->window_height);
	render_manager->g_buffer.render();
#ifdef DEBUG
	render_manager->gbuffer_texture.render();
#endif
	render_manager->outline.render_passthrough();

	//HITBOX PASS

	if (visualize_boxes) {
		render_manager->box_layer.use();
		glViewport(0, 0, render_manager->res_width, render_manager->res_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (int i = 0; i < 2; i++) {
			for (int i2 = 0; i2 < 10; i2++) {
				if (fighter[i]->pushboxes[i2].active) {
					fighter[i]->pushboxes[i2].rect.render();
				}
			}
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
			for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
				if (fighter[i]->projectiles[i2] != nullptr && fighter[i]->projectiles[i2]->active) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->pushboxes[i3].active) {
							fighter[i]->projectiles[i2]->pushboxes[i3].rect.render();
						}
					}
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->hurtboxes[i3].active) {
							fighter[i]->projectiles[i2]->hurtboxes[i3].rect.render();
						}
					}
					if (fighter[i]->projectiles[i2]->blockbox.active) {
						fighter[i]->projectiles[i2]->blockbox.rect.render();
					}
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->grabboxes[i3].active) {
							fighter[i]->projectiles[i2]->grabboxes[i3].rect.render();
						}
					}
					for (int i3 = 0; i3 < 10; i3++) {
						if (fighter[i]->projectiles[i2]->hitboxes[i3].active) {
							fighter[i]->projectiles[i2]->hitboxes[i3].rect.render();
						}
					}
				}
			}
		}
		if (active_hitbox_object) {
			hitbox_sim[active_hitbox_object->get_anim()].update(active_hitbox_object);
			hitbox_sim[active_hitbox_object->get_anim()].render(active_hitbox_object);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, render_manager->window_width, render_manager->window_height);
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
	//TRAINING PASS

	if (game_context == GAME_CONTEXT_TRAINING) {
		for (int i = 0; i < 2; i++) {
			training_info[i].render();
		}
	}

}

void Battle::process_background() {
	render_world();
	render_ui();
}