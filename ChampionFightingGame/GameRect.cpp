#include "GameRect.h"
#include "RenderManager.h"
extern RenderManager g_rendermanager;
using namespace glm;

GameRect::GameRect() {

}

GameRect::GameRect(vec2 c1, vec2 c2) {
	init(c1, c2);
}

void GameRect::init() {
	attach_shader(&g_rendermanager.default_rect_shader);
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameRect::init(vec2 c1, vec2 c2) {
	corners[0] = c1;
	corners[1] = vec2(c1.x, c2.y);
	corners[2] = c2;
	corners[3] = vec2(c2.x, c1.y);

	attach_shader(&g_rendermanager.default_rect_shader);
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
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

void GameRect::update_corners(vec2 c1, vec2 c2) {
	corners[0] = c1;
	corners[1] = vec2(c1.x, c2.y);
	corners[2] = c2;
	corners[3] = vec2(c2.x, c1.y);

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

void GameRect::attach_shader(Shader *shader) {
	this->shader = shader;
}

void GameRect::bind_scale(vec3 *scale) {
	this->scale = scale;
}

void GameRect::set_alpha(float alpha) {
	rgba.w = alpha / 255.0;
}

void GameRect::set_rgb(vec3 rgb) {
	rgba.x = rgb.x;
	rgba.y = rgb.y;
	rgba.z = rgb.z;
}

void GameRect::set_rgba(vec4 rgba) {
	rgba.w /= 255.0;
	this->rgba = rgba;
}

void GameRect::render() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shader->use();
	g_rendermanager.update_shader_cam(shader);

	mat4 mat = mat4(1.0);
	if (scale != nullptr) {
		mat = glm::scale(mat, *scale);
	}
	mat = translate(mat, g_rendermanager.stage_origin);
	shader->set_mat4("matrix", mat);
	shader->set_vec4("f_rgba", rgba);

	glDepthMask(GL_FALSE);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameRect::update_buffer_data() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(corners), corners);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// <summary>
/// SDL's rectangles suck so I made better ones.
/// </summary>
/// <param name="RectA">: The first rectangle</param>
/// <param name="RectB">: The second rectangle</param>
/// <returns>Whether or not any part of the two rectangles are touching</returns>
/// <notes>The reason it's so long is because we run this check a lot so the less we put on the stack, the better
bool is_collide(GameRect RectA, GameRect RectB) {
	return 
		((RectA.corners[0].x >= RectB.corners[0].x && RectA.corners[0].x <= RectB.corners[2].x) || (RectA.corners[2].x >= RectB.corners[0].x 
		&& RectA.corners[2].x <= RectB.corners[2].x) || (RectB.corners[0].x >= RectA.corners[0].x && RectB.corners[0].x <= RectA.corners[2].x) 
		|| (RectB.corners[2].x >= RectA.corners[0].x && RectB.corners[2].x <= RectA.corners[2].x) || (RectA.corners[0].x <= RectB.corners[0].x 
		&& RectA.corners[0].x >= RectB.corners[2].x) || (RectA.corners[2].x <= RectB.corners[0].x && RectA.corners[2].x >= RectB.corners[2].x) 
		|| (RectB.corners[0].x <= RectA.corners[0].x && RectB.corners[0].x >= RectA.corners[2].x) || (RectB.corners[2].x <= RectA.corners[0].x 
		&& RectB.corners[2].x >= RectA.corners[2].x)) 
		&& 
		((RectA.corners[0].y <= RectB.corners[0].y && RectA.corners[0].y >= RectB.corners[2].y) || (RectA.corners[2].y <= RectB.corners[0].y 
		&& RectA.corners[2].y >= RectB.corners[2].y) || (RectB.corners[0].y <= RectA.corners[0].y && RectB.corners[0].y >= RectA.corners[2].y)
		|| (RectB.corners[2].y <= RectA.corners[0].y && RectB.corners[2].y >= RectA.corners[2].y) || (RectA.corners[0].y >= RectB.corners[0].y 
		&& RectA.corners[0].y <= RectB.corners[2].y) || (RectA.corners[2].y >= RectB.corners[0].y && RectA.corners[2].y <= RectB.corners[2].y) 
		|| (RectB.corners[0].y >= RectA.corners[0].y && RectB.corners[0].y <= RectA.corners[2].y) || (RectB.corners[2].y >= RectA.corners[0].y 
		&& RectB.corners[2].y <= RectA.corners[2].y));
}