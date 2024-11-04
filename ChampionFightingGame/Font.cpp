#include "Font.h"
#include <vector>
#include <glew/glew.h>
#include "WindowManager.h"
#include "ShaderManager.h"
#include "utils.h"
#include "debug.h"

Font::Font() {
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ShaderManager* shader_manager = ShaderManager::get_instance();

	shader = shader_manager->get_shader("text", "text", "", 0);
	shader->use();
	shader->set_int("f_texture", 1);
	shader->set_active_uniform_location("matrix");
	border_shader = shader_manager->get_shader("2d_outline", "2d_outline", "", 0);
	border_shader->set_int("f_texture", 1);
	border_shader->set_active_uniform_location("border_offset");

	{
		float coords[6][4] = {
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			1.0f,	0.0f,	1.0f,	1.0f,

			0.0f,	1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	1.0f,	1.0f,
			1.0f,	1.0f,	1.0f,	0.0f
		};
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords, GL_STATIC_DRAW);
	}

	{
		float coords[6][4] = {
			-1.0f,	1.0f,	0.0f,	1.0f,
			-1.0f,	-1.0f,	0.0f,	0.0f,
			1.0f,	-1.0f,	1.0f,	0.0f,

			-1.0f,	1.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,	1.0f,	0.0f,
			1.0f,	1.0f,	1.0f,	1.0f
		};
		glGenVertexArrays(1, &VAO_outline);
		glBindVertexArray(VAO_outline);
		glGenBuffers(1, &VBO_outline);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_outline);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	prev_width = 0.0f;
	prev_height = 0.0f;
	base_height = 0.0f;
	base_y_offset = 0.0f;
}

void Font::unload_font() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_outline);
	glDeleteVertexArrays(1, &VAO_outline);
	for (auto& it : char_map) {
		glDeleteTextures(1, &it.second.texture);
	}
}


unsigned int Font::create_text(std::string text, glm::vec4 rgba, glm::vec4 border_rgbs, unsigned int* existing_texture) {
	WindowManager* window_manager = WindowManager::get_instance();
	//Calculate the size the texture will need to be

	float width = 0.0f;
	
	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		width += (tex_char.advance >> 6);
	}

	float x_offset = border_rgbs.a;
	float y_offset = base_y_offset + border_rgbs.a;
	width = width / 2.0f + border_rgbs.a;
	float height = base_height / 2.0f + border_rgbs.a;

	//Create a texture using our calculated size

	unsigned int texture;
	if (existing_texture == nullptr) {
		glGenTextures(1, &texture);
	}
	else {
		texture = *existing_texture;
	}

#ifdef DEBUG
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
#else
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
#endif

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	//Set our RenderBuffer to the same size as our texture, which we attach to our FrameBuffer

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Render each character in the correct position of our framebuffer, which will also influence
	//the texture

	rgba /= 255.0;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE1);
	GLboolean depth = false;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &depth);
	glDepthMask(GL_FALSE);

	glViewport(0, 0, prev_width, prev_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	shader->use();
	shader->set_vec4("f_colormod", rgba);

	x_offset -= char_map[text[0]].bearing.x;
	x_offset -= width;
	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		float x_pos = (x_offset + tex_char.bearing.x) / width;
		float y_pos = ((y_offset - (tex_char.size.y - tex_char.bearing.y)) / height) - 1.0f;
		float w = tex_char.size.x / width;
		float h = tex_char.size.y / height;

		glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(w, h, 1.0f));
		glm::mat4 pos_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x_pos, y_pos, 0.0f));
		shader->set_active_mat4(pos_mat * scale_mat);
		glBindTexture(GL_TEXTURE_2D, tex_char.texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x_offset += (tex_char.advance >> 6);
	}

	if (border_rgbs.a) {
		glBindVertexArray(VAO_outline);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_outline);
		glm::vec2 border_size = border_rgbs.a / glm::vec2(width, height);
		border_shader->use();
		border_shader->set_active_vec2(border_size);
		border_shader->set_vec4("border_color", border_rgbs.r, border_rgbs.g, border_rgbs.b, rgba.a);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDepthMask(depth);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	prev_width = width;
	prev_height = height;

	glViewport(0, 0, window_manager->window_width, window_manager->window_height);

	return texture;
}

