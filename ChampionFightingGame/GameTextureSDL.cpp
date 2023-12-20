#include "GameTextureSDL.h"
#include "SDL Helpers.h"
#include "RenderManager.h"
#include "WindowConstants.h"
#include "utils.h"

GameTextureSDL::GameTextureSDL() {
	src_rect = SDL_Rect();
	dest_rect = SDL_Rect();
	texture = nullptr;
	h_flipped = false;
	v_flipped = false;
	pos = glm::vec2(0.0);
	base_width = 0;
	base_height = 0;
	whole_texture = false;
	whole_screen = false;
}

GameTextureSDL::GameTextureSDL(std::string path) {
	init(path);
}

void GameTextureSDL::init(std::string path) {
	texture = loadSDLTexture(path);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	dest_rect.x = 0;
	dest_rect.y = 0;
	src_rect.x = 0;
	src_rect.y = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &base_width, &base_height);
	dest_rect.w = base_width;
	dest_rect.h = base_height;
	src_rect.w = base_width;
	src_rect.h = base_height;
}

void GameTextureSDL::process() {
	if (target_left_frames != 0.0) {
		scale_left_percent(left_percent + target_left_crop);
		target_left_frames--;
	}
	if (target_right_frames != 0.0) {
		scale_right_percent(right_percent + target_right_crop);
		target_right_frames--;
	}
	if (target_top_frames != 0.0) {
		scale_top_percent(top_percent + target_top_crop);
		target_top_frames--;
	}
	if (target_bottom_frames != 0.0) {
		scale_bottom_percent(bottom_percent + target_bottom_crop);
		target_bottom_frames--;
	}
	if (target_pos_frames != glm::vec2(0.0)) {
		pos += target_pos_per_frame;
		target_pos_frames -= glm::vec2(1.0);
	}
}

void GameTextureSDL::destroy() {
	SDL_DestroyTexture(texture);
}

void GameTextureSDL::flip_h() {
	h_flipped = !h_flipped;
}

void GameTextureSDL::flip_v() {
	v_flipped = !v_flipped;
}

void GameTextureSDL::set_pos(glm::vec2 pos) {
	this->pos = pos;
}

void GameTextureSDL::add_pos(glm::vec2 pos) {
	this->pos += pos;
}

void GameTextureSDL::set_orientation(int orientation) {
	this->orientation = orientation;
}

void GameTextureSDL::scale_left_percent(float percent, bool crop) {
	if (h_flipped) {
		right_percent = percent;
		if (crop) {
			right_crop = percent;
		}
	}
	else {
		left_percent = percent;
		if (crop) {
			left_crop = percent;
		}
	}
}

void GameTextureSDL::scale_right_percent(float percent, bool crop) {
	if (h_flipped) {
		left_percent = percent;
		if (crop) {
			left_crop = percent;
		}
	}
	else {
		right_percent = percent;
		if (crop) {
			right_crop = percent;
		}
	}
}

void GameTextureSDL::scale_top_percent(float percent, bool crop) {
	if (v_flipped) {
		bottom_percent = percent;
		if (crop) {
			bottom_crop = percent;
		}
	}
	else {
		top_percent = percent;
		if (crop) {
			top_crop = percent;
		}
	}
}

void GameTextureSDL::scale_bottom_percent(float percent, bool crop) {
	if (v_flipped) {
		top_percent = percent;
		if (crop) {
			top_crop = percent;
		}
	}
	else {
		bottom_percent = percent;
		if (crop) {
			bottom_crop = percent;
		}
	}
}

void GameTextureSDL::scale_all_percent(float percent, bool crop) {
	scale_left_percent(percent, crop);
	scale_right_percent(percent, crop);
	scale_top_percent(percent, crop);
	scale_bottom_percent(percent, crop);
}

void GameTextureSDL::set_left_target(float percent, float frames) {
	if (prev_left_target[0] == percent && prev_left_target[1] == frames) {
		return;
	}
	target_left_crop = (percent - left_percent) / frames;
	target_left_frames = frames;
	prev_left_target[0] = percent;
	prev_left_target[1] = frames;
}

void GameTextureSDL::set_right_target(float percent, float frames) {
	if (prev_right_target[0] == percent && prev_right_target[1] == frames) {
		return;
	}
	target_right_crop = (percent - right_percent) / frames;
	target_right_frames = frames;
	prev_right_target[0] = percent;
	prev_right_target[1] = frames;
}

