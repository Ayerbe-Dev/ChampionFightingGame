#include "Particle.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"
#include <fstream>
#include "GLEW Helpers.h"

Particle::Particle() {

}

Particle::Particle(std::string path, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba) {
	init(path);
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->rgba = rgba;
}

void Particle::init(std::string path) {
	tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 4; i++) {
		tex_accessor[i] = &tex_data[i];
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	int width;
	int height;
	texture = loadGLTexture(path.c_str(), &width, &height);
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	this->width = width;
	this->height = height;
	for (int i = 0; i < 4; i++) {
		tex_data[i].pos.x *= width_scale;
		tex_data[i].pos.y *= height_scale;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_STATIC_DRAW);
}

void Particle::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
}

void Particle::load_spritesheet(std::string spritesheet_dir) {
	std::ifstream spritesheet_file;
	spritesheet_file.open(spritesheet_dir);
	if (spritesheet_file.fail()) {
		std::cout << "Failed to open spritesheet at " << spritesheet_dir << "!\n";
		spritesheet_file.close();
		return;
	}
	int frame;
	glm::vec2 corners[4];
	while (spritesheet_file >> frame) {
		spritesheet_file >> corners[TEX_COORD_BOTTOM_LEFT].x >> corners[TEX_COORD_BOTTOM_LEFT].y;
		spritesheet_file >> corners[TEX_COORD_BOTTOM_RIGHT].x >> corners[TEX_COORD_BOTTOM_RIGHT].y;
		spritesheet_file >> corners[TEX_COORD_TOP_RIGHT].x >> corners[TEX_COORD_TOP_RIGHT].y;
		spritesheet_file >> corners[TEX_COORD_TOP_LEFT].x >> corners[TEX_COORD_TOP_LEFT].y;
		for (int i = 0; i < 4; i++) {
			corners[i].x /= width;
			corners[i].y /= height;
			spritesheet[i].push_back(corners[i]);
		}
	}
	spritesheet_file.close();
	width = spritesheet[TEX_COORD_TOP_RIGHT][0].x;
	height = spritesheet[TEX_COORD_TOP_RIGHT][0].y;
	set_sprite(0);
}

void Particle::set_sprite(int index) {
	if (spritesheet[0].size() <= index) return;
	for (int i = 0; i < 4; i++) {
		tex_accessor[i]->tex_coord = spritesheet[i][index];
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_data), tex_data);
}

void Particle::render(Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, int frame) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	pos += this->pos;
	rot += this->rot;
	scale *= this->scale;
	rgba += this->rgba;
//	std::cout << pos.x << ", " << pos.y << ", " << pos.z << "\n";
//	std::cout << rot.x << ", " << rot.y << ", " << rot.z << "\n";
//	std::cout << scale.x << ", " << scale.y << ", " << scale.z << "\n";
//	std::cout << rgba.x << ", " << rgba.y << ", " << rgba.z << ", " << rgba.w << "\n";
	set_sprite(frame);

	rgba.x /= 255.0;
	rgba.y /= 255.0;
	rgba.z /= 255.0;
	rgba.w = (1.0 - (rgba.w / 255.0));

	glm::mat4 matrix = glm::mat4(1.0);
	matrix = glm::scale(matrix, scale);
	matrix = glm::translate(matrix, pos);
	matrix = glm::rotate(matrix, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	shader->set_mat4("matrix", matrix);
	shader->set_vec4("f_colormod", rgba);
	glDrawArrays(GL_QUADS, 0, 4);
}