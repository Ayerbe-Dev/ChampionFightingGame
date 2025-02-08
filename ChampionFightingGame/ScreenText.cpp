#include "ScreenText.h"
#include "WindowConstants.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

ScreenText::ScreenText() {
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
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->anchor = nullptr;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->magnitude = glm::vec3(0.0);
	this->base_width = 0;
	this->base_height = 0;
	this->width_scale = 1.0f;
	this->height_scale = 1.0f;
	this->alpha = 255;
	this->buffer_updates = 0;
	this->loaded = false;
}

ScreenText::ScreenText(Font* font, std::string text, TextSpecifier spec) : ScreenText() {
	init(font, text, spec);
}

ScreenText::ScreenText(ScreenText& other) {
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
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->base_height = other.base_height;
	this->width_scale = other.width_scale;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->spec = other.spec;
	this->buffer_updates = other.buffer_updates;
	if (ResourceManager::get_instance()->is_tex_const_copied(texture)) {
		this->loaded = other.loaded;
		other.loaded = false;
	}
	else {
		this->loaded = false;
	}
}

ScreenText::ScreenText(const ScreenText& other) {
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
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->base_height = other.base_height;
	this->width_scale = other.width_scale;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->spec = other.spec;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	ResourceManager::get_instance()->store_const_copy_addr(texture, (ScreenText*)&other);
}

ScreenText::ScreenText(ScreenText&& other) noexcept {
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
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->base_height = other.base_height;
	this->width_scale = other.width_scale;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->spec = other.spec;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	other.loaded = false;
}

ScreenText& ScreenText::operator=(ScreenText& other) {
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
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->base_height = other.base_height;
		this->width_scale = other.width_scale;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->spec = other.spec;
		this->buffer_updates = other.buffer_updates;
		if (ResourceManager::get_instance()->is_tex_const_copied(texture)) {
			this->loaded = other.loaded;
			other.loaded = false;
		}
		else {
			this->loaded = false;
		}
	}
	return *this;
}

ScreenText& ScreenText::operator=(const ScreenText& other) {
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
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->base_height = other.base_height;
		this->width_scale = other.width_scale;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->spec = other.spec;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		ResourceManager::get_instance()->store_const_copy_addr(texture, (ScreenText*)&other);
	}
	return *this;
}

ScreenText& ScreenText::operator=(ScreenText&& other) noexcept {
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
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->base_height = other.base_height;
		this->width_scale = other.width_scale;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->spec = other.spec;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

ScreenText::~ScreenText() {
	if (loaded && !ResourceManager::get_instance()->is_tex_const_copied(texture, this)) {
		destroy();
	}
}

ScreenText&& ScreenText::init(Font* font, std::string text, TextSpecifier spec) {
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
	this->text = text;
	this->spec = spec;
	this->alpha = spec.rgba.a;
	this->font = font;
	texture = font->create_text(text, spec.rgba, spec.border_rgbs, spec.enable_center, spec.max_line_length, &num_lines, nullptr);

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

	return std::move(*this);
}

void ScreenText::destroy() {
	if (loaded) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteTextures(1, &texture);
		loaded = false;
	}
}

ScreenText&& ScreenText::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("screen_tex", "screen_tex_" + frag_shader, "", 0);
	return std::move(*this);
}

ScreenText&& ScreenText::set_pause(bool pause) {
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
	return std::move(*this);
}

ScreenText&& ScreenText::update_text(std::string text) {
	this->text = text;
	texture = font->create_text(text, spec.rgba, spec.border_rgbs, spec.enable_center, spec.max_line_length, &num_lines, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, base_width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, base_height.get_val_ptr());

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return std::move(*this);
}

ScreenText&& ScreenText::start_scroll(int frames) {
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
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::set_screen_orientation(int orientation) {
	this->screen_orientation = orientation;
	return std::move(*this);
}

ScreenText&& ScreenText::set_texture_orientation(int orientation) {
	this->texture_orientation = orientation;
	return std::move(*this);
}

ScreenText&& ScreenText::set_orientation(int orientation) {
	this->screen_orientation = orientation;
	this->texture_orientation = orientation;
	return std::move(*this);
}

int ScreenText::get_screen_orientation() const {
	return screen_orientation;
}

int ScreenText::get_texture_orientation() const {
	return texture_orientation;
}

ScreenText&& ScreenText::set_anchor(TextureAnchor* anchor) {
	this->anchor = anchor;
	return std::move(*this);
}

ScreenText&& ScreenText::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return std::move(*this);
}

