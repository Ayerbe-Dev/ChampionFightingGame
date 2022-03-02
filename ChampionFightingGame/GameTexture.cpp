#include "GameTexture.h"
#include "Shader.h"
#include "RenderManager.h"
extern SDL_Renderer* g_renderer;
extern RenderManager g_rendermanager;
extern bool debug;

GameTexture::GameTexture() {}

GameTexture::GameTexture(string path) {
	init(path);
}

/// <summary>
/// Copy constructor for GameTextureNew. Creates another instance of a GameTexture which uses the same texture data as the original.
/// Note: Do NOT use this unless both instances of the GameTexture will be rendered, otherwise you will waste memory.
/// </summary>
/// <param name="that"></param>
GameTexture::GameTexture(const GameTexture& that) {
	name = that.name + "_copy";
	pos = vec3(0.0, 0.0, 0.0);
	rot = vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { vec3(-1.0, -1.0, 0.0), vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { vec3(1.0, -1.0, 0.0), vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { vec3(1.0, 1.0, 0.0), vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { vec3(-1.0, 1.0, 0.0), vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	attach_shader(&g_rendermanager.default_2d_shader);
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

void GameTexture::init(string path) {
	name = path;
	pos = vec3(0.0, 0.0, 0.0);
	rot = vec3(0.0, 0.0, 0.0);
	tex_data[TEX_COORD_BOTTOM_LEFT] = { vec3(-1.0, -1.0, 0.0), vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { vec3(1.0, -1.0, 0.0), vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { vec3(1.0, 1.0, 0.0), vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { vec3(-1.0, 1.0, 0.0), vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}

	attach_shader(&g_rendermanager.default_2d_shader);
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
		cout << "Failed to load texture at path: " << path << endl;
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
}

void GameTexture::destroy(bool destroy_texture) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (destroy_texture) {
		glDeleteTextures(1, &texture);
	}
}

void GameTexture::set_pos(vec3 pos) {
	this->pos = pos;
}

void GameTexture::add_pos(vec3 pos) {
	this->pos += pos;
}

void GameTexture::set_rot(vec3 rot) {
	this->rot = rot;
}

void GameTexture::add_rot(vec3 rot) {
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

vec3 GameTexture::get_pos_vacuum(GameTexture *that) {
	vec3 pos = this->pos;
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
	float old_width_scale = (float)width / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x /= old_width_scale;
	}
	float width_scale = (float)new_width / (float)WINDOW_WIDTH;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
	}
	width = new_width;
	update_buffer_data();
}

void GameTexture::set_width_scale(float scale) {
	width_scale_mul = scale;
}

void GameTexture::set_height(int new_height) {
	float old_height_scale = (float)height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y /= old_height_scale;
	}
	float height_scale = (float)new_height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.y *= height_scale;
	}
	height = new_height;
	update_buffer_data();
}

void GameTexture::set_height_scale(float scale) {
	height_scale_mul = scale;
}

int GameTexture::get_width() {
	return width;
}

int GameTexture::get_height() {
	return height;
}

void GameTexture::set_left_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_left_crop = percent;
	this->target_left_max_change = max_change;
}

void GameTexture::set_right_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_right_crop = percent;
	this->target_right_max_change = max_change;
}

void GameTexture::set_top_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_top_crop = percent;
	this->target_top_max_change = max_change;
}

void GameTexture::set_bottom_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_bottom_crop = percent;
	this->target_bottom_max_change = max_change;
}

