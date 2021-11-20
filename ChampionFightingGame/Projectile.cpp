#include "Projectile.h"
#include "Game.h"

extern SDL_Renderer* g_renderer;

void Projectile::projectile_main() {
	prevpos = pos;

	if (canStep()) {
		stepAnimation();
	}

	playoutStatus();

	decrease_common_projectile_variables(this);
	tickOnceProjectileUnique();

	int width;
	int height;
	SDL_QueryTexture(base_texture, NULL, NULL, &width, &height);
	pos.x_spr_offset = width / 2;
	pos.y_spr_offset = height;

	update_hitbox_pos(false);
	update_grabbox_pos();
	update_hurtbox_pos();
}

void Projectile::superInit() {
	load_anim_list();
	load_stats();
	loadStatusScripts();
	change_anim("default", 2, 0);
	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::load_anim_list() {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string name;
	string path;
	string frame_count;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> frame_count;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		loadAnimation(&animation_table[i]);
	}
	anim_list.close();
}

void Projectile::load_stats() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	string stat;
	for (int i = 0; stats_table >> stat; i++) {
		stat_table[i].stat = stat;
		stats_table >> stat_table[i].type;
		switch (stat_table[i].type) {
			case(PARAM_TYPE_INT):
			{
				stats_table >> stat_table[i].value_i;
			} break;
			case(PARAM_TYPE_FLOAT):
			{
				stats_table >> stat_table[i].value_f;
			} break;
			case(PARAM_TYPE_STRING):
			{
				stats_table >> stat_table[i].value_s;
			} break;
			case (PARAM_TYPE_BOOL):
			{
				stats_table >> stat_table[i].value_b;
			} break;
			default:
			{
				stats_table >> stat_table[i].value_i;
			} break;
		}
	}

	stats_table.close();
}

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

void Projectile::loadStatusScripts() {
	status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::status_default;
	enter_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::enter_status_default;
	exit_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::exit_status_default;

	status_script[PROJECTILE_STATUS_MOVE] = &Projectile::status_move;
	enter_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::enter_status_move;
	exit_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::exit_status_move;

	status_script[PROJECTILE_STATUS_HIT] = &Projectile::status_hit;
	enter_status_script[PROJECTILE_STATUS_HIT] = &Projectile::enter_status_hit;
	exit_status_script[PROJECTILE_STATUS_HIT] = &Projectile::exit_status_hit;
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

//Status

bool Projectile::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = false;
		if (call_end_status) {
			if (status_kind < PROJECTILE_STATUS_MAX) {
				(this->*exit_status_script[status_kind])();
			}
			else {
				projectile_unique_exit_status();
			}
		}
		status_kind = new_status_kind;
		if (status_kind < PROJECTILE_STATUS_MAX) {
			(this->*enter_status_script[status_kind])();
		}
		else {
			projectile_unique_enter_status();
		}
		return true;
	}
	else {
		return false;
	}
}

void Projectile::playoutStatus() {
	if (status_kind < PROJECTILE_STATUS_MAX) {
		(this->*status_script[status_kind])();
	}
	else {
		projectile_unique_status();
	}
	move_script();
}

void Projectile::status_default() {

}

void Projectile::enter_status_default() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_param_int("health");
}

void Projectile::exit_status_default() {

}

void Projectile::status_move() {
	pos.x += get_param_float("move_x_speed") * facing_dir;
}

void Projectile::enter_status_move() {
	change_anim("move");
}

void Projectile::exit_status_move() {

}

void Projectile::status_hit() {

}

void Projectile::enter_status_hit() {

}

void Projectile::exit_status_hit() {

}

void Projectile::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
	float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, 
	bool trade, bool continue_launch, bool can_chip_ko) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, meter_gain_on_hit,
			meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable,
			hit_pushback, block_pushback, juggle_set, max_juggle, hit_status, counterhit_status,
			counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, trade, continue_launch, can_chip_ko);
	}
}