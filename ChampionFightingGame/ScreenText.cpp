#include "ScreenText.h"
#include "WindowConstants.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

ScreenText::ScreenText() {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	VAO = 0;
	VBO = 0;
#endif
	texture = 0;
	num_lines = 0;
	shader = nullptr;
	text = "";
	font = nullptr;
	scroll = -1.0f;
	screen_orientation = TEXTURE_MID;
	texture_orientation = TEXTURE_MID;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	base_width = 0;
	base_height = 0;
	width_scale = 1.0f;
	height_scale = 1.0f;
	loaded = false;
}

ScreenText::ScreenText(Font* font, std::string text, TextSpecifier spec) : ScreenText() {
	init(font, text, spec);
}

ScreenText::ScreenText(ScreenText& that) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	VAO = that.VAO;
	VBO = that.VBO;
#endif
	v_pos.resize(that.v_pos.size());
	v_texcoord.resize(that.v_texcoord.size());
	v_data_for_gpu.resize(that.v_data_for_gpu.size());
	for (int i = 0; i < that.v_pos.size(); i++) {
		v_pos[i] = that.v_pos[i];
		v_texcoord[i] = that.v_texcoord[i];
		v_data_for_gpu[i] = that.v_data_for_gpu[i];
	}
	texture = that.texture;
	num_lines = that.num_lines;
	shader = that.shader;
	text = that.text;
	font = that.font;
	scroll = that.scroll;
	screen_orientation = that.screen_orientation;
	texture_orientation = that.texture_orientation;
	pos = that.pos;
	rot = that.rot;
	base_width = that.base_width;
	base_height = that.base_height;
	width_scale = that.width_scale;
	height_scale = that.height_scale;
	spec = that.spec;
	loaded = false;
}

ScreenText::ScreenText(ScreenText&& that) noexcept {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	VAO = that.VAO;
	VBO = that.VBO;
#endif
	v_pos.resize(that.v_pos.size());
	v_texcoord.resize(that.v_texcoord.size());
	v_data_for_gpu.resize(that.v_data_for_gpu.size());
	for (int i = 0; i < that.v_pos.size(); i++) {
		v_pos[i] = that.v_pos[i];
		v_texcoord[i] = that.v_texcoord[i];
		v_data_for_gpu[i] = that.v_data_for_gpu[i];
	}
	texture = that.texture;
	num_lines = that.num_lines;
	shader = that.shader;
	text = that.text;
	font = that.font;
	scroll = that.scroll;
	screen_orientation = that.screen_orientation;
	texture_orientation = that.texture_orientation;
	pos = that.pos;
	rot = that.rot;
	base_width = that.base_width;
	base_height = that.base_height;
	width_scale = that.width_scale;
	height_scale = that.height_scale;
	spec = that.spec;
	loaded = that.loaded;
	that.loaded = false;
}

ScreenText& ScreenText::operator=(ScreenText& that) {
	if (this != &that) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		VAO = that.VAO;
		VBO = that.VBO;
#endif
		v_pos.resize(that.v_pos.size());
		v_texcoord.resize(that.v_texcoord.size());
		v_data_for_gpu.resize(that.v_data_for_gpu.size());
		for (int i = 0; i < that.v_pos.size(); i++) {
			v_pos[i] = that.v_pos[i];
			v_texcoord[i] = that.v_texcoord[i];
			v_data_for_gpu[i] = that.v_data_for_gpu[i];
		}
		texture = that.texture;
		num_lines = that.num_lines;
		shader = that.shader;
		text = that.text;
		font = that.font;
		scroll = that.scroll;
		screen_orientation = that.screen_orientation;
		texture_orientation = that.texture_orientation;
		pos = that.pos;
		rot = that.rot;
		base_width = that.base_width;
		base_height = that.base_height;
		width_scale = that.width_scale;
		height_scale = that.height_scale;
		spec = that.spec;
		loaded = false;
	}
	return *this;
}

