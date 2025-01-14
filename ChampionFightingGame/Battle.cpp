#include "Battle.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <glew/glew.h>

#include "BattleObject.h"
#include "Fighter.h"
#include "FighterInterface.h"
#include "Projectile.h"
#include "Projectiles.h"
#include "Stage.h"

#include "GameManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "ObjectManager.h"
#include "ParamAccessor.h"
#include "WindowManager.h"
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
	WindowManager* window_manager = WindowManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	font_manager->load_face("Fiend-Oblique");

	Battle *battle = new Battle;
	
	window_manager->update_shader_cams();
	window_manager->update_shader_lights();

#ifdef DEBUG
	cotr_imgui_init();
#endif
	while (battle->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();

		battle->process();
		battle->render();

#ifdef DEBUG
		cotr_imgui_debug_battle(battle);
#endif

		window_manager->update_screen();
		battle->process_cpus();
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

	switch (context) {
		case SCENE_CONTEXT_NONE:
		case SCENE_CONTEXT_SPECIAL: {
			timer_setting = TIMER_SETTING_NORMAL;
		} break;
		case SCENE_CONTEXT_TRAINING: {
			timer_setting = TIMER_SETTING_TRAINING;
			player[1]->player_kind = PLAYER_KIND_DUMMY;
		} break;
		case SCENE_CONTEXT_ARCADE: {
			timer_setting = TIMER_SETTING_NORMAL;
			player[1]->player_kind = PLAYER_KIND_CPU;
		} break;
		case SCENE_CONTEXT_STORY: {
			timer_setting = TIMER_SETTING_NONE;
			player[1]->player_kind = PLAYER_KIND_CPU;
		} break;
		case SCENE_CONTEXT_ONLINE: {
			timer_setting = TIMER_SETTING_NORMAL;
			player[1]->player_kind = PLAYER_KIND_PLAYER;
		} break;
	}

	load_world();
	load_ui();

	switch (context) {
		case SCENE_CONTEXT_NONE:
		case SCENE_CONTEXT_ONLINE:
		case SCENE_CONTEXT_SPECIAL: {
			set_active_element(&get_element("root/Pre Intro Sequence"));
			//Pre-Intro
		} break;
		case SCENE_CONTEXT_TRAINING:
		case SCENE_CONTEXT_STORY: {
			set_active_element(&get_element("root/Battle Sequence"));
		} break;
		case SCENE_CONTEXT_ARCADE: {
			set_active_element(&get_element("root/Intro Sequence"));
		} break;
	}

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
	WindowManager* window_manager = WindowManager::get_instance();
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
	window_manager->remove_lights();

	thread_manager->kill_thread(THREAD_KIND_UI);
	camera->stage = nullptr;
	camera->unload_camera_anims();

	stage.unload_stage();
}

