#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum CameraAnimMode {
	CAMERA_ANIM_MODE_NONE,
	CAMERA_ANIM_MODE_STAGE,
	CAMERA_ANIM_MODE_FIGHTER,
};

struct CameraKeyframe {
	glm::vec3 pos_key = glm::vec3(0.0);
	glm::vec3 rot_key = glm::vec3(0.0);
};

class CameraAnim {
public:
	CameraAnim();
	CameraAnim(std::string anim_kind, std::string dir);

	void load_camera_anim(std::string anim_kind, std::string dir);

	std::string name;
	int length;
	CameraAnimMode anim_mode;
	std::vector<CameraKeyframe> keyframes;
	glm::mat4 global_transform;

private:
	void load_as_camera(const aiAnimation* anim);
	void load_as_armature(const aiAnimation* anim);
	void interpolate_keyframe(glm::vec3& rot_key, glm::vec3 prev_rot, glm::vec3 future_rot);
};