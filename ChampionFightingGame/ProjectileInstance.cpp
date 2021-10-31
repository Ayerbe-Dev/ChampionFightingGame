#include "ProjectileInstance.h"
#include "Game.h"

void ProjectileInstance::projectile_main() {
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

void ProjectileInstance::superInit(SDL_Renderer* renderer) {
	load_anim_list(renderer);
	load_stats();
	loadStatusFunctions();
	change_anim("default", 2, 0);
	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void ProjectileInstance::load_anim_list(SDL_Renderer* renderer) {
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
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void ProjectileInstance::load_stats() {
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

void ProjectileInstance::change_anim(string animation_name, int frame_rate, int entry_frame) {
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

void ProjectileInstance::startAnimation(Animation* animation) {
	is_anim_end = false;
	prev_anim_kind = anim_kind;
	anim_kind = animation;
	int width;
	int height;
	SDL_QueryTexture(animation->SPRITESHEET, NULL, NULL, &width, &height);
	pos.x_anim_offset = width / (anim_kind->length + 1) / 2;
	pos.y_anim_offset = height;
	frame_rect = getFrame(frame, anim_kind);
}

void ProjectileInstance::loadStatusFunctions() {
	pStatus[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::status_default;
	pEnter_status[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::enter_status_default;
	pExit_status[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::exit_status_default;

	pStatus[PROJECTILE_STATUS_MOVE] = &ProjectileInstance::status_move;
	pEnter_status[PROJECTILE_STATUS_MOVE] = &ProjectileInstance::enter_status_move;
	pExit_status[PROJECTILE_STATUS_MOVE] = &ProjectileInstance::exit_status_move;

	pStatus[PROJECTILE_STATUS_HIT] = &ProjectileInstance::status_hit;
	pEnter_status[PROJECTILE_STATUS_HIT] = &ProjectileInstance::enter_status_hit;
	pExit_status[PROJECTILE_STATUS_HIT] = &ProjectileInstance::exit_status_hit;
}

bool ProjectileInstance::canStep() {
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

void ProjectileInstance::stepAnimation() {
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

bool ProjectileInstance::change_status(u32 new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = false;
		if (call_end_status) {
			(this->*pExit_status[status_kind])();
		}
		status_kind = new_status_kind;
		(this->*pEnter_status[status_kind])();
		return true;
	}
	else {
		return false;
	}
}

void ProjectileInstance::playoutStatus() {
	(this->*pStatus[status_kind])();
	move_script();
}

void ProjectileInstance::status_default() {

}

void ProjectileInstance::enter_status_default() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_param_int("health");
}

void ProjectileInstance::exit_status_default() {

}

void ProjectileInstance::status_move() {
	pos.x += get_param_float("move_x_speed") * facing_dir;
}

void ProjectileInstance::enter_status_move() {
	change_anim("move");
}

void ProjectileInstance::exit_status_move() {

}

void ProjectileInstance::status_hit() {

}

void ProjectileInstance::enter_status_hit() {

}

void ProjectileInstance::exit_status_hit() {

}

void ProjectileInstance::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
	float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, bool success_hit, int juggle_set, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, bool continue_launch) {
	if (id < 10) {
		hitboxes[id] = Hitbox(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, meter_gain_on_hit,
			meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable,
			hit_pushback, block_pushback, success_hit, juggle_set, max_juggle, hit_status, counterhit_status,
			counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, trade, continue_launch);
	}
}