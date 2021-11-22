#pragma warning(disable : 4996)
#include "Object.h"

void Object::load_stats() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
		stats_table.close();
		return;
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

void Object::load_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/params.yml");

	if (stats_table.fail()) {
		stats_table.close();
		return;
	}

	string stat;
	for (int i = 0; stats_table >> stat; i++) {
		param_table[i].stat = stat;
		stats_table >> param_table[i].type;
		switch (param_table[i].type) {
			case(PARAM_TYPE_INT):
			{
				stats_table >> param_table[i].value_i;
			} break;
			case(PARAM_TYPE_FLOAT):
			{
				stats_table >> param_table[i].value_f;
			} break;
			case(PARAM_TYPE_STRING):
			{
				stats_table >> param_table[i].value_s;
			} break;
			case (PARAM_TYPE_BOOL):
			{
				stats_table >> param_table[i].value_b;
			} break;
			default:
			{
				stats_table >> param_table[i].value_i;
			} break;
		}
	}

	stats_table.close();
}