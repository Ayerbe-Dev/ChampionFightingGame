#include "Projectile.h"

void Projectile::change_anim(string animation_name, float frame_rate, float entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_rate = rate;
	prev_anim_frame = frame;
	int anim_to_use = -1;
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
}

void Projectile::startAnimation(Animation3D* animation) {
	is_anim_end = false;
	prev_anim_kind = anim_kind;
	anim_kind = animation;
}

bool Projectile::canStep() {
	attempted_excutes = 0;
	if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] == 0) {
		return true;
	}
	else {
		return false;
	}
}

void Projectile::stepAnimation() {
	float last_frame = frame;
	if (frame >= anim_kind->length) {
		frame = 0;
	}
	else {
		frame += rate;
	}
	is_anim_end = last_frame > frame;
}