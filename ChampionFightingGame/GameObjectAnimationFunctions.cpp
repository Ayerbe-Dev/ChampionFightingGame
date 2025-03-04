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
	prev_anim_offset = glm::vec3(0.0);
	anim_end = false;

	Animation* new_anim = anim_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		if (new_anim->length == 0) {
			anim_kind = nullptr;
			std::cout << "ERROR: Animation " << animation_name << " is on the table but no file was found\n";
			return false;
		}
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
		model.set_move(new_anim->flag_move);
		prev_anim_kind = anim_kind;
	}
	anim_kind = new_anim;

	return new_anim != nullptr;
}