#include "Bone.h"
#include <fstream>
#include "utils.h"

bool Skeleton::operator!() {
	return bone_data.empty();
}

bool Skeleton::load_skeleton(std::string path) {
	std::ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		smd.close();
		return false;
	}

	Bone new_bone;
	int bone_id;
	std::string bone_name;
	int parent_id;
	int counterpart_id;
	while (smd >> bone_id) {
		smd >> bone_name >> parent_id >> counterpart_id;
		bone_name = Filter(bone_name, "\""); //Remove the "s from the SMD's bone names
		new_bone.name = bone_name;
		new_bone.id = bone_id;
		new_bone.parent_id = parent_id;
		new_bone.counterpart_id = counterpart_id;
		bone_data.push_back(new_bone);
		bone_map[bone_name] = bone_id;
	}

	smd.close();
	return true;
}

int Skeleton::get_bone_id(std::string bone_name) {
	if (bone_name == "model-armature") {
		if (bone_map.contains("")) {
			return 0;
		}
		else {
			return -1;
		}
	}
	if (!bone_map.contains(bone_name)) {
		std::cout << "ERROR: Couldn't find " << bone_name << "\n";
		return -1;
	}
	return bone_map[bone_name];
}