ScreenText& ScreenText::operator=(ScreenText&& that) noexcept {
	if (this != &that) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		VAO = that.VAO;
		VBO = that.VBO;
#endif
		v_pos.resize(that.v_pos.size());
		v_texcoord.resize(that.v_texcoord.size());
		v_data_for_gpu.resize(that.v_data_for_gpu.size());
		for (int i = 0; i < that.v_pos.size(); i++) {
			v_pos[i] = that.v_pos[i];
			v_texcoord[i] = that.v_texcoord[i];
			v_data_for_gpu[i] = that.v_data_for_gpu[i];
		}
		texture = that.texture;
		num_lines = that.num_lines;
		shader = that.shader;
		text = that.text;
		font = that.font;
		scroll = that.scroll;
		screen_orientation = that.screen_orientation;
		texture_orientation = that.texture_orientation;
		pos = that.pos;
		rot = that.rot;
		base_width = that.base_width;
		base_height = that.base_height;
		width_scale = that.width_scale;
		height_scale = that.height_scale;
		spec = that.spec;
		loaded = that.loaded;
		that.loaded = false;
	}
	return *this;
}

ScreenText::~ScreenText() {
	if (loaded) {
		destroy();
	}
}

ScreenText& ScreenText::init(Font* font, std::string text, TextSpecifier spec) {
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
	this->text = text;
	this->spec = spec;
	this->font = font;
	texture = font->create_text(text, spec, &num_lines, nullptr);

	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, base_width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, base_height.get_val_ptr());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	loaded = true;

	return *this;
}

void ScreenText::destroy() {
	if (loaded) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		loaded = false;
	}
}

ScreenText& ScreenText::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("screen_tex", "screen_tex_" + frag_shader, "", 0);
	return *this;
}

ScreenText& ScreenText::set_pause(bool pause) {
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].x.set_pause(pause);
		v_pos[i].y.set_pause(pause);
		v_texcoord[i].x.set_pause(pause);
		v_texcoord[i].y.set_pause(pause);
	}
	pos.set_pause(pause);
	rot.set_pause(pause);
	base_width.set_pause(pause);
	base_height.set_pause(pause);
	width_scale.set_pause(pause);
	height_scale.set_pause(pause);
	return *this;
}

ScreenText& ScreenText::update_text(std::string text) {
	this->text = text;
	texture = font->create_text(text, spec, &num_lines, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, base_width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, base_height.get_val_ptr());

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return *this;
}

ScreenText& ScreenText::start_scroll(int frames) {
	for (int i = 0; i < num_lines; i++) {
		v_pos[(i * 6) + 1].x = -1.0f;
		v_texcoord[(i * 6) + 1].x = 0.0f;
		v_pos[(i * 6) + 3].x = -1.0f;
		v_texcoord[(i * 6) + 3].x = 0.0f;
		v_pos[(i * 6) + 4].x = -1.0f;
		v_texcoord[(i * 6) + 4].x = 0.0f;
	}
	scroll = 0.0f;
	scroll.set_target_val(1.0f, frames);
	return *this;
}

ScreenText& ScreenText::set_screen_orientation(int orientation) {
	this->screen_orientation = orientation;
	return *this;
}

ScreenText& ScreenText::set_texture_orientation(int orientation) {
	this->texture_orientation = orientation;
	return *this;
}

ScreenText& ScreenText::set_orientation(int orientation) {
	this->screen_orientation = orientation;
	this->texture_orientation = orientation;
	return *this;
}

int ScreenText::get_screen_orientation() const {
	return screen_orientation;
}

int ScreenText::get_texture_orientation() const {
	return texture_orientation;
}

ScreenText& ScreenText::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return *this;
}

ScreenText& ScreenText::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return *this;
}

ScreenText& ScreenText::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return *this;
}

glm::vec3 ScreenText::get_pos() const {
	return pos.get_val();
}

ScreenText& ScreenText::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return *this;
}

ScreenText& ScreenText::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return *this;
}

ScreenText& ScreenText::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return *this;
}

