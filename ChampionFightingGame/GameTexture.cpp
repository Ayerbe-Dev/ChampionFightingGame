#include "GameTexture.h"
#include "Shader.h"
extern SDL_Renderer* g_renderer;
extern bool debug;

GameTextureNew::GameTextureNew(string path) {
	pos = vec3(0.0, 0.0, 0.0);
	rot = vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { vec3(0.0, 0.0, 0.0), vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { vec3(1.0, 0.0, 0.0), vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { vec3(1.0, 1.0, 0.0), vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { vec3(0.0, 1.0, 0.0), vec2(0.0, 1.0) };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)offsetof(GameTextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int num_channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture at path: " << path << endl;
	}
	stbi_image_free(data);
	this->width = width;
	this->height = height;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameTextureNew::set_pos(vec3 pos) {
	this->pos = pos;
}

void GameTextureNew::add_pos(vec3 pos) {
	this->pos += pos;
}

void GameTextureNew::set_rot(vec3 rot) {
	this->rot = rot;
}

void GameTextureNew::add_rot(vec3 rot) {
	this->rot += rot;
}

void GameTextureNew::set_orientation(int orientation) {
	if (orientation != GAME_TEXTURE_ORIENTATION_MAX) {
		this->orientation = orientation;
	}
}

void GameTextureNew::attach_shader(Shader* shader) {
	this->shader = shader;
}

void GameTextureNew::crop_left_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_LEFT].pos.x += percent;
	tex_data[TEX_COORD_TOP_LEFT].pos.x += percent;
}

void GameTextureNew::crop_right_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].pos.x -= percent;
}

void GameTextureNew::crop_top_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_TOP_LEFT].pos.y -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].pos.y -= percent;
}

void GameTextureNew::crop_bottom_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_LEFT].pos.y += percent;
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y += percent;
}

void GameTextureNew::scale_left_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_LEFT].pos.x += percent;
	tex_data[TEX_COORD_TOP_LEFT].pos.x += percent;
	tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x += percent;
	tex_data[TEX_COORD_TOP_LEFT].tex_coord.x += percent;
}

void GameTextureNew::scale_right_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].pos.x -= percent;
	tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].tex_coord.x -= percent;
}

void GameTextureNew::scale_top_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_TOP_LEFT].pos.y -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].pos.y -= percent;
	tex_data[TEX_COORD_TOP_LEFT].tex_coord.y -= percent;
	tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y -= percent;
}

void GameTextureNew::scale_bottom_percent(float percent) {
	if (percent > 1.0) {
		percent /= 100.0;
	}
	tex_data[TEX_COORD_BOTTOM_LEFT].pos.y += percent;
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y += percent;
	tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.y += percent;
	tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y += percent;
}

void GameTextureNew::set_left_target(float percent, float max_change) {
	this->target_left_crop = percent;
	this->target_left_max_change = max_change;
}

void GameTextureNew::set_right_target(float percent, float max_change) {
	this->target_right_crop = percent;
	this->target_right_max_change = max_change;
}

void GameTextureNew::set_top_target(float percent, float max_change) {
	this->target_top_crop = percent;
	this->target_top_max_change = max_change;
}

void GameTextureNew::set_bottom_target(float percent, float max_change) {
	this->target_bottom_crop = percent;
	this->target_bottom_max_change = max_change;
}

void GameTextureNew::flip_h() {
	float left_coord = tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x;
	tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x = tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x;
	tex_data[TEX_COORD_TOP_LEFT].tex_coord.x = tex_data[TEX_COORD_TOP_RIGHT].tex_coord.x;
	tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x = left_coord;
	tex_data[TEX_COORD_TOP_RIGHT].tex_coord.x = left_coord;
}

void GameTextureNew::flip_v() {
	float bottom_coord = tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.y;
	tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.y = tex_data[TEX_COORD_TOP_LEFT].tex_coord.y;
	tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y = tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y;
	tex_data[TEX_COORD_TOP_LEFT].tex_coord.y = bottom_coord;
	tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y = bottom_coord;
}

