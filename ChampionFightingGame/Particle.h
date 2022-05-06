#pragma once
#include <glm/glm.hpp>
#include <iostream>
class Shader;

class Particle {
public:
	Particle();

	void init(std::string path);
	void destroy();

	void render(Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, int frame = -1);

	bool spritesheet;
	glm::vec4 rgba;
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int texture;
};