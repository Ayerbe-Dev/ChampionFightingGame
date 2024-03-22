#include "Battle.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <SDL/SDL.h>
#include <glew/glew.h>

#include "BattleObject.h"
#include "Fighter.h"
#include "FighterInterface.h"
#include "Projectile.h"
#include "Projectiles.h"
#include "Stage.h"

#include "EffectManager.h"
#include "FontManager.h"
#include "ObjectManager.h"
#include "ParamAccessor.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "ThreadManager.h"

#include "cotr_imgui_debugger.h"

#include "VectorBoolPointer.h"
#include "utils.h"

void battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	font_manager->load_face("Fiend-Oblique");

	Battle *battle = new Battle;

#ifdef DEBUG
	cotr_imgui_init();
#endif

	while (battle->looping) {
		game_manager->frame_delay_check_fps();
		render_manager->clear_screen();

		#ifdef DEBUG
			render_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);
		#else
			render_manager->handle_window_events();
		#endif

		battle->process_game_state();
		battle->render_game_state();

#ifdef DEBUG
		cotr_imgui_debug_battle(battle);
#endif

		render_manager->update_screen();
	}
#ifdef DEBUG
	cotr_imgui_terminate();
#endif

	font_manager->unload_face("Fiend-Oblique");
	delete battle;
}

Battle::Battle() {
	GameManager* game_manager = GameManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	object_manager = ObjectManager::get_instance();
	thread_manager = ThreadManager::get_instance();

	round_count_setting = save_manager->get_game_setting("round_count");
	ko_timer = 0;
	curr_round = 0;
	frame_pause = false;
	frame_advance = false;

	if (player[0]->chara_kind == player[1]->chara_kind
		&& player[0]->alt_costume == player[1]->alt_costume
		&& player[0]->alt_color == player[1]->alt_color) {
		if (player[0]->alt_color == 0) {
			player[1]->alt_color++;
		}
		else {
			player[1]->alt_color--;
		}
	}

	switch (game_context) {
		case (GAME_CONTEXT_NORMAL): {
			internal_state = BATTLE_STATE_PRE_INTRO;
			timer_setting = TIMER_SETTING_NORMAL;
		} break;
		case (GAME_CONTEXT_TRAINING): {
			internal_state = BATTLE_STATE_BATTLE;
			timer_setting = TIMER_SETTING_TRAINING;
		} break;
		case (GAME_CONTEXT_ARCADE): {
			internal_state = BATTLE_STATE_INTRO;
			timer_setting = TIMER_SETTING_NORMAL;
		} break;
		case (GAME_CONTEXT_STORY): {
			internal_state = BATTLE_STATE_BATTLE;
			timer_setting = TIMER_SETTING_NONE;
		} break;
		case (GAME_CONTEXT_ONLINE): {
			internal_state = BATTLE_STATE_PRE_INTRO;
			timer_setting = TIMER_SETTING_NORMAL;
		} break;
		case (GAME_CONTEXT_SPECIAL): {
			internal_state = BATTLE_STATE_PRE_INTRO;
			timer_setting = TIMER_SETTING_NORMAL;
		} break;
	}

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	load_world();
	load_ui();

	active_hitbox_object = fighter[0];
	active_hitbox_object_index = 0;

	for (int i = 0; i < 2; i++) {
		thread_manager->add_thread(i, gamestate_battle_fighter_thread, (void*)fighter[i]);
	}
	thread_manager->add_thread(THREAD_KIND_UI, gamestate_battle_ui_thread, (void*)this);

	Music music;
	if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_STAGE) {
		music = sound_manager->get_music(stage.default_music_kind);
	}
	else if (save_manager->get_game_setting("music_setting") == MUSIC_SETTING_CHARA) {
//		sound_manager->load_sound("Battle Music", [a random directory for one of the characters here]);
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//sound_manager->load_sound("Battle Music", player[rng(0, 1)]->default_music_kind);
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}
	battle_music = music.instantiate(0.0);
}

Battle::~Battle() {
	RenderManager* render_manager = RenderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	camera->reset_camera();

	//Unload each fighter and related info

	effect_manager->clear_effect_all();
	for (int i = 0; i < 2; i++) {
		player[i]->controller.reset_player_controller();
		thread_manager->kill_thread(i);
		training_info[i].destroy();
		camera->fighter[i] = nullptr;
		delete fighter[i];
	}

	//Sounds and effects were already removed, now fully unload them

	sound_manager->stop_music_all();
	sound_manager->unload_all_sounds();
	sound_manager->unload_all_music();
	sound_manager->clear_sound_players();
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
}

void Battle::load_world() {
	RenderManager* render_manager = RenderManager::get_instance();

	int rng = rand() % 2;
	stage.load_stage(player[rng]->stage_info, object_manager);

	for (int i = 0; i < 2; i++) {
		fighter[i] = create_fighter(player[i]);
	}

	camera = &render_manager->camera;
	camera->reset_camera();
	for (int i = 0; i < 2; i++) {
		camera->fighter[i] = fighter[i];
	}
	camera->stage = &stage;

	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();
}

