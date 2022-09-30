#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct CameraKeyframe {
	glm::vec3 pos_key = glm::vec3(0.0);
	glm::vec3 rot_key = glm::vec3(0.0);
	bool keyframed = false;
};

class CameraAnim {
public:
	CameraAnim();
	CameraAnim(std::string anim_kind, std::string dir);

	void load_camera_anim(std::string anim_kind, std::string dir);
	void interpolate_keyframe(glm::vec3& rot_key, glm::vec3 prev_rot, glm::vec3 future_rot);

	std::string name;
	int length;
	std::vector<CameraKeyframe> keyframes;
	glm::mat4 global_transform;
};