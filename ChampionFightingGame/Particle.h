#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "SceneElementBase.h"

class Shader;

class OldParticle {
public:
	OldParticle();
	OldParticle(std::string path, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, 
		glm::vec3 pos_frame, glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame);

	void init(std::string path);
	void destroy();
	unsigned int get_texture();

	void load_spritesheet(std::string spritesheet_dir);	

	void render(Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, bool flip, float frame = 0);

	GameTextureCoord tex_data[6];
	GameTextureCoord* tex_accessor[6];

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;
	glm::vec3 pos_frame;
	glm::vec3 rot_frame;
	glm::vec3 scale_frame;
	glm::vec4 rgba_frame;
	std::vector<glm::vec2> spritesheet[4];
private:
	void set_sprite(int section);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int texture;
	float width;
	float height;
};