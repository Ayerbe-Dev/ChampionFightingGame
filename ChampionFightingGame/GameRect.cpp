#include "GameRect.h"
#include <glew/glew.h>
#include <glm/ext/matrix_projection.hpp>
#include "BattleObjectManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "ThreadManager.h"
#include "Fighter.h"
#include "utils.h"

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
		corners[i].x /= WINDOW_WIDTH;
		corners[i].y /= WINDOW_HEIGHT;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);
	for (int i = 0; i < 4; i++) {
		corners[i].x *= WINDOW_WIDTH;
		corners[i].y *= WINDOW_HEIGHT;
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
			corners[i].x /= WINDOW_WIDTH;
			corners[i].y /= WINDOW_HEIGHT;
		}
		update_buffer_data();
		for (int i = 0; i < 4; i++) {
			corners[i].x *= WINDOW_WIDTH;
			corners[i].y *= WINDOW_HEIGHT;
		}
	}
}

void GameRect::attach_shader(Shader *shader) {
	this->shader = shader;
}

void GameRect::bind_scale(glm::vec3 *scale) {
	this->scale = scale;
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

void GameRect::prepare_render() {
	if (scale != nullptr) {
		matrix = glm::scale(glm::mat4(1.0), *scale);
	}
	else {
		matrix = glm::scale(glm::mat4(1.0), glm::vec3(0.05));
	}
	matrix = glm::scale(matrix, glm::vec3(100.0)); //Scaling up all GameRects by 100x makes them reasonably sized
}

void GameRect::render() {
	prepare_render();
	render_prepared();
}

void GameRect::render_prepared() {
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shader->set_mat4("matrix", matrix);
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

void calc_resulting_pos(glm::vec2 coord) {
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	Camera& camera = render_manager->camera;
	Fighter* fighter = battle_object_manager->fighter[0];

	coord.x /= WINDOW_WIDTH;
	coord.y /= WINDOW_HEIGHT;

	glm::mat4 scale = glm::scale(glm::mat4(1.0), fighter->scale);
	scale = glm::scale(scale, glm::vec3(100.0));
	glm::vec4 result = camera.camera_matrix * (scale * glm::vec4(coord, 0.0, 1.0));
	std::cout << result.x << ", " << result.y << ", " << result.z << ", " << result.w << "\n";

	glm::vec2 world_pos = (result / camera.camera_matrix) / scale;
	world_pos.x *= WINDOW_WIDTH;
	world_pos.y *= WINDOW_HEIGHT;
	std::cout << world_pos.x << ", " << world_pos.y << "\n\n";
}

glm::vec2 mouse_pos_to_rect_coord(glm::vec2 mouse_coords) {
	calc_resulting_pos(glm::vec2(0, 0));
	calc_resulting_pos(glm::vec2(-1280, -720));
	calc_resulting_pos(glm::vec2(-1280, 720));
	calc_resulting_pos(glm::vec2(1280, 720));
	calc_resulting_pos(glm::vec2(1280, -720));
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	//put the mouse in -1, 1 coords

	mouse_coords.x = ((mouse_coords.x / render_manager->s_window_width) - 0.5) * 2.0;
	mouse_coords.y = (((render_manager->s_window_height - mouse_coords.y) / render_manager->s_window_height) - 0.5) * 2.0;

	//unproject the coords based on the camera and fighter

	Camera& camera = render_manager->camera;
	Fighter* fighter = battle_object_manager->fighter[0];

	glm::mat4 unproject_mat = glm::inverse(camera.camera_matrix);
	glm::mat4 unscale_mat = glm::inverse(
		glm::scale(glm::scale(glm::mat4(1.0), glm::vec3(100.0)), glm::vec3(fighter->scale))
	);


	glm::vec4 unproject_vec(mouse_coords.x, mouse_coords.y, camera.pos.z, 1.0);
	glm::vec2 world_pos = unproject_mat * (unscale_mat * unproject_vec);

	//?????????????????????????????????????????????????????????????????????????????????????

	world_pos.x *= WINDOW_WIDTH;
	world_pos.y *= WINDOW_HEIGHT;


	return world_pos;
}