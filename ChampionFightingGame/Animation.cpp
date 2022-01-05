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
extern SDL_Renderer* g_renderer;

Animation::Animation() {};

Animation3D::Animation3D() {}

Animation3D::Animation3D(string anim_kind, string anim_dir, Model *model) {
	this->name = anim_kind;
	ifstream smd;
	smd.open(anim_dir);
	if (smd.fail()) {
		cout << "Failed to open SMD!" << endl;
		smd.close();
		return;
	}

	string filler;
	getline(smd, filler);
	getline(smd, filler);

	int bone_id;
	string bone_name;
	int parent_id;

	vector<Bone> base_bones;

	while (smd >> bone_id) { //Eventually smd will try to fit the string "end" into the int bone_id. When this fails, we're at the end.
		smd >> bone_name >> parent_id;
		bone_name = Filter(bone_name, "\"");
		Bone new_bone;
		new_bone.id = bone_id;
		new_bone.name = bone_name;
		new_bone.parent_id = parent_id;
		base_bones.push_back(new_bone);
	}
	smd.clear();

	smd >> filler >> filler;
	while (smd >> filler) { //Until we hit the end of the file, we'll read each string of the file
		if (filler == "end") { //If we just read in "end", we reached the end of the file. Thanks Valve.
			this->length = keyframes.size();
			smd.close();
			return;
		}
		int frame;
		smd >> frame; //Each frame declaration begins with "Time X". If filler wasn't end, it will be time, so the next thing smd reads will be the
		//frame.
		vector<Bone> new_vec = base_bones;
		while (smd >> bone_id) { //When it tries to read "Time" as an int, this will fail
			mat4 matrix = model->bones[bone_id].anim_rest_matrix;
			vec3 pos = model->bones[bone_id].pos;
			vec3 rot = model->bones[bone_id].rot;
			vec3 smd_scale = model->bones[bone_id].scale;

			smd >> pos.x >> pos.y >> pos.z >> rot.x >> rot.y >> rot.z; //Read in the contents of the line
			matrix = translate(matrix, pos);
			rot = normalize(rot);
			mat4 mat_rot = orientate4(rot);
			matrix *= mat_rot;
			matrix = scale(matrix, smd_scale);
			Bone new_bone = base_bones[bone_id];
			new_bone.pos = pos;
			new_bone.rot = rot;
			new_bone.scale = smd_scale;
			new_bone.anim_matrix = matrix;
			new_bone.keyframed = true;
			new_vec[bone_id] = new_bone;
		}
		smd.clear();
		keyframes.push_back(new_vec);
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