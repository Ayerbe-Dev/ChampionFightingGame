#include "GameObject.h"
#include "WindowConstants.h"

void GameObject::process_animate() {
	frame_delta = rate;
	frame += rate;
	anim_end = false;
	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length) {
			frame = 0.0;
			anim_end = true;
		}
	}
	if (model.has_skeleton()) {
		model.set_bones(frame, anim_kind);
		Bone& trans_bone = model.bone_data[model.get_bone_id("Trans")];
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
}

void GameObject::process_render_pos() {
	render_pos = pos / scale_vec;
}