#include "debugger.h"



Debugger::Debugger() {
	button_info[BUTTON_DEBUG_ENABLE].mapping = SDL_SCANCODE_LSHIFT;
	button_info[BUTTON_DEBUG_ADVANCE].mapping = SDL_SCANCODE_LCTRL;
	button_info[BUTTON_DEBUG_UP].mapping = SDL_SCANCODE_W;
	button_info[BUTTON_DEBUG_LEFT].mapping = SDL_SCANCODE_A;
	button_info[BUTTON_DEBUG_DOWN].mapping = SDL_SCANCODE_S;
	button_info[BUTTON_DEBUG_RIGHT].mapping = SDL_SCANCODE_D;
	button_info[BUTTON_DEBUG_PICK_1].mapping = SDL_SCANCODE_1;
	button_info[BUTTON_DEBUG_PICK_2].mapping = SDL_SCANCODE_2;
	button_info[BUTTON_DEBUG_CENTER_BOX].mapping = SDL_SCANCODE_3;
	button_info[BUTTON_DEBUG_MOVE_0].mapping = SDL_SCANCODE_4;
	button_info[BUTTON_DEBUG_MOVE_1].mapping = SDL_SCANCODE_5;
	button_info[BUTTON_DEBUG_PRINT_POS].mapping = SDL_SCANCODE_6;
	button_info[BUTTON_DEBUG_PRINT_FRAME].mapping = SDL_SCANCODE_7;
	button_info[BUTTON_DEBUG_ZOOM_OUT].mapping = SDL_SCANCODE_8;
	button_info[BUTTON_DEBUG_RESET].mapping = SDL_SCANCODE_9;
	button_info[BUTTON_DEBUG_FULLSCREEN].mapping = SDL_SCANCODE_ESCAPE;
}

bool Debugger::check_button_on(u32 button) {
	return button_info[button].button_on && enabled;
}

bool Debugger::check_button_trigger(u32 button) {
	return button_info[button].changed && button_info[button].button_on && enabled;
}

void debug_mode(Debugger *debugger, FighterInstance *target, SDL_Renderer *renderer, SDL_Rect *debug_rect, GameCoordinate *debug_anchor, GameCoordinate *debug_offset) {
	if (debugger->check_button_trigger(BUTTON_DEBUG_CENTER_BOX)) {
		debug_anchor->x = (((target->pos.x * target->facing_dir)) * target->facing_dir) + WINDOW_WIDTH / 2;
		debug_anchor->y = WINDOW_HEIGHT - target->pos.y;
		debug_offset->x = (((target->pos.x * target->facing_dir)) * target->facing_dir) + WINDOW_WIDTH / 2;
		debug_offset->y = WINDOW_HEIGHT - target->pos.y;
	}
	if (debugger->check_button_on(BUTTON_DEBUG_MOVE_0)) {
		if (debugger->check_button_on(BUTTON_DEBUG_RIGHT)) {
			debug_anchor->x += 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_LEFT)) {
			debug_anchor->x -= 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_UP)) {
			debug_anchor->y -= 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_DOWN)) {
			debug_anchor->y += 1;
		}
	}
	if (debugger->check_button_on(BUTTON_DEBUG_MOVE_1)) {
		if (debugger->check_button_on(BUTTON_DEBUG_RIGHT)) {
			debug_offset->x += 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_LEFT)) {
			debug_offset->x -= 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_UP)) {
			debug_offset->y -= 1;
		}
		if (debugger->check_button_on(BUTTON_DEBUG_DOWN)) {
			debug_offset->y += 1;
		}
	}
	debug_rect->x = debug_anchor->x;
	debug_rect->y = debug_anchor->y;
	debug_rect->w = debug_offset->x;
	debug_rect->h = debug_offset->y;
	debug_rect->w -= debug_rect->x;
	debug_rect->h -= debug_rect->y;
	if (debugger->check_button_on(BUTTON_DEBUG_PRINT_POS)) {
		SDL_Rect temp_rect;
		temp_rect.x = ((debug_anchor->x - (target->pos.x + WINDOW_WIDTH / 2 * target->facing_dir)));
		temp_rect.y = (debug_anchor->y - WINDOW_HEIGHT) * -1.0 - target->pos.y;
		temp_rect.w = ((debug_offset->x - (target->pos.x + WINDOW_WIDTH / 2 * target->facing_dir)));
		temp_rect.h = (debug_offset->y - WINDOW_HEIGHT) * -1.0 - target->pos.y;
		printf(" GameCoordinate{ %d, %d }, GameCoordinate{ %d, %d }\n",temp_rect.x,temp_rect.y,temp_rect.w,temp_rect.h);
	}
	if (debugger->check_button_trigger(BUTTON_DEBUG_PRINT_FRAME)) {
		debugger->print_frames = !debugger->print_frames;
	}
	if (debugger->check_button_trigger(BUTTON_DEBUG_ZOOM_OUT)) {
		debugger->zoom = !debugger->zoom;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, debug_rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
	SDL_RenderFillRect(renderer, debug_rect);
}