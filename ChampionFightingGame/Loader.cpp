#include "Loader.h"
#include "SDL Helpers.h"
#include <stdlib.h>

GameTextureSDL::GameTextureSDL() {
	src_rect = SDL_Rect();
	dest_rect = SDL_Rect();
	texture = nullptr;
	percent = 0;
	target_percent = -1;
	target_rate = -1;
	target_frames = 1;
	anchor_mode = GAME_TEXTURE_ANCHOR_MODE_DEFAULT;
	vertical_scale = 1.0;
	horizontal_scale = 1.0;
	flip = TEXTURE_FLIP_KIND_NONE;
	drain_kind = METER_DRAIN_KIND_NONE;
	loaded = false;
}

bool GameTextureSDL::init(std::string sTexturePath, bool delay) {
	if (loaded) {
		printf("GameTexture already initialized!\n");
		return false;
	}
	texture = loadSDLTexture(sTexturePath.c_str(), delay);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	dest_rect.x = 0;
	dest_rect.y = 0;
	src_rect.x = 0;
	src_rect.y = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h);
	src_rect.h = dest_rect.h;
	src_rect.w = dest_rect.w;
	loaded = true;
	return true;
}

bool GameTextureSDL::render() {
	if (!loaded) {
		return false;
	}

	if (target_percent != -1.0) {
		change_percent();
	}

	SDL_Rect tmpDestRect = dest_rect;
	SDL_Rect tmpSrcRect = src_rect;
	tmpDestRect.w *= horizontal_scale;
	tmpDestRect.h *= vertical_scale;


	switch (anchor_mode) {
		case (GAME_TEXTURE_ANCHOR_MODE_CENTER):
		{
			tmpDestRect.x -= tmpDestRect.w / 2;
			tmpDestRect.y -= tmpDestRect.h / 2;
		}
		break;
		case (GAME_TEXTURE_ANCHOR_MODE_DEFAULT):
		{
			if (flip) {
				tmpDestRect.x -= tmpDestRect.w;
			}
		}
		break;
		case (GAME_TEXTURE_ANCHOR_MODE_METER):
		{
			tmpDestRect.w = dest_rect.w * (percent);
			tmpDestRect.h *= vertical_scale;
			tmpSrcRect.w = tmpDestRect.w;
			if (drain_kind != METER_DRAIN_KIND_NONE) {
				tmpDestRect.x += WINDOW_WIDTH / 2;
				if (drain_kind == METER_DRAIN_KIND_RIGHT) {
					tmpDestRect.x -= tmpSrcRect.w;
				}
				tmpSrcRect.x += dest_rect.w - tmpSrcRect.w;
			}
			else {
				if (flip == TEXTURE_FLIP_KIND_DRAIN) {
					tmpDestRect.x += WINDOW_WIDTH;
					tmpSrcRect.x += WINDOW_WIDTH;
					tmpDestRect.x -= tmpDestRect.w;
					tmpSrcRect.x -= tmpSrcRect.w;
				}
				if (flip == TEXTURE_FLIP_KIND_NO_DRAIN) {
					tmpDestRect.x += WINDOW_WIDTH;
					tmpDestRect.x -= tmpDestRect.w;
				}
			}
		}
		break;
	}
	SDL_RenderCopyEx(g_renderer,
		texture,
		anchor_mode == GAME_TEXTURE_ANCHOR_MODE_METER ? &tmpSrcRect : nullptr,
		anchor_mode == GAME_TEXTURE_ANCHOR_MODE_BACKGROUND ? nullptr : &tmpDestRect,
		0,
		NULL,
		flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
	);

	return true;
}

void GameTextureSDL::setScaleFactor(float fScaleFactor) {
	this->vertical_scale = fScaleFactor;
	this->horizontal_scale = fScaleFactor;
}

void GameTextureSDL::setHorizontalScaleFactor(float fScaleFactor) {
	this->horizontal_scale = fScaleFactor;
}

void GameTextureSDL::setVerticalScaleFactor(float fScaleFactor) {
	this->vertical_scale = fScaleFactor;
}

void GameTextureSDL::clearTexture() {
	SDL_DestroyTexture(texture);
}

float GameTextureSDL::getScaledWidth() {
	return dest_rect.w * horizontal_scale;
}

float GameTextureSDL::getScaledHeight() {
	return dest_rect.h * vertical_scale;
}

void GameTextureSDL::setAnchorMode(int iMode) {
	anchor_mode = iMode;
}

void GameTextureSDL::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
}

void GameTextureSDL::setPercent(float percent) {
	this->percent = percent;
	if (anchor_mode != GAME_TEXTURE_ANCHOR_MODE_METER) {
		printf("WARNING: GameTexture is using the setPercent() function but its not in the correct mode!\n");
	}
}

