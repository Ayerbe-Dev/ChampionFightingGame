#include "cotr_imgui_debugger.h"
#include "Fighter.h"
#include "WindowManager.h"
#include "ShaderManager.h"
#include "utils.h"

#include "DebugMenu.h"
#include "Battle.h"
#include "StageSelect.h"
#include "Opening.h"

void cotr_imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::get_instance()->window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

}

void cotr_imgui_terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void cotr_imgui_debug_dbmenu(DebugMenu* debug_menu) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	GameManager* game_manager = GameManager::get_instance();

	const char* charas[18]{
		"Rowan",
		"Eric",
		"Leon",
		"Desirae",
		"Nightsabre",
		"Athena",
		"Dante",
		"Zyair",
		"Tessa",
		"Rook",
		"Bruno",
		"Atlas",
		"Julius",
		"Ramona",
		"Dingo",
		"Wallace",
		"Vesuvius",
		"CHAMELEON"
	};
	const char* current_chara_p1 = charas[game_manager->player[0]->chara_kind];
	const char* current_chara_p2 = charas[game_manager->player[1]->chara_kind];

	ImGui::Begin("P1 Chara Kind", nullptr, 
		ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize);
	for (size_t i = 0; i < 18; i++) {
		bool selected = current_chara_p1 == charas[i];
		if (ImGui::Selectable(charas[i], selected)) {
			current_chara_p1 = charas[i];
			game_manager->player[0]->chara_kind = i;
		}
		if (selected) {
			ImGui::SetItemDefaultFocus();
		}
	}
	
	ImGui::End();

	ImGui::Begin("P2 Chara Kind", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize);
	for (size_t i = 0; i < 18; i++) {
		bool selected = current_chara_p2 == charas[i];
		if (ImGui::Selectable(charas[i], selected)) {
			current_chara_p2 = charas[i];
			game_manager->player[1]->chara_kind = i;
		}
		if (selected) {
			ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::End();

	ImGui::Begin("Debug Menu", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
	);

	if (ImGui::MenuItem("Debug Menu")) {
		game_manager->update_scene(SCENE_DEBUG_MENU);
	}
	if (ImGui::MenuItem("Battle (Training)")) {
		game_manager->update_scene(SCENE_BATTLE, SCENE_CONTEXT_TRAINING);
	}
	if (ImGui::MenuItem("Battle (VS)")) {
		game_manager->update_scene(SCENE_BATTLE, SCENE_CONTEXT_NONE);
	}
	if (ImGui::MenuItem("Character Select Screen")) {
		game_manager->update_scene(SCENE_CHARA_SELECT);
	}
	if (ImGui::MenuItem("Stage Select Screen")) {
		game_manager->update_scene(SCENE_STAGE_SELECT);
	}
	if (ImGui::MenuItem("Main Menu")) {
		game_manager->update_scene(SCENE_MAIN_MENU);
	}
	if (ImGui::MenuItem("Title Screen")) {
		game_manager->update_scene(SCENE_TITLE_SCREEN);
	}
	if (ImGui::MenuItem("Pause Menu")) {
		game_manager->scene_main[SCENE_PAUSE_BATTLE]();
	}
	if (ImGui::MenuItem("Opening")) {
		opening_main();
	}
	if (ImGui::Button("exit")) {
		game_manager->update_scene(SCENE_CLOSE);
	}
	for (int i = 0, max = debug_menu->debug_messages.size(); i < max; i++) {
		ImGui::Text("%s", debug_menu->debug_messages[i].c_str());
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cotr_imgui_debug_battle(Battle* battle) {
	WindowManager* window_manager = WindowManager::get_instance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Menu", nullptr, 
		ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoResize);
	{
		if (ImGui::TreeNode("Hitbox Creator")) {
			const char* hit_levels[3]{
				"HIT_LEVEL_LIGHT",
				"HIT_LEVEL_MEDIUM",
				"HIT_LEVEL_HEAVY"
			};
			const char* hit_heights[3]{
				"HIT_HEIGHT_HIGH",
				"HIT_HEIGHT_MID",
				"HIT_HEIGHT_LOW"
			};
			const char* hit_statuses[7]{
				"HIT_STATUS_NORMAL",
				"HIT_STATUS_CRUMPLE",
				"HIT_STATUS_FLOAT",
				"HIT_STATUS_LAUNCH",
				"HIT_STATUS_KNOCKDOWN",
				"HIT_STATUS_CUSTOM",
				"HIT_STATUS_NONE"
			};
			const char* counterhit_types[5]{
				"COUNTERHIT_TYPE_NONE",
				"COUNTERHIT_TYPE_COUNTER",
				"COUNTERHIT_TYPE_ANY",
				"COUNTERHIT_TYPE_PUNISH",
				"COUNTERHIT_TYPE_JUMP_COUNTER"
			};
			const char* damage_kinds[4]{
				"DAMAGE_KIND_NORMAL",
				"DAMAGE_KIND_NO_KO",
				"DAMAGE_KIND_CHIP",
				"DAMAGE_KIND_CHIP_KO"
			};
			const char* hurtbox_kinds[3]{
				"HURTBOX_KIND_NORMAL",
				"HURTBOX_KIND_COUNTER",
				"HURTBOX_KIND_RIGHT_OF_WAY"
			};

			std::vector<std::string> items;
			std::vector<BattleObject*> objects;
			items.push_back("Fighter 1");
			items.push_back("Fighter 2");
			objects.push_back(battle->fighter[0]);
			objects.push_back(battle->fighter[1]);
			for (int i = 0; i < 2; i++) {
				for (int i2 = 0; i2 < battle->fighter[i]->projectiles.size(); i2++) {
					if (battle->fighter[i]->projectiles[i2]->active) {
						items.push_back("Fighter " + std::to_string(i + 1) +
							" Projectile " + std::to_string(i2 + 1));
						objects.push_back(battle->fighter[i]->projectiles[i2]);
					}
				}
			}
			if (battle->active_hitbox_object_index >= objects.size()) {
				battle->active_hitbox_object_index = 0;
				battle->active_hitbox_object = battle->fighter[0];
			}
			const char* current_item = items[battle->active_hitbox_object_index].c_str();
			if (ImGui::BeginCombo("Active Battle Object", current_item)) {
				for (size_t i = 0; i < objects.size(); i++) {
					bool selected = current_item == items[i];
					if (ImGui::Selectable(items[i].c_str(), selected)) {
						current_item = items[i].c_str();
						battle->active_hitbox_object_index = i;
						battle->active_hitbox_object = objects[i];
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::TreeNode("Hitboxes")) {
				for (size_t i = 0, max = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.size(); i < max; i++) {
					SimHitbox& sim_hitbox = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i];
					if (sim_hitbox.frame_start > battle->active_hitbox_object->frame
						|| (sim_hitbox.frame_end <= battle->active_hitbox_object->frame
							&& sim_hitbox.frame_end > sim_hitbox.frame_start)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Hitbox ID " + index;
					std::string internal_label = "##hitbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						const char* current_hit_height = hit_heights[sim_hitbox.hit_height];
						const char* current_hit_status = hit_statuses[sim_hitbox.hit_status];
						const char* current_counterhit_status = hit_statuses[sim_hitbox.critical_status];
						const char* current_counterhit_type = counterhit_types[sim_hitbox.critical_condition];
						const char* current_damage_kind = damage_kinds[sim_hitbox.damage_kind];

						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &sim_hitbox.frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &sim_hitbox.frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &sim_hitbox.id);
						ImGui::InputInt(("Multihit ID" + internal_label).c_str(), &sim_hitbox.multihit);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &sim_hitbox.relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &sim_hitbox.relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &sim_hitbox.relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &sim_hitbox.relative_offset.y);
						ImGui::Checkbox(("Hits Grounded" + internal_label).c_str(), &sim_hitbox.collision_kind_ground);
						ImGui::Checkbox(("Hits Aerial" + internal_label).c_str(), &sim_hitbox.collision_kind_air);
						ImGui::Checkbox(("Hits OTG" + internal_label).c_str(), &sim_hitbox.collision_kind_down);
						ImGui::Checkbox(("Hits Projectiles" + internal_label).c_str(), &sim_hitbox.collision_kind_projectile);
						ImGui::Checkbox(("Hits through Soft Intangible" + internal_label).c_str(), &sim_hitbox.collision_kind_soft_intangible);
						ImGui::Checkbox(("Hits through Armor" + internal_label).c_str(), &sim_hitbox.collision_kind_armor);

						if (ImGui::BeginCombo(("Counterhit Type" + internal_label).c_str(), current_counterhit_type)) {
							for (size_t i2 = 0; i2 < 5; i2++) {
								bool selected = current_counterhit_type == counterhit_types[i2];
								if (ImGui::Selectable(counterhit_types[i2], selected)) {
									current_counterhit_type = counterhit_types[i2];
									sim_hitbox.critical_condition = (CriticalCondition)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}

						if (ImGui::BeginCombo(("Hit Status" + internal_label).c_str(), current_hit_status)) {
							for (size_t i2 = 0; i2 < 7; i2++) {
								bool selected = current_hit_status == hit_statuses[i2];
								if (ImGui::Selectable(hit_statuses[i2], selected)) {
									current_hit_status = hit_statuses[i2];
									sim_hitbox.hit_status = (HitStatus)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (sim_hitbox.hit_status == HIT_STATUS_CUSTOM) {
							ImGui::InputText(("Custom Hit Status" + internal_label).c_str(), &sim_hitbox.custom_hit_status);
						}
						if (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE) {
							if (ImGui::BeginCombo(("Counterhit Status" + internal_label).c_str(), current_counterhit_status)) {
								for (size_t i2 = 0; i2 < 7; i2++) {
									bool selected = current_counterhit_status == hit_statuses[i2];
									if (ImGui::Selectable(hit_statuses[i2], selected)) {
										current_counterhit_status = hit_statuses[i2];
										sim_hitbox.critical_status = (HitStatus)i2;
									}
									if (selected) {
										ImGui::SetItemDefaultFocus();
									}
								}
								ImGui::EndCombo();
							}
							if (sim_hitbox.critical_status == HIT_STATUS_CUSTOM) {
								ImGui::InputText(("Custom Counterhit Status" + internal_label).c_str(), &sim_hitbox.custom_critical_status);
							}
						}

						if (sim_hitbox.hit_status == HIT_STATUS_KNOCKDOWN || sim_hitbox.hit_status == HIT_STATUS_FLOAT
							|| (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE && (sim_hitbox.critical_status == HIT_STATUS_KNOCKDOWN || sim_hitbox.critical_status == HIT_STATUS_FLOAT))) {
							ImGui::Checkbox(("Down Face Down" + internal_label).c_str(), &sim_hitbox.down_face_down);
						}
						if (sim_hitbox.hit_status != HIT_STATUS_LAUNCH && sim_hitbox.collision_kind_air) {
							ImGui::Checkbox(("Continue Launch" + internal_label).c_str(), &sim_hitbox.continue_launch);
						}
						if (sim_hitbox.collision_kind_air || sim_hitbox.hit_status == HIT_STATUS_LAUNCH
							|| (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE && sim_hitbox.critical_status == HIT_STATUS_LAUNCH)) {
							ImGui::InputInt(("Juggle Start" + internal_label).c_str(), &sim_hitbox.juggle_start);
							if (sim_hitbox.collision_kind_air) {
								ImGui::InputInt(("Juggle Increase" + internal_label).c_str(), &sim_hitbox.juggle_increase);
								ImGui::InputInt(("Juggle Max" + internal_label).c_str(), &sim_hitbox.juggle_max);
							}
						}
						if (ImGui::BeginCombo(("Hit Height" + internal_label).c_str(), current_hit_height)) {
							for (size_t i2 = 0; i2 < 3; i2++) {
								bool selected = current_hit_height == hit_heights[i2];
								if (ImGui::Selectable(hit_heights[i2], selected)) {
									current_hit_height = hit_heights[i2];
									sim_hitbox.hit_height = (HitHeight)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputFloat(("Damage" + internal_label).c_str(), &sim_hitbox.damage);
						if (sim_hitbox.collision_kind_ground) {
							ImGui::InputFloat(("Chip Damage" + internal_label).c_str(), &sim_hitbox.chip_damage);
						}
						ImGui::InputInt(("Damage Scale" + internal_label).c_str(), &sim_hitbox.damage_scale);
						ImGui::InputFloat(("Meter Gain" + internal_label).c_str(), &sim_hitbox.meter_gain);

						ImGui::InputInt(("Hitlag" + internal_label).c_str(), &sim_hitbox.hitlag);
						if (sim_hitbox.collision_kind_ground) {
							ImGui::InputInt(("Blocklag" + internal_label).c_str(), &sim_hitbox.blocklag);
						}
						ImGui::InputInt(("Hit Advantage" + internal_label).c_str(), &sim_hitbox.hit_advantage);
						if (sim_hitbox.collision_kind_ground) {
							ImGui::InputInt(("Block Advantage" + internal_label).c_str(), &sim_hitbox.block_advantage);
						}
						if (sim_hitbox.hit_status == HIT_STATUS_NORMAL || sim_hitbox.hit_status == HIT_STATUS_LAUNCH
							|| (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE &&
								(sim_hitbox.critical_status == HIT_STATUS_NORMAL || sim_hitbox.critical_status == HIT_STATUS_LAUNCH))) {
							ImGui::Checkbox(("Disable Hitstun Parry" + internal_label).c_str(), &sim_hitbox.disable_hitstun_parry);
						}
						if (sim_hitbox.hit_status == HIT_STATUS_NORMAL || (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE
							&& sim_hitbox.critical_status == HIT_STATUS_NORMAL)){
							if (sim_hitbox.collision_kind_ground) {
								ImGui::InputFloat(("Pushback Ground Hit" + internal_label).c_str(), &sim_hitbox.pushback_ground_hit);
								ImGui::InputFloat(("Pushback Ground Block" + internal_label).c_str(), &sim_hitbox.pushback_ground_block);
							}
							if (sim_hitbox.collision_kind_air) {
								ImGui::InputFloat(("Pushback Air X" + internal_label).c_str(), &sim_hitbox.pushback_air_x);
								ImGui::InputFloat(("Pushback Air Y" + internal_label).c_str(), &sim_hitbox.pushback_air_y);
							}
							ImGui::InputInt(("Pushback Frames" + internal_label).c_str(), &sim_hitbox.pushback_frames);
						}
						else if (sim_hitbox.collision_kind_ground) {
							ImGui::InputFloat(("Pushback Ground Block" + internal_label).c_str(), &sim_hitbox.pushback_ground_block);
							ImGui::InputInt(("Pushback Frames" + internal_label).c_str(), &sim_hitbox.pushback_frames);
						}
						if (sim_hitbox.hit_status == HIT_STATUS_LAUNCH || sim_hitbox.hit_status == HIT_STATUS_FLOAT
							|| (sim_hitbox.critical_condition != CRITICAL_CONDITION_NONE && (sim_hitbox.critical_status == HIT_STATUS_LAUNCH
							|| sim_hitbox.critical_status == HIT_STATUS_FLOAT)) || sim_hitbox.continue_launch) {
							ImGui::Checkbox(("Has Launch Target Pos" + internal_label).c_str(), &sim_hitbox.has_launch_target_pos);
							if (sim_hitbox.has_launch_target_pos) {
								ImGui::InputFloat(("Launch Target X" + internal_label).c_str(), &sim_hitbox.launch_target_pos.x);
								ImGui::InputFloat(("Launch Target Y" + internal_label).c_str(), &sim_hitbox.launch_target_pos.y);
								ImGui::InputFloat(("Launch Target Frames" + internal_label).c_str(), &sim_hitbox.launch_target_pos.z);
							}
							else {
								ImGui::InputFloat(("Launch Init Y Speed" + internal_label).c_str(), &sim_hitbox.launch_init_y);
								ImGui::InputFloat(("Launch Gravity" + internal_label).c_str(), &sim_hitbox.launch_gravity_y);
								ImGui::InputFloat(("Launch Max Fall Speed" + internal_label).c_str(), &sim_hitbox.launch_max_fall_speed);
								ImGui::InputFloat(("Launch X Speed" + internal_label).c_str(), &sim_hitbox.launch_speed_x);
							}
						}
						
						if (ImGui::BeginCombo(("Damage Kind" + internal_label).c_str(), current_damage_kind)) {
							for (size_t i2 = 0; i2 < 4; i2++) {
								bool selected = current_damage_kind == damage_kinds[i2];
								if (ImGui::Selectable(damage_kinds[i2], selected)) {
									current_damage_kind = damage_kinds[i2];
									sim_hitbox.damage_kind = (DamageKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputText(("Hit Effect" + internal_label).c_str(), &sim_hitbox.hit_effect);
						ImGui::InputText(("Hit Sound" + internal_label).c_str(), &sim_hitbox.hit_sound);
						if (ImGui::Button(("Start On This Frame" + internal_label).c_str())) {
							sim_hitbox.frame_start = battle->active_hitbox_object->frame;
						}
						ImGui::SameLine();
						if (ImGui::Button(("End On This Frame" + internal_label).c_str())) {
							sim_hitbox.frame_end = battle->active_hitbox_object->frame;
						}

						if (ImGui::Button(("Activate" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &sim_hitbox;
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Print" + internal_label).c_str())) {
							sim_hitbox.print_start(battle->active_hitbox_object);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Delete" + internal_label).c_str())) {
							std::vector<SimHitbox>::iterator it = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.begin();
							std::advance(it, i);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.erase(it);
						}
						ImGui::TreePop();
					}
				}
				if (ImGui::Button("New Hitbox")) {
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.push_back(SimHitbox());
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box) {
						battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
					}
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.back();
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->id = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes.size() - 1;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->frame_start = battle->active_hitbox_object->frame;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Hurtboxes")) {
				for (size_t i = 0, max = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.size(); i < max; i++) {
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_start > battle->active_hitbox_object->frame
						|| (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_end <= battle->active_hitbox_object->frame
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_end > battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_start)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Hurtbox ID " + index;
					std::string internal_label = "##hurtbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						const char* current_hurtbox_kind = hurtbox_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].hurtbox_kind];
						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].id);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].relative_offset.y);
						if (ImGui::BeginCombo(("Hurtbox Kind" + internal_label).c_str(), current_hurtbox_kind)) {
							for (size_t i2 = 0; i2 < 3; i2++) {
								bool selected = current_hurtbox_kind == hurtbox_kinds[i2];
								if (ImGui::Selectable(hurtbox_kinds[i2], selected)) {
									current_hurtbox_kind = hurtbox_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].hurtbox_kind = (HurtboxKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputInt(("Armor Hits" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].armor_hits);
						ImGui::Checkbox(("High Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_high);
						ImGui::Checkbox(("Mid Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_mid);
						ImGui::Checkbox(("Low Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_low);
						ImGui::Checkbox(("Aerial Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_aerial);
						ImGui::Checkbox(("Throw Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_throw);
						ImGui::Checkbox(("Projectile Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_projectile);
						ImGui::Checkbox(("Invincible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_invincible);
						ImGui::Checkbox(("Soft Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind_soft);

						if (ImGui::Button(("Start On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_start = battle->active_hitbox_object->frame;
						}
						ImGui::SameLine();
						if (ImGui::Button(("End On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_end = battle->active_hitbox_object->frame;
						}
						if (ImGui::Button(("Activate" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i];
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Print" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].print_start(battle->active_hitbox_object);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Delete" + internal_label).c_str())) {
							std::vector<SimHurtbox>::iterator it = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.begin();
							std::advance(it, i);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.erase(it);
						}


						ImGui::TreePop();
					}
				}
				if (ImGui::Button("New Hurtbox")) {
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.push_back(SimHurtbox());
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box) {
						battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
					}
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.back();
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->id = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes.size() - 1;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->frame_start = battle->active_hitbox_object->frame;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Grabboxes")) {
				for (size_t i = 0, max = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.size(); i < max; i++) {
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_start > battle->active_hitbox_object->frame
						|| (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end <= battle->active_hitbox_object->frame
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end > battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Grabbox ID " + index;
					std::string internal_label = "##grabbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].id);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_offset.y);
						ImGui::Checkbox(("Untechable" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].grabbox_kind_notech);
						ImGui::Checkbox(("Connects in Hitstun" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].grabbox_kind_hitstun);
						ImGui::Checkbox(("Hits Grounded" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_ground);
						ImGui::Checkbox(("Hits Aerial" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_air);
						ImGui::Checkbox(("Hits OTG" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_down);
						ImGui::Checkbox(("Hits Projectiles" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_projectile);
						ImGui::Checkbox(("Hits through Soft Intangible" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_soft_intangible);
						ImGui::Checkbox(("Hits through Armor" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].collision_kind_armor);
						ImGui::InputText(("Attacker Status" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].attacker_status);
						ImGui::InputText(("Defender Status" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].defender_status);

						if (ImGui::Button(("Start On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_start = battle->active_hitbox_object->frame;
						}
						ImGui::SameLine();
						if (ImGui::Button(("End On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end = battle->active_hitbox_object->frame;
						}

						if (ImGui::Button(("Activate" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i];
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Print" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].print_start(battle->active_hitbox_object);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Delete" + internal_label).c_str())) {
							std::vector<SimGrabbox>::iterator it = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.begin();
							std::advance(it, i);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.erase(it);
						}
	
						ImGui::TreePop();
					}
				}
				if (ImGui::Button("New Grabbox")) {
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.push_back(SimGrabbox());
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box) {
						battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
					}
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.back();
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->id = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes.size() - 1;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->frame_start = battle->active_hitbox_object->frame;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Pushboxes")) {
				for (size_t i = 0, max = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.size(); i < max; i++) {
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_start > battle->active_hitbox_object->frame 
						|| (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_end <= battle->active_hitbox_object->frame
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_end > battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_start)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Pushbox ID " + index;
					std::string internal_label = "##pushbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].id);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].relative_offset.y);
						if (ImGui::Button(("Start On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_start = battle->active_hitbox_object->frame;
						}
						ImGui::SameLine();
						if (ImGui::Button(("End On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_end = battle->active_hitbox_object->frame;
						}
						if (ImGui::Button(("Activate" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i];
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Print" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].print_start(battle->active_hitbox_object);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Delete" + internal_label).c_str())) {
							std::vector<SimPushbox>::iterator it = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.begin();
							std::advance(it, i);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.erase(it);
						}
						ImGui::TreePop();
					}
				}
				if (ImGui::Button("New Pushbox")) {
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.push_back(SimPushbox());
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box) {
						battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
					}
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.back();
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->id = battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes.size() - 1;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->frame_start = battle->active_hitbox_object->frame;
					battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
				}
				ImGui::TreePop();
			}
			if (ImGui::Button("Print All")) {
				battle->hitbox_sim[battle->active_hitbox_object->get_anim()].print_all(battle->active_hitbox_object);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera")) {
			bool update_view = false;
			bool update_aim = false;
			bool update_ypr = false;
			if (ImGui::DragFloat("Pos X", &window_manager->camera.pos[0], 0.01)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::DragFloat("Pos Y", &window_manager->camera.pos[1], 0.01)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::DragFloat("Pos Z", &window_manager->camera.pos[2], 0.01)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::SliderFloat("Yaw", &window_manager->camera.yaw, -180.0f, 180.0f)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::SliderFloat("Pitch", &window_manager->camera.pitch, -180.0f, 180.0f)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::SliderFloat("Roll", &window_manager->camera.roll, -180.0f, 180.0f)) {
				update_aim = true;
				update_view = true;
			}
			if (ImGui::DragFloat("Aim X", &window_manager->camera.aim[0], 0.01)) {
				update_ypr = true;
				update_view = true;
			}
			if (ImGui::DragFloat("Aim Y", &window_manager->camera.aim[1], 0.01)) {
				update_ypr = true;
				update_view = true;
			}
			if (ImGui::DragFloat("Aim Z", &window_manager->camera.aim[2], 0.01)) {
				update_ypr = true;
				update_view = true;
			}
			if (ImGui::SliderFloat("FOV", &window_manager->camera.fov, 0.0f, 45.0f)) {
				update_view = true;
			}
			if (ImGui::Checkbox("Camera Locked", &window_manager->camera.camera_locked)) {
				update_view = true;
			}
			if (update_aim) {
				window_manager->camera.calc_aim_from_ypr();
			}
			if (update_ypr) {
				window_manager->camera.calc_ypr_from_aim();
			}
			if (update_view) {
				window_manager->camera.update_view();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Players")) {
			if (ImGui::TreeNode("P1")) {
				glm::vec3 pos = battle->fighter[0]->get_pos();
				ImGui::SliderFloat("Pos X", &pos.x, -3000.0f, 3000.0f);
				ImGui::SliderFloat("Pos Y", &pos.y, -3000.0f, 3000.0f);
				ImGui::SliderFloat("Pos Z", &pos.z, -3000.0f, 3000.0f);
				battle->fighter[0]->set_pos(pos);
				glm::vec3 rot = battle->fighter[0]->get_rot();
				ImGui::SliderFloat("Rot X", &rot.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Rot Y", &rot.y, -180.0f, 180.0f);
				ImGui::SliderFloat("Rot Z", &rot.z, -180.0f, 180.0f);
				battle->fighter[0]->set_rot(rot);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("P2")) {
				glm::vec3 pos = battle->fighter[1]->get_pos();
				ImGui::SliderFloat("Pos X", &pos.x, -3000.0f, 3000.0f);
				ImGui::SliderFloat("Pos Y", &pos.y, -3000.0f, 3000.0f);
				ImGui::SliderFloat("Pos Z", &pos.z, -3000.0f, 3000.0f);
				battle->fighter[1]->set_pos(pos);
				glm::vec3 rot = battle->fighter[1]->get_rot();
				ImGui::SliderFloat("Rot X", &rot.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Rot Y", &rot.y, -180.0f, 180.0f);
				ImGui::SliderFloat("Rot Z", &rot.z, -180.0f, 180.0f);
				battle->fighter[1]->set_rot(rot);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rendering")) {
#ifdef DEBUG
			ImGui::Checkbox("Framecap", &GameManager::get_instance()->frame_capped);
#endif
			for (int i2 = 0; i2 < window_manager->lights.size(); i2++) {
				std::string light_name = "Light [" + std::to_string(i2) + "]";

				if (ImGui::TreeNode(light_name.c_str())) {
					ImGui::SliderFloat((light_name + ".X").c_str(), &window_manager->lights[i2].position[0], -150.0f, 150.0f);
					ImGui::SliderFloat((light_name + ".Y").c_str(), &window_manager->lights[i2].position[1], -150.0f, 150.0f);
					ImGui::SliderFloat((light_name + ".Z").c_str(), &window_manager->lights[i2].position[2], -150.0f, 150.0f);
					ImGui::SliderFloat((light_name + ".R").c_str(), &window_manager->lights[i2].color[0], 0.0f, 1.0f);
					ImGui::SliderFloat((light_name + ".G").c_str(), &window_manager->lights[i2].color[1], 0.0f, 1.0f);
					ImGui::SliderFloat((light_name + ".B").c_str(), &window_manager->lights[i2].color[2], 0.0f, 1.0f);
					ImGui::SliderFloat((light_name + " Brightness").c_str(), &window_manager->lights[i2].brightness, 0.0f, 20.0f);
					ImGui::Checkbox((light_name).c_str(), &window_manager->lights[i2].enabled);
					ImGui::TreePop();
				}
			}
			if (ImGui::SliderFloat("HDR Exposure", &window_manager->hdr_exposure, 0.0f, 10.0f)) {
				window_manager->hdr_buffer.shader->use();
				window_manager->hdr_buffer.shader->set_float("exposure", window_manager->hdr_exposure);
			}
			ImGui::SliderFloat("Shadow Depth", &window_manager->shadow_depth, 0.0f, 256.0f);
			window_manager->update_shader_lights();

			bool diffuse_enabled = window_manager->g_buffer.shader->features & SHADER_FEAT_DIFFUSE;
			bool specular_enabled = window_manager->g_buffer.shader->features & SHADER_FEAT_SPECULAR;
			bool normals_enabled = window_manager->normals_enabled;
			bool ssao_enabled = window_manager->g_buffer.shader->features & SHADER_FEAT_SSAO;
			bool outlines_enabled = window_manager->g_buffer.shader->features & SHADER_FEAT_OUTLINE;
			bool bloom_enabled = window_manager->hdr_buffer.shader->features & SHADER_FEAT_BLOOM;
			ImGui::Checkbox("Diffuse Enabled", &diffuse_enabled);
			ImGui::Checkbox("Specular Enabled", &specular_enabled);
			ImGui::Checkbox("Normal Enabled", &normals_enabled);
			ImGui::Checkbox("SSAO Enabled", &ssao_enabled);
			ImGui::Checkbox("Outlines Enabled", &outlines_enabled);
			ImGui::Checkbox("Bloom Enabled", &bloom_enabled);
			if (ImGui::Button("Print SSAO Vals")) {
				std::cout << "SSAO Samples:\n";
				for (size_t i = 0, max = window_manager->ssao_samples.size(); i < max; i++) {
					std::cout << window_manager->ssao_samples[i].x << ", "
						<< window_manager->ssao_samples[i].y << ", "
						<< window_manager->ssao_samples[i].z << "\n";
				}
				std::cout << "SSAO Noise:\n";
				for (size_t i = 0, max = window_manager->ssao_noise.size(); i < max; i++) {
					std::cout << window_manager->ssao_noise[i].x << ", "
						<< window_manager->ssao_noise[i].y << ", "
						<< window_manager->ssao_noise[i].z << "\n";
				}
			}
			if (diffuse_enabled != (bool)(window_manager->g_buffer.shader->features & SHADER_FEAT_DIFFUSE)
			|| specular_enabled != (bool)(window_manager->g_buffer.shader->features & SHADER_FEAT_SPECULAR)
			|| outlines_enabled != (bool)(window_manager->g_buffer.shader->features & SHADER_FEAT_OUTLINE)
			|| ssao_enabled != (bool)(window_manager->g_buffer.shader->features & SHADER_FEAT_SSAO)) {
				unsigned int add = 0;
				unsigned int remove = 0;
				if (diffuse_enabled) {
					add |= SHADER_FEAT_DIFFUSE;
				}
				else {
					remove |= SHADER_FEAT_DIFFUSE;
				}
				if (specular_enabled) {
					add |= SHADER_FEAT_SPECULAR;
				}
				else {
					remove |= SHADER_FEAT_SPECULAR;
				}
				if (outlines_enabled) {
					add |= SHADER_FEAT_OUTLINE;
				}
				else {
					remove |= SHADER_FEAT_OUTLINE;
				}
				if (ssao_enabled) {
					add |= SHADER_FEAT_SSAO;
				}
				else {
					remove |= SHADER_FEAT_SSAO;
				}
				window_manager->g_buffer.set_feats(remove, add);
			}
			if (bloom_enabled != (bool)(window_manager->hdr_buffer.shader->features & SHADER_FEAT_BLOOM)) {
				unsigned int add = 0;
				unsigned int remove = 0;
				if (bloom_enabled) {
					add |= SHADER_FEAT_BLOOM;
				}
				else {
					remove |= SHADER_FEAT_BLOOM;
				}
				window_manager->hdr_buffer.set_feats(remove, add);
				window_manager->hdr_buffer.shader->use();
				window_manager->hdr_buffer.shader->set_float("exposure", window_manager->hdr_exposure);
			}
			if (normals_enabled != window_manager->normals_enabled) {
				ObjectManager* object_manager = ObjectManager::get_instance();
				ShaderManager* shader_manager = ShaderManager::get_instance();
				unsigned int add = 0;
				unsigned int remove = 0;
				if (normals_enabled) {
					add |= SHADER_FEAT_NORMAL;
				}
				else {
					remove |= SHADER_FEAT_NORMAL;
				}
				for (GameObject* object : object_manager->game_objects) {
					if (!object->shader) continue;
					object->shader = shader_manager->get_shader_switch_features(object->shader, remove, add);
				}
				window_manager->normals_enabled = normals_enabled;
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}