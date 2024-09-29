#include "ScreenText.h"
#include "WindowConstants.h"
#include "Shader.h"
#include "ResourceManager.h"

ScreenText::ScreenText() {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	VAO = 0;
	VBO = 0;
#endif
	texture = 0;
	shader = nullptr;
	text = "";
	font = nullptr;
	screen_orientation = TEXTURE_MID;
	texture_orientation = TEXTURE_MID;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	base_width = 0;
	base_height = 0;
	width_scale = 0.0f;
	height_scale = 0.0f;
}

ScreenText& ScreenText::init(Font* font, std::string text, TextSpecifier spec) {
	this->texture = font->create_text(text, spec);
	return *this;
}

void ScreenText::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

ScreenText& ScreenText::update_text(std::string text) {
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

ScreenText& ScreenText::set_orientation(int screen_orientation, int texture_orientation) {
	this->screen_orientation = screen_orientation;
	this->texture_orientation = texture_orientation;
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
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_base_width(int width) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	base_width += width;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_base_height(int height) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	base_height += height;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_width_scale(float scale) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	width_scale += scale;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
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
	for (int i = 0; i < 6; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenText& ScreenText::add_height_scale(float scale) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	height_scale += scale;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < 6; i++) {
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
		render_pos *= -1.0;
		render_pos.x += WINDOW_WIDTH;
	}

	if (screen_orientation & TEXTURE_TOP) {
		render_pos *= -1.0;
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
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_CULL_FACE);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenText::update_buffer_data() {
	bool update = false;
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / (float)WINDOW_WIDTH,
		get_height() / (float)WINDOW_HEIGHT
	);
	for (int i = 0; i < 6; i++) {
		if (v_data_for_gpu[i].pos != (glm::vec2)v_pos[i] * v_pos_scaler
		|| v_data_for_gpu[i].tex_coord != v_texcoord[i]) {
			for (int i = 0; i < 6; i++) {
				v_data_for_gpu[i] = { (glm::vec2)v_pos[i] * v_pos_scaler, v_texcoord[i] };
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
			return;
		}
	}
}