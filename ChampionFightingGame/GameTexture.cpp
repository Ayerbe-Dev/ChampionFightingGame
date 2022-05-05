#include "GameTexture.h"
#include <glew/glew.h>
#include "Shader.h"
#include "RenderManager.h"
#include "utils.h"
#include <fstream>
extern SDL_Renderer* g_renderer;
extern bool debug;

GameTexture::GameTexture() {}

GameTexture::GameTexture(std::string path) {
	init(path);
}

/// <summary>
/// Copy constructor for GameTextureNew. Creates another instance of a GameTexture which uses the same texture data as the original.
/// Note: Do NOT use this unless both instances of the GameTexture will be rendered, otherwise you will waste memory.
/// </summary>
/// <param name="that"></param>
GameTexture::GameTexture(const GameTexture& that) {
	name = that.name + "_copy";
	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	RenderManager* render_manager = RenderManager::get_instance();
	attach_shader(&render_manager->default_2d_shader);
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)offsetof(GameTextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	texture = that.texture;
	glBindTexture(GL_TEXTURE_2D, texture);

	width = that.width;
	height = that.height;
	width_scale_mul = that.width_scale_mul;
	height_scale_mul = that.height_scale_mul;
	h_flipped = that.h_flipped;
	v_flipped = that.v_flipped;
	width_orientation = that.width_orientation;
	height_orientation = that.height_orientation;

	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
}

GameTexture::~GameTexture() {

}

void GameTexture::init(std::string path) {
	name = path;
	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}

	RenderManager* render_manager = RenderManager::get_instance();
	attach_shader(&render_manager->default_2d_shader);
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)offsetof(GameTextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int num_channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture at path: " << path << "\n";
	}
	stbi_image_free(data);
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	this->width = width;
	this->height = height;
	width_scale_mul = 1.0;
	height_scale_mul = 1.0;

	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
	h_flipped = false;
	v_flipped = false;
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);

}void GameTexture::init(GLuint gl_tex_locatione) {
	texture = gl_tex_locatione;

	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}

	RenderManager* render_manager = RenderManager::get_instance();
	attach_shader(&render_manager->default_2d_shader);
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GameTextureCoord), (void*)offsetof(GameTextureCoord, tex_coord));
	glEnableVertexAttribArray(1);


	int width = 400;
	int height = 400;

	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	this->width = width;
	this->height = height;
	width_scale_mul = 1.0;
	height_scale_mul = 1.0;

	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
	h_flipped = false;
	v_flipped = false;
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);
}

void GameTexture::destroy(bool destroy_texture) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (destroy_texture) {
		glDeleteTextures(1, &texture);
	}
}

void GameTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
}

void GameTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
}

void GameTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
}

void GameTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
}

void GameTexture::set_orientation(int orientation) {
	if (orientation != GAME_TEXTURE_ORIENTATION_MAX) {
		this->orientation = orientation;
	}
}

void GameTexture::attach_shader(Shader* shader) {
	this->shader = shader;
}

