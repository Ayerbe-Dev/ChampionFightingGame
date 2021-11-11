#include "Loader.h"

LoadIcon::LoadIcon() {
	texture.init("resource/ui/menu/loadicon.png");
	texture.setScaleFactor(0.2);

	set_attributes();
	while (!check_corner_distance(true)) {
		set_attributes();
	}
}

void LoadIcon::set_attributes() {
	int v_rng = rand() % 2;
	int h_rng = rand() % 2;
	int x_rng = rand() % WINDOW_WIDTH;
	int y_rng = rand() % WINDOW_HEIGHT;
	move_right = h_rng;
	move_down = v_rng;
	texture.destRect.x = x_rng;
	texture.destRect.y = y_rng;
	texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
}

void LoadIcon::move() {
	if (!panic_mode) {
		texture.destRect.x += 5 * (move_right ? 1 : -1);
		texture.destRect.y += 5 * (move_down ? 1 : -1);
	}
	else if (panic_v) {
		texture.destRect.x += 10 * (move_right ? 1 : -1);
		texture.destRect.y += panic_speed * (move_down ? 1 : -1);
		panic_speed += panic_add;
		panic_add += 0.1;
	}
	else {
		texture.destRect.x += 10 * (move_right ? 1 : -1);
		texture.destRect.y += panic_speed * (move_down ? 1 : -1);
		panic_speed += panic_add;
		panic_add += 0.1;
	}

	if (texture.destRect.x > WINDOW_WIDTH || texture.destRect.x < 0) {
		move_right = !move_right;
		texture.destRect.x = (texture.destRect.x > WINDOW_WIDTH ? WINDOW_WIDTH : 0);
		panic_mode = false;
	}
	if (texture.destRect.y > WINDOW_HEIGHT || texture.destRect.y < 0) {
		move_down = !move_down;
		texture.destRect.y = (texture.destRect.y > WINDOW_HEIGHT ? WINDOW_HEIGHT : 0);
		panic_mode = false;
	}
	check_corner_distance(false);
}

bool LoadIcon::check_corner_distance(bool init) {
	bool will_hit_wall = false;
	bool will_hit_ceiling = false;
	if ((texture.destRect.x < 120 && !move_right)
	|| texture.destRect.x > WINDOW_WIDTH - 120 && move_right) {
		will_hit_wall = true;
	}
	if ((texture.destRect.y < 120 && !move_down)
	|| texture.destRect.y > WINDOW_HEIGHT - 120 && move_down) {
		will_hit_ceiling = true;
	}
	SDL_PumpEvents();
	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
	if (!init && (will_hit_wall && will_hit_ceiling) || keyboard_state[SDL_SCANCODE_0]) {
		panic_speed = -5.0;
		panic_add = 0.0;
		if (!panic_mode) {
			int rng = rand() % 2;
			if (rng) {
				panic_v = true;
			}
			else {
				panic_v = false;
			}

			move_down = !move_down;
			move_right = !move_right;
			panic_mode = true;
		}
	}
	return will_hit_wall && will_hit_ceiling;
}

GameLoader::GameLoader() {

}

GameLoader::GameLoader(PlayerInfo p1, PlayerInfo p2) {
	this->player_info[0] = p1;
	this->player_info[1] = p2;
}