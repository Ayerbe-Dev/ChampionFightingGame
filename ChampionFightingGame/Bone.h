#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <unordered_map>

struct Bone {
	std::string name = "";
	int id = 0;
	int counterpart_id = 0;
	int parent_id = -1;
	glm::mat4* parent_matrix;

	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::mat4 counterpart_model_matrix = glm::mat4(1.0);
	glm::mat4 anim_matrix = glm::mat4(1.0);
	glm::mat4 bind_matrix = glm::mat4(1.0);
	glm::mat4 final_matrix = glm::mat4(1.0);

	glm::vec3 base_pos = glm::vec3(0.0);
	glm::quat base_rot = glm::quat(1.0, 0.0, 0.0, 0.0);

	glm::vec3 pos = glm::vec3(0.0);
	glm::quat rot = glm::quat();
	glm::vec3 scale = glm::vec3(1.0);

	bool keyframed = false;
};

struct AnimBone {
	std::string name = "";
	int id = 0;
	int parent_id = -1;
	glm::mat4 anim_matrix = glm::mat4(1.0);
	glm::vec3 pos = glm::vec3(0.0);
	glm::quat rot = glm::quat(1.0, 0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(1.0);
	bool keyframed = false;
};

struct Skeleton {
	operator bool();
	bool load_skeleton(std::string path);
	int get_bone_id(std::string bone_name);

	std::vector<Bone> bone_data;
	std::unordered_map<std::string, int> bone_map;
};