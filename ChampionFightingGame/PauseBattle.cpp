#include "PauseBattle.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "Fighter.h"
#include "TimeFuncs.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameState* background_menu = game_manager->get_game_state();

	PauseBattle* pause = new PauseBattle;

	while (pause->looping) {
		game_manager->frame_delay_check_fps();
		render_manager->clear_screen();
		render_manager->handle_window_events();

		pause->process_game_state();
		pause->render_game_state();
		
		render_manager->update_screen();
	}

	delete pause;
}

PauseBattle::PauseBattle() {
	GameManager* game_manager = GameManager::get_instance();
	load_font("header", "FiraCode", 24);
	load_font("button", "FiraCode", 16);

	push_menu_object("Pause"); {
		push_menu_bool_var("Button Selected", false);
		push_menu_texture("BG Dimmer", "resource/misc/fade.png");
		last_pushed_texture->set_alpha(127);
		push_menu_texture("Panel", "resource/game_state/battle/ui/pause/overlay.png");
		push_menu_left_event_function([this](MenuObject* object) {
			object->get_active_child("Pause Pages").event_left_press();
		});
		push_menu_right_event_function([this](MenuObject* object) {
			object->get_active_child("Pause Pages").event_right_press();
		});
		push_menu_up_event_function([this](MenuObject* object) {
			object->get_active_child("Pause Pages").event_up_press();
		});
		push_menu_down_event_function([this](MenuObject* object) {
			object->get_active_child("Pause Pages").event_down_press();
		});
		push_menu_select_event_function([this](MenuObject* object) {
			object->get_active_child("Pause Pages").event_select_press();
		});
		push_menu_back_event_function([this](MenuObject* object) {
			if (object->bool_var("Button Selected")) {
				object->get_active_child("Pause Pages").event_back_press();
				return;
			}
			looping = false;
		});
		push_menu_activity_group("Pause Pages", nullptr, true, 3); {
			push_menu_child("Main Page"); {
				push_menu_texture("Label", get_font("header"), "Pause", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				last_pushed_texture->set_pos(glm::vec3(0.0, 300.0, 0.0));
				push_menu_up_event_function([this](MenuObject* object) {
					object->get_activity_group("Buttons").inc_active_child(-1);
				});
				push_menu_down_event_function([this](MenuObject* object) {
					object->get_activity_group("Buttons").inc_active_child(1);
				});
				push_menu_select_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").event_select_press();
				});
				push_menu_back_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").event_back_press();
				});
				push_menu_left_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_left_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("CPU Page");
					}
				});
				push_menu_right_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_right_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("Training Page");
					}
				});
				push_menu_activity_group("Buttons", nullptr, false, 3); {
					push_menu_child("Chara Select"); {
						push_menu_texture("Button", get_font("button"), "Return to Character Select", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						last_pushed_texture->set_scale(1.1f);
						push_menu_on_selected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.1f);
						});
						push_menu_on_deselected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.0f);
						});
						push_menu_activity_group("State", nullptr, true); {
							push_menu_child("Blank"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->set_active_sibling("Confirm");
								});
								push_menu_on_selected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = false;
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = true;
								});
							} pop_menu_stack();
							push_menu_child("Confirm"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->get_active_child("Selection").event_select_press();
								});
								push_menu_texture("Confirm Window", "resource/game_state/battle/ui/pause/confirm.png");
								push_menu_texture("Confirm Text", get_font("header"), "Confirm?", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_pos(glm::vec3(0.0, 90.0, 0.0));								push_menu_left_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("Yes");
								});
								push_menu_right_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("No");
								});
								push_menu_activity_group("Selection", nullptr, false); {
									push_menu_child("Yes"); {
										push_menu_texture("Button", get_font("button"), "Yes", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_scale(1.1f);
										last_pushed_texture->set_pos(glm::vec3(-200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											GameManager* game_manager = GameManager::get_instance();
											game_manager->update_state(GAME_STATE_CHARA_SELECT);
											game_manager->get_game_state(1)->looping = false;
										});
									} pop_menu_stack();
									push_menu_child("No"); {
										push_menu_texture("Button", get_font("button"), "No", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_pos(glm::vec3(200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											object->set_active_sibling("Yes");
											object->get_parent().set_active_sibling("Blank");
										});
									} pop_menu_stack();
								} pop_menu_stack();
							} pop_menu_stack();
						} pop_menu_stack();
						push_menu_left_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_left_press();
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_right_press();
						});
						push_menu_select_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_select_press();
						});
					} pop_menu_stack();
					push_menu_child("Stage Select"); {
						push_menu_texture("Button", get_font("button"), "Return to Stage Select", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						push_menu_on_selected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.1f);
						});
						push_menu_on_deselected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.0f);
						});
						push_menu_activity_group("State", nullptr, true); {
							push_menu_child("Blank"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->set_active_sibling("Confirm");
								});
								push_menu_on_selected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = false;
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = true;
								});
							} pop_menu_stack();
							push_menu_child("Confirm"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->get_active_child("Selection").event_select_press();
								});
								push_menu_texture("Confirm Window", "resource/game_state/battle/ui/pause/confirm.png");
								push_menu_texture("Confirm Text", get_font("header"), "Confirm?", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_pos(glm::vec3(0.0, 90.0, 0.0));								push_menu_left_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("Yes");
								});
								push_menu_right_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("No");
								});
								push_menu_activity_group("Selection", nullptr, false); {
									push_menu_child("Yes"); {
										push_menu_texture("Button", get_font("button"), "Yes", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_scale(1.1f);
										last_pushed_texture->set_pos(glm::vec3(-200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											GameManager* game_manager = GameManager::get_instance();
											game_manager->update_state(GAME_STATE_STAGE_SELECT);
											game_manager->get_game_state(1)->looping = false;
										});
									} pop_menu_stack();
									push_menu_child("No"); {
										push_menu_texture("Button", get_font("button"), "No", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_pos(glm::vec3(200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											object->set_active_sibling("Yes");
											object->get_parent().set_active_sibling("Blank");
										});
									} pop_menu_stack();
								} pop_menu_stack();
							} pop_menu_stack();
						} pop_menu_stack();
						push_menu_left_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_left_press();
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_right_press();
						});
						push_menu_select_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_select_press();
						});
					} pop_menu_stack();
					push_menu_child("Main Menu"); {
						push_menu_texture("Button", get_font("button"), "Return to Main Menu", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						push_menu_on_selected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.1f);
						});
						push_menu_on_deselected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.0f);
						});
						push_menu_activity_group("State", nullptr, true); {
							push_menu_child("Blank"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->set_active_sibling("Confirm");
								});
								push_menu_on_selected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = false;
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									get_menu_object("Pause").bool_var("Button Selected") = true;
								});
							} pop_menu_stack();
							push_menu_child("Confirm"); {
								push_menu_select_event_function([this](MenuObject* object) {
									object->get_active_child("Selection").event_select_press();
								});
								push_menu_texture("Confirm Window", "resource/game_state/battle/ui/pause/confirm.png");
								push_menu_texture("Confirm Text", get_font("header"), "Confirm?", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_pos(glm::vec3(0.0, 90.0, 0.0));
								push_menu_left_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("Yes");
								});
								push_menu_right_event_function([this](MenuObject* object) {
									object->get_activity_group("Selection").set_active_child("No");
								});
								push_menu_activity_group("Selection", nullptr, false); {
									push_menu_child("Yes"); {
										push_menu_texture("Button", get_font("button"), "Yes", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_scale(1.1f);
										last_pushed_texture->set_pos(glm::vec3(-200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											GameManager* game_manager = GameManager::get_instance();
											game_manager->update_state(GAME_STATE_MENU);
											game_manager->get_game_state(1)->looping = false;
										});
									} pop_menu_stack();
									push_menu_child("No"); {
										push_menu_texture("Button", get_font("button"), "No", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
										last_pushed_texture->set_pos(glm::vec3(200.0, 0.0, 0.0));
										push_menu_on_selected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.1f);
										});
										push_menu_on_deselected_event_function([this](MenuObject* object) {
											object->get_texture("Button").set_scale(1.0f);
										});
										push_menu_select_event_function([this](MenuObject* object) {
											object->set_active_sibling("Yes");
											object->get_parent().set_active_sibling("Blank");
										});
									} pop_menu_stack();
								} pop_menu_stack();
							} pop_menu_stack();
						} pop_menu_stack();
						push_menu_left_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_left_press();
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_right_press();
						});
						push_menu_select_event_function([this](MenuObject* object) {
							object->get_active_child("State").event_select_press();
						});
					} pop_menu_stack();
					for (int i = 0, max = activity_group_stack.top()->num_children(); i < max; i++) {
						activity_group_stack.top()->get_child(i).get_texture("Button").set_pos(glm::vec3(0.0, i * -100, 0.0));
					}
				} pop_menu_stack();
			} pop_menu_stack();
			push_menu_child("Training Page"); {
				push_menu_texture("Label", get_font("header"), "Training Settings", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				last_pushed_texture->set_pos(glm::vec3(0.0, 300.0, 0.0));
				push_menu_up_event_function([this](MenuObject* object) {
					object->get_activity_group("Buttons").inc_active_child(-1);
				});
				push_menu_down_event_function([this](MenuObject* object) {
					object->get_activity_group("Buttons").inc_active_child(1);
				});
				push_menu_left_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_left_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("Main Page");
					}
				});
				push_menu_right_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_right_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("CPU Page");
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").event_select_press();
				});
				push_menu_back_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").event_back_press();
				});
				push_menu_activity_group("Buttons", nullptr, false); {
					push_menu_child("Reset"); {
						push_menu_texture("Button", get_font("button"), "Reset", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
						last_pushed_texture->set_scale(1.1f);
						push_menu_on_selected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.1f);
						});
						push_menu_on_deselected_event_function([this](MenuObject* object) {
							object->get_texture("Button").set_scale(1.0f);
						});
						push_menu_select_event_function([this](MenuObject* object) {
							ObjectManager* object_manager = ObjectManager::get_instance();
							object_manager->fighter[0]->reset();
							object_manager->fighter[1]->reset();
							this->looping = false;
						});
					} pop_menu_stack();
					for (int i = 0, max = activity_group_stack.top()->num_children(); i < max; i++) {
						activity_group_stack.top()->get_child(i).get_texture("Button").set_pos(glm::vec3(0.0, i * -100, 0.0));
					}
				} pop_menu_stack();
			} pop_menu_stack();
			push_menu_child("CPU Page"); {
				push_menu_texture("Label", get_font("header"), "CPU Settings", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
				last_pushed_texture->set_pos(glm::vec3(0.0, 300.0, 0.0));
				push_menu_select_event_function([this](MenuObject* object) {
					get_menu_object("Pause").bool_var("Button Selected") = true;
				});
				push_menu_back_event_function([this](MenuObject* object) {
					get_menu_object("Pause").bool_var("Button Selected") = false;
				});
				push_menu_up_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").get_activity_group("Buttons").inc_active_child(-1);
				});
				push_menu_down_event_function([this](MenuObject* object) {
					object->get_active_child("Buttons").get_activity_group("Buttons").inc_active_child(1);
				});
				push_menu_left_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_left_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("Training Page");
					}
				});
				push_menu_right_event_function([this](MenuObject* object) {
					if (object->get_parent().bool_var("Button Selected")) {
						object->get_active_child("Buttons").event_right_press();
						return;
					}
					if (game_context == GAME_CONTEXT_TRAINING) {
						object->set_active_sibling("Main Page");
					}
				});
				push_menu_activity_group("Buttons", &game_manager->player[1]->player_kind, true, 3); {
					push_menu_child("CPU Mode Player"); {
						push_menu_left_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode CPU");
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode Dummy");
						});
						push_menu_activity_group("Buttons", nullptr, false); {
							push_menu_child("CPU Mode"); {
								push_menu_texture("Button", get_font("button"), "Player Controlled", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_scale(1.1f);
								push_menu_on_selected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.1f);
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.0f);
								});
							} pop_menu_stack();
						} pop_menu_stack();
					} pop_menu_stack();
					push_menu_child("CPU Mode CPU"); {
						push_menu_left_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode Dummy");
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode Player");
						});
						push_menu_activity_group("Buttons", nullptr, false); {
							push_menu_child("CPU Mode"); {
								push_menu_texture("Button", get_font("button"), "CPU Controlled", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_scale(1.1f);
								push_menu_on_selected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.1f);
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.0f);
								});
							} pop_menu_stack();
						} pop_menu_stack();
					} pop_menu_stack();
					push_menu_child("CPU Mode Dummy"); {
						push_menu_left_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode Player");
						});
						push_menu_right_event_function([this](MenuObject* object) {
							object->set_active_sibling("CPU Mode CPU");
						});
						push_menu_activity_group("Buttons", nullptr, false); {
							push_menu_child("CPU Mode"); {
								push_menu_texture("Button", get_font("button"), "Dummy", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
								last_pushed_texture->set_scale(1.1f);
								push_menu_on_selected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.1f);
								});
								push_menu_on_deselected_event_function([this](MenuObject* object) {
									object->get_texture("Button").set_scale(1.0f);
								});
							} pop_menu_stack();
						} pop_menu_stack();
					} pop_menu_stack();
				} pop_menu_stack();
			} pop_menu_stack();
		} pop_menu_stack();
	} pop_menu_stack();
}


PauseBattle::~PauseBattle() {
	
}

void PauseBattle::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
	}
}

void PauseBattle::render_main() {
	render_menu_object("Pause");
}

void PauseBattle::event_start_press() {
	looping = false;
}

void PauseBattle::event_select_press() {
	get_menu_object("Pause").event_select_press();
}

void PauseBattle::event_back_press() {
	get_menu_object("Pause").event_back_press();
}

void PauseBattle::event_up_press() {
	get_menu_object("Pause").event_up_press();
}

void PauseBattle::event_down_press() {
	get_menu_object("Pause").event_down_press();
}

void PauseBattle::event_left_press() {
	get_menu_object("Pause").event_left_press();
}

void PauseBattle::event_right_press() {
	get_menu_object("Pause").event_right_press();
}