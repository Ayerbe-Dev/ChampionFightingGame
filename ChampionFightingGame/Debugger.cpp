#include "Debugger.h"
#include "Fighter.h"

extern SDL_Renderer* g_renderer;


Debugger::Debugger() {
	button_info[BUTTON_DEBUG_ENABLE].mapping = SDL_SCANCODE_LSHIFT;
	button_info[BUTTON_DEBUG_ADVANCE].mapping = SDL_SCANCODE_LCTRL;
	button_info[BUTTON_DEBUG_UP].mapping = SDL_SCANCODE_W;
	button_info[BUTTON_DEBUG_LEFT].mapping = SDL_SCANCODE_A;
	button_info[BUTTON_DEBUG_DOWN].mapping = SDL_SCANCODE_S;
	button_info[BUTTON_DEBUG_RIGHT].mapping = SDL_SCANCODE_D;
	button_info[BUTTON_DEBUG_QUERY].mapping = SDL_SCANCODE_1;
	button_info[BUTTON_DEBUG_CHANGE_TARGET].mapping = SDL_SCANCODE_2;
	button_info[BUTTON_DEBUG_CENTER_BOX].mapping = SDL_SCANCODE_3;
	button_info[BUTTON_DEBUG_MOVE_0].mapping = SDL_SCANCODE_4;
	button_info[BUTTON_DEBUG_MOVE_1].mapping = SDL_SCANCODE_5;
	button_info[BUTTON_DEBUG_PRINT_POS].mapping = SDL_SCANCODE_6;
	button_info[BUTTON_DEBUG_PRINT_FRAME].mapping = SDL_SCANCODE_7;
	button_info[BUTTON_DEBUG_ZOOM_OUT].mapping = SDL_SCANCODE_8;
	button_info[BUTTON_DEBUG_RESET].mapping = SDL_SCANCODE_9;
	button_info[BUTTON_DEBUG_FULLSCREEN].mapping = SDL_SCANCODE_ESCAPE;
}

void Debugger::poll_inputs(const Uint8* keyboard_state) {
	for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
		bool old_button = button_info[i].button_on;
		button_info[i].button_on = keyboard_state[button_info[i].mapping];
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

void Debugger::debug_mode(Fighter* target, GameRect* debug_rect, vec2* debug_anchor, vec2* debug_offset) {
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
	cout << "Debug Command List:" << endl;

	cout << "add_hp: Add a specified amount to the target's HP" << endl;
	cout << "set_hp: Set the target's HP" << endl;
	cout << "max_hp: Max out the target's HP" << endl;
	cout << "max_hp_all: Max out all fighters' HP" << endl;
	cout << "add_ex: Add to the EX Meter for the current target by a specified amount" << endl;
	cout << "set_ex: Set the EX Meter for the current target" << endl;
	cout << "max_ex: Max out the EX Meter for the current target" << endl;
	cout << "max_ex_all: Max out the EX Meter for all fighters" << endl;
	cout << "reload_moves: Apply any changes to move scripts for all fighters" << endl;

	cout << endl << "Enter Command: ";
}

void Debugger::debug_query(string command, Fighter* target, Fighter* not_target) {
	if (command == "add_hp") {
		float add;
		cout << "Enter HP Difference: ";
		cin >> add;
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, target->fighter_float[FIGHTER_FLOAT_HEALTH] + add, target->get_param_float("health"));
	}
	if (command == "set_hp") {
		float set;
		cout << "Enter New HP: ";
		cin >> set;
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0, set, target->get_param_float("health"));
	}
	if (command == "max_hp") {
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = target->get_param_float("health");
	}
	if (command == "max_hp_all") {
		target->fighter_float[FIGHTER_FLOAT_HEALTH] = target->get_param_float("health");
		not_target->fighter_float[FIGHTER_FLOAT_HEALTH] = not_target->get_param_float("health");
	}
	if (command == "add_ex") {
		float add;
		cout << "Enter Meter Difference: ";
		cin >> add;
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, target->fighter_float[FIGHTER_FLOAT_SUPER_METER] + add, EX_METER_SIZE);
	}
	if (command == "set_ex") {
		float set;
		cout << "Enter New Meter Amount: ";
		cin >> set;
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, set, EX_METER_SIZE);
	}
	if (command == "max_ex") {
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = EX_METER_SIZE;
	}
	if (command == "max_ex_all") {
		target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = EX_METER_SIZE;
		not_target->fighter_float[FIGHTER_FLOAT_SUPER_METER] = EX_METER_SIZE;
	}
	if (command == "reload_moves") {
		target->wipe_scripts();
		target->load_move_scripts();
		target->attempted_excutes = 0;
		target->excute_count = 0;
		target->last_excute_frame = 0;
		target->move_script.move_script();
		target->update_hitbox_pos();
		target->update_hurtbox_pos();
		target->update_grabbox_pos();

		not_target->wipe_scripts();
		not_target->load_move_scripts();
		not_target->attempted_excutes = 0;
		not_target->excute_count = 0;
		not_target->last_excute_frame = 0;
		not_target->move_script.move_script();
		not_target->update_hitbox_pos();
		not_target->update_hurtbox_pos();
		not_target->update_grabbox_pos();
	}
}