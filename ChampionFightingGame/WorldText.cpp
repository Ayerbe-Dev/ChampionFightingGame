#include "WorldText.h"
#include "WindowConstants.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "utils.h"

WorldText::WorldText() {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = 0;
	this->VBO = 0;
#endif
	this->texture = 0;
	this->num_lines = 0;
	this->shader = nullptr;
	this->text = "";
	this->font = nullptr;
	this->scroll = -1.0f;
	this->texture_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->scale = glm::vec3(1.0);
	this->width = 0;
	this->height = 0;
	this->billboard_setting = BILLBOARD_OFF;
	this->loaded = false;
}

WorldText::WorldText(Font* font, std::string text, TextSpecifier spec) : WorldText() {
	init(font, text, spec);
}

WorldText::WorldText(WorldText& other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = other.VAO;
	this->VBO = other.VBO;
#endif
	this->v_pos.resize(other.v_pos.size());
	this->v_texcoord.resize(other.v_texcoord.size());
	this->v_data_for_gpu.resize(other.v_data_for_gpu.size());
	for (int i = 0; i < other.v_pos.size(); i++) {
		this->v_pos[i] = other.v_pos[i];
		this->v_texcoord[i] = other.v_texcoord[i];
		this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
	}
	this->texture = other.texture;
	this->num_lines = other.num_lines;
	this->shader = other.shader;
	this->text = other.text;
	this->font = other.font;
	this->scroll = other.scroll;
	this->texture_orientation = other.texture_orientation;
	this->texture = other.texture;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->width = other.width;
	this->height = other.height;
	this->billboard_setting = other.billboard_setting;
	this->spec = other.spec;
	this->loaded = false;
}

WorldText::WorldText(WorldText&& other) noexcept {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = other.VAO;
	this->VBO = other.VBO;
#endif
	this->v_pos.resize(other.v_pos.size());
	this->v_texcoord.resize(other.v_texcoord.size());
	this->v_data_for_gpu.resize(other.v_data_for_gpu.size());
	for (int i = 0; i < other.v_pos.size(); i++) {
		this->v_pos[i] = other.v_pos[i];
		this->v_texcoord[i] = other.v_texcoord[i];
		this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
}
	this->texture = other.texture;
	this->num_lines = other.num_lines;
	this->shader = other.shader;
	this->text = other.text;
	this->font = other.font;
	this->scroll = other.scroll;
	this->texture_orientation = other.texture_orientation;
	this->texture = other.texture;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->width = other.width;
	this->height = other.height;
	this->billboard_setting = other.billboard_setting;
	this->spec = other.spec;
	this->loaded = other.loaded;
	other.loaded = false;
}

WorldText& WorldText::operator=(WorldText& other) {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		this->VAO = other.VAO;
		this->VBO = other.VBO;
#endif
		this->v_pos.resize(other.v_pos.size());
		this->v_texcoord.resize(other.v_texcoord.size());
		this->v_data_for_gpu.resize(other.v_data_for_gpu.size());
		for (int i = 0; i < other.v_pos.size(); i++) {
			this->v_pos[i] = other.v_pos[i];
			this->v_texcoord[i] = other.v_texcoord[i];
			this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
		}
		this->texture = other.texture;
		this->num_lines = other.num_lines;
		this->shader = other.shader;
		this->text = other.text;
		this->font = other.font;
		this->scroll = other.scroll;
		this->texture_orientation = other.texture_orientation;
		this->texture = other.texture;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->width = other.width;
		this->height = other.height;
		this->billboard_setting = other.billboard_setting;
		this->spec = other.spec;
		this->loaded = false;
	}
	return *this;
}

WorldText& WorldText::operator=(WorldText&& other) noexcept {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		this->VAO = other.VAO;
		this->VBO = other.VBO;
#endif
		this->v_pos.resize(other.v_pos.size());
		this->v_texcoord.resize(other.v_texcoord.size());
		this->v_data_for_gpu.resize(other.v_data_for_gpu.size());
		for (int i = 0; i < other.v_pos.size(); i++) {
			this->v_pos[i] = other.v_pos[i];
			this->v_texcoord[i] = other.v_texcoord[i];
			this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
		}
		this->texture = other.texture;
		this->num_lines = other.num_lines;
		this->shader = other.shader;
		this->text = other.text;
		this->font = other.font;
		this->scroll = other.scroll;
		this->texture_orientation = other.texture_orientation;
		this->texture = other.texture;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->width = other.width;
		this->height = other.height;
		this->billboard_setting = other.billboard_setting;
		this->spec = other.spec;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

WorldText::~WorldText() {
	if (loaded) {
		destroy();
	}
}

WorldText& WorldText::init(Font* font, std::string text, TextSpecifier spec) {
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
	this->text = text;
	this->spec = spec;
	this->font = font;
	texture = font->create_text(text, spec, &num_lines, nullptr);

	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height.get_val_ptr());

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

void WorldText::destroy() {
	if (loaded) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		loaded = false;
	}
}

WorldText& WorldText::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("world_tex", "world_tex_" + frag_shader, "", billboard_setting);
	return *this;
}

WorldText& WorldText::set_pause(bool pause) {
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].x.set_pause(pause);
		v_pos[i].y.set_pause(pause);
		v_texcoord[i].x.set_pause(pause);
		v_texcoord[i].y.set_pause(pause);
	}
	pos.set_pause(pause);
	rot.set_pause(pause);
	scale.set_pause(pause);
	width.set_pause(pause);
	height.set_pause(pause);
	return *this;
}

WorldText& WorldText::update_text(std::string text) {
	this->text = text;
	texture = font->create_text(text, spec, &num_lines, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height.get_val_ptr());

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return *this;
}

