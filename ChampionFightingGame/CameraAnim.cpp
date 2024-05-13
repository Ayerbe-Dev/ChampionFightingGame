#include "CameraAnim.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLM Helpers.h"
#include "utils.h"

CameraAnim::CameraAnim() {
	name = "";
	length = -1;
	anim_mode = CAMERA_ANIM_MODE_NONE;
	global_transform = glm::mat4(1.0);
}

CameraAnim::CameraAnim(std::string anim_kind, std::string anim_dir) {
	load_camera_anim(anim_kind, anim_dir);
}

void CameraAnim::load_camera_anim(std::string anim_kind, std::string anim_dir) {
	this->name = anim_kind;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(anim_dir, aiProcess_Triangulate);

	if (!scene || !scene->HasAnimations()) {
		if (!scene) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		}
		else {
			std::cout << "Scene has no animations but Assimp doesn't seem to see the problem \n";
		}
		return;
	}

	const aiAnimation* anim = scene->mAnimations[0];
	length = anim->mDuration;
	keyframes.resize(length + 1);

	if (scene->HasCameras()) {
		load_as_camera(anim);
	}
	else {
		load_as_armature(anim);
	}
}

void CameraAnim::load_as_camera(const aiAnimation* anim) {
	anim_mode = CAMERA_ANIM_MODE_STAGE;
	std::vector<char> pos_keyframed;
	pos_keyframed.resize(length + 1, false);
	aiNodeAnim* node = anim->mChannels[0];

	for (int i = 0; i < node->mNumPositionKeys; i++) {
		keyframes[node->mPositionKeys[i].mTime].pos_key = ass_converter(node->mPositionKeys[i].mValue) / ass_converter(node->mScalingKeys[i].mValue);
		glm::quat quat_key = ass_converter(node->mRotationKeys[i].mValue);
		keyframes[node->mPositionKeys[i].mTime].rot_key = glm::eulerAngles(quat_key) * 180.f / 3.14f;

		pos_keyframed[node->mPositionKeys[i].mTime] = true;
	}

	int last_keyframed = 0;
	int next_keyframed = 1;

	for (int i = 0; i < length; i++) {
		if (pos_keyframed[i]) {
			last_keyframed = i;
			next_keyframed = i;
			for (int i2 = i + 1; i2 <= length; i2++) {
				if (pos_keyframed[i2]) {
					next_keyframed = i2;
					break;
				}
			}
		}
		else if (last_keyframed == next_keyframed) {
			for (int i2 = i; i2 < length; i2++) {
				keyframes[i2].pos_key = keyframes[last_keyframed].pos_key;
				keyframes[i2].rot_key = keyframes[last_keyframed].rot_key;
			}
			break;
		}
		else {
			float old_delta = ((i - last_keyframed) / (float)(next_keyframed - last_keyframed));
			float new_delta = ((next_keyframed - i) / (float)(next_keyframed - last_keyframed));
			keyframes[i].pos_key = keyframes[last_keyframed].pos_key * old_delta + keyframes[next_keyframed].pos_key * new_delta;
			interpolate_keyframe(keyframes[i].rot_key, keyframes[last_keyframed].rot_key * old_delta, keyframes[next_keyframed].rot_key * new_delta);
		}
	}
}

