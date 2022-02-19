#include "Projectile.h"

void Projectile::change_anim(string animation_name, float frame_rate, float entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_rate = rate;
	prev_anim_frame = frame;
	set_current_move_script(animation_name);

	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			frame = entry_frame;
			rate = frame_rate;

			set_current_move_script(animation_name);
			startAnimation(&animation_table[i]);
			return;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
	startAnimation(nullptr);
}

void Projectile::startAnimation(Animation* animation) {
	is_anim_end = false;
	if (anim_kind != animation) {
		prev_anim_kind = anim_kind;
	}
	anim_kind = animation;
}