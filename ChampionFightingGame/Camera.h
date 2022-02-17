#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "utils.h"
using namespace glm;

#define DEFAULT_CAM_SPEED 0.5
#define DEFAULT_CAM_SENS 2.5

class Camera {
public:
	vec3 pos = vec3(0.0);
	vec3 front = vec3(0.0, 0.0, 1.0);
	vec3 world_up = vec3(0.0, 1.0, 0.0);
	vec3 up;
	vec3 right;

	float yaw = -90.0;
	float pitch = 0.0;
	const float cam_speed = 0.5;
	const float cam_sens = 2.5;
	float fov = 45.0;

	Camera();
	mat4 get_view();
	void add_pos(float x, float y, float z, float speed = 0.0);
	void adjust_view(float x, float y, float z, float speed = 0.0);

	void update_view();
};