void CameraAnim::load_as_armature(const aiAnimation* anim) {
	anim_mode = CAMERA_ANIM_MODE_FIGHTER;
	int aim_idx = -1;
	int cam_idx = -1;
	for (int i = 0; i < 5; i++) {
		std::string name = anim->mChannels[i]->mNodeName.C_Str();
		if (name == "Aim") {
			aim_idx = i;
			continue;
		}
		if (name == "Camera") {
			cam_idx = i;
			continue;
		}
	}
	std::vector<char> pos_keyframed;
	std::vector<char> aim_keyframed;
	pos_keyframed.resize(length + 1, false);
	aim_keyframed.resize(length + 1, false);
	aiNodeAnim* aim_node = anim->mChannels[aim_idx];
	aiNodeAnim* cam_node = anim->mChannels[cam_idx];
	for (int i = 0; i < aim_node->mNumPositionKeys; i++) {
		glm::vec3 aim_key = ass_converter(aim_node->mPositionKeys[i].mValue) / ass_converter(aim_node->mScalingKeys[i].mValue);		
		keyframes[aim_node->mPositionKeys[i].mTime].rot_key.x = -aim_key.y;
		keyframes[aim_node->mPositionKeys[i].mTime].rot_key.y = aim_key.z;
		keyframes[aim_node->mPositionKeys[i].mTime].rot_key.z = -aim_key.x;
		aim_keyframed[aim_node->mPositionKeys[i].mTime] = true;
	}
	for (int i = 0; i < cam_node->mNumPositionKeys; i++) {
		glm::vec3 pos_key = ass_converter(cam_node->mPositionKeys[i].mValue) / ass_converter(cam_node->mScalingKeys[i].mValue);
		keyframes[cam_node->mPositionKeys[i].mTime].pos_key.x = -pos_key.y;
		keyframes[cam_node->mPositionKeys[i].mTime].pos_key.y = pos_key.z;
		keyframes[cam_node->mPositionKeys[i].mTime].pos_key.z = -pos_key.x;
		pos_keyframed[cam_node->mPositionKeys[i].mTime] = true;
	}

	int last_keyframed = 0;
	int next_keyframed = 1;

	for (int i = 0; i < length; i++) {
		if (aim_keyframed[i]) {
			next_keyframed = i;
			for (int i2 = i + 1; i2 <= length; i2++) {
				if (aim_keyframed[i2]) {
					next_keyframed = i2;
					break;
				}
			}
			last_keyframed = i;
		}
		else if (last_keyframed == next_keyframed) {
			for (int i2 = i; i2 < length; i2++) {
				keyframes[i2].rot_key = keyframes[last_keyframed].rot_key;
			}
			break;
		}
		else {
			float old_delta = ((i - last_keyframed) / (float)(next_keyframed - last_keyframed));
			float new_delta = ((next_keyframed - i) / (float)(next_keyframed - last_keyframed));
			keyframes[i].rot_key = keyframes[last_keyframed].rot_key * old_delta + keyframes[next_keyframed].rot_key * new_delta;
		}
	}

	last_keyframed = 0;
	next_keyframed = 1;

	for (int i = 0; i < length; i++) {
		if (pos_keyframed[i]) {
			next_keyframed = i;
			for (int i2 = i + 1; i2 <= length; i2++) {
				if (pos_keyframed[i2]) {
					next_keyframed = i2;
					break;
				}
			}
			last_keyframed = i;
		}
		else if (last_keyframed == next_keyframed) {
			for (int i2 = i; i2 < length; i2++) {
				keyframes[i2].pos_key = keyframes[last_keyframed].pos_key;
			}
			break;
		}
		else {
			float old_delta = ((i - last_keyframed) / (float)(next_keyframed - last_keyframed));
			float new_delta = ((next_keyframed - i) / (float)(next_keyframed - last_keyframed));
			keyframes[i].pos_key = keyframes[last_keyframed].pos_key * old_delta + keyframes[next_keyframed].pos_key * new_delta;
		}
	}
}

void CameraAnim::interpolate_keyframe(glm::vec3& rot_key, glm::vec3 prev_rot, glm::vec3 future_rot) {
	for (int i = 0; i < 3; i++) {
		if ((prev_rot[i] + future_rot[i]) < (prev_rot[i] + (180.0f - future_rot[i]))) {
			rot_key[i] = future_rot[i] + prev_rot[i];
		}
		else {
			rot_key[i] = prev_rot[i] + (180.0f - future_rot[i]);
		}
	}
}