void GameTextureSDL::set_top_target(float percent, float frames) {
	if (prev_top_target[0] == percent && prev_top_target[1] == frames) {
		return;
	}
	target_top_crop = (percent - top_percent) / frames;
	target_top_frames = frames;
	prev_top_target[0] = percent;
	prev_top_target[1] = frames;
}

void GameTextureSDL::set_bottom_target(float percent, float frames) {
	if (prev_bottom_target[0] == percent && prev_bottom_target[1] == frames) {
		return;
	}
	target_bottom_crop = (percent - bottom_percent) / frames;
	target_bottom_frames = frames;
	prev_bottom_target[0] = percent;
	prev_bottom_target[1] = frames;
}

void GameTextureSDL::set_target_pos(glm::vec2 target, float frames) {
	if (frames == 0.0) {
		return;
	}
	this->target_pos_per_frame = (target_pos_per_frame - pos) / glm::vec2(frames);
	this->target_pos_frames = glm::vec2(frames);
}

FORCE_INLINE glm::vec2 GameTextureSDL::get_pos_offset(glm::vec2 offset) {
	glm::vec2 ret;
	ret.x = (pos.x - offset.x / 2) + (WINDOW_WIDTH / 2.0) - (base_width / 2.0);
	ret.y = (((pos.y + offset.y / 2) + WINDOW_HEIGHT) * -1.0 + WINDOW_HEIGHT) + (WINDOW_HEIGHT / 2.0) - (base_height / 2.0);
	return ret;
}

bool test = false;

void GameTextureSDL::render() {
	SDL_Rect sdl_dest = dest_rect;
	SDL_Rect sdl_src = src_rect;
	SDL_Rect* render_src = &sdl_src;
	SDL_Rect* render_dest = &sdl_dest;
	scale_left_percent_internal(render_dest, left_percent);
	scale_left_percent_internal(render_src, left_crop);
	scale_right_percent_internal(render_dest, right_percent);
	scale_right_percent_internal(render_src, right_crop);
	scale_top_percent_internal(render_dest, top_percent);
	scale_top_percent_internal(render_src, top_crop);
	scale_bottom_percent_internal(render_dest, bottom_percent);
	scale_bottom_percent_internal(render_src, bottom_crop);
	glm::vec2 rect_offset = get_pos_offset(glm::vec2(sdl_dest.x, sdl_dest.y));
	sdl_dest.x += rect_offset.x;
	sdl_dest.y += rect_offset.y;
	if (whole_texture) {
		render_src = nullptr;
	}
	if (whole_screen) {
		render_dest = nullptr;
	}
	SDL_RenderCopyEx(RenderManager::get_instance()->sdl_renderer, texture, render_src, render_dest, 0, nullptr, get_flip_kind(h_flipped, v_flipped));
}

void GameTextureSDL::scale_left_percent_internal(SDL_Rect* rect, float percent) {
	if (percent < 0.0) {
		return;
	}
	if (percent > 1.0) {
		int scale_factor = (base_width * percent) - base_width;
		rect->x -= scale_factor;
		rect->w += scale_factor;
	}
	else {
		percent = 1.0 - percent;
		int scale_factor = base_width * percent;
		rect->x += scale_factor;
		rect->w -= scale_factor;
		test = false;
	}
}

void GameTextureSDL::scale_right_percent_internal(SDL_Rect* rect, float percent) {
	if (percent < 0.0) {
		return;
	}
	int scale_factor = base_width * percent;
	rect->w = scale_factor;
	test = false;
}

void GameTextureSDL::scale_top_percent_internal(SDL_Rect* rect, float percent) {
	if (percent < 0.0) {
		return;
	}
	if (percent > 1.0) {
		int scale_factor = (base_height * percent) - base_height;
		rect->y -= scale_factor;
		rect->h += scale_factor;
	}
	else {
		percent = 1.0 - percent;
		int scale_factor = base_height * percent;
		rect->y += scale_factor;
		rect->h -= scale_factor;
	}
}

void GameTextureSDL::scale_bottom_percent_internal(SDL_Rect* rect, float percent) {
	if (percent < 0.0) {
		return;
	}
	int scale_factor = base_height * percent;
	rect->h = scale_factor;
}