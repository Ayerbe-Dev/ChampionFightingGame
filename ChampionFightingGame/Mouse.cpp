#include "Mouse.h"
#include "RenderManager.h"

MouseButton::MouseButton() {
	mapping = 0;
	button_on = false;
	button_changed = false;
}

Mouse::Mouse() {
	buttons[MOUSE_BUTTON_M1].mapping = SDL_BUTTON_LEFT;
	buttons[MOUSE_BUTTON_M2].mapping = SDL_BUTTON_RIGHT; //SDL_BUTTON_RIGHT being 3 and not 2 fills
	buttons[MOUSE_BUTTON_M3].mapping = SDL_BUTTON_MIDDLE; //me with indescribable rage over the fact
	buttons[MOUSE_BUTTON_M4].mapping = SDL_BUTTON_X1; //over the fact that we can't use a for loop here
	buttons[MOUSE_BUTTON_M5].mapping = SDL_BUTTON_X2;
	pos = glm::ivec2(0);
}

void Mouse::poll_buttons() {
	Uint32 mouse_state = SDL_GetMouseState(&pos.x, &pos.y);
	for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
		bool prev_on = buttons[i].button_on;
		buttons[i].button_on = (mouse_state & SDL_BUTTON(buttons[i].mapping));
		buttons[i].button_changed = (buttons[i].button_on != prev_on);
	}
}

bool Mouse::check_button_on(unsigned int button) {
	return buttons[button].button_on;
}

bool Mouse::check_button_trigger(unsigned int button) {
	return buttons[button].button_on && buttons[button].button_changed;
}

bool Mouse::check_button_release(unsigned int button) {
	return (!buttons[button].button_on) && buttons[button].button_changed;
}

glm::ivec2 Mouse::get_pos() {
	return pos;
}

glm::ivec2 Mouse::get_pos_flip_y() {
	glm::ivec2 ret = pos;
	ret.y = (RenderManager::get_instance()->window_height - ret.y);
	return ret;
}