glm::vec3 GameTexture::get_pos_vacuum(GameTexture *that) {
	glm::vec3 pos = this->pos;
	switch (this->orientation) {
		default:
		case (GAME_TEXTURE_ORIENTATION_MIDDLE):
		{

		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT):
		{
			pos.x -= WINDOW_WIDTH - width;
			pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE):
		{
			pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT):
		{
			pos.x *= -1.0;
			pos.x += WINDOW_WIDTH - width_orientation;
			pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT):
		{
			pos.x -= WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT):
		{
			pos.x *= -1.0;
			pos.x += WINDOW_WIDTH - width_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT):
		{
			pos.x -= WINDOW_WIDTH - width;
			pos.y *= -1.0;
			pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE):
		{
			pos.y *= -1.0;
			pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT):
		{
			pos.x *= -1.0;
			pos.y *= -1.0;
			pos.x += WINDOW_WIDTH - width_orientation;
			pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
	} //Translate from the actual orientation to its middle-oriented equivalent
	switch (that->orientation) {
		default:
		case (GAME_TEXTURE_ORIENTATION_MIDDLE):
		{

		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT):
		{
			pos.x += WINDOW_WIDTH - width;
			pos.y += WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE):
		{
			pos.y += WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - that->width_orientation;
			pos.x *= -1.0;
			pos.y += WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT):
		{
			pos.x += WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - that->width_orientation;
			pos.x *= -1.0;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT):
		{
			pos.y -= WINDOW_HEIGHT - that->height_orientation;
			pos.y *= -1.0;
			pos.x += WINDOW_WIDTH - width;

		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE):
		{
			pos.y -= WINDOW_HEIGHT - that->height_orientation;
			pos.y *= -1.0;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - that->width_orientation;
			pos.y -= WINDOW_HEIGHT - that->height_orientation;
			pos.x *= -1.0;
			pos.y *= -1.0;
		} break;
	} //Translate from there to the new orientation
	return pos;
}

void GameTexture::scale_left_percent(float percent, bool crop) {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (percent < 0.0) {
		return;
	}
	if (h_flipped) {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	else {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_LEFT]->pos.x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}

	update_buffer_data();
}

void GameTexture::scale_right_percent(float percent, bool crop) {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (percent < 0.0) {
		return;
	}
	if (h_flipped) {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_LEFT]->pos.x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
	else {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	update_buffer_data();
}

void GameTexture::scale_top_percent(float percent, bool crop) {
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (percent < 0.0) {
		return;
	}
	if (v_flipped) {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
	else {
		if (crop) {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, percent, 1.0);
		}
		tex_accessor[TEX_COORD_TOP_LEFT]->pos.y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
	update_buffer_data();
}

void GameTexture::scale_bottom_percent(float percent, bool crop) {
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (percent < 0.0) {
		return;
	}
	if (v_flipped) {
		if (crop) {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
		}
		tex_accessor[TEX_COORD_TOP_LEFT]->pos.y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
	else {
		if (crop) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
		}
		tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
	update_buffer_data();
}

void GameTexture::scale_all_percent(float percent, bool crop) {
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

void GameTexture::set_width(int new_width) {
	float old_width_scale = ((float)width * width_scale_mul) / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x /= old_width_scale;
	}
	float width_scale = ((float)new_width * width_scale_mul) / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
	}
	width = new_width;
	update_buffer_data();
}

void GameTexture::set_width_scale(float scale) {
	float old_width_scale = ((float)width * width_scale_mul) / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x /= old_width_scale;
	}
	float width_scale = ((float)width * scale) / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
	}
	width_scale_mul = scale;
	update_buffer_data();
}

void GameTexture::set_height(int new_height) {
	float old_height_scale = ((float)height * height_scale_mul) / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y /= old_height_scale;
	}
	float height_scale = ((float)new_height * height_scale_mul) / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y *= height_scale;
	}
	height = new_height;
	update_buffer_data();
}

void GameTexture::set_height_scale(float scale) {
	float old_height_scale = ((float)height * height_scale_mul) / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y /= old_height_scale;
	}
	float height_scale = ((float)height * scale) / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y *= height_scale;
	}
	height_scale_mul = scale;
	update_buffer_data();
}

void GameTexture::set_scale(float scale) {
	set_width_scale(scale);
	set_height_scale(scale);
}

int GameTexture::get_width() {
	return width;
}

int GameTexture::get_height() {
	return height;
}

void GameTexture::set_left_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (percent == 0.0) {
		if (h_flipped) {
			init_left_crop = tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x;
		}
		else {
			init_left_crop = tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x;
		}
	}
	this->target_left_crop = percent;
	this->target_left_frames = frames;
}

void GameTexture::set_right_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (percent == 0.0) {
		if (h_flipped) {
			init_right_crop = tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x;
		}
		else {
			init_right_crop = tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x;
		}
	}
	this->target_right_crop = percent;
	this->target_right_frames = frames;
}

void GameTexture::set_top_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (percent == 0.0) {
		if (v_flipped) {
			init_top_crop = tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y;
		}
		else {
			init_top_crop = tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y;
		}
	}
	this->target_top_crop = percent;
	this->target_top_frames = frames;
}

void GameTexture::set_bottom_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (percent == 0.0) {
		if (v_flipped) {
			init_bottom_crop = tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y;
		}
		else {
			init_bottom_crop = tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y;
		}
	}
	this->target_bottom_crop = percent;
	this->target_bottom_frames = frames;
}

