#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_CAM_SPEED 0.5
#define DEFAULT_CAM_SENS 2.5

class Stage;

class Camera {
public:
	glm::vec3 pos = glm::vec3(0.0);
	glm::vec3 front = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 world_up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 prev_pos;

	float yaw = -90.0;
	float pitch = 0.0;
	const float cam_speed = 0.5;
	const float cam_sens = 2.5;
	float fov;
	float max_fov = 45.0;
	glm::vec3 base_pos;

	//imagine having self documenting names, couldnt be me. This determines the linear scale on which the camera rotates in auto mode
	float auto_linear_scale;

	bool following_players;

	Camera();
	glm::mat4 get_view();
	void add_pos(float x, float y, float z, float speed = 0.0);
	void adjust_view(float x, float y, float z, float speed = 0.0);

	void update_view();
	void follow_players(glm::vec2 p1, glm::vec2 p2, Stage* stage);
};