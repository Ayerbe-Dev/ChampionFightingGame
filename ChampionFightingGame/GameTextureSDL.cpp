#include "GameTextureSDL.h"
#include "SDL Helpers.h"
#include "RenderManager.h"
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
	width_scale = base_width;
	height_scale = base_height;
}

void GameTextureSDL::process() {
	if (target_left_frames != 0.0) {
		float percent = src_rect.w / (float)width_scale;
		scale_left_percent(percent + target_left_crop);
		target_left_frames--;
	}
	if (target_right_frames != 0.0) {
		float percent = src_rect.w / (float)width_scale;
		scale_right_percent(percent + target_right_crop);
		target_right_frames--;
	}
	if (target_top_frames != 0.0) {
		float percent = src_rect.h / (float)height_scale;
		scale_top_percent(percent + target_top_crop);
		target_top_frames--;
	}
	if (target_bottom_frames != 0.0) {
		float percent = src_rect.h / (float)height_scale;
		scale_bottom_percent(percent + target_bottom_crop);
		target_bottom_frames--;
	}
	if (target_pos_frames != glm::vec2(0.0)) {
		pos += target_pos;
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
	if (percent < 0.0) {
		return;
	}
	if (h_flipped) {
		int scale_factor = base_width * percent;
		src_rect.w = scale_factor;
		if (!crop) {
			dest_rect.w = scale_factor;
		}
	}
	else {
		if (percent > 1.0) {
			int scale_factor = (base_width * percent) - base_width;
			src_rect.x = scale_factor * -1;
			src_rect.w = base_width + scale_factor;
			if (!crop) {
				dest_rect.x = scale_factor * -1;
				dest_rect.w = base_width + scale_factor;
			}
		}
		else {
			percent = 1.0 - percent;
			int scale_factor = base_width * percent;

			src_rect.x = scale_factor;
			src_rect.w = base_width - scale_factor;
			if (!crop) {
				dest_rect.x = scale_factor;
				dest_rect.w = base_width - scale_factor;
			}
		}
	}
	width_scale = base_width * percent;
}

void GameTextureSDL::scale_right_percent(float percent, bool crop) {
	if (percent < 0.0) {
		return;
	}
	if (h_flipped) {
		if (percent > 1.0) {
			int scale_factor = (base_width * percent) - base_width;
			src_rect.x = scale_factor * -1;
			src_rect.w = base_width + scale_factor;
			if (!crop) {
				dest_rect.x = scale_factor * -1;
				dest_rect.w = base_width + scale_factor;
			}
		}
		else {
			percent = 1.0 - percent;
			int scale_factor = base_width * percent;
			src_rect.x = scale_factor;
			src_rect.w = base_width - scale_factor;
			if (!crop) {
				dest_rect.x = scale_factor;
				dest_rect.w = base_width - scale_factor;
			}
		}
	}
	else {
		int scale_factor = base_width * percent;
		src_rect.w = scale_factor;
		if (!crop) {
			dest_rect.w = scale_factor;
		}
	}
	width_scale = base_width * percent;
}

void GameTextureSDL::scale_top_percent(float percent, bool crop) {
	if (percent < 0.0) {
		return;
	}
	if (v_flipped) {
		int scale_factor = base_height * percent;
		src_rect.h = scale_factor;
		if (!crop) {
			dest_rect.h = scale_factor;
		}
	}
	else {
		if (percent > 1.0) {
			int scale_factor = (base_height * percent) - base_height;
			src_rect.y = scale_factor * -1;
			if (!crop) {
				dest_rect.y = scale_factor * -1;
			}
		}
		else {
			percent = 1.0 - percent;
			int scale_factor = base_height * percent;
			src_rect.y = scale_factor;
			src_rect.h = base_height - scale_factor;
			if (!crop) {
				dest_rect.y = scale_factor;
				dest_rect.h = base_height - scale_factor;
			}
		}
	}
	height_scale = base_height * percent;
}

void GameTextureSDL::scale_bottom_percent(float percent, bool crop) {
	if (percent < 0.0) {
		return;
	}
	if (v_flipped) {
		if (percent > 1.0) {
			int scale_factor = (base_height * percent) - base_height;
			src_rect.y = scale_factor * -1;
			if (!crop) {
				dest_rect.y = scale_factor * -1;
			}
		}
		else {
			percent = 1.0 - percent;
			int scale_factor = base_height * percent;
			src_rect.y = scale_factor;
			src_rect.h = base_height - scale_factor;
			if (!crop) {
				dest_rect.y = scale_factor;
				dest_rect.h = base_height - scale_factor;
			}
		}
	}
	else {
		int scale_factor = base_height * percent;
		src_rect.h = scale_factor;
		if (!crop) {
			dest_rect.h = scale_factor;
		}
	}
	height_scale = base_height * percent;
}

void GameTextureSDL::scale_all_percent(float percent, bool crop) {
	if (percent < 1.0) {
		scale_left_percent(percent * 2.0, crop);
		scale_right_percent(percent * 2.0, crop);
		scale_top_percent(percent * 2.0, crop);
		scale_bottom_percent(percent * 2.0, crop);
	}
	else {
		scale_left_percent(percent / 2.0, crop);
		scale_right_percent(percent / 2.0, crop);
		scale_top_percent(percent / 2.0, crop);
		scale_bottom_percent(percent / 2.0, crop);
	}
}

void GameTextureSDL::set_left_target(float percent, float frames) {
	float left_percent = 1.0 - (src_rect.w / (float)width_scale);
	target_left_crop = (percent - left_percent) / frames;
	target_left_frames = frames;
}

void GameTextureSDL::set_right_target(float percent, float frames) {
	float right_percent = src_rect.w / (float)width_scale;
	target_right_crop = (percent - right_percent) / frames;
	target_right_frames = frames;
}

void GameTextureSDL::set_top_target(float percent, float frames) {
	float top_percent = 1.0 - (src_rect.h / (float)height_scale);
	target_top_crop = (percent - top_percent) / frames;
	target_top_frames = frames;
}

void GameTextureSDL::set_bottom_target(float percent, float frames) {
	float bottom_percent = src_rect.h / (float)height_scale;
	target_bottom_crop = (percent - bottom_percent) / frames;
	target_bottom_frames = frames;
}

void GameTextureSDL::set_target_pos(glm::vec2 target, float frames) {
	if (frames == 0.0) {
		return;
	}
	this->target_pos = (target_pos - pos) / glm::vec2(frames);
	this->target_pos_frames = glm::vec2(frames);
}

glm::vec2 GameTextureSDL::get_pos_offset(glm::vec2 pos, SDL_Rect rect) {
	glm::vec2 ret;
	ret.x = pos.x + (WINDOW_WIDTH / 2.0) - (rect.x + (base_width / 2.0));
	ret.y = ((pos.y + WINDOW_HEIGHT) * -1.0 + WINDOW_HEIGHT) + (WINDOW_HEIGHT / 2.0) - (rect.y + (base_height / 2.0));
	return ret;
}

void GameTextureSDL::render() {
	SDL_Rect sdl_dest = dest_rect;
	SDL_Rect sdl_src = src_rect;
	glm::vec2 sdl_pos = pos;
	glm::vec2 src_offset = get_pos_offset(sdl_pos, src_rect);
	glm::vec2 dest_offset = get_pos_offset(sdl_pos, dest_rect);
	sdl_src.x = src_offset.x;
	sdl_src.y = src_offset.y;
	sdl_dest.x = dest_offset.x;
	sdl_dest.y = dest_offset.y;
//	std::cout << "Source: " << sdl_src.x << ", " << sdl_src.y << ", " << sdl_src.w << ", " << sdl_src.h << "\n";
//	std::cout << "Dest: " << sdl_dest.x << ", " << sdl_dest.y << ", " << sdl_dest.w << ", " << sdl_dest.h << "\n\n";
	SDL_Rect* render_src = &sdl_src;
	SDL_Rect* render_dest = &sdl_dest;
	if (whole_texture) {
		render_src = nullptr;
	}
	if (whole_screen) {
		render_dest = nullptr;
	}
	SDL_RenderCopyEx(RenderManager::get_instance()->sdl_renderer, texture, render_src, render_dest, 0, nullptr, get_flip_kind(h_flipped, v_flipped));
}