void GameTexture::set_target_pos(glm::vec3 target_pos, float frames) {
	this->target_pos = target_pos;
	this->target_pos_frames.x = glm::distance(target_pos.x, pos.x) / frames;
	this->target_pos_frames.y = glm::distance(target_pos.y, pos.y) / frames;
	this->target_pos_frames.z = glm::distance(target_pos.z, pos.z) / frames;
}

void GameTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
}

void GameTexture::flip_h(bool update) {
	GameTextureCoord* temp_tex_accessor[4] = { tex_accessor[0], tex_accessor[1], tex_accessor[2], tex_accessor[3] };
	float left_coord = tex_data[TEX_COORD_BOTTOM_LEFT].pos.x;

	tex_data[TEX_COORD_BOTTOM_LEFT].pos.x = tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x;
	tex_data[TEX_COORD_TOP_LEFT].pos.x = tex_data[TEX_COORD_TOP_RIGHT].pos.x;
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.x = left_coord;
	tex_data[TEX_COORD_TOP_RIGHT].pos.x = left_coord;

	tex_accessor[TEX_COORD_BOTTOM_LEFT] = temp_tex_accessor[TEX_COORD_BOTTOM_RIGHT];
	tex_accessor[TEX_COORD_BOTTOM_RIGHT] = temp_tex_accessor[TEX_COORD_BOTTOM_LEFT];
	tex_accessor[TEX_COORD_TOP_LEFT] = temp_tex_accessor[TEX_COORD_TOP_RIGHT];
	tex_accessor[TEX_COORD_TOP_RIGHT] = temp_tex_accessor[TEX_COORD_TOP_LEFT];
	h_flipped = !h_flipped;
	if (update) {
		update_buffer_data();
	}
}

void GameTexture::flip_v(bool update) {
	GameTextureCoord* temp_tex_accessor[4] = { tex_accessor[0], tex_accessor[1], tex_accessor[2], tex_accessor[3] };
	float bottom_coord = tex_data[TEX_COORD_BOTTOM_LEFT].pos.y;

	tex_data[TEX_COORD_BOTTOM_LEFT].pos.y = tex_data[TEX_COORD_TOP_LEFT].pos.y;
	tex_data[TEX_COORD_BOTTOM_RIGHT].pos.y = tex_data[TEX_COORD_TOP_RIGHT].pos.y;
	tex_data[TEX_COORD_TOP_LEFT].pos.y = bottom_coord;
	tex_data[TEX_COORD_TOP_RIGHT].pos.y = bottom_coord;
	
	tex_accessor[TEX_COORD_BOTTOM_LEFT] = temp_tex_accessor[TEX_COORD_TOP_LEFT];
	tex_accessor[TEX_COORD_TOP_LEFT] = temp_tex_accessor[TEX_COORD_BOTTOM_LEFT];
	tex_accessor[TEX_COORD_BOTTOM_RIGHT] = temp_tex_accessor[TEX_COORD_TOP_RIGHT];
	tex_accessor[TEX_COORD_TOP_RIGHT] = temp_tex_accessor[TEX_COORD_BOTTOM_RIGHT];
	v_flipped = !v_flipped;
	if (update) {
		update_buffer_data();
	}
}

void GameTexture::reorient() {
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	width_scale_mul = 1.0;
	height_scale_mul = 1.0;
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}
	update_buffer_data();
}