ScreenText&& ScreenText::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return std::move(*this);
}

glm::vec3 ScreenText::get_pos() const {
	return pos.get_val();
}

TargetVar<glm::vec3> ScreenText::get_pos_target() const {
	return pos;
}

ScreenText&& ScreenText::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return std::move(*this);
}

ScreenText&& ScreenText::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return std::move(*this);
}

glm::vec3 ScreenText::get_rot() const {
	return rot.get_val();
}

ScreenText&& ScreenText::set_magnitude(glm::vec3 magnitude) {
	this->magnitude = magnitude;
	return std::move(*this);
}

ScreenText&& ScreenText::set_magnitude(glm::vec3 magnitude, int frames) {
	this->magnitude.set_target_val(magnitude, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_magnitude(glm::vec3 magnitude) {
	this->magnitude += magnitude;
	return std::move(*this);
}

glm::vec3 ScreenText::get_magnitude() const {
	return magnitude.get_val();
}

ScreenText&& ScreenText::set_base_width(int new_width) {
	base_width = new_width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenText&& ScreenText::set_base_width(int new_width, int frames) {
	base_width.set_target_val(new_width, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_base_width(int width) {
	base_width += width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

int ScreenText::get_base_width() const {
	return base_width.get_val();
}

ScreenText&& ScreenText::set_base_height(int new_height) {
	base_height = new_height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenText&& ScreenText::set_base_height(int new_height, int frames) {
	base_height.set_target_val(new_height, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_base_height(int height) {
	base_height += height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
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

ScreenText&& ScreenText::set_width_scale(float scale) {
	width_scale = scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenText&& ScreenText::set_width_scale(float scale, int frames) {
	width_scale.set_target_val(scale, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_width_scale(float scale) {
	width_scale += scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

float ScreenText::get_width_scale() const {
	return width_scale.get_val();
}

ScreenText&& ScreenText::set_height_scale(float scale) {
	height_scale = scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenText&& ScreenText::set_height_scale(float scale, int frames) {
	height_scale.set_target_val(scale, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_height_scale(float scale) {
	height_scale += scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

float ScreenText::get_height_scale() const {
	return height_scale.get_val();
}

ScreenText&& ScreenText::set_scale(float scale) {
	set_width_scale(scale);
	set_height_scale(scale);
	return std::move(*this);
}

ScreenText&& ScreenText::set_scale(float scale, int frames) {
	set_width_scale(scale, frames);
	set_height_scale(scale, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
	return std::move(*this);
}

ScreenText&& ScreenText::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
	return std::move(*this);
}

ScreenText&& ScreenText::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
	return std::move(*this);
}

unsigned char ScreenText::get_alpha() const {
	return alpha.get_val();
}

ScreenText&& ScreenText::set_color(glm::vec3 color) {
	spec.rgba = glm::vec4(color, alpha.get_val());
	texture = font->create_text(text, spec.rgba, spec.border_rgbs, spec.enable_center, spec.max_line_length, &num_lines, &texture);
	return std::move(*this);
}

ScreenText&& ScreenText::add_color(glm::vec3 color) {
	spec.rgba.r += color.r;
	spec.rgba.g += color.g;
	spec.rgba.b += color.b;
	texture = font->create_text(text, spec.rgba, spec.border_rgbs, spec.enable_center, spec.max_line_length, &num_lines, &texture);
	return std::move(*this);
}

glm::vec3 ScreenText::get_color() const {
	return glm::vec3(spec.rgba);
}

void ScreenText::render() {
	glm::vec3 render_pos = pos.get_val();
	float window_width = WINDOW_WIDTH;
	float window_height = WINDOW_HEIGHT;
	if (anchor) {
		window_width = anchor->w;
		window_height = anchor->h;
	}
	if (screen_orientation & TEXTURE_LEFT) {
		render_pos.x -= window_width;
	}
	else if (screen_orientation & TEXTURE_RIGHT) {
		render_pos.x *= -1.0;
		render_pos.x += window_width;
	}

	if (screen_orientation & TEXTURE_TOP) {
		render_pos.y *= -1.0;
		render_pos.y += window_height;
	}
	else if (screen_orientation & TEXTURE_BOTTOM) {
		render_pos.y -= window_height;
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
	render_pos.x /= WINDOW_WIDTH;
	render_pos.y /= WINDOW_HEIGHT;
	glm::mat4 matrix = glm::translate(glm::mat4(1.0), render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	matrix = glm::translate(matrix, magnitude.get_val() / glm::vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f));
	if (anchor) {
		matrix = anchor->screen_mat * matrix;
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (buffer_updates) {
		update_buffer_data();
		buffer_updates--;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->use();
	shader->set_float("f_alphamod", alpha.get_val() / 255.0f);
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
	if (new_size) {
		v_pos.resize(num_lines * 6);
		v_texcoord.resize(num_lines * 6);
		v_data_for_gpu.resize(num_lines * 6);
	}
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / (float)WINDOW_WIDTH,
		get_height() / (float)WINDOW_HEIGHT
	);
	for (int i = 0; i < num_lines; i++) {
		float y_bottom_coord = (float)(num_lines - i - 1) / (float)num_lines;
		float y_bottom_pos = (y_bottom_coord * 2.0f) - 1.0f;
		float y_top_coord = y_bottom_coord + (1.0f / (float)num_lines);
		float y_top_pos = (y_top_coord * 2.0f) - 1.0f;

		v_pos[6 * i].x = -1.0f;
		v_pos[6 * i].y = y_top_pos;
		v_texcoord[6 * i].x = 0.0f;
		v_texcoord[6 * i].y = y_top_coord;
		v_data_for_gpu[6 * i].pos.x = -1.0f;
		v_data_for_gpu[6 * i].pos.y = y_top_pos;
		v_data_for_gpu[6 * i].tex_coord.x = 0.0f;
		v_data_for_gpu[6 * i].tex_coord.y = y_top_coord;

		v_pos[6 * i + 1].x = 1.0f;
		v_pos[6 * i + 1].y = y_top_pos;
		v_texcoord[6 * i + 1].x = 1.0f;
		v_texcoord[6 * i + 1].y = y_top_coord;
		v_data_for_gpu[6 * i + 1].pos.x = 1.0f;
		v_data_for_gpu[6 * i + 1].pos.y = y_top_pos;
		v_data_for_gpu[6 * i + 1].tex_coord.x = 1.0f;
		v_data_for_gpu[6 * i + 1].tex_coord.y = y_top_coord;

		v_pos[6 * i + 2].x = -1.0f;
		v_pos[6 * i + 2].y = y_bottom_pos;
		v_texcoord[6 * i + 2].x = 0.0f;
		v_texcoord[6 * i + 2].y = y_bottom_coord;
		v_data_for_gpu[6 * i + 2].pos.x = -1.0f;
		v_data_for_gpu[6 * i + 2].pos.y = y_bottom_pos;
		v_data_for_gpu[6 * i + 2].tex_coord.x = 0.0f;
		v_data_for_gpu[6 * i + 2].tex_coord.y = y_bottom_coord;

		v_pos[6 * i + 3].x = 1.0f;
		v_pos[6 * i + 3].y = y_bottom_pos;
		v_texcoord[6 * i + 3].x = 1.0f;
		v_texcoord[6 * i + 3].y = y_bottom_coord;
		v_data_for_gpu[6 * i + 3].pos.x = 1.0f;
		v_data_for_gpu[6 * i + 3].pos.y = y_bottom_pos;
		v_data_for_gpu[6 * i + 3].tex_coord.x = 1.0f;
		v_data_for_gpu[6 * i + 3].tex_coord.y = y_bottom_coord;

		v_pos[6 * i + 4].x = 1.0f;
		v_pos[6 * i + 4].y = y_top_pos;
		v_texcoord[6 * i + 4].x = 1.0f;
		v_texcoord[6 * i + 4].y = y_top_coord;
		v_data_for_gpu[6 * i + 4].pos.x = 1.0f;
		v_data_for_gpu[6 * i + 4].pos.y = y_top_pos;
		v_data_for_gpu[6 * i + 4].tex_coord.x = 1.0f;
		v_data_for_gpu[6 * i + 4].tex_coord.y = y_top_coord;

		v_pos[6 * i + 5].x = -1.0f;
		v_pos[6 * i + 5].y = y_bottom_pos;
		v_texcoord[6 * i + 5].x = 0.0f;
		v_texcoord[6 * i + 5].y = y_bottom_coord;
		v_data_for_gpu[6 * i + 5].pos.x = -1.0f;
		v_data_for_gpu[6 * i + 5].pos.y = y_bottom_pos;
		v_data_for_gpu[6 * i + 5].tex_coord.x = 0.0f;
		v_data_for_gpu[6 * i + 5].tex_coord.y = y_bottom_coord;
	}

	for (int i = 0; i < v_data_for_gpu.size(); i++) {
		v_data_for_gpu[i].pos *= v_pos_scaler;
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