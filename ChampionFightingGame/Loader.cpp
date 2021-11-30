#include "Loader.h"

LoadIcon::LoadIcon() {
	texture.init("resource/ui/menu/loading/loadicon.png");
	texture.setScaleFactor(0.4);

	panic_setting = rand() % 2;

	set_attributes();
	while (check_corner_distance(true)) {
		set_attributes();
	}
}

void LoadIcon::set_attributes() {
	int v_rng = rand() % 2;
	int h_rng = rand() % 2;
	int x_rng = WINDOW_WIDTH * 0.2 + (rand() % WINDOW_WIDTH * 0.6);
	int y_rng = WINDOW_HEIGHT * 0.2 + rand() % WINDOW_HEIGHT * 0.6;
	move_right = h_rng;
	move_down = v_rng;
	texture.destRect.x = x_rng;
	texture.destRect.y = y_rng;
	texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
}

void LoadIcon::move() {
	if (!panic_mode) {
		texture.destRect.x += 7 * (move_right ? 1 : -1);
		texture.destRect.y += 7 * (move_down ? 1 : -1);
	}
	else if (panic_setting) {
		if (panic_v) {
			texture.destRect.x += 17 * (move_right ? 1 : -1);
			texture.destRect.y += panic_speed * (move_down ? 1 : -1);
			panic_speed += panic_add;
			panic_add += 0.1;
		}
		else {
			texture.destRect.x += 17 * (move_right ? 1 : -1);
			texture.destRect.y += panic_speed * (move_down ? 1 : -1);
			panic_speed += panic_add;
			panic_add += 0.1;
		}
	}
	else {
		texture.destRect.x += panic_speed * (move_right ? 1 : -1);
		texture.destRect.y += panic_speed * (move_down ? 1 : -1);
		panic_speed = clampf(0.0, panic_speed - 0.4, panic_speed);
	}

	if (texture.destRect.x > WINDOW_WIDTH || texture.destRect.x < 0) {
		move_right = !move_right;
		texture.destRect.x = (texture.destRect.x > WINDOW_WIDTH ? WINDOW_WIDTH : 0);
		if (panic_setting) {
			panic_mode = false;
		}
	}
	if (texture.destRect.y > WINDOW_HEIGHT || texture.destRect.y < 0) {
		move_down = !move_down;
		texture.destRect.y = (texture.destRect.y > WINDOW_HEIGHT ? WINDOW_HEIGHT : 0);
		if (panic_setting) {
			panic_mode = false;
		}
	}
	check_corner_distance(false);
}

bool LoadIcon::check_corner_distance(bool init) {
	bool will_hit_wall = false;
	bool will_hit_ceiling = false;
	int panic_distance = 80;
	if (!panic_setting) {
		panic_distance = 200;
	}
	if ((texture.destRect.x < panic_distance && !move_right)
	|| texture.destRect.x > WINDOW_WIDTH - panic_distance && move_right) {
		will_hit_wall = true;
	}
	if ((texture.destRect.y < panic_distance && !move_down)
	|| texture.destRect.y > WINDOW_HEIGHT - panic_distance && move_down) {
		will_hit_ceiling = true;
	}
	bool close_enough = abs((texture.destRect.y * 1.78) - texture.destRect.x) < 75 || abs((texture.destRect.y * 1.78) - texture.destRect.x) > (WINDOW_WIDTH - 75);
	
	if (!init && (will_hit_wall && will_hit_ceiling) && close_enough) {
		if (panic_setting) {
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
		else {
			if (!panic_mode) {
				panic_speed = 5.0;
				panic_add = 0.0;
				panic_mode = true;
			}
		}
	}
	return will_hit_wall && will_hit_ceiling && close_enough;
}

BattleLoader::BattleLoader() {

}

BattleLoader::BattleLoader(PlayerInfo p1, PlayerInfo p2) {
	this->player_info[0] = p1;
	this->player_info[1] = p2;
}