void GameTexture::process() {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (target_bottom_frames != 0.0) {
		if (v_flipped) {
			if (target_bottom_crop > tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) {
				scale_bottom_percent(clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y + (target_bottom_crop / target_bottom_frames), target_bottom_crop));
			}
			else if (target_bottom_crop < tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) {
				if (target_bottom_crop == 0.0) {
					scale_bottom_percent(clampf(target_bottom_crop, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y - (init_bottom_crop / target_bottom_frames), 1.0));
				}
				else {
					scale_bottom_percent(clampf(target_bottom_crop, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y - (target_bottom_crop / target_bottom_frames), 1.0));
				}
			}
			else {
				target_bottom_frames = 0.0;
			}
		}
		else {
			if (target_bottom_crop > tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) {
				scale_bottom_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y + (target_bottom_crop / target_bottom_frames), target_bottom_crop));
			}
			else if (target_bottom_crop < tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) {
				if (target_bottom_crop == 0.0) {
					scale_bottom_percent(clampf(target_bottom_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y - (init_bottom_crop / target_bottom_frames), 1.0));
				}
				else {
					scale_bottom_percent(clampf(target_bottom_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y - (target_bottom_crop / target_bottom_frames), 1.0));
				}
			}
			else {
				target_bottom_frames = 0.0;
			}
		}
	}
	if (target_top_frames != 0.0) {
		if (v_flipped) {
			if (target_top_crop > tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) {
				scale_top_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y + (target_top_crop / target_top_frames), target_top_crop));
			}
			else if (target_top_crop < tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) {
				if (target_top_crop == 0.0) {
					scale_top_percent(clampf(target_top_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y - (init_top_crop / target_top_frames), 1.0));
				}
				else {
					scale_top_percent(clampf(target_top_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y - (target_top_crop / target_top_frames), 1.0));
				}
			}
			else {
				target_top_frames = 0.0;
			}
		}
		else {
			if (target_top_crop > tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) {
				scale_top_percent(clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y + (target_top_crop / target_top_frames), target_top_crop));
			}
			else if (target_top_crop < tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) {
				if (target_top_crop == 0.0) {
					scale_top_percent(clampf(target_top_crop, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y - (init_top_crop / target_top_frames), 1.0));
				}
				else {
					scale_top_percent(clampf(target_top_crop, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y - (target_top_crop / target_top_frames), 1.0));
				}
			}
			else {
				target_top_frames = 0.0;
			}
		}
	}
	if (target_left_frames != 0.0) {
		if (h_flipped) {
			if (target_left_crop > tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) {
				scale_left_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x + (target_left_crop / target_left_frames), target_left_crop));
			}
			else if (target_left_crop < tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) {
				if (target_left_crop == 0.0) {
					scale_left_percent(clampf(target_left_crop, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x - (init_left_crop / target_left_frames), 1.0));
				}
				else {
					scale_left_percent(clampf(target_left_crop, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x - (target_left_crop / target_left_frames), 1.0));
				}
			}
			else {
				target_left_frames = 0.0;
			}
		}
		else {
			if (target_left_crop > tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) {
				scale_left_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + (target_left_crop / target_left_frames), target_left_crop));
			}
			else if (target_left_crop < tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) {
				if (target_left_crop == 0.0) {
					scale_left_percent(clampf(target_left_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x - (init_left_crop / target_left_frames), 1.0));
				}
				else {
					scale_left_percent(clampf(target_left_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x - (target_left_crop / target_left_frames), 1.0));
				}
			}
			else {
				target_left_frames = 0.0;
			}
		}
	}
	if (target_right_frames != 0.0) {
		if (h_flipped) {
			if (target_right_crop > tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) {
				scale_right_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + (target_right_crop / target_right_frames), target_right_crop));
			}
			else if (target_right_crop < tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) {
				if (target_right_crop == 0.0) {
					scale_right_percent(clampf(target_right_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x - (init_right_crop / target_right_frames), 1.0));
				}
				else {
					scale_right_percent(clampf(target_right_crop, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x - (target_right_crop / target_right_frames), 1.0));
				}
			}
			else {
				target_right_frames = 0.0;
			}
		}
		else {
			if (target_right_crop > tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) {
				scale_right_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x + (target_right_crop / target_right_frames), target_right_crop));
			}
			else if (target_right_crop < tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) {
				if (target_right_crop == 0.0) {
					scale_right_percent(clampf(target_right_crop, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x - (init_right_crop / target_right_frames), 1.0));
				}
				else {
					scale_right_percent(clampf(target_right_crop, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x - (target_right_crop / target_right_frames), 1.0));
				}
			}
			else {
				target_right_frames = 0.0;
			}
		}
	}
	if (target_pos_frames != glm::vec3(0.0)) {
		if (target_pos_frames.x != 0.0) {
			if (target_pos.x > pos.x) {
				pos.x = clampf(pos.x, pos.x + target_pos_frames.x, target_pos.x);
			}
			else if (target_pos.x < pos.x) {
				pos.x = clampf(target_pos.x, pos.x - target_pos_frames.x, pos.x);
			}
			else {
				target_pos_frames.x = 0.0;
			}
		}
		if (target_pos_frames.y != 0.0) {
			if (target_pos.y > pos.y) {
				pos.y = clampf(pos.y, pos.y + target_pos_frames.y, target_pos.y);
			}
			else if (target_pos.y < pos.y) {
				pos.y = clampf(target_pos.y, pos.y - target_pos_frames.y, pos.y);
			}
			else {
				target_pos_frames.y = 0.0;
			}
		}
		if (target_pos_frames.z != 0.0) {
			if (target_pos.z > pos.z) {
				pos.z = clampf(pos.z, pos.z + target_pos_frames.z, target_pos.z);
			}
			else if (target_pos.z < pos.z) {
				pos.z = clampf(target_pos.z, pos.z - target_pos_frames.z, pos.z);
			}
			else {
				target_pos_frames.z = 0.0;
			}
		}
	}
}

