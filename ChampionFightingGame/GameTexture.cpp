#include "GameTexture.h"
#include "Shader.h"
#include "RenderManager.h"
extern SDL_Renderer* g_renderer;
extern RenderManager g_rendermanager;
extern bool debug;

GameTextureNew::GameTextureNew() {}

GameTextureNew::GameTextureNew(string path) {
	init(path);
}

/// <summary>
/// Copy constructor for GameTextureNew. Creates another instance of a GameTexture which uses the same texture data as the original.
/// Note: Do NOT use this unless both instances of the GameTexture will be rendered, otherwise you will waste memory.
/// </summary>
/// <param name="that"></param>
GameTextureNew::GameTextureNew(const GameTextureNew& that) {
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

GameTextureNew::~GameTextureNew() {

}

void GameTextureNew::init(string path) {
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

	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->set_int("f_texture", 0);
}

void GameTextureNew::destroy(bool destroy_texture) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (destroy_texture) {
		glDeleteTextures(1, &texture);
	}
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

vec3 GameTextureNew::get_pos_vacuum(int orientation) {
	vec3 pos = this->pos;
	float width_post_scale = this->width * (this->tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + this->tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x);
	float height_post_scale = this->height * (this->tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y + this->tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y);
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
			pos.x += WINDOW_WIDTH - width_post_scale;
			pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT):
		{
			pos.x -= WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT):
		{
			pos.x *= -1.0;
			pos.x += WINDOW_WIDTH - width_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT):
		{
			pos.y *= -1.0;
			pos.x -= WINDOW_WIDTH - width;
			pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE):
		{
			pos.y *= -1.0;
			pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT):
		{
			pos.x *= -1.0;
			pos.y *= -1.0;
			pos.x += WINDOW_WIDTH - width_post_scale;
			pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
	} //Translate from the actual orientation to its middle-oriented equivalent
	switch (orientation) {
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
			pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - width_post_scale;
			pos.x *= -1.0;
			pos.y += WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT):
		{
			pos.x += WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - width_post_scale;
			pos.x *= -1.0;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT):
		{
			pos.y -= WINDOW_HEIGHT - height_post_scale;
			pos.y *= -1.0;
			pos.x -= WINDOW_WIDTH - width;

		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE):
		{
			pos.y -= WINDOW_HEIGHT - height_post_scale;
			pos.y *= -1.0;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT):
		{
			pos.x -= WINDOW_WIDTH - width_post_scale;
			pos.y -= WINDOW_HEIGHT - height_post_scale;
			pos.x *= -1.0;
			pos.y *= -1.0;
		} break;
	} //Translate from there to the new orientation
	return pos;
}

void GameTextureNew::scale_left_percent(float percent, bool crop) {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (percent < 0.0) {
		return;
	}
	bool flipped = tex_accessor[TEX_COORD_BOTTOM_LEFT] != &tex_data[TEX_COORD_BOTTOM_LEFT] && tex_accessor[TEX_COORD_BOTTOM_LEFT] != &tex_data[TEX_COORD_TOP_LEFT];
	if (flipped) {
		flip_h();
	}
	if (crop) {
		if (1.0 - percent <= percent) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
		}
		else {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.x = clampf(0.0, percent, 1.0);
		}
	}
	tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.x = clampf(-1.0, percent * width_scale * -1.0, 1.0);
	tex_accessor[TEX_COORD_TOP_LEFT]->pos.x = clampf(-1.0, percent * width_scale * -1.0, 1.0);
	if (flipped) {
		flip_h();
	}
	else {
		update_buffer_data();
	}
}

void GameTextureNew::scale_right_percent(float percent, bool crop) {
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (percent < 0.0) {
		return;
	}
	bool flipped = tex_accessor[TEX_COORD_BOTTOM_RIGHT] != &tex_data[TEX_COORD_BOTTOM_RIGHT] && tex_accessor[TEX_COORD_BOTTOM_RIGHT] != &tex_data[TEX_COORD_TOP_RIGHT];
	if (flipped) {
		flip_h();
	}
	if (crop) {
		if (1.0 - percent >= percent) {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, 1.0 - percent, 1.0);
		}
		else {
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.x = clampf(0.0, percent, 1.0);
		}
	}
	tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.x = clampf(-1.0, percent * width_scale, 1.0);
	tex_accessor[TEX_COORD_TOP_RIGHT]->pos.x = clampf(-1.0, percent * width_scale, 1.0);
	if (flipped) {
		flip_h();
	}
	else {
		update_buffer_data();
	}
}

