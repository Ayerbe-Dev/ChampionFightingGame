#include "Projectile.h"

void Projectile::change_anim(std::string animation_name, float frame_rate, float entry_frame) {
	prev_anim_rate = rate;
	prev_anim_frame = frame;

	set_current_move_script(animation_name);

	Animation* new_anim = anim_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		frame = entry_frame;
		rate = frame_rate;
	}
	is_anim_end = false;
	if (anim_kind != new_anim) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = new_anim;
}