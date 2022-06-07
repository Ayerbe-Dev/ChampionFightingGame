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

unsigned int Font::create_text(std::string text, glm::vec4 rgba, int* width_ret, int* height_ret, unsigned int* existing_texture) {
	//Calculate the size the texture will need to be

	float width = 0;
	float height = 0;

	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		width += (tex_char.advance >> 6);
		if (height < tex_char.size.y) {
			height = tex_char.size.y;
		}
	}

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Set our RenderBuffer to the same size as our texture, which we attach to our FrameBuffer

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Render each character in the correct position of our framebuffer, which will also influence
	//the texture

	rgba /= 255.0;

	shader->use();
	shader->set_vec4("f_colormod", rgba);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE1);

	float x = -char_map[text[0]].bearing.x;
	for (char c = 0, max = text.size(); c < max; c++) {
		TexChar tex_char = char_map[text[c]];
		float x_pos = (x + tex_char.bearing.x) / width;
		float y_pos = (tex_char.bearing.y) / height;
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
		x += (tex_char.advance >> 6);
	}
	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	*width_ret = width;
	*height_ret = height;
	return texture;
}