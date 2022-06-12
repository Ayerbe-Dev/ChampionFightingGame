#include "CameraAnim.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLM Helpers.h"
#include "utils.h"

CameraAnim::CameraAnim() {
	length = -1;
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
	length = scene->mAnimations[0]->mDuration;
	keyframes.resize(length + 1);

	aiNodeAnim* node = scene->mAnimations[0]->mChannels[0];

	for (int i = 0; i < node->mNumPositionKeys; i++) {
		glm::vec3 container_vec = ass_converter(node->mScalingKeys[i].mValue); //This thing handles 3 different
		//temp vars lmao
		glm::vec3 pos_key = ass_converter(node->mPositionKeys[i].mValue) / container_vec;
		container_vec = pos_key;
		pos_key.x = container_vec.y;
		pos_key.y = container_vec.z * -1;
		pos_key.z = container_vec.x * -1;
		//I have a migraine, I don't care enough to make this easy to read rn (Y and Z need to be swapped, 
		//and both need to be made negative)
		glm::quat quat_key = ass_converter(node->mRotationKeys[i].mValue);
		container_vec = glm::eulerAngles(quat_key) * 3.14159f / 180.f;
		glm::vec3 rot_key;
		rot_key.x = container_vec.x;
		rot_key.y = container_vec.z;
		rot_key.z = container_vec.y;

		keyframes[node->mPositionKeys[i].mTime].pos_key = pos_key;
		keyframes[node->mPositionKeys[i].mTime].rot_key = glm::normalize(rot_key);

		keyframes[node->mPositionKeys[i].mTime].keyframed = true;
	}

	int last_keyframed = 0;
	int next_keyframed = 1;

	for (int i = 0; i < length; i++) { //Interpolate to find all of the keyframes that aren't baked
		if (keyframes[i].keyframed) {
			//If we find a keyframe that was already baked, set this to our last baked keyframe, then search for the next one after this and 
			//mark that as the next baked keyframe.

			//This approach means that we won't look for the next keyframe until we've found the current one, avoiding unnecessary recalculations

			for (int i2 = i + 1; i2 <= length; i2++) {
				if (keyframes[i2].keyframed) {
					next_keyframed = i2;
					break;
				}
			}
			last_keyframed = i;
		}
		else {
			float old_delta = ((i - last_keyframed) / (float)(next_keyframed - last_keyframed));
			float new_delta = ((next_keyframed - i) / (float)(next_keyframed - last_keyframed));
			keyframes[i].pos_key = keyframes[last_keyframed].pos_key * old_delta + keyframes[next_keyframed].pos_key * new_delta;
			interpolate_keyframe(keyframes[i].rot_key, keyframes[last_keyframed].rot_key * old_delta, keyframes[next_keyframed].rot_key * new_delta);
		}
	}
}

void CameraAnim::interpolate_keyframe(glm::vec3& rot_key, glm::vec3 prev_rot, glm::vec3 future_rot) {
	for (int i = 0; i < 3; i++) {
		if (future_rot[i] + prev_rot[i] < prev_rot[i] + 1.0 - future_rot[i]) {
			rot_key[i] = future_rot[i] + prev_rot[i];
		}
		else {
			rot_key[i] = prev_rot[i] + 1.0 - future_rot[i];
		}
	}
}