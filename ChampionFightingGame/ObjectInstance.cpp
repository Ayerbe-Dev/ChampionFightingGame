#include "ObjectInstance.h"
#include "ParamTable.h"

ObjectInstance::ObjectInstance() {
	this->id = -1;
}

void ObjectInstance::set_current_move_script(string anim_name) {}

int ObjectInstance::get_param_int(string param, Param param_table[]) {
	if (!param_table) {
		param_table = this->param_table;
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
		param_table = this->param_table;
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
		param_table = this->param_table;
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
		param_table = this->param_table;
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

bool ObjectInstance::is_excute_frame(int excute_count, int frame)
{
	if (this->frame >= frame)
	{ //If we've reached the statement in question
		if (this->excute_count >= excute_count)
		{ //If we've already executed the statement
			return false;
		}
		else
		{ //If we are at the correct frame and haven't executed the statement yet
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else {
		last_excute_frame = frame;
		return false;
	}
}

bool ObjectInstance::is_excute_wait(int excute_count, int frames) {
	if (frame >= last_excute_frame + frames) { //If it's been enough time since the last non-executed statement
		if (this->excute_count >= excute_count)
		{ //If we already executed the statement
			return false;
		}
		else
		{ //Success
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else
	{ //Still waiting
		return false;
	}
}

void ObjectInstance::update_hitbox_pos() {
	for (int i = 0; i < 10; i++)
	{
		if (hitboxes[i].id != -1)
		{
			hitboxes[i].update_pos(this);
		}
	}
}

void ObjectInstance::clear_hitbox(int id) {
	hitboxes[id].clear();
}

void ObjectInstance::clear_hitbox_all() {
	for (int i = 0; i < 10; i++)
	{
		hitboxes[i].clear();
	}
}

void ObjectInstance::update_grabbox_pos() {
	for (int i = 0; i < 10; i++)
	{
		if (grabboxes[i].id != -1)
		{
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

void ObjectInstance::update_hurtbox_pos()
{
	for (int i = 0; i < 10; i++)
	{
		if (hurtboxes[i].id != -1)
		{
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