#include "Debugger.h"
#include "Fighter.h"
#include "GameRect.h"
#include "ParamAccessor.h"
#include "RenderManager.h"
#include "DebugMenu.h"
#include "utils.h"

Debugger::Debugger() {
	button_info[BUTTON_DEBUG_ENABLE].k_mapping = SDL_SCANCODE_LSHIFT;
	button_info[BUTTON_DEBUG_ADVANCE].k_mapping = SDL_SCANCODE_LCTRL;
	button_info[BUTTON_DEBUG_UP].k_mapping = SDL_SCANCODE_W;
	button_info[BUTTON_DEBUG_LEFT].k_mapping = SDL_SCANCODE_A;
	button_info[BUTTON_DEBUG_DOWN].k_mapping = SDL_SCANCODE_S;
	button_info[BUTTON_DEBUG_RIGHT].k_mapping = SDL_SCANCODE_D;
	button_info[BUTTON_DEBUG_QUERY].k_mapping = SDL_SCANCODE_1;
	button_info[BUTTON_DEBUG_CHANGE_TARGET].k_mapping = SDL_SCANCODE_2;
	button_info[BUTTON_DEBUG_CENTER_BOX].k_mapping = SDL_SCANCODE_3;
	button_info[BUTTON_DEBUG_MOVE_0].k_mapping = SDL_SCANCODE_4;
	button_info[BUTTON_DEBUG_MOVE_1].k_mapping = SDL_SCANCODE_5;
	button_info[BUTTON_DEBUG_PRINT_POS].k_mapping = SDL_SCANCODE_6;
	button_info[BUTTON_DEBUG_PRINT_FRAME].k_mapping = SDL_SCANCODE_7;
	button_info[BUTTON_DEBUG_ZOOM_OUT].k_mapping = SDL_SCANCODE_8;
	button_info[BUTTON_DEBUG_RESET].k_mapping = SDL_SCANCODE_9;
	button_info[BUTTON_DEBUG_FULLSCREEN].k_mapping = SDL_SCANCODE_ESCAPE;
}

void Debugger::poll_inputs(const Uint8* keyboard_state) {
	for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
		bool old_button = button_info[i].button_on;
		button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
		bool new_button = button_info[i].button_on;
		button_info[i].changed = (old_button != new_button);
	}
}

bool Debugger::check_button_on(unsigned int button) {
	return button_info[button].button_on && enabled;
}

bool Debugger::check_button_trigger(unsigned int button) {
	return button_info[button].changed && button_info[button].button_on && enabled;
}

void Debugger::debug_mode(Fighter* target, GameRect* debug_rect, glm::vec2* debug_anchor, glm::vec2* debug_offset) {
	if (check_button_trigger(BUTTON_DEBUG_CENTER_BOX)) {
		debug_anchor->x = (((target->pos.x * target->facing_dir)) * target->facing_dir) + WINDOW_WIDTH / 2;
		debug_anchor->y = WINDOW_HEIGHT - target->pos.y;
		debug_offset->x = (((target->pos.x * target->facing_dir)) * target->facing_dir) + WINDOW_WIDTH / 2;
		debug_offset->y = WINDOW_HEIGHT - target->pos.y;
	}
	if (check_button_on(BUTTON_DEBUG_MOVE_0)) {
		if (target->facing_right) {
			if (check_button_on(BUTTON_DEBUG_RIGHT)) {
				debug_anchor->x += 1;
			}
			if (check_button_on(BUTTON_DEBUG_LEFT)) {
				debug_anchor->x -= 1;
			}
		}
		else {
			if (check_button_on(BUTTON_DEBUG_RIGHT)) {
				debug_offset->x += 1;
			}
			if (check_button_on(BUTTON_DEBUG_LEFT)) {
				debug_offset->x -= 1;
			}
		}
		if (check_button_on(BUTTON_DEBUG_UP)) {
			debug_anchor->y += 1;
		}
		if (check_button_on(BUTTON_DEBUG_DOWN)) {
			debug_anchor->y -= 1;
		}
	}
	if (check_button_on(BUTTON_DEBUG_MOVE_1)) {
		if (target->facing_right) {
			if (check_button_on(BUTTON_DEBUG_RIGHT)) {
				debug_offset->x += 1;
			}
			if (check_button_on(BUTTON_DEBUG_LEFT)) {
				debug_offset->x -= 1;
			}
		}
		else {
			if (check_button_on(BUTTON_DEBUG_RIGHT)) {
				debug_anchor->x += 1;
			}
			if (check_button_on(BUTTON_DEBUG_LEFT)) {
				debug_anchor->x -= 1;
			}
		}
		if (check_button_on(BUTTON_DEBUG_UP)) {
			debug_offset->y += 1;
		}
		if (check_button_on(BUTTON_DEBUG_DOWN)) {
			debug_offset->y -= 1;
		}
	}
	debug_rect->update_corners(*debug_anchor, *debug_offset);

	if (check_button_on(BUTTON_DEBUG_PRINT_POS)) {
		SDL_Rect temp_rect;
		int x_anchor = (debug_anchor->x - (target->pos.x + WINDOW_WIDTH / 2)) * target->facing_dir;
		int x_offset = (debug_offset->x - (target->pos.x + WINDOW_WIDTH / 2)) * target->facing_dir;
		temp_rect.y = (debug_anchor->y - WINDOW_HEIGHT) * -1.0 - target->pos.y;
		temp_rect.h = (debug_offset->y - WINDOW_HEIGHT) * -1.0 - target->pos.y;
		if (target->facing_right) {
			temp_rect.x = x_anchor;
			temp_rect.w = x_offset;
		}
		else {
			temp_rect.x = x_offset;
			temp_rect.w = x_anchor;
		}

		printf(" GameCoordinate{ %d, %d }, GameCoordinate{ %d, %d }\n", temp_rect.x,temp_rect.y,temp_rect.w,temp_rect.h);
	}
	if (check_button_trigger(BUTTON_DEBUG_PRINT_FRAME)) {
		print_frames = !print_frames;
	}
	if (check_button_trigger(BUTTON_DEBUG_ZOOM_OUT)) {
		zoom = !zoom;
	}
}

