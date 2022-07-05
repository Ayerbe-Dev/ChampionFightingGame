#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Bone {
	std::string name = "";
	int id = 0;
	int counterpart_id = 0;
	int parent_id = -1;
	glm::mat4* parent_matrix;
	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::mat4 model_flip_matrix = glm::mat4(1.0);
	glm::mat4 anim_matrix = glm::mat4(1.0);
	glm::mat4 final_matrix = glm::mat4(1.0);

	glm::mat4 model_pos_matrix;
	glm::mat4 model_pos_flip_matrix;
	glm::mat4 final_pos_matrix;

	bool keyframed = false;
};

//idk, this is probably slightly more optimized

struct AnimBone {
	std::string name = "";
	int id = 0;
	int parent_id = -1;
	glm::mat4 anim_matrix = glm::mat4(1.0);
	bool keyframed = false;
};