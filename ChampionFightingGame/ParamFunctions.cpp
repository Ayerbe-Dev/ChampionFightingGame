#include "Object.h"

int Object::get_param_int(string param, Param param_table[]) {
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

float Object::get_param_float(string param, Param param_table[]) {
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

string Object::get_param_string(string param, Param param_table[]) {
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

bool Object::get_param_bool(string param, Param param_table[]) {
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

#include "Fighter.h"

int Fighter::get_param_int_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_int(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_int(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_int(param + "_h", param_table);
	}
	else {
		return get_param_int(param + "_ex", param_table);
	}
}

float Fighter::get_param_float_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_float(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_float(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_float(param + "_h", param_table);
	}
	else {
		return get_param_float(param + "_ex", param_table);
	}
}

bool Fighter::get_param_bool_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_bool(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_bool(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_bool(param + "_h", param_table);
	}
	else {
		return get_param_bool(param + "_ex", param_table);
	}
}

string Fighter::get_param_string_special(string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return get_param_string(param + "_l", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return get_param_string(param + "_m", param_table);
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return get_param_string(param + "_h", param_table);
	}
	else {
		return get_param_string(param + "_ex", param_table);
	}
}