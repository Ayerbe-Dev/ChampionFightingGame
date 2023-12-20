#include "GameRect.h"
#include <glew/glew.h>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/string_cast.hpp>
#include "ObjectManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "ThreadManager.h"
#include "Fighter.h"
#include "WindowConstants.h"

GameRect::GameRect() {
	thread_manager = ThreadManager::get_instance();
	render_manager = RenderManager::get_instance();
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

	attach_shader(shader_manager->get_shader("rect", "rect", "", 0));
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	for (int i = 0; i < 4; i++) {
		corners[i].x /= WINDOW_WIDTH / 100;
		corners[i].y /= WINDOW_HEIGHT / 100;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);
	for (int i = 0; i < 4; i++) {
		corners[i].x *= WINDOW_WIDTH / 100;
		corners[i].y *= WINDOW_HEIGHT / 100;
	}

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
		for (int i = 0; i < 4; i++) {
			corners[i].x /= WINDOW_WIDTH / 100;
			corners[i].y /= WINDOW_HEIGHT / 100;
		}
		update_buffer_data();
		for (int i = 0; i < 4; i++) {
			corners[i].x *= WINDOW_WIDTH / 100;
			corners[i].y *= WINDOW_HEIGHT / 100;
		}
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
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(GL_TRUE);
}

void GameRect::update_buffer_data() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(corners), corners);
}

/// <summary>
/// SDL's rectangles suck so I made better ones.
/// </summary>
/// <param name="RectA">: The first rectangle</param>
/// <param name="RectB">: The second rectangle</param>
/// <returns>Whether or not any part of the two rectangles are touching</returns>
/// <notes>The reason it's so long is because we run this check a lot so the less we put on the stack, the better
bool is_collide(GameRect &RectA, GameRect &RectB) {
	return 
		((RectA.corners[0].x >= RectB.corners[0].x && RectA.corners[0].x <= RectB.corners[2].x) 
		|| (RectA.corners[2].x >= RectB.corners[0].x && RectA.corners[2].x <= RectB.corners[2].x) 
		|| (RectB.corners[0].x >= RectA.corners[0].x && RectB.corners[0].x <= RectA.corners[2].x) 
		|| (RectB.corners[2].x >= RectA.corners[0].x && RectB.corners[2].x <= RectA.corners[2].x) 
		|| (RectA.corners[0].x <= RectB.corners[0].x && RectA.corners[0].x >= RectB.corners[2].x) 
		|| (RectA.corners[2].x <= RectB.corners[0].x && RectA.corners[2].x >= RectB.corners[2].x) 
		|| (RectB.corners[0].x <= RectA.corners[0].x && RectB.corners[0].x >= RectA.corners[2].x) 
		|| (RectB.corners[2].x <= RectA.corners[0].x && RectB.corners[2].x >= RectA.corners[2].x)) 
		&& 
		((RectA.corners[0].y <= RectB.corners[0].y && RectA.corners[0].y >= RectB.corners[2].y) 
		|| (RectA.corners[2].y <= RectB.corners[0].y && RectA.corners[2].y >= RectB.corners[2].y) 
		|| (RectB.corners[0].y <= RectA.corners[0].y && RectB.corners[0].y >= RectA.corners[2].y)
		|| (RectB.corners[2].y <= RectA.corners[0].y && RectB.corners[2].y >= RectA.corners[2].y) 
		|| (RectA.corners[0].y >= RectB.corners[0].y && RectA.corners[0].y <= RectB.corners[2].y) 
		|| (RectA.corners[2].y >= RectB.corners[0].y && RectA.corners[2].y <= RectB.corners[2].y) 
		|| (RectB.corners[0].y >= RectA.corners[0].y && RectB.corners[0].y <= RectA.corners[2].y) 
		|| (RectB.corners[2].y >= RectA.corners[0].y && RectB.corners[2].y <= RectA.corners[2].y));
}


glm::vec2 mouse_pos_to_rect_coord(glm::vec2 mouse_pos) {
	RenderManager* render_manager = RenderManager::get_instance();
	Camera& camera = render_manager->camera;

	mouse_pos.y = render_manager->res_height - mouse_pos.y;
	glm::vec3 screen_pos = glm::vec3(mouse_pos.x, mouse_pos.y, 0.0f);
	glReadPixels(mouse_pos.x, mouse_pos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &screen_pos.z);

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, render_manager->res_width, render_manager->res_height);
	glm::mat4 modelview = camera.view_matrix;
	glm::mat4 projection = camera.projection_matrix;

	glm::vec3 world_pos = glm::unProject(screen_pos, modelview, projection, viewport);

	float l = -world_pos.z / (world_pos.z - camera.pos.z);

	world_pos.x = (world_pos.x + l * (world_pos.x - camera.pos.x)) * (WINDOW_WIDTH / 100);
	world_pos.y = (world_pos.y + l * (world_pos.y - camera.pos.y)) * (WINDOW_HEIGHT / 100);

	return world_pos;
}
