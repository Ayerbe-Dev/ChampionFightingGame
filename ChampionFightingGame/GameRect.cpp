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

float get_rect_intersection(GameRect& RectA, GameRect& RectB) {
	float AB = RectA.corners[0].y;
	float AT = RectA.corners[2].y;
	float BB = RectB.corners[0].y;
	float BT = RectB.corners[2].y;
	if (AB < AT) {
		if (BB < BT) {
			if (AT < BB || BT < AB) return -1.0f;
			
		}
		else {
			if (AT < BT || BB < AB) return -1.0f;
		}
	}
	else {
		if (BB < BT) {
			if (AB < BB || BT < AT) return -1.0f;
		}
		else {
			if (AB < BT || BB < AT) return -1.0f;
		}
	}
	float AL = RectA.corners[0].x;
	float AR = RectA.corners[2].x;
	float BL = RectB.corners[0].x;
	float BR = RectB.corners[2].x;

	if (AL < AR) {
		if (BL < BR) {
			if (AL < BL) {
				if (AR < BL) return -1.0f;
				if (AR < BR) return AR - BL;
				return std::min(AR - BL, BR - AL);
			}
			else {
				if (BR < AL) return -1.0f;
				if (BR < AR) return BR - AL;	
				return std::min(BR - AL, AR - BL);
			}
		}
		else {
			if (AL < BR) {
				if (AR < BR) return -1.0f;
				if (AR < BL) return AR - BR;
				return std::min(AR - BR, BL - AL);
			}
			else {
				if (BL < AL) return -1.0f;
				if (BL < AR) return BL - AL;
				return std::min(BL - AL, AR - BR);				
			}
		}
	}
	else {
		if (BL < BR) {
			if (AR < BL) {
				if (AL < BL) return -1.0f;
				if (AL < BR) return AL - BL;
				return std::min(AL - BL, BR - AR);
			}
			else {
				if (BR < AR) return -1.0f;
				if (BR < AL) return BR - AR;				
				return std::min(BR - AR, AL - BL);
			}
		}
		else {
			if (AR < BR) {
				if (AL < BR) return -1.0f;
				if (AL < BL) return AL - BR;				
				return std::min(AL - BR, BL - AR);
			}
			else {
				if (BL < AR) return -1.0f;
				if (BL < AL) return BL - AR;				
				return std::min(BL - AR, AL - BR);
			}
		}
	}
}

float get_rect_intersection(glm::vec2 c1a, glm::vec2 c2a, glm::vec2 c1b, glm::vec2 c2b) {
	float AB = c1a.y;
	float AT = c2a.y;
	float BB = c1b.y;
	float BT = c2b.y;
	if (AB < AT) {
		if (BB < BT) {
			if (AT < BB || BT < AB) return -1.0f;

		}
		else {
			if (AT < BT || BB < AB) return -1.0f;
		}
	}
	else {
		if (BB < BT) {
			if (AB < BB || BT < AT) return -1.0f;
		}
		else {
			if (AB < BT || BB < AT) return -1.0f;
		}
	}
	float AL = c1a.x;
	float AR = c2a.x;
	float BL = c1b.x;
	float BR = c2b.x;

	if (AL < AR) {
		if (BL < BR) {
			if (AL < BL) {
				if (AR < BL) return -1.0f;
				if (AR < BR) return AR - BL;
				return std::min(AR - BL, BR - AL);
			}
			else {
				if (BR < AL) return -1.0f;
				if (BR < AR) return BR - AL;
				return std::min(BR - AL, AR - BL);
			}
		}
		else {
			if (AL < BR) {
				if (AR < BR) return -1.0f;
				if (AR < BL) return AR - BR;
				return std::min(AR - BR, BL - AL);
			}
			else {
				if (BL < AL) return -1.0f;
				if (BL < AR) return BL - AL;
				return std::min(BL - AL, AR - BR);
			}
		}
	}
	else {
		if (BL < BR) {
			if (AR < BL) {
				if (AL < BL) return -1.0f;
				if (AL < BR) return AL - BL;
				return std::min(AL - BL, BR - AR);
			}
			else {
				if (BR < AR) return -1.0f;
				if (BR < AL) return BR - AR;
				return std::min(BR - AR, AL - BL);
			}
		}
		else {
			if (AR < BR) {
				if (AL < BR) return -1.0f;
				if (AL < BL) return AL - BR;
				return std::min(AL - BR, BL - AR);
			}
			else {
				if (BL < AR) return -1.0f;
				if (BL < AL) return BL - AR;
				return std::min(BL - AR, AL - BR);
			}
		}
	}
}

bool is_rect_collide(GameRect &RectA, GameRect &RectB) {
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

bool is_rect_collide(glm::vec2 c1a, glm::vec2 c2a, glm::vec2 c1b, glm::vec2 c2b) {
	return
		((c1a.x >= c1b.x && c1a.x <= c2b.x)
			|| (c2a.x >= c1b.x && c2a.x <= c2b.x)
			|| (c1b.x >= c1a.x && c1b.x <= c2a.x)
			|| (c2b.x >= c1a.x && c2b.x <= c2a.x)
			|| (c1a.x <= c1b.x && c1a.x >= c2b.x)
			|| (c2a.x <= c1b.x && c2a.x >= c2b.x)
			|| (c1b.x <= c1a.x && c1b.x >= c2a.x)
			|| (c2b.x <= c1a.x && c2b.x >= c2a.x))
		&&
		((c1a.y <= c1b.y && c1a.y >= c2b.y)
			|| (c2a.y <= c1b.y && c2a.y >= c2b.y)
			|| (c1b.y <= c1a.y && c1b.y >= c2a.y)
			|| (c2b.y <= c1a.y && c2b.y >= c2a.y)
			|| (c1a.y >= c1b.y && c1a.y <= c2b.y)
			|| (c2a.y >= c1b.y && c2a.y <= c2b.y)
			|| (c1b.y >= c1a.y && c1b.y <= c2a.y)
			|| (c2b.y >= c1a.y && c2b.y <= c2a.y));
}


glm::vec2 mouse_pos_to_rect_coord(glm::vec2 mouse_pos) {
	WindowManager* window_manager = WindowManager::get_instance();
	Camera& camera = window_manager->camera;

	mouse_pos.y = window_manager->res_height - mouse_pos.y;
	glm::vec3 screen_pos = glm::vec3(mouse_pos.x, mouse_pos.y, 0.0f);
	glReadPixels(mouse_pos.x, mouse_pos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &screen_pos.z);

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, window_manager->res_width, window_manager->res_height);
	glm::mat4 modelview = camera.view_matrix;
	glm::mat4 projection = camera.projection_matrix;

	glm::vec3 world_pos = glm::unProject(screen_pos, modelview, projection, viewport);

	float l = -world_pos.z / (world_pos.z - camera.pos.z);

	world_pos.x = (world_pos.x + l * (world_pos.x - camera.pos.x)) * (WINDOW_WIDTH / 100);
	world_pos.y = (world_pos.y + l * (world_pos.y - camera.pos.y)) * (WINDOW_HEIGHT / 100);

	return world_pos;
}
