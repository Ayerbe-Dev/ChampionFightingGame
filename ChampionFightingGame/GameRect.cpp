#include "GameRect.h"
#include <glew/glew.h>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/string_cast.hpp>
#include "ObjectManager.h"
#include "WindowManager.h"
#include "ShaderManager.h"
#include "ThreadManager.h"
#include "Fighter.h"
#include "WindowConstants.h"

GameRect::GameRect() {
	thread_manager = ThreadManager::get_instance();
	window_manager = WindowManager::get_instance();
	shader_manager = ShaderManager::get_instance();
}

GameRect::GameRect(glm::vec2 c1, glm::vec2 c2) {
	init(c1, c2);
}

void GameRect::init() {
	attach_shader(shader_manager->get_shader("rect", "rect", "", 0));
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameRect::init(glm::vec2 c1, glm::vec2 c2) {
	corners[0] = c1;
	corners[1] = glm::vec2(c1.x, c2.y);
	corners[2] = c2;
	corners[3] = glm::vec2(c2.x, c1.y);
	corners[4] = corners[0];
	corners[5] = corners[2];

	attach_shader(shader_manager->get_shader("rect", "rect", "", 0));
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameRect::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void GameRect::update_corners(glm::vec2 c1, glm::vec2 c2) {
	corners[0] = c1;
	corners[1] = glm::vec2(c1.x, c2.y);
	corners[2] = c2;
	corners[3] = glm::vec2(c2.x, c1.y);

	if (thread_manager->is_main_thread()) {
		update_buffer_data();
	}
}

void GameRect::attach_shader(Shader *shader) {
	this->shader = shader;
}

void GameRect::set_alpha(float alpha) {
	rgba.w = alpha;
}

void GameRect::set_rgb(glm::vec3 rgb) {
	rgba.x = rgb.x;
	rgba.y = rgb.y;
	rgba.z = rgb.z;
}

void GameRect::set_rgba(glm::vec4 rgba) {
	this->rgba = rgba;
}

void GameRect::render() {
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shader->set_vec4("f_rgba", rgba / glm::vec4(255.0));
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDepthMask(GL_TRUE);
}

void GameRect::update_buffer_data() {
	corners[4] = corners[0];
	corners[5] = corners[2];
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(corners), corners);
}