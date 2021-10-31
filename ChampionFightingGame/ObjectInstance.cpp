#include "ObjectInstance.h"
#include "ParamTable.h"

MoveScript::MoveScript() {};

MoveScript::MoveScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->move_script = move_script;
	this->id = id;
}

ObjectInstance::ObjectInstance() {
	this->id = -1;
}

int ObjectInstance::get_param_int(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->stat_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_i;
		}
	}

	return 0;
}

float ObjectInstance::get_param_float(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->stat_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_f;
		}
	}

	return 0.0;
}

string ObjectInstance::get_param_string(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->stat_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_s;
		}
	}

	return "";
}

bool ObjectInstance::get_param_bool(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->stat_table;
	}
	for (int i = 0; i < PARAM_TABLE_LENGTH; i++) {
		if (param_table[i].type == -1) {
			cout << "Param " << param << " not found" << endl;
			break;
		}
		else if (param_table[i].stat == param) {
			return param_table[i].value_b;
		}
	}

	return false;
}

bool ObjectInstance::is_excute_frame(int frame) {
	bool ret = false;
	attempted_excutes++;
	if (this->frame >= frame) {
		if (excute_count < attempted_excutes) {
			excute_count++;
			ret = true;
		}
	}

	last_excute_frame = frame;

	return ret;
}

bool ObjectInstance::is_excute_wait(int frames) {
	bool ret = false;
	attempted_excutes++;
	if (frame >= last_excute_frame + frames) {
		if (excute_count < attempted_excutes) {
			excute_count++;
			ret = true;
		}
	}
	last_excute_frame += frames;

	return ret;
}

void ObjectInstance::script(string name, function<void()> move_script) {
	for (int i = 0; i < MOVE_SCRIPT_MAX; i++) {
		if (move_scripts[i].id == -1) {
			move_scripts[i] = MoveScript(name, move_script, i);
			break;
		}
	}
}

void ObjectInstance::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (move_scripts[i].name == anim_name) {
			move_script = move_scripts[i].move_script;
			break;
		}
		else {
			move_script = move_scripts[0].move_script;
		}
	}
}

void ObjectInstance::update_hitbox_pos(bool add_window_width) {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1) {
			hitboxes[i].update_pos(this, add_window_width);
		}
	}
}

void ObjectInstance::clear_hitbox(int id) {
	hitboxes[id].clear();
}

void ObjectInstance::clear_hitbox_all() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].clear();
	}
}

void ObjectInstance::update_grabbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (grabboxes[i].id != -1) {
			grabboxes[i].update_pos(this);
		}
	}
}

void ObjectInstance::clear_grabbox(int id) {
	grabboxes[id].clear();
}

void ObjectInstance::clear_grabbox_all() {
	for (int i = 0; i < 10; i++) {
		grabboxes[i].clear();
	}
}

void ObjectInstance::update_hurtbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (hurtboxes[i].id != -1) {
			hurtboxes[i].update_pos(this);
		}
	}
}

void ObjectInstance::clear_hurtbox(int id) {
	hurtboxes[id].clear();
}

void ObjectInstance::clear_hurtbox_all() {
	for (int i = 0; i < 10; i++) {
		hurtboxes[i].clear();
	}
}

void ObjectInstance::load_stats() {
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

void ObjectInstance::load_params() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/params.yml");

	if (stats_table.fail()) {
		cerr << "Could not open stats table!" << endl;
		exit(1);
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

void ObjectInstance::update_hitbox_connect(int multihit_index) {
	for (int i = 0; i < 10; i++) {
		if (hitboxes[i].id != -1) {
			hitboxes[i].update_connect(multihit_index);
		}
	}
}