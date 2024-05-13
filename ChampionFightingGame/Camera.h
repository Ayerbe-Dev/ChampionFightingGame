#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "CameraAnim.h"
#include "TargetVar.h"

class GameObject;
class BattleObject;
class Fighter;
class Stage;

class Camera {
public:
	Camera();

	void camera_main();

	void load_camera_anim(std::string anim_kind, std::string anim_dir);
	void play_camera_anim(std::string anim_kind, float rate, float frame);
	void play_camera_anim(CameraAnim* anim_kind, float rate, float frame);
	void play_camera_anim(GameObject* target, std::string anim_kind, float rate, float frame, bool end_cinematic_on_anim_end);
	void play_camera_anim(GameObject* target, CameraAnim* anim_kind, float rate, float frame, bool end_cinematic_on_anim_end);
	void play_camera_anim(BattleObject* target, std::string anim_kind, float rate, float frame, bool end_cinematic_on_anim_end);
	void play_camera_anim(BattleObject* target, CameraAnim* anim_kind, float rate, float frame, bool end_cinematic_on_anim_end);
	void stop_camera_anim();
	void unload_camera_anims();

	void set_pos(float x, float y, float z);
	void set_rot(float x, float y, float z);
	void add_pos(float x, float y, float z);
	void add_rot(float x, float y, float z);

	void set_fov(float fov);
	
	void reset_camera();
	void calc_aim_from_ypr();
	void calc_ypr_from_aim();

	void update_view();

	void follow_players();
	void follow_anim();

	std::string get_anim_name();

	const glm::vec3 base_pos = glm::vec3(0.0, 8.0, 55.0);
	const glm::vec3 world_up = glm::vec3(0.0, 1.0, 0.0);
	const glm::mat4 flip_matrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	const glm::vec3 scale_vec = glm::vec3(19.2, 10.8, 19.2);

	glm::vec3 pos;
	glm::vec3 prev_pos;
	TargetVar<float> pos_x_interpolator;

	float yaw;
	float pitch;
	float roll;
	float fov;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 aim;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 camera_matrix;

	Fighter* fighter[2];
	Stage* stage;

	CameraAnim* anim_kind;
	float frame;
	float rate;

	GameObject* target;
	glm::vec3 target_base_pos;
	float target_facing_dir;
	bool target_end_cinematic_on_anim_end;

	bool camera_locked;
	bool anim_end;

	std::vector<CameraAnim> camera_anims;
	std::map<std::string, int> camera_anim_map;
};