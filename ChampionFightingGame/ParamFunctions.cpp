#include "BattleObject.h"

/// <summary>
/// Search the specified param table for a given stat, and return the value of that stat. If no table is given, the object's stat table is used by default.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <param name="param_table">: The param table to look in, defaults to the object's regular stats table.</param>
/// <returns>The value of the specified param, or 0 if the value is not found.</returns>
int BattleObject::get_param_int(string param, Param param_table[]) {
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

/// <summary>
/// Search the specified param table for a given stat, and return the value of that stat. If no table is given, the object's stat table is used by default.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <param name="param_table">: The param table to look in, defaults to the object's regular stats table.</param>
/// <returns>The value of the specified param, or 0.0 if the value is not found.</returns>
float BattleObject::get_param_float(string param, Param param_table[]) {
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

/// <summary>
/// Search the specified param table for a given stat, and return the value of that stat. If no table is given, the object's stat table is used by default.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <param name="param_table">: The param table to look in, defaults to the object's regular stats table.</param>
/// <returns>The value of the specified param, or "" if the value is not found.</returns>
string BattleObject::get_param_string(string param, Param param_table[]) {
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

/// <summary>
/// Search the specified param table for a given stat, and return the value of that stat. If no table is given, the object's stat table is used by default.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <param name="param_table">: The param table to look in, defaults to the object's regular stats table.</param>
/// <returns>The value of the specified param, or false if the value is not found.</returns>
bool BattleObject::get_param_bool(string param, Param param_table[]) {
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

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or 0 if the value is not found.</returns>
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
/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or 0.0 if the value is not found.</returns>
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

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or false if the value is not found.</returns>
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

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or "" if the value is not found.</returns>
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