void GameTextureSDL::setTargetPercent(float percent, float rate, int frames) {
	target_percent = percent;
	target_rate = rate;
	target_frames = frames;
	if (anchor_mode != GAME_TEXTURE_ANCHOR_MODE_METER) {
		printf("WARNING: GameTexture is using the setTargetPercent() function but its not in the correct mode!\n");
	}
}

void GameTextureSDL::change_percent(float rate) {
	if (rate == -1.0) {
		rate = target_rate;
	}
	if (rate == -1.0) {
		std::cout << "WARNING: Target rate was not set through setTargetPercent, but rate was not given a non-default arg!" << "\n";
	}

	if (target_percent != percent) {
		if (target_percent < percent) {
			percent = clampf(target_percent, percent - (rate / target_frames), percent);
		}
		else {
			percent = clampf(percent, percent + (rate / target_frames), target_percent);
		}
	}
	else {
		target_percent = -1.0;
		target_rate = -1.0;
	}
}

void GameTextureSDL::setFlip(int flip) {
	this->flip = flip;
}

int GameTextureSDL::getFlipKind() {
	return flip;
}

void GameTextureSDL::setDrainKind(int meter_drain_kind) {
	drain_kind = meter_drain_kind;
}

GameLoader::GameLoader() {}
GameLoader::GameLoader(int total_items) {
	this->total_items = total_items;
}

LoadIcon::LoadIcon() {
	texture.init("resource/ui/menu/loading/loadicon.png");
	texture.setScaleFactor(0.4);

	panic_setting = rng(0, 1);

	set_attributes();
	while (check_corner_distance(true)) {
		set_attributes();
	}
}

void LoadIcon::set_attributes() {
	int v_rng = rng(0, 1);
	int h_rng = rng(0, 1);
	int x_rng = WINDOW_WIDTH * 0.2 + rng(WINDOW_WIDTH * 0.2, WINDOW_WIDTH * 0.8);
	int y_rng = WINDOW_HEIGHT * 0.2 + rng(WINDOW_HEIGHT * 0.2, WINDOW_HEIGHT * 0.8);
	move_right = h_rng;
	move_down = v_rng;
	texture.dest_rect.x = x_rng;
	texture.dest_rect.y = y_rng;
	texture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
}

void LoadIcon::move() {
	if (!panic_mode) {
		texture.dest_rect.x += 7 * (move_right ? 1 : -1);
		texture.dest_rect.y += 7 * (move_down ? 1 : -1);
	}
	else if (panic_setting) {
		if (panic_v) {
			texture.dest_rect.x += 17 * (move_right ? 1 : -1);
			texture.dest_rect.y += panic_speed * (move_down ? 1 : -1);
			panic_speed += panic_add;
			panic_add += 0.1;
		}
		else {
			texture.dest_rect.x += 17 * (move_right ? 1 : -1);
			texture.dest_rect.y += panic_speed * (move_down ? 1 : -1);
			panic_speed += panic_add;
			panic_add += 0.1;
		}
	}
	else {
		texture.dest_rect.x += panic_speed * (move_right ? 1 : -1);
		texture.dest_rect.y += panic_speed * (move_down ? 1 : -1);
		panic_speed = clampf(0.0, panic_speed - 0.4, panic_speed);
	}

	if (texture.dest_rect.x > WINDOW_WIDTH || texture.dest_rect.x < 0) {
		move_right = !move_right;
		texture.dest_rect.x = (texture.dest_rect.x > WINDOW_WIDTH ? WINDOW_WIDTH : 0);
		if (panic_setting) {
			panic_mode = false;
		}
	}
	if (!(panic_mode && !panic_setting && panic_speed > 3.0)) {
		if (texture.dest_rect.y > WINDOW_HEIGHT || texture.dest_rect.y < 0) {
			move_down = !move_down;
			texture.dest_rect.y = (texture.dest_rect.y > WINDOW_HEIGHT ? WINDOW_HEIGHT : 0);
			if (panic_setting) {
				panic_mode = false;
			}
		}
		check_corner_distance(false);
	}
}

bool LoadIcon::check_corner_distance(bool init) {
	bool will_hit_wall = false;
	bool will_hit_ceiling = false;
	int panic_distance = 80;
	if (!panic_setting) {
		panic_distance = 200;
	}
	if ((texture.dest_rect.x < panic_distance && !move_right)
	|| texture.dest_rect.x > WINDOW_WIDTH - panic_distance && move_right) {
		will_hit_wall = true;
	}
	if ((texture.dest_rect.y < panic_distance && !move_down)
	|| texture.dest_rect.y > WINDOW_HEIGHT - panic_distance && move_down) {
		will_hit_ceiling = true;
	}
	bool close_enough = abs((texture.dest_rect.y * 1.78) - texture.dest_rect.x) < 75 || abs((texture.dest_rect.y * 1.78) - texture.dest_rect.x) > (WINDOW_WIDTH - 75);
	
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