#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "TextureCoord.h"

class Shader;

class Particle {
public:
	Particle();
	Particle(std::string path, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba);

	void init(std::string path);
	void destroy();

	void load_spritesheet(std::string spritesheet_dir);	
	
	void render(Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, int frame = 0);

	TextureCoord tex_data[4];
	TextureCoord* tex_accessor[4];

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;
	std::vector<glm::vec2> spritesheet[4];
private:
	void set_sprite(int section);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int texture;
	float width;
	float height;
};