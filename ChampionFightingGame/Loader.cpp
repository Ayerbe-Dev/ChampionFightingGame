#include "Loader.h"
#include "SDL Helpers.h"
#include "utils.h"
#include <stdlib.h>

GameLoader::GameLoader() {
	total_items = 0;
}

GameLoader::GameLoader(int total_items) {
	this->total_items = total_items;
}

void GameLoader::loading_screen() {
	RenderManager* render_manager = RenderManager::get_instance();
	SDL_Texture* screen_texture = SDL_CreateTexture(render_manager->sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);

	LoadIcon load_icon;
	GameTextureSDL splash_screen("resource/misc/loading/splashload.png");
	GameTextureSDL loading_flavor("resource/misc/loading/FlavorBar.png");
	GameTextureSDL loading_bar("resource/misc/loading/loadingbar.png");

	loading_flavor.set_pos(glm::vec2(0.0, -480.0));
	loading_bar.set_pos(glm::vec2(0.0, -480.0));
	loading_bar.scale_right_percent(0.0);

	while (!finished) {
		wait_ms();

		load_icon.move();
		file_mutex.lock();

		SDL_RenderClear(render_manager->sdl_renderer);
		SDL_SetRenderTarget(render_manager->sdl_renderer, screen_texture);
		
		splash_screen.render();
		loading_bar.set_right_target(loaded_items / (float)total_items, 12);
		loading_bar.process();
		loading_bar.render();
		loading_flavor.render();

		load_icon.texture.render();

		SDL_SetRenderTarget(render_manager->sdl_renderer, nullptr);
		SDL_RenderCopy(render_manager->sdl_renderer, screen_texture, nullptr, nullptr);
		SDL_RenderPresent(render_manager->sdl_renderer);

		file_mutex.unlock();
	}
	splash_screen.destroy();
	loading_flavor.destroy();
	loading_bar.destroy();
	load_icon.texture.render();

	SDL_DestroyTexture(screen_texture);

	file_mutex.lock();
	can_ret = true;
	file_mutex.unlock();
}

LoadIcon::LoadIcon() {
	texture.init("resource/misc/loading/loadicon.png");
	texture.scale_all_percent(0.4, false);

	panic_setting = rng(0, 1);

	set_attributes();
	while (check_corner_distance()) {
		set_attributes();
	}
}

void LoadIcon::set_attributes() {
	int v_rng = rng(0, 1);
	int h_rng = rng(0, 1);
	int x_rng = rng(-SDL_WINDOW_WIDTH * 0.8, SDL_WINDOW_WIDTH * 0.8);
	int y_rng = rng(-SDL_WINDOW_HEIGHT * 0.8, SDL_WINDOW_HEIGHT * 0.8);
	move_right = h_rng;
	move_down = v_rng;
	texture.set_pos(glm::vec2(x_rng, y_rng));
}

void LoadIcon::move() {
	if (!panic_mode) {
		texture.add_pos(glm::vec2(7 * (move_right ? 1 : -1), 7 * (move_down ? -1 : 1)));
	}
	else {
		switch (panic_setting) {
			case (PANIC_SETTING_STOP): {
				texture.add_pos(glm::vec2(panic_speed * (move_right ? 1 : -1), panic_speed * (move_down ? -1 : 1)));
				panic_speed = clampf(0.0, panic_speed - 0.4, panic_speed);
			} break;
			case (PANIC_SETTING_FLY):
			default: {
				panic_timer--;
				texture.add_pos(glm::vec2(23 * (move_right ? 1 : -1), panic_speed * (move_down ? -1 : 1)));
				panic_speed += panic_add;
				panic_add += 0.1;
			} break;
		}
	}

	if (texture.pos.x > SDL_WINDOW_WIDTH || texture.pos.x < -SDL_WINDOW_WIDTH) {
		if ((!panic_mode) || panic_timer <= 0) {
			move_right = !move_right;
			texture.pos.x = clampf(-SDL_WINDOW_WIDTH, texture.pos.x, SDL_WINDOW_WIDTH);
			panic_mode = false;
		}
	}
	if (texture.pos.y > SDL_WINDOW_HEIGHT || texture.pos.y < -SDL_WINDOW_HEIGHT) {
		if ((!panic_mode) || panic_timer <= 0) {
			move_down = !move_down;
			texture.pos.y = clampf(-SDL_WINDOW_HEIGHT, texture.pos.y, SDL_WINDOW_HEIGHT);
			panic_mode = false;
		}
	}
	if (check_corner_distance() && !panic_mode) {
		switch (panic_setting) {
			case (PANIC_SETTING_STOP): {
				panic_speed = 5.0;
				panic_add = 0.0;
			} break;
			case (PANIC_SETTING_FLY):
			default: {
				panic_timer = 30;
				panic_speed = -5.0;
				panic_add = 0.0;

				move_down = !move_down;
				move_right = !move_right;
			} break;
		}
		panic_mode = true;
	}
}

bool LoadIcon::check_corner_distance() {
	bool will_hit_wall = false;
	bool will_hit_ceiling = false;
	int panic_distance;
	switch (panic_setting) {
		case (PANIC_SETTING_STOP): {
			panic_distance = 200;
		} break;
		case (PANIC_SETTING_FLY):
		default: {
			panic_distance = 80;
		} break;
	}

	if ((texture.pos.x < -SDL_WINDOW_WIDTH + panic_distance && !move_right)
	|| (texture.pos.x > SDL_WINDOW_WIDTH - panic_distance && move_right)) {
		will_hit_wall = true;
	}
	if ((texture.pos.y < -SDL_WINDOW_HEIGHT + panic_distance && move_down)
	|| (texture.pos.y > SDL_WINDOW_HEIGHT - panic_distance && !move_down)) {
		will_hit_ceiling = true;
	}
	float x_relative = abs(texture.pos.x) / SDL_WINDOW_WIDTH * 1000;
	float y_relative = abs(texture.pos.y) / SDL_WINDOW_HEIGHT * 1000;
	bool close_enough = abs(std::max(x_relative, y_relative) - std::min(x_relative, y_relative)) <= 10.0;

	return will_hit_wall && will_hit_ceiling && close_enough;
}