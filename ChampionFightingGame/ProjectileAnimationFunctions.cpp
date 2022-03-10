#include "Projectile.h"

void Projectile::change_anim(std::string animation_name, float frame_rate, float entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_rate = rate;
	prev_anim_frame = frame;

	set_current_move_script(animation_name);

	Animation* new_anim = animation_table.get_anim(animation_name, true);
	if (new_anim != nullptr) {
		frame = entry_frame;
		rate = frame_rate;
	}
	startAnimation(new_anim);
}

void Projectile::startAnimation(Animation* animation) {
	is_anim_end = false;
	if (anim_kind != animation) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = animation;
}