#include "Projectile.h"

void Projectile::change_anim(string animation_name, int frame_rate, int entry_frame) {
	excute_count = 0;
	attempted_excutes = 0;
	last_excute_frame = 0;

	prev_anim_max_ticks = max_ticks;
	prev_anim_frame = frame;
	prev_anim_render_frame = render_frame;
	int anim_to_use = -1;
	for (int i = 0; i < ANIM_TABLE_LENGTH; i++) {
		if (animation_table[i].name == animation_name) {
			render_frame = entry_frame;
			frame = entry_frame;
			max_ticks = frame_rate;
			ticks = 0;

			set_current_move_script(animation_name);
			startAnimation(&animation_table[i]);
			return;
		}
	}
	cout << "Invalid Animation '" << animation_name << "'" << endl;
}

void Projectile::startAnimation(Animation* animation) {
	is_anim_end = false;
	prev_anim_kind = anim_kind;
	anim_kind = animation;
	int width;
	int height;
	SDL_QueryTexture(animation->spritesheet, NULL, NULL, &width, &height);
	pos.x_anim_offset = width / (anim_kind->length + 1) / 2;
	pos.y_anim_offset = height;
	frame_rect = getFrame(frame, anim_kind);
}

bool Projectile::canStep() {
	attempted_excutes = 0;
	if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] == 0) {
		frame++;
		ticks++;

		if (ticks >= max_ticks) {
			ticks = 0;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void Projectile::stepAnimation() {
	int last_frame = render_frame;
	frame_rect = getFrame(render_frame, anim_kind);
	if (render_frame == anim_kind->length) {
		render_frame = 0;
		frame = 0;
	}
	else {
		render_frame++;
	}
	is_anim_end = last_frame > frame;
}