void GameTextureNew::render() {
	if (target_bottom_crop != -1.0) {
		if (target_bottom_crop > tex_data[TEX_COORD_BOTTOM_LEFT].pos.y) {
			tex_data[TEX_COORD_BOTTOM_LEFT].pos.y = clampf(0.0, tex_data[TEX_COORD_BOTTOM_LEFT].pos.y + target_bottom_max_change, target_bottom_crop);
			tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y = clampf(0.0, tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y + target_bottom_max_change, target_bottom_crop);
		}
		else if (target_bottom_crop < tex_data[TEX_COORD_BOTTOM_LEFT].pos.y) {
			tex_data[TEX_COORD_BOTTOM_LEFT].pos.y = clampf(target_bottom_crop, tex_data[TEX_COORD_BOTTOM_LEFT].pos.y - target_bottom_max_change, 1.0);
			tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y = clampf(target_bottom_crop, tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y - target_bottom_max_change, 1.0);
		}
		else {
			target_bottom_crop = -1.0;
		}
	}
	if (target_top_crop != -1.0) {
		if (target_top_crop > tex_data[TEX_COORD_TOP_LEFT].pos.y) {
			tex_data[TEX_COORD_TOP_LEFT].pos.y = clampf(0.0, tex_data[TEX_COORD_TOP_LEFT].pos.y + target_top_max_change, target_top_crop);
			tex_data[TEX_COORD_TOP_RIGHT].pos.y = clampf(0.0, tex_data[TEX_COORD_TOP_RIGHT].pos.y + target_top_max_change, target_top_crop);
		}
		else if (target_top_crop < tex_data[TEX_COORD_TOP_LEFT].pos.y) {
			tex_data[TEX_COORD_TOP_LEFT].pos.y = clampf(target_top_crop, tex_data[TEX_COORD_TOP_LEFT].pos.y - target_top_max_change, 1.0);
			tex_data[TEX_COORD_TOP_RIGHT].pos.y = clampf(target_top_crop, tex_data[TEX_COORD_TOP_RIGHT].pos.y - target_top_max_change, 1.0);
		}
		else {
			target_top_crop = -1.0;
		}
	}
	if (target_left_crop != -1.0) {
		if (target_left_crop > tex_data[TEX_COORD_BOTTOM_LEFT].pos.x) {
			tex_data[TEX_COORD_BOTTOM_LEFT].pos.x = clampf(0.0, tex_data[TEX_COORD_BOTTOM_LEFT].pos.x + target_left_max_change, target_left_crop);
			tex_data[TEX_COORD_TOP_LEFT].pos.x = clampf(0.0, tex_data[TEX_COORD_TOP_LEFT].pos.x + target_left_max_change, target_left_crop);
		}
		else if (target_left_crop < tex_data[TEX_COORD_BOTTOM_LEFT].pos.x) {
			tex_data[TEX_COORD_BOTTOM_LEFT].pos.x = clampf(target_left_crop, tex_data[TEX_COORD_BOTTOM_LEFT].pos.x - target_left_max_change, 1.0);
			tex_data[TEX_COORD_TOP_LEFT].pos.x = clampf(target_left_crop, tex_data[TEX_COORD_TOP_LEFT].pos.x - target_left_max_change, 1.0);
		}
		else {
			target_left_crop = -1.0;
		}
	}
	if (target_right_crop != -1.0) {
		if (target_right_crop > tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x) {
			tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x = clampf(0.0, tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x + target_right_max_change, target_right_crop);
			tex_data[TEX_COORD_TOP_RIGHT].pos.x = clampf(0.0, tex_data[TEX_COORD_TOP_RIGHT].pos.x + target_right_max_change, target_right_crop);
		}
		else if (target_right_crop < tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x) {
			tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x = clampf(target_right_crop, tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x - target_right_max_change, 1.0);
			tex_data[TEX_COORD_TOP_RIGHT].pos.x = clampf(target_right_crop, tex_data[TEX_COORD_TOP_RIGHT].pos.x - target_right_max_change, 1.0);
		}
		else {
			target_right_crop = -1.0;
		}
	}

	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	vec3 gl_pos = pos;
/*	switch (orientation) {
		default:
		case (GAME_TEXTURE_ORIENTATION_MIDDLE): {
		
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT): {
			gl_pos.x -= width / 2;
			gl_pos.y -= height / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE): {
			gl_pos.y -= height / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT): {
			gl_pos.x += width / 2;
			gl_pos.y -= height / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			gl_pos.x -= width / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			gl_pos.x += width / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT): {
			gl_pos.x -= width / 2;
			gl_pos.y += height / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			gl_pos.y += height / 2;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT): {
			gl_pos.x += width / 2;
			gl_pos.y += height / 2;
		} break;
	}*/
//	gl_pos.x /= (float)WINDOW_WIDTH;
//	gl_pos.y /= (float)WINDOW_HEIGHT;
	mat4 matrix = mat4(1.0);
	matrix = translate(matrix, gl_pos);
	matrix = rotate(matrix, radians(rot.x), vec3(1.0, 0.0, 0.0));
	matrix = rotate(matrix, radians(rot.y), vec3(0.0, 1.0, 0.0));
	matrix = rotate(matrix, radians(rot.z), vec3(0.0, 0.0, 1.0));
	shader->set_int("f_texture", 0);
	shader->set_mat4("matrix", matrix);
	glDrawArrays(GL_TRIANGLES, 0, 4);
}