void Battle::load_world() {
	WindowManager* window_manager = WindowManager::get_instance();

	int rng = rand() % 2;
	stage.load_stage(player[rng]->stage_info, object_manager);

	for (int i = 0; i < 2; i++) {
		fighter[i] = create_fighter(player[i]);
	}
	fighter[1]->object_int[FIGHTER_INT_AUTO_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;

	camera = &window_manager->camera;
	camera->reset_camera();
	for (int i = 0; i < 2; i++) {
		camera->fighter[i] = fighter[i];
	}
	camera->stage = &stage;

	window_manager->update_shader_cams();
	window_manager->update_shader_lights();
}

void Battle::load_ui() {
	FontManager* font_manager = FontManager::get_instance();
	load_font("combo", "Fiend-Oblique", 256);
	load_font("message", "Fiend-Oblique", 96);
	load_font("info", "FiraCode", 64);

	const int scene_context = context;

	load_event("Process Health Bar", [this, scene_context](SceneElement* e) {
		Fighter* fighter = (Fighter*)e->ptr_var("fighter");
		float max_health = e->float_var("max_health");
		float &health = fighter->object_float[FIGHTER_FLOAT_HEALTH];
		float &partial_health = fighter->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
		bool ended_hitstun = fighter->object_flag[FIGHTER_FLAG_ENDED_HITSTUN];
		if (partial_health < health) {
			e->get_screen_texture("Combo Health").scale_left_edge(partial_health / max_health);
		}
		if (ended_hitstun) {
			e->get_screen_texture("Combo Health").scale_left_edge(clampf(0.0, health / max_health, 1.0), 5);
		}
		if (scene_context == SCENE_CONTEXT_TRAINING) {
			if (fighter->object_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] == 0) {
				e->get_screen_texture("Combo Health").scale_left_edge(clampf(0.0, health / max_health, 1.0));
				health = std::min(health + 10.0f, max_health);
				partial_health = health;
			}
		}
		e->get_screen_texture("Health").scale_left_edge(health / max_health);
		e->get_screen_texture("Partial Health").scale_left_edge(partial_health / max_health);
	});
	load_event("Process Scale Bar", [this](SceneElement* e) {
		float scale = *(float*)e->ptr_var("damage_scale");
		e->get_screen_texture("Damage Scale 120").set_alpha(255 * (scale >= 1.2f));
		e->get_screen_texture("Damage Scale 110").set_alpha(255 * (scale >= 1.1f));
		e->get_screen_texture("Damage Scale").scale_top_left_corner(clampf(0.0f, scale + (1.0f - scale) / 10.0f - 0.002f, 1.0f), 3);
		e->get_screen_texture("Damage Scale").scale_bottom_left_corner(clampf(0.0f, scale + (1.0f - scale) / 10.0f - 0.052f, 1.0f), 3);
	});
	load_event("Process EX Bar", [this](SceneElement* e) {
		float max_ex = e->float_var("max_ex");
		float ex = *(float*)e->ptr_var("ex");
		float& prev_segments = e->float_var("prev_segments");
		e->get_screen_texture("EX").scale_left_edge(ex / max_ex, 6);
		float segments = std::floor(ex / 100.0);
		if (prev_segments != segments) {
			e->get_screen_texture("EX Segment").scale_left_edge(clampf(0.0, segments / 6.0, 1.0), 4);
		}
		prev_segments = segments;
	});
	load_event("Process Super Bar", [this](SceneElement* e) {
		float super = *(float*)e->ptr_var("super");
		float max_super = e->float_var("max_super");
		if (super == max_super) {
			e->get_screen_texture("Super").set_alpha(0);
			e->get_screen_texture("Super Full").set_alpha(255);
			e->get_screen_texture("Super Full").next_sprite();
		}
		else {
			e->get_screen_texture("Super").set_alpha(255);
			e->get_screen_texture("Super").set_sprite((int)((super / max_super) * 100.0));
			e->get_screen_texture("Super Full").set_alpha(0);
			e->get_screen_texture("Super Full").set_sprite(0);
		}
	});
	load_event("Process Combo Counter", [this](SceneElement* e) {
		int& timer = e->int_var("post_combo_fadeout");
		Fighter* fighter = (Fighter*)e->ptr_var("fighter");
		Fighter* opponent = (Fighter*)e->ptr_var("opponent");
		e->get_screen_text("Counter").set_scale(1.0f);
		e->get_screen_text("Hits").set_scale(1.0f);
		if (opponent->object_flag[FIGHTER_FLAG_ENDED_HITSTUN]) {
			timer = 5;
		}
		else if (!(opponent->object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN)) {
			switch (timer) {
				case 0: {
					e->get_screen_text("Counter").set_alpha(0, 20);
					e->get_screen_text("Hits").set_alpha(0, 20);
				} break;
				case -1: {

				} break;
				default: {
					timer--;
				} break;
			}
		}
	});
	load_event("Combo Counter On Hit", [this](SceneElement* e) {
		int& timer = e->int_var("post_combo_fadeout");
		Fighter* fighter = (Fighter*)e->ptr_var("fighter");
		Fighter* opponent = (Fighter*)e->ptr_var("opponent");
		if (fighter->object_int[FIGHTER_INT_COMBO_COUNT] != 1) {
			e->get_screen_text("Counter")
				.update_text(std::to_string(fighter->object_int[FIGHTER_INT_COMBO_COUNT]))
				.set_scale(1.1f)
				.set_alpha(255);
			e->get_screen_text("Hits")
				.set_scale(1.1f)
				.set_alpha(255);
		}
		else if (timer != -1) {
			e->get_screen_text("Counter").set_alpha(0, opponent->object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			e->get_screen_text("Hits").set_alpha(0, opponent->object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
		}
		timer = -1;
	});

	root.add_elements({
		{"Battle UI", 
			SceneElement({
				{"P1 Health",
					SceneElement({
						{"Combo Health", ScreenTexture("resource/scene/battle/ui/meter/combo_health.png")},
						{"Health", ScreenTexture("resource/scene/battle/ui/meter/health.png")
							.set_alpha(127)
						},
						{"Partial Health", ScreenTexture("resource/scene/battle/ui/meter/health.png")}
					})
					.set_orientation(TEXTURE_TOP_LEFT)
					.set_pos(glm::vec3(66.0, 114.0, 0.0))
					.float_var("max_health", fighter[0]->get_param_float("health"))
					.ptr_var("fighter", fighter[0])
					.add_event("process", get_event("Process Health Bar"))
				},
				{"P2 Health",
					SceneElement({
						{"Combo Health", ScreenTexture("resource/scene/battle/ui/meter/combo_health.png")
							.flip_h()
						},
						{"Health", ScreenTexture("resource/scene/battle/ui/meter/health.png")
							.set_alpha(127)
							.flip_h()
						},
						{"Partial Health", ScreenTexture("resource/scene/battle/ui/meter/health.png")
							.flip_h()
						}
					})
					.set_orientation(TEXTURE_TOP_RIGHT)
					.set_pos(glm::vec3(66.0, 114.0, 0.0))
					.float_var("max_health", fighter[1]->get_param_float("health"))
					.ptr_var("fighter", fighter[1])
					.add_event("process", get_event("Process Health Bar"))
				},
				{"P1 Scale",
					SceneElement({
						{"Damage Scale", ScreenTexture("resource/scene/battle/ui/meter/damage_scale.png", TEX_FEAT_4T5V)},
						{"Damage Scale 110", ScreenTexture("resource/scene/battle/ui/meter/damage_scale_110.png")
							.set_alpha(0)
						},
						{"Damage Scale 120", ScreenTexture("resource/scene/battle/ui/meter/damage_scale_120.png")
							.set_alpha(0)
						}
					})
					.ptr_var("damage_scale", &fighter[0]->object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI])
					.set_orientation(TEXTURE_TOP_LEFT)
					.set_pos(glm::vec3(1026.0f, 244.5f, 0.0f))
					.add_event("process", get_event("Process Scale Bar"))
				},
				{"P2 Scale",
					SceneElement({
						{"Damage Scale", ScreenTexture("resource/scene/battle/ui/meter/damage_scale.png", TEX_FEAT_4T5V)
							.flip_h()
						},
						{"Damage Scale 110", ScreenTexture("resource/scene/battle/ui/meter/damage_scale_110.png")
							.set_alpha(0)
							.flip_h()
						},
						{"Damage Scale 120", ScreenTexture("resource/scene/battle/ui/meter/damage_scale_120.png")
							.set_alpha(0)
							.flip_h()
						}
					})
					.ptr_var("damage_scale", &fighter[1]->object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI])
					.set_orientation(TEXTURE_TOP_RIGHT)
					.set_pos(glm::vec3(1026.0f, 244.5f, 0.0f))
					.add_event("process", get_event("Process Scale Bar"))
				},
				{"P1 EX",
					SceneElement({
						{"EX", ScreenTexture("resource/scene/battle/ui/meter/ex.png")
							.scale_left_edge(0.0f)
						},
						{"EX Segment", ScreenTexture("resource/scene/battle/ui/meter/ex_segment.png")
							.scale_left_edge(0.0f)
						}
					})
					.float_var("max_ex", get_global_param_float(PARAM_FIGHTER, "ex_meter_size"))
					.ptr_var("ex", &fighter[0]->object_float[FIGHTER_FLOAT_EX_METER])
					.float_var("prev_segments", 0.0f)
					.set_orientation(TEXTURE_BOTTOM_LEFT)
					.set_pos(glm::vec3(324.0, 180.0, 0.0))
					.add_event("process", get_event("Process EX Bar"))
				},
				{"P2 EX",
					SceneElement({
						{"EX", ScreenTexture("resource/scene/battle/ui/meter/ex.png")
							.scale_left_edge(0.0f)
							.flip_h()
						},
						{"EX Segment", ScreenTexture("resource/scene/battle/ui/meter/ex_segment.png")
							.scale_left_edge(0.0f)
							.flip_h()
						}
					})
					.float_var("max_ex", get_global_param_float(PARAM_FIGHTER, "ex_meter_size"))
					.ptr_var("ex", &fighter[1]->object_float[FIGHTER_FLOAT_EX_METER])
					.float_var("prev_segments", 0.0f)
					.set_orientation(TEXTURE_BOTTOM_RIGHT)
					.set_pos(glm::vec3(324.0, 180.0, 0.0))
					.add_event("process", get_event("Process EX Bar"))
				},
				{"P1 Super",
					SceneElement({
						{"Super", ScreenTexture("resource/scene/battle/ui/meter/super.gif")},
						{"Super Full", ScreenTexture("resource/scene/battle/ui/meter/super_full.gif")}
					})
					.ptr_var("super", &fighter[0]->object_float[FIGHTER_FLOAT_SUPER_METER])
					.float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"))
					.set_orientation(TEXTURE_BOTTOM_LEFT)
					.set_pos(glm::vec3(66.0f, 70.0f, 0.0f))
					.add_event("process", get_event("Process Super Bar"))
				},
				{"P2 Super",
					SceneElement({
						{"Super", ScreenTexture("resource/scene/battle/ui/meter/super.gif")
							.flip_h()
						},
						{"Super Full", ScreenTexture("resource/scene/battle/ui/meter/super_full.gif")
							.flip_h()
						}
					})
					.ptr_var("super", &fighter[1]->object_float[FIGHTER_FLOAT_SUPER_METER])
					.float_var("max_super", get_global_param_float(PARAM_FIGHTER, "super_meter_size"))
					.set_orientation(TEXTURE_BOTTOM_RIGHT)
					.set_pos(glm::vec3(66.0f, 70.0f, 0.0f))
					.add_event("process", get_event("Process Super Bar"))
				},
				{"P1 Combo Counter", 
					SceneElement({
						{"Counter", ScreenText(&get_font("message"), "1", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))
							.set_pos(glm::vec3(275.0f, 500.0f, 0.0f))
							.set_orientation(TEXTURE_TOP_LEFT)
							.set_alpha(0)
						},
						{"Hits", ScreenText(&get_font("message"), "hits", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))
							.set_pos(glm::vec3(275.0f, 800.0f, 0.0f))
							.set_orientation(TEXTURE_TOP_LEFT)
							.set_alpha(0)
						}
					})
					.ptr_var("fighter", fighter[0])
					.ptr_var("opponent", fighter[1])
					.int_var("post_combo_fadeout", -1)
					.add_event("process", get_event("Process Combo Counter"))
					.add_event("on_hit", get_event("Combo Counter On Hit"))
				},
				{ "P2 Combo Counter",
					SceneElement({
						{"Counter", ScreenText(&get_font("message"), "1", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))
							.set_pos(glm::vec3(275.0f, 500.0f, 0.0f))
							.set_orientation(TEXTURE_TOP_RIGHT)
							.set_alpha(0)
						},
						{"Hits", ScreenText(&get_font("message"), "hits", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))
							.set_pos(glm::vec3(275.0f, 800.0f, 0.0f))
							.set_orientation(TEXTURE_TOP_RIGHT)
							.set_alpha(0)
						}
					})
					.ptr_var("fighter", fighter[1])
					.ptr_var("opponent", fighter[0])
					.int_var("post_combo_fadeout", -1)
					.add_event("process", get_event("Process Combo Counter"))
					.add_event("on_hit", get_event("Combo Counter On Hit"))
				},
				{ "Round Counters", 
					SceneElementLoop(2, [this](SceneElement* e, int i) {
						if (context == SCENE_CONTEXT_TRAINING) return;
						std::string elem_name = "Round Counter P" + std::to_string(i + 1);
						std::string lr = i ? "r" : "l";
						float mul = i ? -1.0f : 1.0f;
						int orientation = i ? TEXTURE_TOP_RIGHT : TEXTURE_TOP_LEFT;
						switch (round_count_setting) {
							case 1: {
								e->add_element(elem_name, 
									SceneElement({
										{"Round Win1", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo1_" + lr + ".gif")
											.set_pos(glm::vec3(6.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Bar", ScreenTexture("resource/scene/battle/ui/rounds/round_bar_bo1.png")
											.set_h_flipped(i)
										}
									})
									.set_pos(glm::vec3(1002.0, 62.0, 0.0))
									.set_orientation(orientation)
									.int_var("Wins", 0)
								);
							} break;
							case 2: {
								e->add_element(elem_name,
									SceneElement({
										{"Round Win1", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo3_" + lr + ".gif")
											.set_pos(glm::vec3(204.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Win2", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo3_" + lr + ".gif")
											.set_pos(glm::vec3(-192.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Bar", ScreenTexture("resource/scene/battle/ui/rounds/round_bar_bo3.png")
											.set_h_flipped(i)
										}
									})
									.set_pos(glm::vec3(1002.0, 62.0, 0.0))
									.set_orientation(orientation)
									.int_var("Wins", 0)
								);
							} break;
							case 3: {
								e->add_element(elem_name,
									SceneElement({
										{"Round Win1", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo5_" + lr + ".gif")
											.set_pos(glm::vec3(270.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Win2", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo5_" + lr + ".gif")
											.set_pos(glm::vec3(6.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Win3", ScreenTexture("resource/scene/battle/ui/rounds/round_win_bo5_" + lr + ".gif")
											.set_pos(glm::vec3(-258.0f * mul, -5.0f, 0.0f))
											.set_alpha(0)
										},
										{"Round Bar", ScreenTexture("resource/scene/battle/ui/rounds/round_bar_bo5.png")
											.set_h_flipped(i)
										}
									})
									.set_pos(glm::vec3(1002.0, 62.0, 0.0))
									.set_orientation(orientation)
									.int_var("Wins", 0)
								);
							} break;
						}
					}) 
				},
				{"Timer",
					SceneElementLoop(1, [this](SceneElement* e, int i) {
						switch (timer_setting) {
							case TIMER_SETTING_NORMAL: {
								e->add_element("Timer", 
									SceneElement({
										{"Background", ScreenTexture("resource/scene/battle/ui/timer/timer.png")},
										{"Deca Second", ScreenTexture("resource/scene/battle/ui/timer/sec.gif")
											.set_pos(glm::vec3(-38.0f, 19.0f, 0.0f))
											.set_sprite(9)
										},
										{"Second", ScreenTexture("resource/scene/battle/ui/timer/sec.gif")
											.set_pos(glm::vec3(38.0f, 19.0f, 0.0f))
											.set_sprite(9)
										},
										{"Deca Frame", ScreenTexture("resource/scene/battle/ui/timer/frame.gif")
											.set_pos(glm::vec3(-18.0f, -84.0f, 0.0f))
											.set_sprite(5)
										},
										{"Frame", ScreenTexture("resource/scene/battle/ui/timer/frame.gif")
											.set_pos(glm::vec3(18.0f, -84.0f, 0.0f))
											.set_sprite(9)
										},
									})
									.bool_var("pause", false)
									.bool_var("time_up", false)
									.int_var("deca_sec", 9)
									.int_var("sec", 9)
									.int_var("deca_frame", 5)
									.int_var("frame", 9)
									.set_orientation(TEXTURE_TOP)
									.set_pos(glm::vec3(0.0f, 74.0f, 0.0f))
									.add_event("process", [this](SceneElement* e) {
										if (e->bool_var("pause") || e->bool_var("time_up")) return;
										int& frame = e->int_var("frame");
										if (frame == 0) {
											frame = 9;
											int& deca_frame = e->int_var("deca_frame");
											if (deca_frame == 0) {
												deca_frame = 5;
												e->get_screen_texture("Deca Frame").set_sprite(6);
												int& sec = e->int_var("sec");
												if (sec == 0) {
													sec = 9;
													int& deca_sec = e->int_var("deca_sec");
													if (deca_sec == 0) {
														e->bool_var("time_up") = true;
													}
													else {
														deca_sec--;
													}
													e->get_screen_texture("Deca Second").prev_sprite();
												}
												else {
													sec--;
												}
												e->get_screen_texture("Second").prev_sprite();
											}
											else {
												deca_frame--;
											}
											e->get_screen_texture("Deca Frame").prev_sprite();
										}
										else {
											frame--;
										}
										e->get_screen_texture("Frame").prev_sprite();
									})
									.add_event("reset", [this](SceneElement* e) {
										e->bool_var("time_up") = false;
										e->int_var("deca_sec") = 9;
										e->get_screen_texture("Deca Sec").set_sprite(9);
										e->int_var("sec") = 9;
										e->get_screen_texture("Sec").set_sprite(9);
										e->int_var("deca_frame") = 5;
										e->get_screen_texture("Deca Frame").set_sprite(5);
										e->int_var("frame") = 9;
										e->get_screen_texture("Frame").set_sprite(9);
									})
								);
							} break;
							case TIMER_SETTING_TRAINING: {

							} break;
							case TIMER_SETTING_NONE: {

							} break;
						}
					})
				},
				{"HUD", ScreenTexture("resource/scene/battle/ui/battle_hud.png")}
			})
			.add_event("process", [this](SceneElement* e) {
				for (size_t i = 0; i < e->get_num_children(); i++) {
					e->get_child(i).execute_event("process");
				}
			})
		}, //UI
		{"Pre Intro Sequence", 
			SceneElement()
			.add_event("activate", [this](SceneElement* e) {
				get_element("root/Battle UI").hide();
			})
			.add_event("process", [this](SceneElement* e) {
				set_active_element(&get_element("root/Intro Sequence"));
				for (int i = 0; i < 2; i++) {
					fighter[i]->change_anim("intro");
					fighter[i]->set_rot(glm::vec3(0.0, 0.0, 90.0));
				}
			})
		}, //Pre Intro
		{"Intro Sequence",
			SceneElement()
			.add_event("activate", [this](SceneElement* e) {
				get_element("root/Battle UI").hide();
			})
			.add_event("process", [this](SceneElement* e) {
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
					e->execute_event("start_press");
				}
			})
			.add_event("start_press", [this](SceneElement* e) {
				for (int i = 0; i < 2; i++) {
					fighter[i]->change_status(BATTLE_OBJECT_STATUS_NONE, false);
				}
				set_active_element(&get_element("root/Round Start Sequence"));
			})
		}, //Intro
		{"Round Start Sequence",
			SceneElement({
				{"Text", ScreenText(&get_font("combo"), "Round 1", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))}
			})
			.hide()
			.int_var("fight_counter", -1)
			.add_event("activate", [this](SceneElement* e) {
				for (int i = 0; i < 2; i++) {
					fighter[i]->object_flag[FIGHTER_FLAG_ROUND_START] = true;
					fighter[i]->reset();
				}
				if (timer_setting == TIMER_SETTING_NORMAL) {
					get_element("root/Battle UI/Timer").execute_event("reset");
					get_element("root/Battle UI/Timer").bool_var("pause") = true;
				}
				e->int_var("fight_counter") = -1;
				std::string display_text = "";
				switch (curr_round) {
					case 1: {
						if (round_count_setting == 1) {
							display_text = "Ready...";
						}
						else {
							display_text = "Round 1";
						}
					} break;
					case 2: {
						display_text = "Round 2";
					} break;
					case 3: {
						if (round_count_setting == 2) {
							display_text = "Final Round";
						}
						else {
							display_text = "Round 3";
						}
					} break;
					case 4: {
						display_text = "Round 4";
					} break;
					case 5: {
						display_text = "Final Round";
					} break;
					case 6: {
						display_text = "Sudden Death";
					} break;
				}
				e->get_screen_text("Text").update_text(display_text);
				e->show();
				e->sound_player.play_reserved_sound(display_text, 0.0f);
			})
			.add_event("process", [this](SceneElement* e) {
				SoundManager* sound_manager = SoundManager::get_instance();
				for (int i = 0; i < 2; i++) {
					player[i]->poll_controller_fighter();
				}
				process_fighters();
				process_ui();
				stage.process();
				post_process_fighters();
				thread_manager->wait_thread(THREAD_KIND_UI);
				camera->camera_main();
				switch (e->int_var("fight_counter")) {
					case -1: {
						if (e->sound_player.is_sound_end()) {
							e->int_var("fight_counter") = 60;
						}
					} break;
					case 0: {
						set_active_element(&get_element("root/Battle Sequence"));
						e->hide();
					} break;
					default: {
						e->int_var("fight_counter")--;
					} break;
				}
			})
			.load_sound("Ready...", "resource/sound/ui/ko.wav")
			.load_sound("Round 1", "resource/sound/ui/ko.wav")
			.load_sound("Round 2", "resource/sound/ui/ko.wav")
			.load_sound("Round 3", "resource/sound/ui/ko.wav")
			.load_sound("Round 4", "resource/sound/ui/ko.wav")
			.load_sound("Final Round", "resource/sound/ui/ko.wav")
			.load_sound("Sudden Death", "resource/sound/ui/ko.wav")
			.load_sound("Fight", "resource/sound/ui/ko.wav")
		}, //Round Start
		{ "Battle Sequence",
			SceneElement()
			.bool_var("frame_advance", false)
			.bool_var("frame_pause", false)
			.add_event("activate", [this](SceneElement* e) {
				get_element("root/Battle UI").show();
				if (timer_setting == TIMER_SETTING_NORMAL) {
					get_element("root/Battle UI/Timer").bool_var("pause") = false;
				}
				for (int i = 0; i < 2; i++) {
					fighter[i]->object_flag[FIGHTER_FLAG_ROUND_START] = false;
				}
			})
			.add_event("process", [this](SceneElement* e) {
				GameManager* game_manager = GameManager::get_instance();
				SoundManager* sound_manager = SoundManager::get_instance();
				process_debug_boxes();
				if (e->bool_var("frame_advance") || !e->bool_var("frame_pause")) {
					for (int i = 0; i < 2; i++) {
						player[i]->poll_controller_fighter();
					}
					process_fighters();
					process_ui();
					stage.process();
					post_process_fighters();
					thread_manager->wait_thread(THREAD_KIND_UI);
					camera->camera_main();
					if (context == SCENE_CONTEXT_TRAINING) {
						process_training();
					}
				}
				if (timer_setting == TIMER_SETTING_NORMAL) {
					if (get_element("root/Battle UI/Timer").bool_var("time_up")) {
						set_active_element(&get_element("root/KO Sequence/KO Text"));
					}
				}
			})
			.add_event("start_press", [this](SceneElement* e) {
				if (context != SCENE_CONTEXT_ONLINE) {
					GameManager::get_instance()->scene_main[SCENE_PAUSE_BATTLE]();
					for (int i = 0; i < 2; i++) {
						player[i]->poll_controller_fighter();
						player[i]->controller.reset_buffer();
					}
				}
			})
		}, //Battle
		{"KO Sequence", 
			SceneElement({
				{"KO Text", 
					SceneElement({
						{"Message", ScreenText(&get_font("combo"), "", TextSpecifier().color(glm::vec3(255.0f, 127.0f, 0.0f)).border(2))}
					})
					.hide()
					.string_var("KO Sound", "")
					.load_sound("KO", "resource/sound/ui/ko.wav")
					.load_sound("Perfect KO", "resource/sound/ui/ko.wav")
					.load_sound("Time", "resource/sound/ui/ko.wav")
					.load_sound("Double KO", "resource/sound/ui/ko.wav")
					.add_event("activate", [this](SceneElement* e) {
						e->show();

						SceneElement* round[2];
						round[0] = &get_element("root/Battle UI/Round Counter P1");
						round[1] = &get_element("root/Battle UI/Round Counter P2");
						int* wins[2];
						wins[0] = &round[0]->int_var("Wins");
						wins[1] = &round[1]->int_var("Wins");

						if (object_manager->real_time_object == nullptr) {
							object_manager->set_world_rate(nullptr, 0.0067);
						}
						for (int i = 0; i < 2; i++) {
							player[i]->controller.reset_all_buttons();
							fighter[i]->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
						}
						get_element("root/Battle UI/Timer").bool_var("pause") = true;

						if (curr_round != 5) {
							if (get_element("root/Battle UI/Timer").bool_var("time_up")) {
								round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_sprite(ROUND_ICON_TIMEOUT);
								round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_sprite(ROUND_ICON_TIMEOUT);
								e->string_var("KO Sound") = "Time";
								e->get_screen_text("Message").update_text("Time");
							}
							else if (fighter[0]->object_float[FIGHTER_FLOAT_HEALTH] == 0.0f && fighter[1]->object_float[FIGHTER_FLOAT_HEALTH] == 0.0f) {
								round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_sprite(ROUND_ICON_DOUBLE);
								round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_sprite(ROUND_ICON_DOUBLE);
								e->string_var("KO Sound") = "Double KO";
								e->get_screen_text("Message").update_text("Double KO");
							}
							else {
								for (int i = 0; i < 2; i++) {
									if (fighter[i]->object_float[FIGHTER_FLOAT_HEALTH] == 0.0f) continue;
									if (fighter[i]->object_float[FIGHTER_FLOAT_HEALTH] == fighter[i]->get_param_float("health")) {
										round[i]->get_screen_texture(
											"Round Win" + std::to_string(*wins[i] + 1)
										).set_sprite(ROUND_ICON_PERFECT);
										e->string_var("KO Sound") = "Perfect KO";
										e->get_screen_text("Message").update_text("Perfect KO");
										break;
									}
									e->string_var("KO Sound") = "KO";
									e->get_screen_text("Message").update_text("KO");
									if (fighter[!i]->object_flag[FIGHTER_FLAG_HIT_BY_EX_SUPER]) {
										round[i]->get_screen_texture(
											"Round Win" + std::to_string(*wins[i] + 1)
										).set_sprite(ROUND_ICON_EX_SUPER);
										break;
									}
									if (fighter[1]->object_flag[FIGHTER_FLAG_HIT_BY_CHAMPION_SUPER]) {
										round[i]->get_screen_texture(
											"Round Win" + std::to_string(*wins[i] + 1)
										).set_sprite(ROUND_ICON_CHAMPION_SUPER);
										break;
									}
									round[i]->get_screen_texture(
										"Round Win" + std::to_string(*wins[i] + 1)
									).set_sprite(ROUND_ICON_KO);
								}
							}
						}
						e->sound_player.play_reserved_sound(e->string_var("KO Sound"), 0.0f);
					})
					.add_event("process", [this](SceneElement* e) {
						process_fighters();
						process_ui();
						stage.process();
						post_process_fighters();
						for (int i = 0; i < 2; i++) {
							if (fighter[i]->object_float[FIGHTER_FLOAT_HEALTH] > 0.0) {
								fighter[i]->clear_hurtbox_all();
							}
						}
						thread_manager->wait_thread(THREAD_KIND_UI);
						camera->camera_main();
						if (e->sound_player.is_sound_end()) {
							object_manager->reset_world_rate(nullptr);
							e->hide();
							set_active_element(&get_element("root/KO Sequence/Post KO Text"));
						}
					})
				},
				{"Post KO Text",
					SceneElement()
					.int_var("ko_timer", 120)
					.int_var("actionable_timer", 600)
					.int_var("winner", 0)
					.add_event("activate", [this](SceneElement* e) {
						e->int_var("ko_timer") = 120;
						e->int_var("actionable_timer") = 600;
						e->int_var("winner") = ROUND_WIN_DOUBLE_KO;
						if (fighter[0]->object_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
							e->int_var("winner") = ROUND_WIN_P1;
						}
						if (fighter[1]->object_float[FIGHTER_FLOAT_HEALTH] != 0.0) {
							e->int_var("winner")--; //If player 1's health is at 0, this will drop our winner from 2 to 1, indicating
							//player 2 winning. If not, it will drop our winner from 0 to -1, indicating a timeout
						}
					})
					.add_event("process", [this](SceneElement* e) {
						process_fighters();
						process_ui();
						stage.process();
						post_process_fighters();
						for (int i = 0; i < 2; i++) {
							if (fighter[i]->object_float[FIGHTER_FLOAT_HEALTH] > 0.0) {
								fighter[i]->clear_hurtbox_all();
							}
						}
						thread_manager->wait_thread(THREAD_KIND_UI);
						camera->camera_main();

						SceneElement* round[2];
						round[0] = &get_element("root/Battle UI/Round Counter P1");
						round[1] = &get_element("root/Battle UI/Round Counter P2");
						int* wins[2];
						wins[0] = &round[0]->int_var("Wins");
						wins[1] = &round[1]->int_var("Wins");

						WindowManager* window_manager = WindowManager::get_instance();

						switch (e->int_var("winner")) {
							case (ROUND_WIN_TIMEOUT): {
								if (fighter[0]->object_float[FIGHTER_FLOAT_HEALTH] == fighter[1]->object_float[FIGHTER_FLOAT_HEALTH]) {
									if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !e->int_var("actionable_timer")) {
										if (curr_round == 5) {
											window_manager->start_fade_sequence(40, [&]() {
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
										else if (*wins[0] + 1 == round_count_setting
											|| *wins[1] + 1 == round_count_setting) {
											if (*wins[0] + 1 == round_count_setting
												&& *wins[1] + 1 == round_count_setting) {
												round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_alpha(255, 20);
												round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_alpha(255, 20);
												window_manager->start_fade_sequence(40, [&]() {
													round[0]->int_var("Wins")++;
													round[1]->int_var("Wins")++;
													curr_round = 5;
													set_active_element(&get_element("root/Round Start Sequence"));
												});
											}
											else {
												round[0]->get_screen_texture("Round Win" + std::to_string(++*wins[0])).set_alpha(255, 20);
												round[1]->get_screen_texture("Round Win" + std::to_string(++*wins[1])).set_alpha(255, 20);
												set_active_element(&get_element("root/Outro Sequence"));
											}
										}
										else if (fighter[0]->anim_end || fighter[1]->anim_end || !e->int_var("actionable_timer")) {
											round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_alpha(255, 20);
											round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_alpha(255, 20);
											window_manager->start_fade_sequence(40, [&]() {
												(*wins[0])++;
												(*wins[1])++;
												curr_round += 2;
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
									}
									else {
										if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
											&& fighter[0]->fighter_context == FIGHTER_CONTEXT_GROUND
											&& fighter[1]->fighter_context == FIGHTER_CONTEXT_GROUND) {
											if (e->int_var("ko_timer")) {
												e->int_var("ko_timer")--;
											}
											else {
												fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[0]->change_anim("round_lose_timeout");
												fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[1]->change_anim("round_lose_timeout");
											}
											return;
										}
										e->int_var("actionable_timer")--;
									}
								}
								else if (fighter[0]->object_float[FIGHTER_FLOAT_HEALTH] > fighter[1]->object_float[FIGHTER_FLOAT_HEALTH]) {
									if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !e->int_var("actionable_timer")) {
										if (*wins[0] + 1 == round_count_setting) {
											round[0]->get_screen_texture("Round Win" + std::to_string(++*wins[0])).set_alpha(255, 20);
											set_active_element(&get_element("root/Outro Sequence"));
										}
										else if (fighter[0]->anim_end || !e->int_var("actionable_timer")) {
											round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_alpha(255, 20);
											window_manager->start_fade_sequence(40, [&]() {
												(*wins[0])++;
												curr_round++;
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
									}
									else {
										if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
											&& fighter[0]->fighter_context == FIGHTER_CONTEXT_GROUND
											&& fighter[1]->fighter_context == FIGHTER_CONTEXT_GROUND) {
											if (e->int_var("ko_timer")) {
												e->int_var("ko_timer")--;
											}
											else {
												fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[0]->change_anim("round_win_timeout");
												fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[1]->change_anim("round_lose_timeout");
											}
											return;
										}
										e->int_var("actionable_timer")--;
									}
								}
								else {
									if ((fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END && fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) || !e->int_var("actionable_timer")) {
										if (*wins[1] + 1 == round_count_setting) {
											round[1]->get_screen_texture("Round Win" + std::to_string(++*wins[1])).set_alpha(255, 20);
											set_active_element(&get_element("root/Outro Sequence"));
										}
										else if (fighter[1]->anim_end || !e->int_var("actionable_timer")) {
											round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_alpha(255, 20);
											window_manager->start_fade_sequence(40, [&]() {
												(*wins[1])++;
												curr_round++;
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
									}
									else {
										if (fighter[0]->is_actionable() && fighter[1]->is_actionable()
											&& fighter[0]->fighter_context == FIGHTER_CONTEXT_GROUND
											&& fighter[1]->fighter_context == FIGHTER_CONTEXT_GROUND) {
											if (e->int_var("ko_timer")) {
												e->int_var("ko_timer")--;
											}
											else {
												fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[1]->change_anim("round_win_timeout");
												fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
												fighter[0]->change_anim("round_lose_timeout");
											}
											return;
										}
										e->int_var("actionable_timer")--;
									}
								}
							} break;
							case (ROUND_WIN_DOUBLE_KO): {
								if (fighter[0]->status_kind == FIGHTER_STATUS_KO && fighter[1]->status_kind == FIGHTER_STATUS_KO) {
									if (e->int_var("ko_timer")) {
										e->int_var("ko_timer")--;
									}
									else {
										if (curr_round == 5) {
											window_manager->start_fade_sequence(40, [&]() {
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
										else if (*wins[0] + 1 == round_count_setting
											|| *wins[1] + 1 == round_count_setting) {
											if (*wins[0] + 1 == round_count_setting
												&& *wins[1] + 1 == round_count_setting) {
												round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_alpha(255, 20);
												round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_alpha(255, 20);
												window_manager->start_fade_sequence(40, [&]() {
													(*wins[0])++;
													(*wins[1])++;
													curr_round = 5;
													set_active_element(&get_element("root/Round Start Sequence"));
												});
											}
											else {
												round[0]->get_screen_texture("Round Win" + std::to_string(++*wins[0])).set_alpha(255, 20);
												round[1]->get_screen_texture("Round Win" + std::to_string(++*wins[1])).set_alpha(255, 20);
												curr_round += 2;
												set_active_element(&get_element("root/Outro Sequence"));
											}
										}
										else {
											round[0]->get_screen_texture("Round Win" + std::to_string(++*wins[0])).set_alpha(255, 20);
											round[1]->get_screen_texture("Round Win" + std::to_string(++*wins[1])).set_alpha(255, 20);

											window_manager->start_fade_sequence(40, [&]() {
												curr_round += 2;
												set_active_element(&get_element("root/Round Start Sequence"));
											});
										}
									}
								}
							} break;
							case (ROUND_WIN_P1): {
								if (fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END || !e->int_var("actionable_timer")) {
									if (*wins[0] + 1 == round_count_setting) {
										round[0]->get_screen_texture("Round Win" + std::to_string(++*wins[0])).set_alpha(255, 20);
										set_active_element(&get_element("root/Outro Sequence"));
										return;
									}
									else if (fighter[0]->anim_end || !e->int_var("actionable_timer")) {
										round[0]->get_screen_texture("Round Win" + std::to_string(*wins[0] + 1)).set_alpha(255, 20);
										window_manager->start_fade_sequence(40, [&]() {
											(*wins[0])++;
											curr_round++;
											set_active_element(&get_element("root/Round Start Sequence"));
										});
									}
								}
								else {
									if (fighter[0]->is_actionable()
										&& fighter[0]->fighter_context == FIGHTER_CONTEXT_GROUND) {
										if (e->int_var("ko_timer")) {
											e->int_var("ko_timer")--;
										}
										else {
											fighter[0]->change_status(FIGHTER_STATUS_ROUND_END);
										}
										return;
									}
									e->int_var("actionable_timer")--;
								}
							} break;
							case (ROUND_WIN_P2): {
								if (fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END || !e->int_var("actionable_timer")) {
									if (*wins[1] + 1 == round_count_setting) {
										round[1]->get_screen_texture("Round Win" + std::to_string(++*wins[1])).set_alpha(255, 20);
										set_active_element(&get_element("root/Outro Sequence"));
										return;
									}
									else if (fighter[1]->anim_end || !e->int_var("actionable_timer")) {
										round[1]->get_screen_texture("Round Win" + std::to_string(*wins[1] + 1)).set_alpha(255, 20);
										window_manager->start_fade_sequence(40, [&]() {
											(*wins[1])++;
											curr_round++;
											set_active_element(&get_element("root/Round Start Sequence"));
										});
									}
								}
								else {
									if (fighter[1]->is_actionable()
										&& fighter[1]->fighter_context == FIGHTER_CONTEXT_GROUND) {
										if (e->int_var("ko_timer")) {
											e->int_var("ko_timer")--;
										}
										else {
											fighter[1]->change_status(FIGHTER_STATUS_ROUND_END);
										}
										return;
									}
									e->int_var("actionable_timer")--;
								}
							} break;
						}
					})
					.add_event("start_press", [this](SceneElement* e) {
						if (fighter[0]->status_kind == FIGHTER_STATUS_ROUND_END
						|| fighter[1]->status_kind == FIGHTER_STATUS_ROUND_END) {
							e->int_var("actionable_timer") = 0;
						}
					})
				}
			})
		}, //KO
		{ "Outro Sequence", 
			SceneElement()
			.add_event("activate", [this](SceneElement* e) {

			})
			.add_event("process", [this](SceneElement* e) {
				GameManager::get_instance()->update_scene(SCENE_DEBUG_MENU);
			})
		} //Outro
	});

	if (context == SCENE_CONTEXT_TRAINING) {
		get_element("root/Battle Sequence")
			.add_event("frame_pause_press", [this](SceneElement* e) {
				SoundManager* sound_manager = SoundManager::get_instance();
				if (e->bool_var("frame_pause")) {
					camera->pos_x_interpolator.set_pause(false);
					sound_manager->resume_all_sounds();
					sound_manager->resume_all_reserved_sounds();
				}
				else {
					camera->pos_x_interpolator.set_pause(true);
					sound_manager->pause_all_sounds();
					sound_manager->pause_all_reserved_sounds();
				}
				bool& timer_pause = get_element("root/Battle UI").bool_var("pause");
				timer_pause = !timer_pause;
				e->bool_var("frame_pause") = !e->bool_var("frame_pause");
			})
			.add_event("frame_advance_press", [this](SceneElement* e) {
				if (e->bool_var("frame_pause")) {
					SoundManager* sound_manager = SoundManager::get_instance();
					sound_manager->resume_all_sounds();
					sound_manager->resume_all_reserved_sounds();
					e->bool_var("frame_advance") = true;
				}
			})
			.add_event("record_input_press", [this](SceneElement* e) {
				if (player[1]->player_kind == PLAYER_KIND_DUMMY) {
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
			})
			.add_event("replay_input_press", [this](SceneElement* e) {
				if (player[1]->player_kind == PLAYER_KIND_DUMMY) {
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
			})
			.add_event("switch_input_press", [this](SceneElement* e) {
				if (player[1]->player_kind == PLAYER_KIND_DUMMY) {
					player[0]->controller.swap_player_controller(&player[1]->controller);
				}
			});
		for (int i = 0; i < 2; i++) {
			training_info[i].init(fighter[i], &get_font("info"));
		}
	}
}

void Battle::process_pre_event() {
	if (frame_advance) {
		SoundManager* sound_manager = SoundManager::get_instance();
		sound_manager->pause_all_sounds();
		sound_manager->pause_all_reserved_sounds();
		if (camera->anim_kind == nullptr || camera->anim_kind->anim_mode == CAMERA_ANIM_MODE_STAGE) {
			camera->pos_x_interpolator.process();
			camera->calc_aim_from_ypr();
		}
		camera->update_view();
		frame_advance = false;
	}
}

void Battle::process_main() {
	execute_event("process");
	process_collisions();
}

void Battle::process_ko() {
	
}

void Battle::process_outro() {

}

void Battle::process_debug_boxes() {
	if (mouse.check_button_on(GLFW_MOUSE_BUTTON_2) && active_hitbox_object != nullptr &&
		hitbox_sim[active_hitbox_object->get_anim()].active_box != nullptr) {
		glm::vec2 game_rect_coords = mouse_pos_to_rect_coord(mouse.get_pos());
		hitbox_sim[active_hitbox_object->get_anim()].active_box->offset = game_rect_coords;
		if (mouse.check_button_trigger(GLFW_MOUSE_BUTTON_2)) {
			hitbox_sim[active_hitbox_object->get_anim()].active_box->anchor = game_rect_coords;
		}
	}
}

void Battle::process_fighters() {
	if (!(fighter[0]->object_flag[FIGHTER_FLAG_LOCK_DIRECTION]
		|| fighter[1]->object_flag[FIGHTER_FLAG_LOCK_DIRECTION])) {
		for (int i = 0; i < 2; i++) {
			if (fighter[i]->fighter_context == FIGHTER_CONTEXT_GROUND) {
				if (fighter[i]->get_pos().x > fighter[!i]->get_pos().x) {
					fighter[i]->internal_facing_right = false;
					fighter[i]->internal_facing_dir = -1.0;
				}
				else if (fighter[i]->get_pos().x < fighter[!i]->get_pos().x) {
					fighter[i]->internal_facing_right = true;
					fighter[i]->internal_facing_dir = 1.0;
				}
				else { //If both players are stuck inside each other, stop that !
					if (fighter[!i]->fighter_context == FIGHTER_CONTEXT_GROUND) {
						if (fighter[i]->get_pos().x == fighter[!i]->get_pos().x
							&& fighter[i]->internal_facing_dir == fighter[!i]->internal_facing_dir) {
							fighter[i]->internal_facing_right = true;
							fighter[i]->internal_facing_dir = 1.0;
							fighter[!i]->internal_facing_right = false;
							fighter[!i]->internal_facing_dir = -1.0;
						}
						fighter[i]->add_pos_validate(glm::vec3(-10.0 * fighter[i]->internal_facing_dir, 0, 0));
						fighter[!i]->add_pos_validate(glm::vec3(-10.0 * fighter[!i]->internal_facing_dir, 0, 0));
					}
				}
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		thread_manager->notify_thread(i);
	}
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
		if (fighter[i]->object_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] == 0) {
			fighter[i]->gain_ex(1.0);
		}

		short new_input_code = fighter[i]->get_stick_dir_no_lr() << 7;
		for (int i2 = 0; i2 < 6; i2++) {
			if (fighter[i]->check_button_on(BUTTON_LP + i2)) {
				new_input_code |= 1 << i2;
			}
		}
		if (new_input_code != training_info[i].mini_visualizers.newest().input_code) {
			training_info[i].mini_visualizers.newest().keep_frames = false;
			training_info[i].mini_visualizers.cycle();
			training_info[i].mini_visualizers.newest().reset();
			for (int i2 = 0; i2 < training_info[i].mini_visualizers.size(); i2++) {
				for (int i3 = 0; i3 < 6; i3++) {
					training_info[i].mini_visualizers.newest(i2).buttons[i3].add_pos(glm::vec3(0.0, 90.0, 0.0));
				}
				
				for (int i3 = 0; i3 < 9; i3++) {
					training_info[i].mini_visualizers.newest(i2).stick[i3].add_pos(glm::vec3(0.0, 90.0, 0.0));
				}
				training_info[i].mini_visualizers.newest(i2).num_frames.add_pos(glm::vec3(0.0, 90.0, 0.0));
				training_info[i].mini_visualizers.newest(i2).background.add_pos(glm::vec3(0.0, 90.0, 0.0));
			}
			training_info[i].mini_visualizers.newest().input_code = new_input_code;
		}
		else {
			training_info[i].mini_visualizers.newest().frame_timer = std::min(
				training_info[i].mini_visualizers.newest().frame_timer + 1, 99
			);
			training_info[i].mini_visualizers.newest().num_frames.update_text(
				std::to_string(training_info[i].mini_visualizers.newest().frame_timer)
			);
		}
	}
}

void Battle::process_cpus() {
	if (&get_active_element() != &get_element("root/Battle Sequence")) return;
	if (get_active_element().bool_var("frame_advance") || !get_active_element().bool_var("frame_pause")) {
		for (int i = 0; i < 2; i++) {
			if (player[i]->player_kind == PLAYER_KIND_CPU) {
				fighter[i]->process_cpu();
			}
		}
	}
}

void gamestate_battle_fighter_thread(void* fighter_arg) {
	Fighter* fighter = (Fighter*)fighter_arg;
	fighter->fighter_main();
}

void gamestate_battle_ui_thread(void* battle_arg) {
	Battle* battle = (Battle*)battle_arg;
	battle->get_element("root/Battle UI").execute_event("process");
}

void Battle::render_main() {
	WindowManager* window_manager = WindowManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	window_manager->execute_buffered_events();

	//SHADOW PASS - We render our Fighters, Projectiles and the Stage to our Shadow Map.

	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	window_manager->shadow_map.use();
	glViewport(0, 0, 2048, 2048);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	for (int i = 0; i < 2; i++) {
		fighter[i]->render_shadow();
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->get_alpha() != 0 && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render_shadow();
			}
		}
	}
	stage.render_shadow();
	glCullFace(GL_BACK);

	//COLOR PASS - Now we actually render to the Geometry Buffer, getting a ton of output that we're
	//going to use in the Lighting pass.

	window_manager->g_buffer.use();

	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_manager->shadow_map.bind_textures();

	shader_manager->set_global_float("Outline", 0.5f);

	glEnable(GL_CULL_FACE);
	stage.render();

	glDisable(GL_CULL_FACE); //This line will be removed, but current models have some vertices such
	//that we can see both sides. Until then, fighters don't get culled.
	for (int i = 0; i < 2; i++) {
		shader_manager->set_global_float("Outline", 1.0f);
		fighter[i]->render();
		shader_manager->set_global_float("Outline", 0.5f);
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->get_alpha() != 0 && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render();
			}
		}
	}

	//ALPHA PASS

	for (GameObject* object : object_manager->game_objects) {
		if (!object->shader) continue;
		object->shader = shader_manager->get_shader_switch_features(object->shader, 0, SHADER_FEAT_ALPHA_PASS);
	}

	glEnable(GL_CULL_FACE);
	stage.render();

	glDisable(GL_CULL_FACE);
	for (int i = 0; i < 2; i++) {
		shader_manager->set_global_float("Outline", 1.0f);
		fighter[i]->render();
		shader_manager->set_global_float("Outline", 0.5f);
		for (int i2 = 0; i2 < fighter[i]->projectiles.size(); i2++) {
			if (fighter[i]->projectiles[i2]->get_alpha() != 0 && fighter[i]->projectiles[i2]->has_model) {
				fighter[i]->projectiles[i2]->render();
			}
		}
	}

	for (GameObject* object : object_manager->game_objects) {
		if (!object->shader) continue;
		object->shader = shader_manager->get_shader_switch_features(object->shader, SHADER_FEAT_ALPHA_PASS, 0);
	}

	//EFFECT RENDERING

	glDisable(GL_CULL_FACE); //Currently we disable culling on all particle effects since they 
	//are single textures that can be rotated. In the future particle effects will also be able
	//to be models, so we'll start by rendering the model particle effects before turning off
	//culling and rendering the texture particle effects.

	glDepthMask(GL_FALSE); //Particles don't write to the depth buffer so as not to have the
	//invisible part of one particle overwrite the visible part of another.
	EffectManager::get_instance()->render();
	glDepthMask(GL_TRUE);

	//SSAO PASS - Draws and blurs the SSAO buffer.

	window_manager->render_ssao();

	//LIGHTING PASS - Handles all of the light calculations and renders all of our geometry to the
	//HDR buffer with lighting.

	window_manager->hdr_buffer.use();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	window_manager->g_buffer.render();

	//BLOOM PASS - Draws and blurs the Emission buffer.

	window_manager->render_bloom();

	//SCREEN PASS - Draws the HDR buffer to the screen with bloom.

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	window_manager->hdr_buffer.render();

	//HITBOX PASS - Draws all collision boxes to their own framebuffer, then draws it to the screen.

	if (context == SCENE_CONTEXT_TRAINING && SaveManager::get_instance()->get_game_setting("visualize_boxes") == 1) {
		window_manager->box_layer.use();
		glViewport(0, 0, window_manager->res_width, window_manager->res_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		glViewport(0, 0, window_manager->window_width, window_manager->window_height);
		window_manager->box_layer.render();
	}

	//UI PASS

	root.render();

	//TRAINING PASS

	if (context == SCENE_CONTEXT_TRAINING) {
		for (int i = 0; i < 2; i++) {
			training_info[i].render();
		}
	}
}