glm::vec3 ScreenText::get_rot() const {
	return rot.get_val();
}

ScreenText& ScreenText::set_base_width(int new_width) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	base_width = new_width;
	float curr_width_scaler = get_width() / WINDOW_WIDTH;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

ScreenText& ScreenText::set_base_width(int new_width, int frames) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	float curr_width_scaler = (width_scale * new_width) / WINDOW_WIDTH;
	base_width.set_target_val(new_width, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_base_width(int width) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	base_width += width;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

int ScreenText::get_base_width() const {
	return base_width.get_val();
}

ScreenText& ScreenText::set_base_height(int new_height) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	base_height = new_height;
	float curr_height_scaler = get_height() / WINDOW_HEIGHT;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

ScreenText& ScreenText::set_base_height(int new_height, int frames) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	float curr_height_scaler = (height_scale * new_height) / WINDOW_HEIGHT;
	base_height.set_target_val(new_height, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_base_height(int height) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	base_height += height;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

int ScreenText::get_base_height() const {
	return base_height.get_val();
}

float ScreenText::get_width() const {
	return width_scale * base_width;
}

float ScreenText::get_height() const {
	return height_scale * base_height;
}

ScreenText& ScreenText::set_width_scale(float scale) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	width_scale = scale;
	float curr_width_scaler = get_width() / WINDOW_WIDTH;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

ScreenText& ScreenText::set_width_scale(float scale, int frames) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	float curr_width_scaler = ((float)base_width * scale) / WINDOW_WIDTH;
	width_scale.set_target_val(scale, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_width_scale(float scale) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	width_scale += scale;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

float ScreenText::get_width_scale() const {
	return width_scale.get_val();
}

ScreenText& ScreenText::set_height_scale(float scale) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	height_scale = scale;
	float curr_height_scaler = get_height() / WINDOW_HEIGHT;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

ScreenText& ScreenText::set_height_scale(float scale, int frames) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	float curr_height_scaler = ((float)base_height * scale) / WINDOW_HEIGHT;
	height_scale.set_target_val(scale, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_height_scale(float scale) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	height_scale += scale;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 6 * num_lines; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

float ScreenText::get_height_scale() const {
	return height_scale.get_val();
}

ScreenText& ScreenText::set_scale(float scale) {
	set_width_scale(scale);
	set_height_scale(scale);
	return *this;
}

ScreenText& ScreenText::set_scale(float scale, int frames) {
	set_width_scale(scale, frames);
	set_height_scale(scale, frames);
	return *this;
}

void ScreenText::render() {
	glm::vec3 render_pos = pos.get_val();

	if (screen_orientation & TEXTURE_LEFT) {
		render_pos.x -= WINDOW_WIDTH;
	}
	else if (screen_orientation & TEXTURE_RIGHT) {
		render_pos.x *= -1.0;
		render_pos.x += WINDOW_WIDTH;
	}

	if (screen_orientation & TEXTURE_TOP) {
		render_pos.y *= -1.0;
		render_pos.y += WINDOW_HEIGHT;
	}
	else if (screen_orientation & TEXTURE_BOTTOM) {
		render_pos.y -= WINDOW_HEIGHT;
	}
	if (texture_orientation & TEXTURE_LEFT) {
		render_pos.x += get_width();
	}
	else if (texture_orientation & TEXTURE_RIGHT) {
		render_pos.x -= get_width();
	}
	if (texture_orientation & TEXTURE_TOP) {
		render_pos.y -= get_height();
	}
	else if (texture_orientation & TEXTURE_BOTTOM) {
		render_pos.y += get_height();
	}
	render_pos.x /= (float)WINDOW_WIDTH;
	render_pos.y /= (float)WINDOW_HEIGHT;
	glm::mat4 matrix = glm::translate(glm::mat4(1.0), render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	update_buffer_data();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->use();
	shader->set_mat4("matrix", matrix);
	if (glIsEnabled(GL_CULL_FACE)) {
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 6 * num_lines);
		glEnable(GL_CULL_FACE);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 6 * num_lines);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenText::set_default_vertex_data() {
	if (!spec.enable_multiline_scroll) num_lines = 1;
	bool new_size = (num_lines != v_data_for_gpu.size());
	v_pos.clear();
	v_texcoord.clear();
	v_data_for_gpu.clear();
	for (int i = 0; i < num_lines; i++) {
		float y_bottom_coord = (float)(num_lines - i - 1) / (float)num_lines;
		float y_bottom_pos = (y_bottom_coord * 2.0f) - 1.0f;
		float y_top_coord = y_bottom_coord + (1.0f / (float)num_lines);
		float y_top_pos = (y_top_coord * 2.0f) - 1.0f;

		v_pos.push_back(TargetVec2(-1.0, y_top_pos));
		v_texcoord.push_back(TargetVec2(0.0, y_top_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });

		v_pos.push_back(TargetVec2(1.0, y_top_pos));
		v_texcoord.push_back(TargetVec2(1.0, y_top_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });

		v_pos.push_back(TargetVec2(-1.0, y_bottom_pos));
		v_texcoord.push_back(TargetVec2(0.0, y_bottom_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });

		v_pos.push_back(TargetVec2(1.0, y_bottom_pos));
		v_texcoord.push_back(TargetVec2(1.0, y_bottom_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });

		v_pos.push_back(TargetVec2(1.0, y_top_pos));
		v_texcoord.push_back(TargetVec2(1.0, y_top_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });

		v_pos.push_back(TargetVec2(-1.0, y_bottom_pos));
		v_texcoord.push_back(TargetVec2(0.0, y_bottom_coord));
		v_data_for_gpu.push_back({ v_pos.back(), v_texcoord.back() });
	}

	for (int i = 0; i < v_data_for_gpu.size(); i++) {
		v_data_for_gpu[i].pos.x *= get_width() / (float)WINDOW_WIDTH;
		v_data_for_gpu[i].pos.y *= get_height() / (float)WINDOW_HEIGHT;
	}

	if (new_size) {
		glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
	}
}

void ScreenText::update_buffer_data() {
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / (float)WINDOW_WIDTH,
		get_height() / (float)WINDOW_HEIGHT
	);
	if (scroll != -1.0f) {
		int i = 0;
		for (float add = 1.0f / (float)num_lines; scroll > add * (float)(i + 1); i++);
		float val = (scroll - ((float)i / (float)num_lines)) * num_lines;

		v_pos[(i * 6) + 1].x = ((val - 0.5f) * 2.0f);
		v_texcoord[(i * 6) + 1].x = val;
		v_pos[(i * 6) + 3].x = ((val - 0.5f) * 2.0f) ;
		v_texcoord[(i * 6) + 3].x = val;
		v_pos[(i * 6) + 4].x = ((val - 0.5f) * 2.0f) ;
		v_texcoord[(i * 6) + 4].x = val;

		while (i --> 0) {
			v_pos[(i * 6) + 1].x = 1.0f;
			v_texcoord[(i * 6) + 1].x = 1.0f;
			v_pos[(i * 6) + 3].x = 1.0f;
			v_texcoord[(i * 6) + 3].x = 1.0f;
			v_pos[(i * 6) + 4].x = 1.0f;
			v_texcoord[(i * 6) + 4].x = 1.0f;
		}

		if (scroll.get_frames() == 0) {
			scroll = -1.0f;
		}
	}
	for (int i = 0; i < v_data_for_gpu.size(); i++) {
		if (v_data_for_gpu[i].pos != (glm::vec2)v_pos[i] * v_pos_scaler
		|| v_data_for_gpu[i].tex_coord != v_texcoord[i]) {
			for (int i = 0; i < v_data_for_gpu.size(); i++) {
				v_data_for_gpu[i] = { (glm::vec2)v_pos[i] * v_pos_scaler, v_texcoord[i] };
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
			return;
		}
	}
}