//todo: Optimize, probably rewrite the targeting systems for everything except pos
void GameTexture::render() {
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glm::vec3 gl_pos = pos;
	switch (orientation) {
		default:
		case (GAME_TEXTURE_ORIENTATION_MIDDLE): {
		
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT): {
			gl_pos.x -= WINDOW_WIDTH - (width * width_scale_mul);
			gl_pos.y -= WINDOW_HEIGHT - (height * height_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE): {
			gl_pos.y -= WINDOW_HEIGHT - (height * height_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.x += WINDOW_WIDTH - (width_orientation * width_scale_mul);
			gl_pos.y -= WINDOW_HEIGHT - (height * height_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			gl_pos.x -= WINDOW_WIDTH - (width * width_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.x += WINDOW_WIDTH - (width_orientation * width_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT): {
			gl_pos.y *= -1.0;
			gl_pos.x -= WINDOW_WIDTH - (width * width_scale_mul);
			gl_pos.y += WINDOW_HEIGHT - (height_orientation * height_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			gl_pos.y *= -1.0;
			gl_pos.y += WINDOW_HEIGHT - (height_orientation * height_scale_mul);
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.y *= -1.0;
			gl_pos.x += WINDOW_WIDTH - (width_orientation * width_scale_mul);
			gl_pos.y += WINDOW_HEIGHT - (height_orientation * height_scale_mul);
		} break;
	}
	gl_pos.x /= (float)WINDOW_WIDTH;
	gl_pos.y /= (float)WINDOW_HEIGHT;
	glm::mat4 matrix = glm::mat4(1.0);
	matrix = translate(matrix, gl_pos);
	matrix = rotate(matrix, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	matrix = rotate(matrix, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	matrix = rotate(matrix, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	shader->set_mat4("matrix", matrix);
	shader->set_float("f_alphamod", 1.0 - ((float)alpha / 255.0));
	glDepthMask(gl_pos.z != 0.0);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameTexture::load_spritesheet(std::string spritesheet_dir) {
	std::ifstream spritesheet_file;
	spritesheet_file.open(spritesheet_dir);
	if (spritesheet_file.fail()) {
		std::cout << "Failed to open spritesheet at " << spritesheet_dir << "!\n";
		spritesheet_file.close();
		return;
	}
	int frame;
	glm::vec2 corners[4];
	while (spritesheet_file >> frame) {
		spritesheet_file >> corners[TEX_COORD_BOTTOM_LEFT].x >> corners[TEX_COORD_BOTTOM_LEFT].y;
		spritesheet_file >> corners[TEX_COORD_BOTTOM_RIGHT].x >> corners[TEX_COORD_BOTTOM_RIGHT].y;
		spritesheet_file >> corners[TEX_COORD_TOP_RIGHT].x >> corners[TEX_COORD_TOP_RIGHT].y;
		spritesheet_file >> corners[TEX_COORD_TOP_LEFT].x >> corners[TEX_COORD_TOP_LEFT].y;
		for (int i = 0; i < 4; i++) {
			corners[i].x /= width;
			corners[i].y /= height;
			spritesheet[i].push_back(corners[i]);
		}
	}
	spritesheet_file.close();
	set_width(spritesheet[TEX_COORD_TOP_RIGHT][0].x * width);
	set_height(spritesheet[TEX_COORD_TOP_RIGHT][0].y * height);
	set_sprite(0);
}

void GameTexture::set_sprite(int index) {
	if (spritesheet[0].size() <= index) return;
	for (int i = 0; i < 4; i++) {
		tex_accessor[i]->tex_coord = spritesheet[i][index];
	}
	update_buffer_data();
}

void GameTexture::update_buffer_data() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}