void Debugger::print_commands() {
	std::cout << "Debug Command List:" << "\n";

	std::cout << "add_hp: Add a specified amount to the target's HP" << "\n";
	std::cout << "set_hp: Set the target's HP" << "\n";
	std::cout << "max_hp: Max out the target's HP" << "\n";
	std::cout << "max_hp_all: Max out all fighters' HP" << "\n";
	std::cout << "add_ex: Add to the EX Meter for the current target by a specified amount" << "\n";
	std::cout << "set_ex: Set the EX Meter for the current target" << "\n";
	std::cout << "max_ex: Max out the EX Meter for the current target" << "\n";
	std::cout << "max_ex_all: Max out the EX Meter for all fighters" << "\n";
	std::cout << "reload_moves: Apply any changes to move scripts for all fighters" << "\n";

	std::cout << "\n" << "Enter Command: ";
}

void Debugger::debug_query(std::string command, Fighter* target, Fighter* not_target) {
	if (command == "add_hp") {
		float add;
		std::cout << "Enter HP Difference: ";
		std::cin >> add;
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, target->fighter_float[FIGHTER_FLOAT_HEALTH] + add, target->get_local_param_float("health"));
	}
	if (command == "set_hp") {
		float set;
		std::cout << "Enter New HP: ";
		std::cin >> set;
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, set, target->get_local_param_float("health"));
	}
	if (command == "max_hp") {
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = target->get_local_param_float("health");
	}
	if (command == "max_hp_all") {
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = target->get_local_param_float("health");
		not_target->fighter_float[FIGHTER_FLOAT_HEALTH] = not_target->get_local_param_float("health");
	}
	if (command == "add_ex") {
		float add;
		std::cout << "Enter Meter Difference: ";
		std::cin >> add;
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, target->fighter_float[FIGHTER_FLOAT_SUPER_METER] + add, get_param_int("ex_meter_size", PARAM_FIGHTER));
	}
	if (command == "set_ex") {
		float set;
		std::cout << "Enter New Meter Amount: ";
		std::cin >> set;
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, set, get_param_int("ex_meter_size", PARAM_FIGHTER));
	}
	if (command == "max_ex") {
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = get_param_int("ex_meter_size", PARAM_FIGHTER);
	}
	if (command == "max_ex_all") {
		int ex_meter_size = get_param_int("ex_meter_size", PARAM_FIGHTER);
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = ex_meter_size;
		not_target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = ex_meter_size;
	}
	if (command == "reload_moves") {
		target->wipe_scripts();
		target->load_move_scripts();
		target->update_hitbox_pos();
		target->update_hurtbox_pos();
		target->update_grabbox_pos();

		not_target->wipe_scripts();
		not_target->load_move_scripts();
		not_target->update_hitbox_pos();
		not_target->update_hurtbox_pos();
		not_target->update_grabbox_pos();
	}
}

void cotr_imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
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
	
	if (ImGui::MenuItem("Debug Menu", "This screen")) {
		game_manager->update_state(GAME_STATE_DEBUG_MENU);
		game_manager->looping[game_manager->layer] = false;
	}
	if (ImGui::MenuItem("1v1 Game", "With Default settings")) {
		game_manager->update_state(GAME_STATE_BATTLE);
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
		//ImGui::PlotLines("Frame Times", ftime, IM_ARRAYSIZE(ftime));

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

		if (ImGui::TreeNode("Shadow Light")) { //tbh this block doesn't really make sense anymore since
			//by design it should always be the same as render_manager->lights[0]
			ImGui::DragFloat("Shadow Camera X", &render_manager->shadow_map.light_pos.x, 0.01);
			ImGui::DragFloat("Shadow Camera Y", &render_manager->shadow_map.light_pos.y, 0.01);
			ImGui::DragFloat("Shadow Camera Z", &render_manager->shadow_map.light_pos.z, 0.01);
			
			ImGui::DragFloat("fov", &render_manager->shadow_map.fov, 0.01);
			ImGui::DragFloat("depth", &render_manager->shadow_map.depth, 0.01);

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
					ImGui::SliderFloat((light_name + ".X").c_str(), &render_manager->lights[i2]->position[0], -15.0f, 15.0f);
					ImGui::SliderFloat((light_name + ".Y").c_str(), &render_manager->lights[i2]->position[1], -15.0f, 15.0f);
					ImGui::SliderFloat((light_name + ".Z").c_str(), &render_manager->lights[i2]->position[2], -15.0f, 15.0f);
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
