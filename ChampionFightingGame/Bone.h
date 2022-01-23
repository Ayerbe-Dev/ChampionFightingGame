#pragma once
#include "utils.h"
using namespace glm;

struct Bone {
	string name = "";
	int id = 0;
	int parent_id = -1;
	mat4 *parent_matrix;
	vec3 pos = vec3(0.0, 0.0, 0.0);
	vec3 rot = vec3(0.0, 0.0, 0.0);
	vec3 scale = vec3(0.0, 0.0, 0.0);
	mat4 model_matrix = mat4(1.0);
	mat4 anim_matrix = mat4(1.0);
	mat4 anim_rest_matrix = mat4(1.0);
	mat4 transform_matrix = mat4(1.0);
	mat4 final_matrix = mat4(1.0);
	bool keyframed = false;
};

void offset_base_bone(Bone* base, Bone* offset);