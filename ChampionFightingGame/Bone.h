#pragma once
#include <iostream>
#include <glm/glm.hpp>

struct Bone {
	std::string name = "";
	int id = 0;
	int parent_id = -1;
	glm::mat4* parent_matrix;
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
	glm::quat rot = glm::quat(0.0, 0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::mat4 anim_matrix = glm::mat4(1.0);
	glm::mat4 anim_rest_matrix = glm::mat4(1.0);
	glm::mat4 final_matrix = glm::mat4(1.0);
	bool keyframed = false;
};