void GameTexture::set_target_pos(vec3 target_pos, float frames) {
	this->target_pos = target_pos;
	this->target_pos_max_change.x = distance(target_pos.x, pos.x) / frames;
	this->target_pos_max_change.y = distance(target_pos.y, pos.y) / frames;
	this->target_pos_max_change.z = distance(target_pos.z, pos.z) / frames;
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
	tex_data[TEX_COORD_BOTTOM_LEFT] = { vec3(-1.0, -1.0, 0.0), vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { vec3(1.0, -1.0, 0.0), vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { vec3(1.0, 1.0, 0.0), vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { vec3(-1.0, 1.0, 0.0), vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}
	update_buffer_data();
}
void GameTexture::render() {
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (target_bottom_max_change != 0.0) {
		if (target_bottom_crop * height_scale * -1.0 > tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y + target_bottom_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y + target_bottom_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y + target_bottom_max_change, target_bottom_crop * height_scale * -1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y + target_bottom_max_change, target_bottom_crop * height_scale * -1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else if (target_bottom_crop * height_scale * -1.0 < tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y - target_bottom_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y - target_bottom_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y = clampf(target_bottom_crop * height_scale * -1.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y - target_bottom_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y = clampf(target_bottom_crop * height_scale * -1.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y - target_bottom_max_change, 1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else {
			target_bottom_max_change = 0.0;
		}
	}
	if (target_top_max_change != 0.0) {
		if (target_top_crop * height_scale > tex_accessor[TEX_COORD_TOP_LEFT]->pos.y) {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y + target_top_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y + target_top_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->pos.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->pos.y + target_top_max_change, target_top_crop * height_scale);
			tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y + target_top_max_change, target_top_crop * height_scale);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else if (target_top_crop * height_scale < tex_accessor[TEX_COORD_TOP_LEFT]->pos.y) {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y - target_top_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y - target_top_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->pos.y = clampf(target_top_crop * height_scale, tex_accessor[TEX_COORD_TOP_LEFT]->pos.y - target_top_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y = clampf(target_top_crop * height_scale, tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y - target_top_max_change, 1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else {
			target_top_max_change = 0.0;
		}
	}
	if (target_left_max_change != 0.0) {
		if (target_left_crop * width_scale * -1.0 > tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + target_left_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x + target_left_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x + target_left_max_change, target_left_crop * width_scale * -1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->pos.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->pos.x + target_left_max_change, target_left_crop * width_scale * -1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else if (target_left_crop * width_scale * -1.0 < tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x - target_left_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x - target_left_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x = clampf(target_left_crop * width_scale * -1.0, tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x - target_left_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->pos.x = clampf(target_left_crop * width_scale * -1.0, tex_accessor[TEX_COORD_TOP_LEFT]->pos.x - target_left_max_change, 1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else {
			target_left_max_change = 0.0;
		}
	}
	if (target_right_max_change != 0.0) {
		if (target_right_crop * width_scale > tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x) {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x + target_right_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x + target_right_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x + target_right_max_change, target_right_crop * width_scale);
			tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x + target_right_max_change, target_right_crop * width_scale);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else if (target_right_crop * width_scale < tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x) {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x - target_right_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x - target_right_max_change, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x = clampf(target_right_crop * width_scale, tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x - target_right_max_change, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x = clampf(target_right_crop * width_scale, tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x - target_right_max_change, 1.0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
		}
		else {
			target_right_max_change = 0.0;
		}
	}
	if (target_pos_max_change != vec3(0.0)) {
		if (target_pos_max_change.x != 0.0) {
			if (target_pos.x > pos.x) {
				pos.x = clampf(pos.x, pos.x + target_pos_max_change.x, target_pos.x);
			}
			else if (target_pos.x < pos.x) {
				pos.x = clampf(target_pos.x, pos.x - target_pos_max_change.x, pos.x);
			}
			else {
				target_pos_max_change.x = 0.0;
			}
		}
		if (target_pos_max_change.y != 0.0) {
			if (target_pos.y > pos.y) {
				pos.y = clampf(pos.y, pos.y + target_pos_max_change.y, target_pos.y);
			}
			else if (target_pos.y < pos.y) {
				pos.y = clampf(target_pos.y, pos.y - target_pos_max_change.y, pos.y);
			}
			else {
				target_pos_max_change.y = 0.0;
			}
		}
		if (target_pos_max_change.z != 0.0) {
			if (target_pos.z > pos.z) {
				pos.z = clampf(pos.z, pos.z + target_pos_max_change.z, target_pos.z);
			}
			else if (target_pos.z < pos.z) {
				pos.z = clampf(target_pos.z, pos.z - target_pos_max_change.z, pos.z);
			}
			else {
				target_pos_max_change.z = 0.0;
			}
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	vec3 gl_pos = pos;
	switch (orientation) {
		default:
		case (GAME_TEXTURE_ORIENTATION_MIDDLE): {
		
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT): {
			gl_pos.x -= WINDOW_WIDTH - width;
			gl_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE): {
			gl_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.x += WINDOW_WIDTH - width_orientation;
			gl_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			gl_pos.x -= WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.x += WINDOW_WIDTH - width_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT): {
			gl_pos.y *= -1.0;
			gl_pos.x -= WINDOW_WIDTH - width;
			gl_pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			gl_pos.y *= -1.0;
			gl_pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.y *= -1.0;
			gl_pos.x += WINDOW_WIDTH - width_orientation;
			gl_pos.y += WINDOW_HEIGHT - height_orientation;
		} break;
	}
	gl_pos.x /= (float)WINDOW_WIDTH;
	gl_pos.y /= (float)WINDOW_HEIGHT;
	float shader_alpha = 1.0 - ((float)alpha / 255.0);
	mat4 matrix = mat4(1.0);
	matrix = translate(matrix, gl_pos);
	matrix = rotate(matrix, radians(rot.x), vec3(1.0, 0.0, 0.0));
	matrix = rotate(matrix, radians(rot.y), vec3(0.0, 1.0, 0.0));
	matrix = rotate(matrix, radians(rot.z), vec3(0.0, 0.0, 1.0));
	shader->set_float("f_alphamod", shader_alpha);
	shader->set_mat4("matrix", matrix);
	glDepthMask(gl_pos.z != 0.0);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameTexture::update_buffer_data() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}