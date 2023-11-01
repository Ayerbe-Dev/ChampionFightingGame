#include "Bone.h"
#include <fstream>
#include "utils.h"

bool Skeleton::operator!() {
	return bone_data.empty();
}

bool Skeleton::load_skeleton(std::string path) {
	std::unordered_map<std::string, int> bone_pair_map;
	std::ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		smd.close();
		return false;
	}
	char version_num = '2';
	if (smd.peek() == 'v') {
		smd.get(); //'v'
		version_num = smd.get(); //The actual version num
		smd.get(); //'\n'
	}

	Bone new_bone;
	while (smd >> new_bone.id) {
		smd >> new_bone.name >> new_bone.parent_id;
		new_bone.name = filter_string(new_bone.name, "\""); //Remove the "s from the SMD's bone names
		switch (version_num) {
			case '1': {
				smd >> new_bone.counterpart_id;
			} break;
			case '2': {
				if (new_bone.name.ends_with("_L") || new_bone.name.ends_with("_R")) {
					std::string pair_name = new_bone.name.ends_with("_L") ? 
						filter_string(new_bone.name, "_L") : filter_string(new_bone.name, "_R");
					if (bone_pair_map.contains(pair_name)) {
						new_bone.counterpart_id = bone_pair_map[pair_name];
						bone_data[new_bone.counterpart_id].counterpart_id = new_bone.id;
					}
					else {
						bone_pair_map[pair_name] = new_bone.id;
					}
				}
				else {
					new_bone.counterpart_id = new_bone.id;
				}
			} break;
			default: {
	
			} break;
		}
		bone_data.push_back(new_bone);
		bone_map[new_bone.name] = new_bone.id;
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