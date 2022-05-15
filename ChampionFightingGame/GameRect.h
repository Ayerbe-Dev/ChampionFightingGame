#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class ThreadManager;
class RenderManager;

class GameRect {
public:
	GameRect();
	GameRect(glm::vec2 c1, glm::vec2 c2);
	void init();
	void init(glm::vec2 c1, glm::vec2 c2);
	void destroy();
	void update_corners(glm::vec2 c1, glm::vec2 c2);
	void attach_shader(Shader* shader);
	void bind_scale(glm::vec3* scale);
	void set_alpha(float alpha);
	void set_rgb(glm::vec3 rgb);
	void set_rgba(glm::vec4 rgba);

	void prepare_render();

	void render();
	void render_prepared();

	Shader* shader;

	glm::vec2 corners[4] = { glm::vec2(0.0) };

	glm::vec3 *scale = nullptr;
	glm::vec4 rgba = glm::vec4(0.0);
private:
	void update_buffer_data();
	RenderManager* render_manager;
	ThreadManager* thread_manager;
	glm::mat4 matrix;

	unsigned int VAO;
	unsigned int VBO;
};

bool is_collide(GameRect &RectA, GameRect &RectB);