void GameTextureNew::scale_top_percent(float percent, bool crop) {
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (percent < 0.0) {
		return;
	}
	bool flipped = tex_accessor[TEX_COORD_TOP_LEFT] != &tex_data[TEX_COORD_TOP_LEFT] && tex_accessor[TEX_COORD_TOP_LEFT] != &tex_data[TEX_COORD_TOP_RIGHT];
	if (flipped) {
		flip_v();
	}
	if (crop) {
		if (1.0 - percent >= percent) {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
		}
		else {
			tex_accessor[TEX_COORD_TOP_LEFT]->tex_coord.y = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y = clampf(0.0, percent, 1.0);
		}
	}
	tex_accessor[TEX_COORD_TOP_LEFT]->pos.y = clampf(-1.0, percent * height_scale, 1.0);
	tex_accessor[TEX_COORD_TOP_RIGHT]->pos.y = clampf(-1.0, percent * height_scale, 1.0);
	if (flipped) {
		flip_v();
	}
	else {
		update_buffer_data();
	}
}

void GameTextureNew::scale_bottom_percent(float percent, bool crop) {
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (percent < 0.0) {
		return;
	}
	bool flipped = tex_accessor[TEX_COORD_BOTTOM_LEFT] != &tex_data[TEX_COORD_BOTTOM_LEFT] && tex_accessor[TEX_COORD_BOTTOM_LEFT] != &tex_data[TEX_COORD_BOTTOM_RIGHT];
	if (flipped) {
		flip_v();
	}
	if (crop) {
		if (1.0 - percent <= percent) {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, 1.0 - percent, 1.0);
		}
		else {
			tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.y = clampf(0.0, percent, 1.0);
			tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y = clampf(0.0, percent, 1.0);
		}
	}
	tex_accessor[TEX_COORD_BOTTOM_LEFT]->pos.y = clampf(-1.0, percent * height_scale * -1.0, 1.0);
	tex_accessor[TEX_COORD_BOTTOM_RIGHT]->pos.y = clampf(-1.0, percent * height_scale * -1.0, 1.0);
	if (flipped) {
		flip_v();
	}
	else {
		update_buffer_data();
	}
}

void GameTextureNew::scale_all_percent(float percent, bool crop) {
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

void GameTextureNew::set_width(int new_width) {
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

void GameTextureNew::set_height(int new_height) {
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

int GameTextureNew::get_width() {
	return width;
}

int GameTextureNew::get_height() {
	return height;
}

void GameTextureNew::set_left_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_left_crop = percent;
	this->target_left_max_change = max_change;
}

void GameTextureNew::set_right_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_right_crop = percent;
	this->target_right_max_change = max_change;
}

void GameTextureNew::set_top_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_top_crop = percent;
	this->target_top_max_change = max_change;
}

void GameTextureNew::set_bottom_target(float percent, float max_change) {
	if (percent < 0.0 || max_change <= 0.0) {
		return;
	}
	this->target_bottom_crop = percent;
	this->target_bottom_max_change = max_change;
}

void GameTextureNew::set_target_pos(vec3 target_pos, float frames) {
	this->target_pos = target_pos;
	this->target_pos_max_change = distance(target_pos, pos) / vec3(frames);
}

void GameTextureNew::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
}

void GameTextureNew::flip_h() {
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
	update_buffer_data();
}

void GameTextureNew::flip_v() {
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
	update_buffer_data();
}

void GameTextureNew::reorient() {
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
void GameTextureNew::render() {
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
	float width_post_scale = width * (tex_accessor[TEX_COORD_BOTTOM_LEFT]->tex_coord.x + tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.x);
	float height_post_scale = height * (tex_accessor[TEX_COORD_BOTTOM_RIGHT]->tex_coord.y + tex_accessor[TEX_COORD_TOP_RIGHT]->tex_coord.y);

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
			gl_pos.x += WINDOW_WIDTH - width_post_scale;
			gl_pos.y -= WINDOW_HEIGHT - height;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT): {
			gl_pos.x -= WINDOW_WIDTH - width;
		} break;
		case (GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.x += WINDOW_WIDTH - width_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_LEFT): {
			gl_pos.y *= -1.0;
			gl_pos.x -= WINDOW_WIDTH - width;
			gl_pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_MIDDLE): {
			gl_pos.y *= -1.0;
			gl_pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
		case (GAME_TEXTURE_ORIENTATION_TOP_RIGHT): {
			gl_pos.x *= -1.0;
			gl_pos.y *= -1.0;
			gl_pos.x += WINDOW_WIDTH - width_post_scale;
			gl_pos.y += WINDOW_HEIGHT - height_post_scale;
		} break;
	}
	gl_pos.x /= (float)WINDOW_WIDTH;
	gl_pos.y /= (float)WINDOW_HEIGHT;
	float shader_alpha = 1.0 - ((float)alpha / 255.0);
	mat4 matrix = mat4(1.0);
	matrix = rotate(matrix, radians(rot.x), vec3(1.0, 0.0, 0.0));
	matrix = rotate(matrix, radians(rot.y), vec3(0.0, 1.0, 0.0));
	matrix = rotate(matrix, radians(rot.z), vec3(0.0, 0.0, 1.0));
	matrix = translate(matrix, gl_pos);
	shader->set_float("f_alphamod", shader_alpha);
	shader->set_mat4("matrix", matrix);
	glDepthMask(gl_pos.z != 0.0);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameTextureNew::update_buffer_data() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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