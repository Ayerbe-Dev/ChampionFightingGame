#include "Font.h"
#include <vector>
#include <glew/glew.h>
#include "RenderManager.h"
#include "utils.h"

Font::Font() {
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader = &RenderManager::get_instance()->text_shader;
	shader->use();
	shader->set_int("f_texture", 1);

	float coords[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords, GL_DYNAMIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	prev_width = 0.0;
	prev_height = 0.0;
}

void Font::unload_font() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	for (auto& it : char_map) {
		glDeleteTextures(1, &it.second.texture);
	}
}

unsigned int Font::create_text(std::string text, glm::vec4 rgba, float border_x, float border_y, unsigned int* existing_texture) {
	RenderManager* render_manager = RenderManager::get_instance();
	//Calculate the size the texture will need to be

	float base_width = 0;
	float base_height = 0;
	float x_offset = 0;
	float y_offset = 0;

	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		base_width += (tex_char.advance >> 6);
		if (base_height < tex_char.size.y) {
			base_height = tex_char.size.y;
		}
		if (x_offset < tex_char.size.x - tex_char.bearing.x) {
			x_offset = tex_char.size.x - tex_char.bearing.x;
		}
		if (y_offset < tex_char.size.y - tex_char.bearing.y) {
			y_offset = tex_char.size.y - tex_char.bearing.y;
		}
	}

	float width = base_width + abs(border_x);
	float height = base_height + abs(border_y);

	width += x_offset;
	height += y_offset;

	base_width /= 2;
	base_height /= 2;

	width /= 2;
	height /= 2;

	//Create a texture using our calculated size

	unsigned int texture;
	if (existing_texture == nullptr) {
		glGenTextures(1, &texture);
	}
	else {
		texture = *existing_texture;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Set our RenderBuffer to the same size as our texture, which we attach to our FrameBuffer

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Render each character in the correct position of our framebuffer, which will also influence
	//the texture

	rgba /= 255.0;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE1);
	glDepthMask(GL_FALSE);

	shader->use();
	glViewport(0, 0, prev_width, prev_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, width, height);

	if (border_x != 0.0 || border_y != 0.0) {
		shader->set_vec4("f_colormod", glm::vec4(0.0, 0.0, 0.0, rgba.a));
		write_to_fbo(text, border_x, border_y + y_offset, width, height, base_width, base_height);
	}

	shader->set_vec4("f_colormod", rgba);
	write_to_fbo(text, 0, y_offset, width, height, base_width, base_height);

	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	prev_width = width;
	prev_height = height;

	glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);

	return texture;
}

void Font::write_to_fbo(std::string text, float x_offset, float y_offset, float width, float height, float base_width, float base_height) {
	x_offset -= char_map[text[0]].bearing.x;
	x_offset -= base_width;
	y_offset -= base_height;
	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		float x_pos = (x_offset + tex_char.bearing.x) / width;
		float y_pos = (y_offset + tex_char.bearing.y) / height;
		float w = tex_char.size.x / width;
		float h = -tex_char.size.y / height;

		float coords[] = {
			x_pos, y_pos + h,		0.0f, 1.0f,
			x_pos, y_pos,			0.0f, 0.0f,
			x_pos + w, y_pos,		1.0f, 0.0f,

			x_pos, y_pos + h,		0.0f, 1.0f,
			x_pos + w, y_pos,		1.0f, 0.0f,
			x_pos + w, y_pos + h,	1.0f, 1.0f,
		};

		glBindTexture(GL_TEXTURE_2D, tex_char.texture);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x_offset += (tex_char.advance >> 6);
	}
}