WorldText& WorldText::start_scroll(int frames) {
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

WorldText& WorldText::set_orientation(int orientation) {
	this->texture_orientation = orientation;
	return *this;
}

int WorldText::get_texture_orientation() const {
	return texture_orientation;
}

WorldText& WorldText::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return *this;
}

WorldText& WorldText::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return *this;
}

WorldText& WorldText::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return *this;
}

glm::vec3 WorldText::get_pos() const {
	return pos.get_val();
}

WorldText& WorldText::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return *this;
}

WorldText& WorldText::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return *this;
}

WorldText& WorldText::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return *this;
}

glm::vec3 WorldText::get_rot() const {
	return rot.get_val();
}

WorldText& WorldText::set_width(int new_width) {
	float prev_width_scaler = width / (float)WINDOW_WIDTH;
	float curr_width_scaler = new_width / (float)WINDOW_WIDTH;
	width = new_width;
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

WorldText& WorldText::set_width(int new_width, int frames) {
	float prev_width_scaler = width / (float)WINDOW_WIDTH;
	float curr_width_scaler = new_width / (float)WINDOW_WIDTH;
	width.set_target_val(new_width, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

WorldText& WorldText::add_width(int width) {
	float prev_width_scaler = width / (float)WINDOW_WIDTH;
	width += width;
	float curr_width_scaler = width / (float)WINDOW_WIDTH;
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

int WorldText::get_width() const {
	return width.get_val();
}

WorldText& WorldText::set_height(int new_height) {
	float prev_height_scaler = height / (float)WINDOW_HEIGHT;
	float curr_height_scaler = new_height / (float)WINDOW_HEIGHT;
	height = new_height;
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

WorldText& WorldText::set_height(int new_height, int frames) {
	float prev_height_scaler = height / (float)WINDOW_HEIGHT;
	float curr_height_scaler = new_height / (float)WINDOW_HEIGHT;
	height.set_target_val(new_height, frames);
	for (int i = 0; i < 6 * num_lines; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

WorldText& WorldText::add_height(int height) {
	float prev_height_scaler = height / (float)WINDOW_HEIGHT;
	height += height;
	float curr_height_scaler = height / (float)WINDOW_HEIGHT;
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

int WorldText::get_height() const {
	return height.get_val();
}

WorldText& WorldText::set_scale(glm::vec3 scale) {
	this->scale = scale;
	return *this;
}

WorldText& WorldText::set_scale(glm::vec3 scale, int frames) {
	this->scale.set_target_val(scale, frames);
	return *this;
}

WorldText& WorldText::add_scale(glm::vec3 scale) {
	this->scale += scale;
	return *this;
}

glm::vec3 WorldText::get_scale() const {
	return scale.get_val();
}

WorldText& WorldText::set_billboard_setting(int billboard_setting) {
	this->shader = ShaderManager::get_instance()->get_shader_switch_features(shader, this->billboard_setting, billboard_setting);
	this->billboard_setting = billboard_setting;
	return *this;
}

int WorldText::get_billboard_setting() const {
	return billboard_setting;
}

void WorldText::render() {
	glm::vec3 scale_vec = glm::vec3(
		WINDOW_WIDTH / (100 * scale.get_val().x),
		WINDOW_HEIGHT / (100 * scale.get_val().y),
		WINDOW_DEPTH / (100 * scale.get_val().z)
	);
	glm::vec3 render_pos = pos.get_val() / scale_vec;
	if (texture_orientation & TEXTURE_LEFT) {
		render_pos.x += scale_vec.x;
	}
	else if (texture_orientation & TEXTURE_RIGHT) {
		render_pos.x -= scale_vec.x;
	}
	if (texture_orientation & TEXTURE_TOP) {
		render_pos.y -= scale_vec.y;
	}
	else if (texture_orientation & TEXTURE_BOTTOM) {
		render_pos.y += scale_vec.y;
	}
	glm::mat4 matrix = glm::translate(glm::mat4(1.0), render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	matrix = glm::scale(matrix, scale.get_val());
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	update_buffer_data();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->use();
	shader->set_mat4("matrix", matrix);
	switch (billboard_setting) {
	case BILLBOARD_OFF:
	default: {

	} break;
	case BILLBOARD_ON: {
	} break;
	case BILLBOARD_ON_FIXED_SIZE: {

	} break;
	}
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
#endif
}

void WorldText::set_default_vertex_data() {
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
		v_data_for_gpu[i].pos *= glm::vec2(
			(float)get_width() / (WINDOW_WIDTH / (10 * scale.get_val().x)),
			(float)get_height() / (WINDOW_WIDTH / (10 * scale.get_val().y))
		);
	}

	if (new_size) {
		glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
	}
}

void WorldText::update_buffer_data() {
	bool update = false;
	glm::vec2 v_pos_scaler = glm::vec2(
		(float)get_width() / (WINDOW_WIDTH / (10 * scale.get_val().x)),
		(float)get_height() / (WINDOW_WIDTH / (10 * scale.get_val().y))
	);
	if (scroll != -1.0f) {
		int i = 0;
		for (float add = 1.0f / (float)num_lines; scroll > add * (float)(i + 1); i++);
		float val = (scroll - ((float)i / (float)num_lines)) * num_lines;

		v_pos[(i * 6) + 1].x = ((val - 0.5f) * 2.0f);
		v_texcoord[(i * 6) + 1].x = val;
		v_pos[(i * 6) + 3].x = ((val - 0.5f) * 2.0f);
		v_texcoord[(i * 6) + 3].x = val;
		v_pos[(i * 6) + 4].x = ((val - 0.5f) * 2.0f);
		v_texcoord[(i * 6) + 4].x = val;

		while (i-- > 0) {
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