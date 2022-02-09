#pragma once
#include <vector>
#include "glm.hpp"
#include "Model.h"
#include "Animation.h"

using namespace glm;

class RenderObject {
public:
	vec3 pos;
	vec3 rot;
	vec3 scale;

	Model* model;
	vector<Animation3D> animation_table;

	void render();
};