void Battle::load_ui() {
	FontManager* font_manager = FontManager::get_instance();
	combo_font = font_manager->load_font("Fiend-Oblique", 64);
	message_font = font_manager->load_font("Fiend-Oblique", 24);
	info_font = font_manager->load_font("FiraCode", 16);

	set_hints(1, 10, 3); {
		push_menu_child("P1 Health", 3); {
			push_menu_process_function([this](MenuObject* object) {
				float max_health = object->float_var("max_health");
				float health = *(float*)(object->ptr_var("health"));
				float partial_health = *(float*)(object->ptr_var("partial_health"));
				bool ended_hitstun = get_value_at_vector_bool_addr((unsigned*)(object->ptr_var("ended_hitstun")), FIGHTER_FLAG_ENDED_HITSTUN);
				int training_health_recovery_timer = *(int*)(object->ptr_var("health_recovery_timer"));
				if (partial_health < health) {
					object->get_texture("Combo Health").scale_left_percent(partial_health / max_health);
				}
				if (ended_hitstun) {
					object->get_texture("Combo Health").set_left_target(clampf(0.0, health / max_health, 1.0), 5.0);
				}
				if (training_health_recovery_timer == 0) {
					object->get_texture("Combo Health").scale_left_percent(clampf(0.0, health / max_health, 1.0));
				}
				object->get_texture("Health").scale_left_percent(health / max_health);
				object->get_texture("Partial Health").scale_left_percent(partial_health / max_health);
			});

			push_menu_float_var("max_health", fighter[0]->get_param_float("health"));
			push_menu_ptr_var("health", &fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH]);
			push_menu_ptr_var("partial_health", &fighter[0]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			push_menu_ptr_var("ended_hitstun", (void*)fighter[0]->fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN]._Getptr());
			push_menu_ptr_var("health_recovery_timer", &fighter[0]->fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER]);

			push_menu_texture("Combo Health", "resource/game_state/battle/ui/meter/combo_health.png");
			push_menu_texture("Health", "resource/game_state/battle/ui/meter/health.png");
			last_pushed_texture->set_alpha(127);
			push_menu_texture("Partial Health", "resource/game_state/battle/ui/meter/health.png");
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
			push_menu_pos(glm::vec3(66.0, 114.0, 0.0));
			push_menu_dimensions("Health");
		} pop_menu_stack();
		push_menu_child("P2 Health", 3); {
			push_menu_process_function([this](MenuObject* object) {
				float max_health = object->float_var("max_health");
				float health = *(float*)(object->ptr_var("health"));
				float partial_health = *(float*)(object->ptr_var("partial_health"));
				bool ended_hitstun = get_value_at_vector_bool_addr((unsigned*)(object->ptr_var("ended_hitstun")), FIGHTER_FLAG_ENDED_HITSTUN);
				int training_health_recovery_timer = *(int*)(object->ptr_var("health_recovery_timer"));
				if (partial_health < health) {
					object->get_texture("Combo Health").scale_left_percent(partial_health / max_health);
				}
				if (ended_hitstun) {
					object->get_texture("Combo Health").set_left_target(clampf(0.0, health / max_health, 1.0), 5.0);
				}
				if (training_health_recovery_timer == 0) {
					object->get_texture("Combo Health").scale_left_percent(clampf(0.0, health / max_health, 1.0));
				}
				object->get_texture("Health").scale_left_percent(health / max_health);
				object->get_texture("Partial Health").scale_left_percent(partial_health / max_health);
			});
			push_menu_float_var("max_health", fighter[1]->get_param_float("health"));
			push_menu_ptr_var("health", &fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]);
			push_menu_ptr_var("partial_health", &fighter[1]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			push_menu_ptr_var("ended_hitstun", (void*)fighter[1]->fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN]._Getptr());
			push_menu_ptr_var("health_recovery_timer", &fighter[1]->fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER]);			
			push_menu_texture("Combo Health", "resource/game_state/battle/ui/meter/combo_health.png");
			last_pushed_texture->flip_h();
			push_menu_texture("Health", "resource/game_state/battle/ui/meter/health.png");
			last_pushed_texture->flip_h();
			last_pushed_texture->set_alpha(127);
			push_menu_texture("Partial Health", "resource/game_state/battle/ui/meter/health.png");
			last_pushed_texture->flip_h();
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
			push_menu_pos(glm::vec3(66.0, 114.0, 0.0));
			push_menu_dimensions("Health");
		} pop_menu_stack();
		push_menu_child("P1 Scale", 3); {
			push_menu_process_function([this](MenuObject* object) {
				float scale = *(float*)object->ptr_var("damage_scale");
				object->get_texture("Damage Scale 120").set_alpha(255 * (scale >= 1.2f));
				object->get_texture("Damage Scale 110").set_alpha(255 * (scale >= 1.1f));
				object->get_texture("Damage Scale").set_left_target(clampf(scale, scale, 1.0f), 3);
			});
			push_menu_ptr_var("damage_scale", &fighter[0]->fighter_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI]);
			push_menu_texture("Damage Scale", "resource/game_state/battle/ui/meter/damage_scale.png");
			push_menu_texture("Damage Scale 110", "resource/game_state/battle/ui/meter/damage_scale_110.png");
			last_pushed_texture->set_alpha(0);
			push_menu_texture("Damage Scale 120", "resource/game_state/battle/ui/meter/damage_scale_120.png");
			last_pushed_texture->set_alpha(0);
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
			push_menu_pos(glm::vec3(1026.0, 244.0, 0.0));
			push_menu_dimensions("Damage Scale");
		} pop_menu_stack();
		push_menu_child("P2 Scale", 3); {
			push_menu_process_function([this](MenuObject* object) {
				float scale = *(float*)object->ptr_var("damage_scale");
				object->get_texture("Damage Scale 120").set_alpha(255 * (scale >= 1.2f));
				object->get_texture("Damage Scale 110").set_alpha(255 * (scale >= 1.1f));
				object->get_texture("Damage Scale").set_left_target(clampf(scale, scale, 1.0f), 3);
			});
			push_menu_ptr_var("damage_scale", &fighter[1]->fighter_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI]);
			push_menu_texture("Damage Scale", "resource/game_state/battle/ui/meter/damage_scale.png");
			last_pushed_texture->flip_h();
			push_menu_texture("Damage Scale 110", "resource/game_state/battle/ui/meter/damage_scale_110.png");
			last_pushed_texture->set_alpha(0);
			last_pushed_texture->flip_h();
			push_menu_texture("Damage Scale 120", "resource/game_state/battle/ui/meter/damage_scale_120.png");
			last_pushed_texture->set_alpha(0);
			last_pushed_texture->flip_h();
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
			push_menu_pos(glm::vec3(1026.0, 244.0, 0.0));
			push_menu_dimensions("Damage Scale");
		} pop_menu_stack();
		push_menu_child("P1 EX"); {
			push_menu_process_function([this](MenuObject* object) {
				float max_ex = object->float_var("max_ex");
				float ex = *(float*)object->ptr_var("ex");
				float &prev_segments = object->float_var("prev_segments");
				object->get_texture("EX").set_left_target(ex / max_ex, 6);
				float segments = floor(ex / 100.0);
				if (prev_segments != segments) {
					object->get_texture("EX Segment").set_left_target(clampf(0.0, segments / 6.0, 1.0), 4);
				}
				prev_segments = segments;
			});

			push_menu_float_var("max_ex", get_global_param_float(PARAM_FIGHTER, "ex_meter_size"));
			push_menu_ptr_var("ex", &fighter[0]->fighter_float[FIGHTER_FLOAT_EX_METER]);
			push_menu_float_var("prev_segments", 0.0);
			
			push_menu_texture("EX", "resource/game_state/battle/ui/meter/ex.png");
			last_pushed_texture->scale_left_percent(0.0);
			push_menu_texture("EX Segment", "resource/game_state/battle/ui/meter/ex_segment.png");
			last_pushed_texture->scale_left_percent(0.0);
			push_menu_pos(glm::vec3(324.0, 180.0, 0.0));
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
			push_menu_dimensions("EX");
		} pop_menu_stack();
		push_menu_child("P2 EX"); {
			push_menu_process_function([this](MenuObject* object) {
				float max_ex = object->float_var("max_ex");
				float ex = *(float*)object->ptr_var("ex");
				float &prev_segments = object->float_var("prev_segments");
				object->get_texture("EX").set_left_target(ex / max_ex, 6);
				float segments = floor(ex / 100.0);
				if (prev_segments != segments) {
					object->get_texture("EX Segment").set_left_target(clampf(0.0, segments / 6.0, 1.0), 4);
				}
				prev_segments = segments;
			});

			push_menu_float_var("max_ex", get_global_param_float(PARAM_FIGHTER, "ex_meter_size"));
			push_menu_ptr_var("ex", &fighter[1]->fighter_float[FIGHTER_FLOAT_EX_METER]);
			push_menu_float_var("prev_segments", 0.0);

			push_menu_texture("EX", "resource/game_state/battle/ui/meter/ex.png");
			last_pushed_texture->flip_h();
			last_pushed_texture->scale_left_percent(0.0);
			push_menu_texture("EX Segment", "resource/game_state/battle/ui/meter/ex_segment.png");
			last_pushed_texture->flip_h();
			last_pushed_texture->scale_left_percent(0.0);
			push_menu_pos(glm::vec3(324.0, 180.0, 0.0));
			push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
			push_menu_dimensions("EX");
		} pop_menu_stack();
		push_menu_activity_group("P1 Super", nullptr, true); {
			push_menu_child("SuperAG"); {
				push_menu_process_function([this](MenuObject* object) {
					float super = *(float*)object->ptr_var("super");
					float max_super = object->float_var("max_super");
					if (super == max_super) {
						object->set_active_sibling("Super FullAG");
						return;
					}
					object->get_texture("Super").set_sprite((int)((super / max_super) * 100.0));
				});
				push_menu_ptr_var("super", &fighter[0]->fighter_float[FIGHTER_FLOAT_SUPER_METER]);
				push_menu_float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"));
				push_menu_texture("Super", "resource/game_state/battle/ui/meter/super.gif");
				push_menu_pos(glm::vec3(66.0, 70.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
				push_menu_dimensions("Super");
			} pop_menu_stack();
			push_menu_child("Super FullAG"); {
				push_menu_process_function([this](MenuObject* object) {	
					float super = *(float*)object->ptr_var("super");
					if (super != object->float_var("max_super")) {
						object->set_active_sibling("Super AG");
						return;
					}
					object->get_texture("Super Full").next_sprite();					
				});
				push_menu_ptr_var("super", &fighter[0]->fighter_float[FIGHTER_FLOAT_SUPER_METER]);
				push_menu_float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"));
				push_menu_texture("Super Full", "resource/game_state/battle/ui/meter/super_full.gif");
				push_menu_pos(glm::vec3(66.0, 70.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
				push_menu_dimensions("Super Full");
			} pop_menu_stack();
		} pop_menu_stack();
		push_menu_activity_group("P2 Super", nullptr, true); {
			push_menu_child("SuperAG"); {
				push_menu_process_function([this](MenuObject* object) {				
					float super = *(float*)object->ptr_var("super");
					float max_super = object->float_var("max_super");
					if (super == max_super) {
						object->set_active_sibling("Super FullAG");
						return;
					}
					object->get_texture("Super").set_sprite((int)((super / max_super) * 100.0));
				});
				push_menu_ptr_var("super", &fighter[1]->fighter_float[FIGHTER_FLOAT_SUPER_METER]);
				push_menu_float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"));
				push_menu_texture("Super", "resource/game_state/battle/ui/meter/super.gif");
				last_pushed_texture->flip_h();
				push_menu_pos(glm::vec3(66.0, 70.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
				push_menu_dimensions("Super");
			} pop_menu_stack();
			push_menu_child("Super FullAG"); {
				push_menu_process_function([this](MenuObject* object) {
					float super = *(float*)object->ptr_var("super");
					if (super != object->float_var("max_super")) {
						object->set_active_sibling("Super AG");
						return;
					}
					object->get_texture("Super Full").next_sprite();
				});
				push_menu_ptr_var("super", &fighter[1]->fighter_float[FIGHTER_FLOAT_SUPER_METER]);
				push_menu_float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"));
				push_menu_texture("Super Full", "resource/game_state/battle/ui/meter/super_full.gif");
				last_pushed_texture->flip_h();
				push_menu_pos(glm::vec3(66.0, 70.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
				push_menu_dimensions("Super Full");
			} pop_menu_stack();
		} pop_menu_stack();
		push_menu_activity_group("KO Text", nullptr, true, 5); {
			push_menu_child("None"); {
				push_menu_texture("Message", combo_font, "", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				last_pushed_texture->set_alpha(0);
			} pop_menu_stack();
			push_menu_child("KO"); {
				push_menu_texture("Message", combo_font, "K.O.", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				object_stack.top()->sound_player.load_sound("KO", "resource/sound/ui/ko.wav");
				last_pushed_texture->set_alpha(0);
			} pop_menu_stack();
			push_menu_child("Perfect KO"); {
				push_menu_texture("Message", combo_font, "Perfect K.O.", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				object_stack.top()->sound_player.load_sound("KO", "resource/sound/ui/ko.wav");
				last_pushed_texture->set_alpha(0);
			} pop_menu_stack();
			push_menu_child("Time"); {
				push_menu_texture("Message", combo_font, "Time", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				object_stack.top()->sound_player.load_sound("KO", "resource/sound/ui/ko.wav");
				last_pushed_texture->set_alpha(0);
			} pop_menu_stack();
			push_menu_child("Double KO"); {
				push_menu_texture("Message", combo_font, "Double K.O.", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				object_stack.top()->sound_player.load_sound("KO", "resource/sound/ui/ko.wav");
				last_pushed_texture->set_alpha(0);
			} pop_menu_stack();
		} pop_menu_stack();
		push_menu_child("Round Start Text"); {
			push_menu_activity_group("Round Start AG", &curr_round, true, 6); {
				push_menu_child("Round1"); {
					if (round_count_setting == 1) {
						push_menu_texture("Round", combo_font, "Ready...", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					}
					else {
						push_menu_texture("Round", combo_font, "Round 1", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					}
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
				push_menu_child("Round2"); {
					push_menu_texture("Round", combo_font, "Round 2", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
				push_menu_child("Round3"); {
					if (round_count_setting == 2) {
						push_menu_texture("Round", combo_font, "Final Round", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					}
					else {
						push_menu_texture("Round", combo_font, "Round 3", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					}
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
				push_menu_child("Round4"); {
					push_menu_texture("Round", combo_font, "Round 4", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
				push_menu_child("Round5"); {
					push_menu_texture("Round", combo_font, "Final Round", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
				push_menu_child("Round6"); {
					push_menu_texture("Round", combo_font, "Sudden Death", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
					last_pushed_texture->set_alpha(0);
				} pop_menu_stack();
			} pop_menu_stack();
			push_menu_texture("Fight", combo_font, "Fight!", glm::vec4(255.0, 127.0, 0.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
			last_pushed_texture->set_alpha(0);
		} pop_menu_stack();

		push_menu_texture("HUD", "resource/game_state/battle/ui/battle_hud.png");

		if (game_context != GAME_CONTEXT_TRAINING) {
			push_menu_child("Round Counter P1", round_count_setting + 1); {
				push_menu_int_var("Wins", 0);
				switch (round_count_setting) {
					case (1): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo1_l.gif");
						last_pushed_texture->set_pos(glm::vec3(6.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo1.png");
					} break;
					case (2): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo3_l.gif");
						last_pushed_texture->set_pos(glm::vec3(204.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win2", "resource/game_state/battle/ui/rounds/round_win_bo3_l.gif");
						last_pushed_texture->set_pos(glm::vec3(-192.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo3.png");
					} break;
					case (3): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo5_l.gif");
						last_pushed_texture->set_pos(glm::vec3(270.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win2", "resource/game_state/battle/ui/rounds/round_win_bo5_l.gif");
						last_pushed_texture->set_pos(glm::vec3(6.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win3", "resource/game_state/battle/ui/rounds/round_win_bo5_l.gif");
						last_pushed_texture->set_pos(glm::vec3(-258.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo5.png");
					} break;
					default: {

					} break;
				}
				push_menu_pos(glm::vec3(1002.0, 62.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
				push_menu_dimensions("Round Bar");
			} pop_menu_stack();
			push_menu_child("Round Counter P2", round_count_setting + 1); {
				push_menu_int_var("Wins", 0);
				switch (round_count_setting) {
					case (1): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo1_r.gif");
						last_pushed_texture->set_pos(glm::vec3(-6.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo1.png");
					} break;
					case (2): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo3_r.gif");
						last_pushed_texture->set_pos(glm::vec3(-204.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win2", "resource/game_state/battle/ui/rounds/round_win_bo3_r.gif");
						last_pushed_texture->set_pos(glm::vec3(192.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo3.png");
					} break;
					case (3): {
						push_menu_texture("Round Win1", "resource/game_state/battle/ui/rounds/round_win_bo5_r.gif");
						last_pushed_texture->set_pos(glm::vec3(-270.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win2", "resource/game_state/battle/ui/rounds/round_win_bo5_r.gif");
						last_pushed_texture->set_pos(glm::vec3(-6.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Win3", "resource/game_state/battle/ui/rounds/round_win_bo5_r.gif");
						last_pushed_texture->set_pos(glm::vec3(258.0, -5.0, 0.0));
						last_pushed_texture->set_alpha(0);
						push_menu_texture("Round Bar", "resource/game_state/battle/ui/rounds/round_bar_bo5.png");
					} break;
					default: {

					} break;
				}
				last_pushed_texture->flip_h();
				push_menu_pos(glm::vec3(1002.0, 62.0, 0.0));
				push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
				push_menu_dimensions("Round Bar");
			} pop_menu_stack();
		}
		else {
			for (int i = 0; i < 2; i++) {
				training_info[i].init(fighter[i], &info_font);
			}
		}

		push_menu_child("Timer", 5); {
			push_menu_bool_var("pause", false);
			push_menu_bool_var("time_up", false);
			switch (timer_setting) {
				case (TIMER_SETTING_NORMAL): {
					push_menu_process_function([this](MenuObject* object) {
						if (!object->bool_var("pause") && !object->bool_var("time_up")) {
							int& frame = object->int_var("frame");
							if (frame == 0) {
								frame = 9;
								int& deca_frame = object->int_var("deca_frame");
								if (deca_frame == 0) {
									deca_frame = 5;
									object->get_texture("Deca Frame").set_sprite(6);
									int& sec = object->int_var("sec");
									if (sec == 0) {
										sec = 9;
										int& deca_sec = object->int_var("deca_sec");
										if (deca_sec == 0) {
											object->bool_var("time_up") = true;
										}
										else {
											deca_sec--;
										}
										object->get_texture("Deca Second").prev_sprite();
									}
									else {
										sec--;
									}
									object->get_texture("Second").prev_sprite();
								}
								else {
									deca_frame--;
								}
								object->get_texture("Deca Frame").prev_sprite();
							}
							else {
								frame--;
							}
							object->get_texture("Frame").prev_sprite();
						}
					});
					push_menu_int_var("deca_sec", 9);
					push_menu_int_var("sec", 9);
					push_menu_int_var("deca_frame", 5);
					push_menu_int_var("frame", 9);
					push_menu_texture("Background", "resource/game_state/battle/ui/timer/timer.png");
					push_menu_texture("Deca Second", "resource/game_state/battle/ui/timer/sec.gif");
					last_pushed_texture->set_pos(glm::vec3(-38.0, 19.0, 0.0));
					last_pushed_texture->set_sprite(9);
					push_menu_texture("Second", "resource/game_state/battle/ui/timer/sec.gif");
					last_pushed_texture->set_pos(glm::vec3(38.0, 19.0, 0.0));
					last_pushed_texture->set_sprite(9);
					push_menu_texture("Deca Frame", "resource/game_state/battle/ui/timer/frame.gif");
					last_pushed_texture->set_pos(glm::vec3(-18.0, -84.0, 0.0));
					last_pushed_texture->set_sprite(5);
					push_menu_texture("Frame", "resource/game_state/battle/ui/timer/frame.gif");
					last_pushed_texture->set_pos(glm::vec3(18.0, -84.0, 0.0));
					last_pushed_texture->set_sprite(9);
					push_menu_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_MIDDLE);
					push_menu_dimensions("Background");
					push_menu_pos(glm::vec3(0.0, 74.0, 0.0));
				} break;
				case (TIMER_SETTING_TRAINING): {
					//Timer in this mode will show like the status and shit
				} break;
				case (TIMER_SETTING_NONE): {

				} break;
			}
		}
	} pop_menu_stack();
}

void Battle::pre_event_process_main() {
	if (frame_advance) {
		SoundManager* sound_manager = SoundManager::get_instance();
		sound_manager->pause_all_sounds();
		sound_manager->pause_all_reserved_sounds();
		camera->update_view();
		frame_advance = false;
	}
}

void Battle::process_main() {
	switch (internal_state) {
		case(BATTLE_STATE_PRE_INTRO): {
			process_pre_intro();
		} break;
		case (BATTLE_STATE_INTRO): {
			process_intro();
		} break;
		case (BATTLE_STATE_ROUND_START): {
			process_round_start();
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
	process_collisions();
}

void Battle::process_pre_intro() {
	internal_state = BATTLE_STATE_INTRO;
	for (int i = 0; i < 2; i++) {
		fighter[i]->change_anim("intro");
		fighter[i]->rot.z = glm::radians(90.0f);
	}
}

void Battle::process_intro() {
	if (!fighter[0]->anim_end) {
		fighter[0]->process_animate();
		fighter[0]->execute_active_script();
		fighter[0]->process_post_animate();
	}
	else if (!fighter[1]->anim_end) {
		fighter[1]->process_animate();
		fighter[1]->execute_active_script();
		fighter[1]->process_post_animate();
	}
	else {
		for (int i = 0; i < 2; i++) {
			fighter[i]->change_status(FIGHTER_STATUS_NONE, false);
		}
		internal_state = BATTLE_STATE_ROUND_START;
	}
}

void Battle::process_round_start() {
	SoundManager* sound_manager = SoundManager::get_instance();
	MenuObject& round_start = get_child("Round Start Text");
	if (execute_if("Init", 1, true)) {
		for (int i = 0; i < 2; i++) {
			fighter[i]->fighter_flag[FIGHTER_FLAG_ROUND_START] = true;
			fighter[i]->reset();
		}
		MenuObject& timer = get_child("Timer");
		timer.bool_var("time_up") = false;
		timer.bool_var("pause") = true;
		timer.int_var("deca_frame") = 5;
		timer.int_var("frame") = 9;
		timer.int_var("sec") = 9;
		timer.int_var("deca_sec") = 9;
		timer.get_texture("Deca Second").set_sprite(9);
		timer.get_texture("Second").set_sprite(9);
		timer.get_texture("Deca Frame").set_sprite(5);
		timer.get_texture("Frame").set_sprite(9);
		round_start.get_active_child("Round Start AG").get_texture("Round").set_alpha(255);
	}
	for (int i = 0; i < 2; i++) {
		player[i]->poll_controller_fighter();
	}
	process_fighters();
	process_ui();
	stage.process();
	post_process_fighters();
	thread_manager->wait_thread(THREAD_KIND_UI);
	camera->camera_main();
	if (execute_wait("Round X Timer", 1, 120)) {
		round_start.get_active_child("Round Start AG").get_texture("Round").set_alpha(0);
		round_start.get_texture("Fight").set_alpha(255);
	}
	if (execute_wait("Fight! Timer", 1, 60)) {
		round_start.get_texture("Fight").set_alpha(0);
		internal_state = BATTLE_STATE_BATTLE;
		get_child("Timer").bool_var("pause") = false;
		for (int i = 0; i < 2; i++) {
			fighter[i]->fighter_flag[FIGHTER_FLAG_ROUND_START] = false;
		}
	}
}

void Battle::process_battle() {
	GameManager* game_manager = GameManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	process_debug_boxes();
	if (frame_advance || !frame_pause) {
		for (int i = 0; i < 2; i++) {
			player[i]->poll_controller_fighter();
		}
		process_fighters();
		process_ui();
		stage.process();
		post_process_fighters();
		thread_manager->wait_thread(THREAD_KIND_UI);
		camera->camera_main();
		if (game_context == GAME_CONTEXT_TRAINING) {
			process_training();
		}
	}
	if (get_child("Timer").bool_var("time_up")) {
		internal_state = BATTLE_STATE_KO;
	}
}

void Battle::process_ko() {
	SoundManager* sound_manager = SoundManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	MenuObject& p1_round = get_child("Round Counter P1");
	MenuObject& p2_round = get_child("Round Counter P2");
	int& p1_wins = p1_round.int_var("Wins");
	int& p2_wins = p2_round.int_var("Wins");

	if (execute_if("Init", 1, true)) {
		if (object_manager->real_time_id == -1) {
			object_manager->set_world_rate(nullptr, 0.0067);
		}
		for (int i = 0; i < 2; i++) {
			player[i]->controller.reset_all_buttons();
			fighter[i]->fighter_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
		}
		ko_timer = 120;
		actionable_timer = 600;
		get_child("Timer").bool_var("pause") = true;
		if (curr_round != 5) {
			if (get_child("Timer").bool_var("time_up")) {
				p1_round.get_texture("Round Win" + std::to_string(
					p1_round.int_var("Wins") + 1)
				).set_sprite(ROUND_ICON_TIMEOUT);
				p2_round.get_texture("Round Win" + std::to_string(
					p2_round.int_var("Wins") + 1)
				).set_sprite(ROUND_ICON_TIMEOUT);
			}
			else if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0 && fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0) {
				p1_round.get_texture("Round Win" + std::to_string(
					p1_round.int_var("Wins") + 1)
				).set_sprite(ROUND_ICON_DOUBLE);
				p2_round.get_texture("Round Win" + std::to_string(
					p2_round.int_var("Wins") + 1)
				).set_sprite(ROUND_ICON_DOUBLE);
			}
			else {
				if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[0]->get_param_float("health")) {
					p1_round.get_texture("Round Win" + std::to_string(
						p1_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_PERFECT);
				}
				/*
				else if (p1 did ex super) {
					p1_round.get_texture("Round Win" + std::to_string(
						p1_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_EX_SUPER);
				}
				else if (p1 did champion super) {
					p1_round.get_texture("Round Win" + std::to_string(
						p1_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_CHAMPION_SUPER);
				}
				*/
				else {
					p1_round.get_texture("Round Win" + std::to_string(
						p1_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_KO);
				}

				if (fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->get_param_float("health")) {
					p2_round.get_texture("Round Win" + std::to_string(
						p2_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_PERFECT);
				}
				/*
				else if (p2 did ex super) {
					p2_round.get_texture("Round Win" + std::to_string(
						p2_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_EX_SUPER);
				}
				else if (p2 did champion super) {
					p2_round.get_texture("Round Win" + std::to_string(
						p2_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_CHAMPION_SUPER);
				}
				*/
				else {
					p2_round.get_texture("Round Win" + std::to_string(
						p2_round.int_var("Wins") + 1)
					).set_sprite(ROUND_ICON_KO);
				}
			}
		}
		if (get_child("Timer").bool_var("time_up")) {
			get_activity_group("KO Text").set_active_child("Time");
		}
		else if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[0]->get_param_float("health")
		|| fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->get_param_float("health")) {
			get_activity_group("KO Text").set_active_child("Perfect KO");
		}
		else if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]) {
			get_activity_group("KO Text").set_active_child("Double KO");
		}
		else {
			get_activity_group("KO Text").set_active_child("KO");
		}
		get_activity_group("KO Text").get_active_child().get_texture("Message").set_alpha(255);
		get_activity_group("KO Text").get_active_child().sound_player.play_reserved_sound("KO", 0.0);
	}

	if (execute_if("Remove KO Message", 1, get_activity_group("KO Text")
		.get_active_child().sound_player.is_sound_end())) {

		object_manager->reset_world_rate(nullptr);
		get_activity_group("KO Text").get_active_child().get_texture("Message").set_alpha(0);
		get_activity_group("KO Text").set_active_child("None");
	}

	process_fighters();
	process_ui();
	stage.process();
	post_process_fighters();
	for (int i = 0; i < 2; i++) {
		if (fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] > 0.0) {
			fighter[i]->clear_hurtbox_all();
		}
	}
	thread_manager->wait_thread(THREAD_KIND_UI);
	camera->camera_main();

	int winner = ROUND_WIN_DOUBLE_KO;
	if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
		winner = ROUND_WIN_P1;
	}
	if (fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
		winner--; //If player 1's health is at 0, this will drop our winner from 2 to 1, indicating
		//player 2 winning. If not, it will drop our winner from 0 to -1, indicating a timeout
	}

	switch (winner) {
		case (ROUND_WIN_TIMEOUT): {
			if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]) {
				if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !actionable_timer) {
					if (curr_round == 5) {
						render_manager->start_fade_sequence(40, [&]() {
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
					else if (p1_wins + 1 == round_count_setting
					|| p2_wins + 1 == round_count_setting) {
						if (p1_wins + 1 == round_count_setting
						&& p2_wins + 1 == round_count_setting) {
							p1_round.get_texture("Round Win" + std::to_string(p1_wins + 1)).alpha.set_target_val(255, 20);
							p2_round.get_texture("Round Win" + std::to_string(p2_wins + 1)).alpha.set_target_val(255, 20);
							render_manager->start_fade_sequence(40, [&]() {
								p1_round.int_var("Wins")++;
								p2_round.int_var("Wins")++;
								curr_round = 5;
								internal_state = BATTLE_STATE_ROUND_START;
							});
						}
						else {
							p1_round.get_texture("Round Win" + std::to_string(++p1_wins)).alpha.set_target_val(255, 20);
							p2_round.get_texture("Round Win" + std::to_string(++p2_wins)).alpha.set_target_val(255, 20);
							internal_state = BATTLE_STATE_OUTRO;
						}
					}
					else if (fighter[0]->anim_end || fighter[1]->anim_end || !actionable_timer) {
						p1_round.get_texture("Round Win" + std::to_string(p1_wins + 1)).alpha.set_target_val(255, 20);
						p2_round.get_texture("Round Win" + std::to_string(p2_wins + 1)).alpha.set_target_val(255, 20);
						render_manager->start_fade_sequence(40, [&]() {
							p1_wins++;
							p2_wins++;
							curr_round += 2;
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
				}
				else {
					if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
						&& fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND
						&& fighter[1]->situation_kind == FIGHTER_SITUATION_GROUND) {
						if (ko_timer) {
							ko_timer--;
						}
						else {
							fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[0]->change_anim("round_lose_timeout");
							fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[1]->change_anim("round_lose_timeout");
						}
						return;
					}
					actionable_timer--;
				}
			}
			else if (fighter[0]->fighter_float[FIGHTER_FLOAT_HEALTH] > fighter[1]->fighter_float[FIGHTER_FLOAT_HEALTH]) {
				if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !actionable_timer) {
					if (p1_wins + 1 == round_count_setting) {
						p1_round.get_texture("Round Win" + std::to_string(++p1_wins)).alpha.set_target_val(255, 20);
						internal_state = BATTLE_STATE_OUTRO;
					}
					else if (fighter[0]->anim_end || !actionable_timer) {
						p1_round.get_texture("Round Win" + std::to_string(p1_wins + 1)).alpha.set_target_val(255, 20);
						render_manager->start_fade_sequence(40, [&]() {
							p1_wins++;
							curr_round++;
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
				}
				else {
					if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
						&& fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND
						&& fighter[1]->situation_kind == FIGHTER_SITUATION_GROUND) {
						if (ko_timer) {
							ko_timer--;
						}
						else {
							fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[0]->change_anim("round_win_timeout");
							fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[1]->change_anim("round_lose_timeout");
						}
						return;
					}
					actionable_timer--;
				}
			}
			else {
				if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !actionable_timer) {
					if (p2_wins + 1 == round_count_setting) {
						p2_round.get_texture("Round Win" + std::to_string(++p2_wins)).alpha.set_target_val(255, 20);
						internal_state = BATTLE_STATE_OUTRO;
					}
					else if (fighter[1]->anim_end || !actionable_timer) {
						p2_round.get_texture("Round Win" + std::to_string(p2_wins + 1)).alpha.set_target_val(255, 20);
						render_manager->start_fade_sequence(40, [&]() {
							p2_wins++;
							curr_round++;
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
				}
				else {
					if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
						&& fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND
						&& fighter[1]->situation_kind == FIGHTER_SITUATION_GROUND) {
						if (ko_timer) {
							ko_timer--;
						}
						else {
							fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[1]->change_anim("round_win_timeout");
							fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
							fighter[0]->change_anim("round_lose_timeout");
						}
						return;
					}
					actionable_timer--;
				}
			}
		} break;
		case (ROUND_WIN_DOUBLE_KO): {
			if (fighter[0]->status_kind == FIGHTER_STATUS_KO && fighter[1]->status_kind == FIGHTER_STATUS_KO) {
				if (ko_timer) {
					ko_timer--;
				}
				else {
					if (curr_round == 5) {
						render_manager->start_fade_sequence(40, [&]() {
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
					else if (p1_wins + 1 == round_count_setting
					|| p2_wins + 1 == round_count_setting) {
						if (p1_wins + 1 == round_count_setting
						&& p2_wins + 1 == round_count_setting) {
							p1_round.get_texture("Round Win" + std::to_string(p1_wins + 1)).alpha.set_target_val(255, 20);
							p2_round.get_texture("Round Win" + std::to_string(p2_wins + 1)).alpha.set_target_val(255, 20);
							render_manager->start_fade_sequence(40, [&]() {
								p1_wins++;
								p2_wins++;
								curr_round = 5;
								internal_state = BATTLE_STATE_ROUND_START;
							});
						}
						else {
							p1_round.get_texture("Round Win" + std::to_string(++p1_wins)).alpha.set_target_val(255, 20);
							p2_round.get_texture("Round Win" + std::to_string(++p2_wins)).alpha.set_target_val(255, 20);
							curr_round += 2;
							internal_state = BATTLE_STATE_OUTRO;
						}
					}
					else {
						p1_round.get_texture("Round Win" + std::to_string(++p1_wins)).alpha.set_target_val(255, 20);
						p2_round.get_texture("Round Win" + std::to_string(++p2_wins)).alpha.set_target_val(255, 20);

						render_manager->start_fade_sequence(40, [&]() {
							curr_round += 2;
							internal_state = BATTLE_STATE_ROUND_START;
						});
					}
				}
			}
		} break;
		case (ROUND_WIN_P1): {
			if (fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END || !actionable_timer) {
				if (p1_wins + 1 == round_count_setting) {
					p1_round.get_texture("Round Win" + std::to_string(++p1_wins)).alpha.set_target_val(255, 20);
					internal_state = BATTLE_STATE_OUTRO;
					return;
				}
				else if (fighter[0]->anim_end || !actionable_timer) {
					p1_round.get_texture("Round Win" + std::to_string(p1_wins + 1)).alpha.set_target_val(255, 20);
					render_manager->start_fade_sequence(40, [&]() {
						p1_wins++;
						curr_round++;
						internal_state = BATTLE_STATE_ROUND_START;
					});
				}
			}
			else {
				if (fighter[0]->is_actionable()
					&& fighter[0]->situation_kind == FIGHTER_SITUATION_GROUND) {
					if (ko_timer) {
						ko_timer--;
					}
					else {
						fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
					}
					return;
				}
				actionable_timer--;
			}
		} break;
		case (ROUND_WIN_P2): {
			if (fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END || !actionable_timer) {
				if (p2_wins + 1 == round_count_setting) {
					p2_round.get_texture("Round Win" + std::to_string(++p2_wins)).alpha.set_target_val(255, 20);
					internal_state = BATTLE_STATE_OUTRO;
					return;
				}
				else if (fighter[1]->anim_end || !actionable_timer) {
					p2_round.get_texture("Round Win" + std::to_string(p2_wins + 1)).alpha.set_target_val(255, 20);
					render_manager->start_fade_sequence(40, [&]() {
						p2_wins++;
						curr_round++;
						internal_state = BATTLE_STATE_ROUND_START;
					});
				}
			}
			else {
				if (fighter[1]->is_actionable()
					&& fighter[1]->situation_kind == FIGHTER_SITUATION_GROUND) {
					if (ko_timer) {
						ko_timer--;
					}
					else {
						fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
					}
					return;
				}
				actionable_timer--;
			}
		} break;
	}
}

void Battle::process_outro() {
	GameManager::get_instance()->update_state(GAME_STATE_DEBUG_MENU);
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

void Battle::process_fighters() {	
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
		case UI_TEXT_TYPE_PUNISH: {
			texts[i].push_back(BattleText());
			texts[i].back().init(&message_font, "Punish", 40, fighter[i], glm::vec2(275.0, 450.0));
		} break;
		case UI_TEXT_TYPE_CRITICAL: {
			texts[i].push_back(BattleText());
			texts[i].back().init(&message_font, "Critical Hit", 40, fighter[i], glm::vec2(275.0, 450.0));
		} break;
		case UI_TEXT_TYPE_REVERSAL: {
			texts[i].push_back(BattleText());
			texts[i].back().init(&message_font, "Reversal", 40, fighter[i], glm::vec2(275.0, 450.0));
		} break;
		}
		fighter[i]->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_NONE;
	}
	if (!(fighter[0]->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION]
		|| fighter[1]->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION])) {
		for (int i = 0; i < 2; i++) {
			if (fighter[i]->situation_kind == FIGHTER_SITUATION_GROUND) {
				if (fighter[i]->pos.x > fighter[!i]->pos.x) {
					fighter[i]->internal_facing_right = false;
					fighter[i]->internal_facing_dir = -1.0;
				}
				else if (fighter[i]->pos.x < fighter[!i]->pos.x) {
					fighter[i]->internal_facing_right = true;
					fighter[i]->internal_facing_dir = 1.0;
				}
				else { //If both players are stuck inside each other, stop that !
					if (fighter[!i]->situation_kind == FIGHTER_SITUATION_GROUND) {
						if (fighter[i]->pos.x == fighter[!i]->pos.x
							&& fighter[i]->internal_facing_dir == fighter[!i]->internal_facing_dir) {
							fighter[i]->internal_facing_right = true;
							fighter[i]->internal_facing_dir = 1.0;
							fighter[!i]->internal_facing_right = false;
							fighter[!i]->internal_facing_dir = -1.0;
						}
						fighter[i]->add_pos(glm::vec3(-10.0 * fighter[i]->internal_facing_dir, 0, 0));
						fighter[!i]->add_pos(glm::vec3(-10.0 * fighter[!i]->internal_facing_dir, 0, 0));
					}
				}
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		thread_manager->notify_thread(i);
	}
}

void gamestate_battle_fighter_thread(void* fighter_arg) {
	Fighter* fighter = (Fighter*)fighter_arg;
	fighter->fighter_main();
}

void Battle::post_process_fighters() {
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
			fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH], fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH] + 10.0, fighter[i]->get_param_float("health"));
			fighter[i]->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter[i]->fighter_float[FIGHTER_FLOAT_HEALTH];
		}
		if (fighter[i]->fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] == 0) {
			fighter[i]->gain_ex(1.0);
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
		else {
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
	battle->main_object.event_process();
	for (size_t i = 0, max = battle->menu_objects.size(); i < max; i++) {
		battle->menu_objects[i].event_process();
	}
}

void Battle::render_main() {
	render_world();
	if (internal_state != BATTLE_STATE_PRE_INTRO && internal_state != BATTLE_STATE_INTRO) {
		render_ui();
	}
}

void Battle::render_world() {
	RenderManager* render_manager = RenderManager::get_instance();
	render_manager->execute_buffered_events();

	//Uncomment the following line if the Rowan model ever becomes capable of rendering 
	//his sleeves with culling enabled
//	glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);

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

	if (SaveManager::get_instance()->get_game_setting("visualize_boxes") == 1) {
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
	main_object.render();
	for (int i = 0; i < 2; i++) {
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
	//TRAINING PASS

	if (game_context == GAME_CONTEXT_TRAINING) {
		for (int i = 0; i < 2; i++) {
			training_info[i].render();
		}
	}
}

void Battle::event_start_press() {
	switch (internal_state) {
		case(BATTLE_STATE_PRE_INTRO): {

		} break;
		case (BATTLE_STATE_INTRO): {
			for (int i = 0; i < 2; i++) {
				fighter[i]->change_status(FIGHTER_STATUS_NONE, false);
			}
			internal_state = BATTLE_STATE_ROUND_START;
		} break;
		case (BATTLE_STATE_ROUND_START): {

		} break;
		default:
		case (BATTLE_STATE_BATTLE): {
			if (game_context != GAME_CONTEXT_ONLINE) {
				GameManager::get_instance()->game_main[GAME_STATE_PAUSE_BATTLE]();
			}
		} break;
		case (BATTLE_STATE_KO): {
			if (fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END 
			|| fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) {
				actionable_timer = 0;
			}
		} break;
		case (BATTLE_STATE_OUTRO): {

		} break;
	}

}

void Battle::event_frame_pause_press() {
	if (game_context == GAME_CONTEXT_TRAINING && internal_state == BATTLE_STATE_BATTLE) {
		SoundManager* sound_manager = SoundManager::get_instance();
		if (frame_pause) {
			sound_manager->resume_all_sounds();
			sound_manager->resume_all_reserved_sounds();
		}
		else {
			sound_manager->pause_all_sounds();
			sound_manager->pause_all_reserved_sounds();
		}
		bool& timer_pause = get_child("Timer").bool_var("pause");
		timer_pause = !timer_pause;
		frame_pause = !frame_pause;
	}
}

void Battle::event_frame_advance_press() {
	if (frame_pause) {
		SoundManager* sound_manager = SoundManager::get_instance();
		sound_manager->resume_all_sounds();
		sound_manager->resume_all_reserved_sounds();
		frame_advance = true;
	}
}

void Battle::event_record_input_press() {
	if (game_context == GAME_CONTEXT_TRAINING) {
		switch (player[1]->input_mode) {
			case (INPUT_MODE_POLL):
			case (INPUT_MODE_PLAY_SEQ): {
				player[0]->controller.swap_player_controller(&player[1]->controller);
				player[1]->manual_seq.reset_idx();
				player[1]->input_mode = INPUT_MODE_RECORD_SEQ;
			} break;
			case (INPUT_MODE_RECORD_SEQ): {
				player[0]->controller.reset_player_controller();
				player[1]->controller.reset_player_controller();
				player[1]->manual_seq.reset_idx();
				player[1]->input_mode = INPUT_MODE_POLL;
			} break;
			case (INPUT_MODE_REPLAY):
			case (INPUT_MODE_ATLAS_REWIND):
			case (INPUT_MODE_ROLLBACK): {
				return;
			} break;
		}
	}
}

void Battle::event_replay_input_press() {
	if (game_context == GAME_CONTEXT_TRAINING) {
		switch (player[1]->input_mode) {
			case (INPUT_MODE_PLAY_SEQ): {
				player[1]->manual_seq.reset_idx();
				player[1]->input_mode = INPUT_MODE_POLL;
			} break;
			case (INPUT_MODE_POLL):
			case (INPUT_MODE_RECORD_SEQ): {
				player[0]->controller.reset_player_controller();
				player[1]->controller.reset_player_controller();
				player[1]->manual_seq.reset_idx();
				player[1]->input_mode = INPUT_MODE_PLAY_SEQ;
			} break;
			case (INPUT_MODE_REPLAY):
			case (INPUT_MODE_ATLAS_REWIND):
			case (INPUT_MODE_ROLLBACK): {
				return;
			} break;
		}
	}
}

void Battle::event_switch_input_press() {
	if (game_context == GAME_CONTEXT_TRAINING) {
		player[0]->controller.swap_player_controller(&player[1]->controller);
	}
}