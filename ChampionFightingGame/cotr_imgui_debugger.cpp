#include "cotr_imgui_debugger.h"
#include "Fighter.h"
#include "GameRect.h"
#include "RenderManager.h"
#include "utils.h"

#include "DebugMenu.h"
#include "Battle.h"
#include "StageSelect.h"

void cotr_imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(RenderManager::get_instance()->window, RenderManager::get_instance()->sdl_context);
	ImGui_ImplOpenGL3_Init();

	io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
}

void cotr_imgui_terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void cotr_imgui_debug_dbmenu(DebugMenu* debug_menu) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(RenderManager::get_instance()->window);
	ImGui::NewFrame();

	GameManager* game_manager = GameManager::get_instance();
	
	ImGui::Begin("Debug Menu\n");		
	
	if (ImGui::MenuItem("Debug Menu")) {
		game_manager->update_state(GAME_STATE_DEBUG_MENU);
	}
	if (ImGui::MenuItem("Training Mode (Battle)")) {
		game_manager->update_state(GAME_STATE_BATTLE, GAME_CONTEXT_TRAINING);
	}
	if (ImGui::MenuItem("1v1 Game")) {
		game_manager->update_state(GAME_STATE_BATTLE, GAME_CONTEXT_NORMAL);
	}
	if (ImGui::MenuItem("Character Select Screen")) {
		game_manager->update_state(GAME_STATE_CHARA_SELECT);
	}
	if (ImGui::MenuItem("Stage Select Screen")) {
		game_manager->update_state(GAME_STATE_STAGE_SELECT);
	}
	if (ImGui::MenuItem("Main Menu")) {
		game_manager->update_state(GAME_STATE_MENU);
	}
	if (ImGui::MenuItem("Title Screen")) {
		game_manager->update_state(GAME_STATE_TITLE_SCREEN);
	}
	if (ImGui::Button("exit")) {
		game_manager->update_state(GAME_STATE_CLOSE);
	}
	for (int i = 0, max = debug_menu->debug_messages.size(); i < max; i++) {
		ImGui::Text("%s", debug_menu->debug_messages[i].c_str());
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cotr_imgui_debug_battle(Battle* battle) {
	RenderManager* render_manager = RenderManager::get_instance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(render_manager->window);
	ImGui::NewFrame();

	ImGui::Begin("Debug Menu");
	{
		if (ImGui::TreeNode("Hitbox Creator")) {
			const char* hit_kinds[6]{
				"HIT_KIND_GROUND",
				"HIT_KIND_AIR",
				"HIT_KIND_DOWN",
				"HIT_KIND_PROJECTILE",
				"HIT_KIND_SOFT_INTANGIBLE",
				"HIT_KIND_ARMOR",
			};
			const char* attack_levels[3]{
				"ATTACK_LEVEL_LIGHT",
				"ATTACK_LEVEL_MEDIUM",
				"ATTACK_LEVEL_HEAVY"
			};
			const char* attack_heights[3]{
				"ATTACK_HEIGHT_HIGH",
				"ATTACK_HEIGHT_MID",
				"ATTACK_HEIGHT_LOW"
			};
			const char* hit_statuses[5]{
				"HIT_STATUS_NORMAL",
				"HIT_STATUS_CRUMPLE",
				"HIT_STATUS_FLOAT",
				"HIT_STATUS_LAUNCH",
				"HIT_STATUS_KNOCKDOWN"
			};
			const char* counterhit_types[6]{
				"COUNTERHIT_TYPE_NONE",
				"COUNTERHIT_TYPE_NORMAL",
				"COUNTERHIT_TYPE_PUNISH",
				"COUNTERHIT_TYPE_COUNTER_ONLY",
				"COUNTERHIT_TYPE_PUNISH_ONLY",
				"COUNTERHIT_TYPE_AERIAL_ONLY"
			};
			const char* clank_kinds[3]{
				"CLANK_KIND_NORMAL",
				"CLANK_KIND_CLANK",
				"CLANK_KIND_CONTINUE"
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
			const char* intangible_kinds[10]{
				"INTANGIBLE_KIND_HIGH",
				"INTANGIBLE_KIND_MID",
				"INTANGIBLE_KIND_LOW",
				"INTANGIBLE_KIND_NONE",
				"INTANGIBLE_KIND_AERIAL",
				"INTANGIBLE_KIND_THROW",
				"INTANGIBLE_KIND_STRIKE",
				"INTANGIBLE_KIND_PROJECTILE",
				"INTANGIBLE_KIND_INVINCIBLE",
				"INTANGIBLE_KIND_ALL"
			};
			const char* grabbox_kinds[4]{
				"GRABBOX_KIND_NORMAL",
				"GRABBOX_KIND_HITSTUN",
				"GRABBOX_KIND_NOTECH",
				"GRABBOX_KIND_HITSTUN_NOTECH"
			};

			std::vector<std::string> items;
			std::vector<BattleObject*> objects;
			items.push_back("Fighter 1");
			items.push_back("Fighter 2");
			objects.push_back(battle->fighter[0]);
			objects.push_back(battle->fighter[1]);
			for (int i = 0; i < 2; i++) {
				for (int i2 = 0; i2 < battle->fighter[i]->num_projectiles; i2++) {
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
					if (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_start > battle->active_hitbox_object->frame
						|| (battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_end <= battle->active_hitbox_object->frame
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_end != -1.0)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Hitbox ID " + index;
					std::string internal_label = "##hitbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						const char* current_hit_kind = hit_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_kind];
						const char* current_attack_level = attack_levels[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].attack_level];
						const char* current_attack_height = attack_heights[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].attack_height];
						const char* current_hit_status = hit_statuses[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_status];
						const char* current_counterhit_status = hit_statuses[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].counterhit_status];
						const char* current_counterhit_type = counterhit_types[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].counterhit_type];
						const char* current_clank_kind = clank_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].clank_kind];
						const char* current_damage_kind = damage_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].damage_kind];

						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].id);
						ImGui::InputInt(("Multihit ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].multihit);
						ImGui::InputFloat(("Damage" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].damage);
						ImGui::InputFloat(("Chip Damage" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].chip_damage);
						ImGui::InputInt(("Damage Scale" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].damage_scale);
						ImGui::InputFloat(("Meter Gain" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].meter_gain);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].relative_offset.y);
						if (ImGui::BeginCombo(("Hit Kind" + internal_label).c_str(), current_hit_kind)) {
							for (size_t i2 = 0; i2 < 6; i2++) {
								bool selected = current_hit_kind == hit_kinds[i2];
								if (ImGui::Selectable(hit_kinds[i2], selected)) {
									current_hit_kind = hit_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_kind = (HitKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Attack Level" + internal_label).c_str(), current_attack_level)) {
							for (size_t i2 = 0; i2 < 3; i2++) {
								bool selected = current_attack_level == attack_levels[i2];
								if (ImGui::Selectable(attack_levels[i2], selected)) {
									current_attack_level = attack_levels[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].attack_level = (AttackLevel)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Attack Height" + internal_label).c_str(), current_attack_height)) {
							for (size_t i2 = 0; i2 < 3; i2++) {
								bool selected = current_attack_height == attack_heights[i2];
								if (ImGui::Selectable(attack_heights[i2], selected)) {
									current_attack_height = attack_heights[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].attack_height = (AttackHeight)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputInt(("Hitlag" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hitlag);
						ImGui::InputInt(("Blocklag" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].blocklag);
						ImGui::InputInt(("Hit Advantage" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_advantage);
						ImGui::InputInt(("Block Advantage" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].block_advantage);
						ImGui::InputFloat(("Hit Pushback" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_pushback);
						ImGui::InputFloat(("Block Pushback" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].block_pushback);
						if (ImGui::BeginCombo(("Hit Status" + internal_label).c_str(), current_hit_status)) {
							for (size_t i2 = 0; i2 < 5; i2++) {
								bool selected = current_hit_status == hit_statuses[i2];
								if (ImGui::Selectable(hit_statuses[i2], selected)) {
									current_hit_status = hit_statuses[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].hit_status = (HitStatus)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Counterhit Status" + internal_label).c_str(), current_counterhit_status)) {
							for (size_t i2 = 0; i2 < 5; i2++) {
								bool selected = current_counterhit_status == hit_statuses[i2];
								if (ImGui::Selectable(hit_statuses[i2], selected)) {
									current_counterhit_status = hit_statuses[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].counterhit_status = (HitStatus)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Counterhit Type" + internal_label).c_str(), current_counterhit_type)) {
							for (size_t i2 = 0; i2 < 6; i2++) {
								bool selected = current_counterhit_type == counterhit_types[i2];
								if (ImGui::Selectable(counterhit_types[i2], selected)) {
									current_counterhit_type = counterhit_types[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].counterhit_type = (CounterhitType)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputInt(("Juggle Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].juggle_start);
						ImGui::InputInt(("Juggle Increase" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].juggle_increase);
						ImGui::InputInt(("Juggle Max" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].juggle_max);
						if (ImGui::BeginCombo(("Clank Kind" + internal_label).c_str(), current_clank_kind)) {
							for (size_t i2 = 0; i2 < 3; i2++) {
								bool selected = current_clank_kind == clank_kinds[i2];
								if (ImGui::Selectable(clank_kinds[i2], selected)) {
									current_clank_kind = clank_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].clank_kind = (ClankKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Damage Kind" + internal_label).c_str(), current_damage_kind)) {
							for (size_t i2 = 0; i2 < 4; i2++) {
								bool selected = current_damage_kind == damage_kinds[i2];
								if (ImGui::Selectable(damage_kinds[i2], selected)) {
									current_damage_kind = damage_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].damage_kind = (DamageKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::Checkbox(("Continue Launch" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].continue_launch);
						ImGui::Checkbox(("Disable Hitstun Parry" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].disable_hitstun_parry);
						ImGui::InputFloat(("Launch Init Y Speed" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].launch_init_y);
						ImGui::InputFloat(("Launch Gravity" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].launch_gravity_y);
						ImGui::InputFloat(("Launch Max Fall Speed" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].launch_max_fall_speed);
						ImGui::InputFloat(("Launch X Speed" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].launch_speed_x);

						if (ImGui::Button(("Start On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_start = battle->active_hitbox_object->frame;
						}
						ImGui::SameLine();
						if (ImGui::Button(("End On This Frame" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].frame_end = battle->active_hitbox_object->frame;
						}

						if (ImGui::Button(("Activate" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(127.0);
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box = &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i];
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].active_box->rect.set_alpha(191.0);
						}
						ImGui::SameLine();
						if (ImGui::Button(("Print" + internal_label).c_str())) {
							battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hitboxes[i].print_start(battle->active_hitbox_object);
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
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].frame_end != -1.0)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Hurtbox ID " + index;
					std::string internal_label = "##hurtbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						const char* current_hurtbox_kind = hurtbox_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].hurtbox_kind];
						const char* current_intangible_kind = intangible_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind];
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
						ImGui::Checkbox(("Armor" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].armor);
						if (ImGui::BeginCombo(("Intangible Kind" + internal_label).c_str(), current_intangible_kind)) {
							for (size_t i2 = 0; i2 < 10; i2++) {
								bool selected = current_intangible_kind == intangible_kinds[i2];
								if (ImGui::Selectable(intangible_kinds[i2], selected)) {
									current_intangible_kind = intangible_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].hurtboxes[i].intangible_kind = (IntangibleKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}

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
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end != -1.0)
						) {
						continue;
					}
					std::string index = std::to_string(i);
					std::string index_label = "Grabbox ID " + index;
					std::string internal_label = "##grabbox" + index;
					if (ImGui::TreeNode(index_label.c_str())) {
						const char* current_grabbox_kind = grabbox_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].grabbox_kind];
						const char* current_hit_kind = hit_kinds[battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].hit_kind];
						ImGui::InputFloat(("Frame Start" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_start);
						ImGui::InputFloat(("Frame End" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].frame_end);
						ImGui::InputInt(("ID" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].id);
						ImGui::InputFloat(("Anchor X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_anchor.x);
						ImGui::InputFloat(("Anchor Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_anchor.y);
						ImGui::InputFloat(("Offset X" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_offset.x);
						ImGui::InputFloat(("Offset Y" + internal_label).c_str(), &battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].relative_offset.y);
						if (ImGui::BeginCombo(("Grabbox Kind" + internal_label).c_str(), current_grabbox_kind)) {
							for (size_t i2 = 0; i2 < 4; i2++) {
								bool selected = current_grabbox_kind == grabbox_kinds[i2];
								if (ImGui::Selectable(grabbox_kinds[i2], selected)) {
									current_grabbox_kind = grabbox_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].grabbox_kind = (GrabboxKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo(("Hit Kind" + internal_label).c_str(), current_hit_kind)) {
							for (size_t i2 = 0; i2 < 6; i2++) {
								bool selected = current_hit_kind == hit_kinds[i2];
								if (ImGui::Selectable(hit_kinds[i2], selected)) {
									current_hit_kind = hit_kinds[i2];
									battle->hitbox_sim[battle->active_hitbox_object->get_anim()].grabboxes[i].hit_kind = (HitKind)i2;
								}
								if (selected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
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
							&& battle->hitbox_sim[battle->active_hitbox_object->get_anim()].pushboxes[i].frame_end != -1.0)
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
			ImGui::DragFloat("Camera X", &render_manager->camera.pos[0], 0.01);
			ImGui::DragFloat("Camera Y", &render_manager->camera.pos[1], 0.01);
			ImGui::DragFloat("Camera Z", &render_manager->camera.pos[2], 0.01);
			if (ImGui::TreeNode("Camera Properties")) {
				ImGui::Checkbox("Auto Camera", &render_manager->camera.following_players);
				ImGui::SliderFloat("Yaw", &render_manager->camera.yaw, -180.0f, 180.0f);
				ImGui::SliderFloat("Pitch", &render_manager->camera.pitch, -180.0f, 180.0f);
				ImGui::SliderFloat("Roll", &render_manager->camera.roll, -180.0f, 180.0f);
				ImGui::SliderFloat("FOV", &render_manager->camera.fov, 0.0f, render_manager->camera.max_fov);
				ImGui::SliderFloat("Auto Yaw Scale", &render_manager->camera.auto_linear_scale, 1.0f, 6.0f);
				ImGui::TreePop();
				render_manager->camera.update_view();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Players")) {
			ImGui::SliderFloat("p1 X", &battle->fighter[0]->pos[0], -3000.0f, 3000.0f);
			ImGui::SliderFloat("p1 Y", &battle->fighter[0]->pos[1], -3000.0f, 3000.0f);
			ImGui::SliderFloat("p1 Z", &battle->fighter[0]->pos[2], -3000.0f, 3000.0f);
			ImGui::SliderFloat("p2 X", &battle->fighter[1]->pos[0], -3000.0f, 3000.0f);
			ImGui::SliderFloat("p2 Y", &battle->fighter[1]->pos[1], -3000.0f, 3000.0f);
			ImGui::SliderFloat("p2 Z", &battle->fighter[1]->pos[2], -3000.0f, 3000.0f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lights")) {
			for (int i2 = 0; i2 < render_manager->lights.size(); i2++) {
				std::string light_name = "Light [" + std::to_string(i2) + "]";

				if (ImGui::TreeNode(light_name.c_str())) {
					ImGui::SliderFloat((light_name + ".X").c_str(), &render_manager->lights[i2]->position[0], -150.0f, 150.0f);
					ImGui::SliderFloat((light_name + ".Y").c_str(), &render_manager->lights[i2]->position[1], -150.0f, 150.0f);
					ImGui::SliderFloat((light_name + ".Z").c_str(), &render_manager->lights[i2]->position[2], -150.0f, 150.0f);
					ImGui::Checkbox((light_name).c_str(), &render_manager->lights[i2]->enabled);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
			render_manager->update_shader_lights();
		}
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}