#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "CameraAnim.h"

#define DEFAULT_CAM_SPEED 0.5
#define DEFAULT_CAM_SENS 2.5

class Fighter;
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
	float roll = 0.0;
	const float cam_speed = 0.5;
	const float cam_sens = 2.5;
	float fov;
	float max_fov = 45.0;
	glm::vec3 base_pos;
	glm::mat4 flip_matrix;

	//imagine having self documenting names, couldnt be me. This determines the linear scale on which the camera rotates in auto mode
	float auto_linear_scale;

	Fighter* fighter[2];

	Stage* stage;
	int follow_id;

	bool following_players;

	CameraAnim* anim_kind;
	float frame;
	float rate;

	std::vector<CameraAnim> camera_anims;
	std::map<std::string, int> camera_anim_map;

	glm::mat4 camera_matrix;

	Camera();

	void camera_main();

	void load_camera_anim(std::string anim_kind, std::string anim_dir);
	void play_camera_anim(int follow_id, std::string anim_kind, float rate);
	void unload_camera_anims();

	void add_pos(float x, float y, float z, float speed = 0.0);
	void adjust_view(float x, float y, float z, float speed = 0.0);

	void update_view();
	void follow_players();
	void follow_anim();
};