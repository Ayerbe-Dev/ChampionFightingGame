using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <string>
#include <iostream>
#include <functional>
#include "Animation.h"
#include "Model.h"
#include <gtx/euler_angles.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtx/string_cast.hpp>
extern SDL_Renderer* g_renderer;

Animation::Animation() {};

Animation3D::Animation3D() {}

Animation3D::Animation3D(string anim_kind, string anim_dir, Model *model) {
	this->name = anim_kind;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(anim_dir, aiProcess_Triangulate);

	if (!scene) {
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}

	length = scene->mAnimations[0]->mDuration;
	vector<Bone> base_bones;

	for (int i = 0; i < model->bones.size(); i++) {
		Bone new_bone;
		new_bone.id = model->bones[i].id;
		new_bone.name = model->bones[i].name;
		new_bone.parent_id = model->bones[i].parent_id;
		new_bone.anim_matrix = mat4(1.0);
		base_bones.push_back(new_bone);
	}

	for (int i = 0; i <= length; i++) {
		keyframes.push_back(base_bones);
	}
		
	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		aiNodeAnim* node = scene->mAnimations[0]->mChannels[i];
		int index = model->get_bone_id(node->mNodeName.C_Str());
		if (index == -1) {
			continue;
		}
		for (int i2 = 0; i2 < node->mNumPositionKeys; i2++) { //Load the keyframes that are actually baked
			mat4 pos_mat = translate(mat4(1.0), ass_converter(node->mPositionKeys[i2].mValue));
			mat4 rot_mat = toMat4(normalize(ass_converter(node->mRotationKeys[i2].mValue)));
			mat4 scale_mat = scale(mat4(1.0), ass_converter(node->mScalingKeys[i2].mValue));
			keyframes[node->mPositionKeys[i2].mTime][index].anim_matrix = pos_mat * rot_mat * scale_mat;
			keyframes[node->mPositionKeys[i2].mTime][index].keyframed = true;
		}

		int last_keyframed = 0;
		int next_keyframed = 1;
		for (int i2 = 0; i2 < length; i2++) { //Interpolate to find all of the keyframes that aren't baked
			if (keyframes[i2][index].keyframed) {
				//If we find a keyframe that was already baked, set this to our last baked keyframe, then search for the next one after this and 
				//mark that as the next baked keyframe.

				//This approach means that we won't look for the next keyframe until we've found the current one, avoiding unnecessary recalculations

				last_keyframed = i2;
				for (int i3 = last_keyframed + 1; i3 <= length; i3++) {
					if (keyframes[i3][index].keyframed) {
						next_keyframed = i3;
						break;
					}
				}
			}
			else {
				float delta = ((float)i2 - last_keyframed) / (next_keyframed - last_keyframed);
				keyframes[i2][index].anim_matrix = interpolate(keyframes[last_keyframed][index].anim_matrix, keyframes[next_keyframed][index].anim_matrix, delta);
			}
		}
	}
}

//Calculates which region of the currently loaded spritesheet to render

SDL_Rect getFrame(int frame, Animation* animation) {
	int width;
	int height;
	SDL_QueryTexture(animation->spritesheet, NULL, NULL, &width, &height);
	SDL_Rect frame_rect = animation->anim_map[frame];
	if (frame_rect.x == 0 && frame_rect.y == 0 && frame_rect.w == 0 && frame_rect.h == 0) {
		frame_rect.x = frame * (width / (animation->length + 1));
		frame_rect.y = 0;
		frame_rect.w = width / (animation->length + 1);
		frame_rect.h = height;
	}

	/* 

	⢀⡴⠑⡄⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
   ⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉

	*/

	return frame_rect;
}

void loadAnimation(Animation* animation) {
	animation->spritesheet = loadSDLTexture((animation->path).c_str());
}