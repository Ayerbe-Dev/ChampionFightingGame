#pragma once
#include "utils.h"
using namespace glm;

struct Bone {
	string name = "";
	int id = 0;
	int parent_id = -1;
	vec3 pos = vec3(0.0, 0.0, 0.0);
	vec3 rot = vec3(0.0, 0.0, 0.0);
	vec3 scale = vec3(0.0, 0.0, 0.0);
	mat4 base_matrix;
};

void offset_base_bone(Bone* base, Bone* offset);