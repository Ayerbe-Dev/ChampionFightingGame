#include "BattleObject.h"
#include "utils.h"

glm::vec3 BattleObject::get_trans_offset() {
	if (has_model) {
		Bone& trans_bone = model.bone_data[model.get_bone_id("Trans")];
		glm::vec3 trans_offset = glm::vec3(
			trans_bone.anim_matrix[3].z * facing_dir,
			trans_bone.anim_matrix[3].y,
			trans_bone.anim_matrix[3].x
		);
		trans_offset -= prev_anim_offset;
		trans_offset *= glm::vec3(16.5, 11.5, 11.5);
		return trans_offset;
	}
	return glm::vec3(0.0);
}

std::string BattleObject::get_anim() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		return anim_kind->name;
	}
}

std::string BattleObject::get_anim_broad() {
	if (anim_kind == nullptr) {
		return "default";
	}
	else {
		std::string ret = anim_kind->name;
		if (ret.find("_air") != std::string::npos) {
			ret = Filter(ret, "_air");
		}
		return ret;
	}
}

int BattleObject::get_anim_length(std::string anim_name) {
	Animation* anim = anim_table.get_anim(anim_name, true);
	if (anim) {
		if (anim->faf == -1) {
			return anim->length;
		}
		return anim->faf;
	}
	return 0;
}