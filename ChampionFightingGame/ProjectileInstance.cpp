#include "ProjectileInstance.h"

void ProjectileInstance::superInit(SDL_Renderer* renderer) {
	load_anim_list(renderer);
	load_params();
	loadStatusFunctions();
	change_anim("default", 2, 0);
	status_kind = PROJECTILE_STATUS_DEFAULT;
}

void ProjectileInstance::load_anim_list(SDL_Renderer* renderer)
{
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail())
	{
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string line_1;
	anim_list >> line_1;
	int num_anims = ymlChopInt(line_1);

	for (int i = 0; i < num_anims; i++) {
		string name;
		string path;
		string frame_count;
		anim_list >> name >> path >> frame_count;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}

void ProjectileInstance::change_anim(string animation_name, int frame_rate, int entry_frame) {
	excute_count = 0;
	last_excute_frame = 0;
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

void ProjectileInstance::startAnimation(Animation* animation)
{
	is_anim_end = false;
	anim_kind = animation;
	int width;
	int height;
	SDL_QueryTexture(animation->SPRITESHEET, NULL, NULL, &width, &height);
	pos.x_anim_offset = width / (anim_kind->length + 1) / 2;
	pos.y_anim_offset = height;
	frame_rect = getFrame(frame, anim_kind);
}

void ProjectileInstance::load_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	int i = 0;
	string stat;
	while (stats_table >> stat) {
		param_table[i].stat = stat;
		stats_table >> param_table[i].type;
		switch (param_table[i].type) {
		case(PARAM_TYPE_INT): {
			stats_table >> param_table[i].value_i;
		} break;
		case(PARAM_TYPE_FLOAT): {
			stats_table >> param_table[i].value_f;
		} break;
		case(PARAM_TYPE_STRING): {
			stats_table >> param_table[i].value_s;
		} break;
		case (PARAM_TYPE_BOOL): {
			stats_table >> param_table[i].value_b;
		} break;
		default: {
			stats_table >> param_table[i].value_i;
		} break;
		}
		i++;
	}

	stats_table.close();
}

void ProjectileInstance::loadStatusFunctions() {
	pStatus[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::status_default;
	pEnter_status[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::enter_status_default;
	pExit_status[PROJECTILE_STATUS_DEFAULT] = &ProjectileInstance::exit_status_default;
}

void ProjectileInstance::status_default() {

}

void ProjectileInstance::enter_status_default() {

}

void ProjectileInstance::exit_status_default() {

}