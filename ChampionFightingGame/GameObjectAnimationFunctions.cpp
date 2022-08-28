#include "GameObject.h"

void GameObject::set_rate(float rate) {
	this->rate = rate;
}

void GameObject::set_frame(float frame) {
	this->frame = frame;
}

bool GameObject::change_anim(std::string animation_name, float rate, float frame) {
	prev_anim_rate = this->rate;
	prev_anim_frame = this->frame;

	Animation* new_anim = anim_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		if (frame != -1) {
			this->rate = rate;
			this->frame = frame;
		}
		else {
			float target_frame = new_anim->faf;
			if (target_frame < 1) {
				target_frame = new_anim->length;
			}
			this->rate = (target_frame / rate) * 0.8;
			this->frame = 0.0;
		}
		model.set_move(new_anim->move);
		prev_anim_kind = anim_kind;
	}
	prev_anim_offset = glm::vec3(0.0);
	is_anim_end = false;
	anim_kind = new_anim;

	return new_anim != nullptr;
}

void GameObject::animate() {
	frame += rate;
	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length) {
			frame = 0.0;
			is_anim_end = true;
		}
		else {
			is_anim_end = false;
		}
	}
	model.set_bones(frame, anim_kind, false);
	Bone& trans_bone = model.bones[model.get_bone_id("Trans")];
	glm::vec3 trans_offset = glm::vec3(
		trans_bone.anim_matrix[3].z,
		trans_bone.anim_matrix[3].y,
		trans_bone.anim_matrix[3].x
	);
	trans_offset -= prev_anim_offset;
	trans_offset /= scale;

	pos += trans_offset;

	prev_anim_offset = glm::vec3(trans_bone.anim_matrix[3].z, trans_bone.anim_matrix[3].y, trans_bone.anim_matrix[3].x);
}