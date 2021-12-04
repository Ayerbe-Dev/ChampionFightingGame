#include "Projectile.h"

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