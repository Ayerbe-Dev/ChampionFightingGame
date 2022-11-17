#include "GameTexture.h"
#include "GLEW Helpers.h"
#include <glew/glew.h>
#include "Shader.h"
#include "RenderManager.h"
#include "utils.h"
#include <fstream>
#include "stb_image.h"
#include "ThreadManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

GameTexture::GameTexture() {}

GameTexture::GameTexture(std::string path) {
	init(path);
}

GameTexture::GameTexture(Font font, std::string text, glm::vec4 rgba, float border_x, float border_y) {
	init(font, text, rgba, border_x, border_y);
}

/// <summary>
/// Copy constructor for GameTexture. Uses the same VAO, VBO and texture as the original, but with different
/// tex_data. Don't call destroy() on a GameTexture that was copied, or you will destroy all instances of it.
/// </summary>
/// <param name="that"></param>
GameTexture::GameTexture(const GameTexture& that) {
	name = that.name;
	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	ShaderManager* shader_manager = ShaderManager::get_instance();
	attach_shader(shader_manager->get_shader("2d_texture", "2d_texture", "", 0));
	shader->use();

	VAO = that.VAO;
	VBO = that.VBO;
	texture = that.texture;
	text = that.text;

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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
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

	ShaderManager* shader_manager = ShaderManager::get_instance();
	attach_shader(shader_manager->get_shader("2d_texture", "2d_texture", "", 0)); 
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	ResourceManager* resource_manager = ResourceManager::get_instance();

	texture = resource_manager->use_texture(path);
	int width;
	int height;
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	text = "";
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);
	loaded = true;
	using_resource = true;
}

void GameTexture::init(GLuint texture, int width, int height) {
	name = "Unnamed GLuint Texture";
	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}

	ShaderManager* shader_manager = ShaderManager::get_instance();
	attach_shader(shader_manager->get_shader("2d_texture", "2d_texture", "", 0));
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, texture);
	if (width == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	}
	if (height == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	this->texture = texture;
	this->width = width;
	this->height = height;
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	width_scale_mul = 1.0;
	height_scale_mul = 1.0;

	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
	h_flipped = false;
	v_flipped = false;
	text = "";
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);
	loaded = true;
}

void GameTexture::init(Font font, std::string text, glm::vec4 rgba, float border_x, float border_y) {
	name = text + " Texture";
	texture = font.create_text(text, rgba, border_x, border_y);
	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = glm::vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}

	ShaderManager* shader_manager = ShaderManager::get_instance();
	attach_shader(shader_manager->get_shader("2d_texture", "2d_texture", "", 0));
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
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
	this->text = text;
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);
	loaded = true;
}

void GameTexture::destroy() {
	if (loaded) {
		if (using_resource) {
			ResourceManager::get_instance()->unuse_texture(name);
		}
		else {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteTextures(1, &texture);
		}
		loaded = false;
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
}

void GameTexture::scale_all_percent(float percent, bool crop) {
	scale_left_percent(percent, crop);
	scale_right_percent(percent, crop);
	scale_top_percent(percent, crop);
	scale_bottom_percent(percent, crop);
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

std::string GameTexture::get_text() {
	return text;
}

void GameTexture::set_left_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (h_flipped) {
		target_left_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) / frames;
	}
	else {
		target_left_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) / frames;
	}
	this->target_left_frames = frames;
}

void GameTexture::set_right_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (h_flipped) {
		target_right_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x) / frames;
	}
	else {
		target_right_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x) / frames;
	}
	this->target_right_frames = frames;
}

void GameTexture::set_top_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (v_flipped) {
		target_top_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) / frames;
	}
	else {
		target_top_crop = (percent - tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) / frames;
	}
	this->target_top_frames = frames;
}

void GameTexture::set_bottom_target(float percent, float frames) {
	if (percent < 0.0 || frames <= 0.0) {
		return;
	}
	if (v_flipped) {
		target_bottom_crop = (percent - tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y) / frames;
	}
	else {
		target_bottom_crop = (percent - tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y) / frames;
	}
	this->target_bottom_frames = frames;
}

void GameTexture::set_target_pos(glm::vec3 target_pos, float frames) {
	if (frames == 0.0) {
		return;
	}
	this->target_pos = (target_pos - pos) / glm::vec3(frames);
	this->target_pos_frames = glm::vec3(frames);
}

void GameTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
}

void GameTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
}

void GameTexture::flip_h() {
	TextureCoord* temp_tex_accessor[4] = { tex_accessor[0], tex_accessor[1], tex_accessor[2], tex_accessor[3] };
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
}

void GameTexture::flip_v() {
	TextureCoord* temp_tex_accessor[4] = { tex_accessor[0], tex_accessor[1], tex_accessor[2], tex_accessor[3] };
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
}

void GameTexture::process() {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (target_bottom_frames != 0.0) {
		if (v_flipped) {
			scale_bottom_percent(clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y + target_bottom_crop, 1.0));
		}
		else {
			scale_bottom_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y + target_bottom_crop, 1.0));
		}
		target_bottom_frames--;
	}
	if (target_top_frames != 0.0) {
		if (v_flipped) {
			scale_top_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y + target_top_crop, 1.0));
		}
		else {
			scale_top_percent(clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y + target_top_crop, 1.0));
		}
		target_top_frames--;
	}
	if (target_left_frames != 0.0) {
		if (h_flipped) {
			scale_left_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x + target_left_crop, 1.0));
		}
		else {
			scale_left_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + target_left_crop, 1.0));
		}
		target_left_frames--;
	}
	if (target_right_frames != 0.0) {
		if (h_flipped) {
			scale_right_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + target_right_crop, 1.0));
		}
		else {
			scale_right_percent(clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x + target_right_crop, 1.0));
		}
		target_right_frames--;
	}
	if (target_pos_frames != glm::vec3(0.0)) {
		pos += target_pos;
		target_pos_frames -= glm::vec3(1.0);
	}
}

void GameTexture::prepare_render() {
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
	matrix = translate(glm::mat4(1.0), gl_pos);
	matrix = rotate(matrix, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	matrix = rotate(matrix, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	matrix = rotate(matrix, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	depth = gl_pos.z != 0.0;
}

void GameTexture::render() {
	prepare_render();
	render_prepared();
}

void GameTexture::render_prepared() {
	update_buffer_data();
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->set_mat4("matrix", matrix);
	shader->set_float("f_alphamod", 1.0 - ((float)alpha / 255.0));
	glDepthMask(depth);
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
}

void GameTexture::update_text(Font font, std::string text, glm::vec4 rgba, float border_x, float border_y) {
	this->text = text;
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;

	if (width_scale == 0.0 || height_scale == 0.0) {
		tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
		tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
		tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
		tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	}
	else {
		for (int i = 0; i < 4; i++) {
			tex_data[i].pos.x /= width_scale;
			tex_data[i].pos.y /= height_scale;
		}
	}

	texture = font.create_text(text, rgba, border_x, border_y, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	width_scale = (float)width / (float)WINDOW_WIDTH;
	height_scale = (float)height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}
	update_buffer_data();
	width_orientation = width * (tex_data[TEX_COORD_BOTTOM_LEFT].tex_coord.x + tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.x);
	height_orientation = height * (tex_data[TEX_COORD_BOTTOM_RIGHT].tex_coord.y + tex_data[TEX_COORD_TOP_RIGHT].tex_coord.y);
}

void GameTexture::update_buffer_data() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}