bool GameTexture::init(string sTexturePath, bool delay){
	if (bIsInitialized){
		printf("GameTexture already initialized!\n");
		return false;
	}

	bIsInitialized = true;
	pTexture = loadSDLTexture(sTexturePath.c_str(), delay);
	destRect.x = 0;
	destRect.y = 0;
	srcRect.x = 0;
	srcRect.y = 0;
	SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);
	srcRect.h=destRect.h;
	srcRect.w=destRect.w;
	return true;
}

bool GameTexture::render() {
	if (!bIsInitialized){
		return false;
	}

	if (target_percent != -1.0) {
		changePercent();
	}

	SDL_Rect tmpDestRect = destRect;
	SDL_Rect tmpSrcRect = srcRect;
	tmpDestRect.w *= fHorizontalScaleFactor;
	tmpDestRect.h *= fVerticalScaleFactor;


	switch (iAnchorMode) {
		case (GAME_TEXTURE_ANCHOR_MODE_CENTER): {
			tmpDestRect.x -= tmpDestRect.w / 2;
			tmpDestRect.y -= tmpDestRect.h / 2;
		} 
		break;
		case (GAME_TEXTURE_ANCHOR_MODE_DEFAULT): {
			if (flip) {
				tmpDestRect.x -= tmpDestRect.w;
			}
		}
		break;
		case (GAME_TEXTURE_ANCHOR_MODE_METER): {
			tmpDestRect.w = destRect.w * (percent);
			tmpDestRect.h *= fVerticalScaleFactor;
			tmpSrcRect.w = tmpDestRect.w;
			if (drain_kind != METER_DRAIN_KIND_NONE) {
				tmpDestRect.x += WINDOW_WIDTH / 2;
				if (drain_kind == METER_DRAIN_KIND_RIGHT) {
					tmpDestRect.x -= tmpSrcRect.w;
				}
				tmpSrcRect.x += destRect.w - tmpSrcRect.w;
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
		pTexture, 
		iAnchorMode == GAME_TEXTURE_ANCHOR_MODE_METER?&tmpSrcRect:nullptr, 
		iAnchorMode == GAME_TEXTURE_ANCHOR_MODE_BACKGROUND?nullptr:&tmpDestRect,
		0, 
		NULL, 
		flip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE
	);

	return true;
}

void GameTexture::setScaleFactor(float fScaleFactor){
	this->fVerticalScaleFactor = fScaleFactor;
	this->fHorizontalScaleFactor = fScaleFactor;
}

void GameTexture::setHorizontalScaleFactor(float fScaleFactor){
	this->fHorizontalScaleFactor = fScaleFactor;
}

void GameTexture::setVerticalScaleFactor(float fScaleFactor){
	this->fVerticalScaleFactor = fScaleFactor;
}

void GameTexture::clearTexture() {
	SDL_DestroyTexture(pTexture);
}

float GameTexture::getScaledWidth(){
	return destRect.w * fHorizontalScaleFactor;
}

float GameTexture::getScaledHeight(){
	return destRect.h * fVerticalScaleFactor;
}

void GameTexture::setAnchorMode(int iMode){
	iAnchorMode = iMode;
}

void GameTexture::setAlpha(Uint8 alpha){
	SDL_SetTextureAlphaMod(pTexture,alpha);
}

void GameTexture::setPercent(float percent){
	this->percent = percent;
	if (iAnchorMode != GAME_TEXTURE_ANCHOR_MODE_METER){
		printf("WARNING: GameTexture is using the setPercent() function but its not in the correct mode!\n");
	}
}

void GameTexture::setTargetPercent(float percent, float rate, int frames) {
	target_percent = percent;
	target_rate = rate;
	target_frames = frames;
	if (iAnchorMode != GAME_TEXTURE_ANCHOR_MODE_METER) {
		printf("WARNING: GameTexture is using the setTargetPercent() function but its not in the correct mode!\n");
	}
}

void GameTexture::changePercent(float rate) {
	if (rate == -1.0) {
		rate = target_rate;
	}
	if (rate == -1.0) {
		cout << "WARNING: Target rate was not set through setTargetPercent, but rate was not given a non-default arg!" << endl;
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

void GameTexture::setFlip(int flip) {
	this->flip = flip;
}

int GameTexture::getFlipKind() {
	return flip;
}

void GameTexture::setDrainKind(int meter_drain_kind) {
	drain_kind = meter_drain_kind;
}