unsigned int Font::create_text(std::string text, TextSpecifier spec, unsigned int* num_lines, unsigned int* existing_texture) {
	WindowManager* window_manager = WindowManager::get_instance();
	//Calculate the width of our texture by determining the width of each line, adding a new one
	//every time either:
	//- The width of the current line exceeds a user-specified limit
	//- We encounter a '\n' character
	std::vector<float> width_lines = { 0.0f };
	std::vector<size_t> newline_indices = { 0 };
	int longest_line = 0;
	for (size_t i = 0, max = text.size(); i < max; i++) {
		if (!char_map.contains(text[i])) continue;
		TexChar	tex_char = char_map[text[i]];
		unsigned int char_width = (tex_char.advance >> 6);
		if (width_lines.back() + char_width > spec.max_line_length || text[i] == '\n') {
			if (width_lines.back() > width_lines[longest_line]) {
				longest_line = width_lines.size() - 1;
			}
			width_lines.push_back(0.0f);
			newline_indices.push_back(i+1);
			
		}
		if (text[i] != '\n') {
			width_lines.back() += char_width;
		}
	}
	*num_lines = width_lines.size();

	float width = width_lines[longest_line] + spec.border_rgbs.a * 2.0f;
	float height = (base_height + spec.border_rgbs.a * 2.0f) * *num_lines; //Border size

	//Texture setup

	unsigned int texture;
	if (existing_texture == nullptr) {
		glGenTextures(1, &texture);
	}
	else {
		texture = *existing_texture;
	}
#ifdef DEBUG
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
#else
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
#endif
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	//Renderbuffer setup

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Render Time

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE1);
	GLboolean depth = false;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &depth);
	glDepthMask(GL_FALSE);

	glViewport(0, 0, prev_width, prev_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width * 2, height * 2);

	std::vector<float> x_offsets;
	std::vector<float> y_offsets;
	for (size_t i = 0; i < *num_lines; i++) {
		if (spec.enable_center) {
			x_offsets.push_back((width - width_lines[i]) / 2.0f + spec.border_rgbs.a - width);
		}
		else {
			x_offsets.push_back(spec.border_rgbs.a - width);
		}
		y_offsets.push_back(
			(base_height + spec.border_rgbs.a * 2.0f) * (*num_lines - i - 1)
			+ base_y_offset
			+ spec.border_rgbs.a 
			- height
		);
	}

	shader->use();
	shader->set_vec4("f_colormod", spec.rgba / 255.0f);
	for (size_t i = 0, line_idx = -1; i < text.size(); i++) {
		if (text[i] == '\n') continue;
		if (newline_indices[line_idx + 1] == i) {
			x_offsets[++line_idx] -= char_map[text[i]].bearing.x;
		}
		TexChar tex_char = char_map[text[i]];

		float x_pos = (x_offsets[line_idx] + tex_char.bearing.x) / width;
		float y_pos = (y_offsets[line_idx] - (tex_char.size.y - tex_char.bearing.y)) / height;
		float w = tex_char.size.x / width;
		float h = tex_char.size.y / height;

		glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(w, h, 1.0f));
		glm::mat4 pos_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x_pos, y_pos, 0.0f));
		shader->set_active_mat4(pos_mat * scale_mat);

		glBindTexture(GL_TEXTURE_2D, tex_char.texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x_offsets[line_idx] += (tex_char.advance >> 6);
	}

	if (spec.border_rgbs.a != 0.0f) {
		glBindVertexArray(VAO_outline);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_outline);
		glViewport(0, 0, width, height);
		glm::vec2 border_size = spec.border_rgbs.a / glm::vec2(width * 2, height * 2);
		border_shader->use();
		border_shader->set_active_vec2(border_size);
		border_shader->set_vec4("border_color", spec.border_rgbs.r / 255.0f, spec.border_rgbs.g / 255.0f, spec.border_rgbs.b / 255.0f, spec.rgba.a / 255.0f);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//Post Render

	glDepthMask(depth);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	prev_width = width * 2;
	prev_height = height * 2;

	glViewport(0, 0, window_manager->window_width, window_manager->window_height);

	return texture;
}