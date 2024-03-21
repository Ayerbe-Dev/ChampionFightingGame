#include "BattleObject.h"
#include "Fighter.h"

/// <summary>
/// Search the param table for a given stat, and return the value of that stat.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param, or 0 if the value is not found.</returns>
int BattleObject::get_param_int(std::string param) {
	return param_table.get_param_int(param);
}


/// <summary>
/// Search the param table for a given stat, and return the value of that stat.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param, or 0.0 if the value is not found.</returns>
float BattleObject::get_param_float(std::string param) {
	return param_table.get_param_float(param);
}

/// <summary>
/// Search the param table for a given stat, and return the value of that stat.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param, or "" if the value is not found.</returns>
std::string BattleObject::get_param_string(std::string param) {
	return param_table.get_param_string(param);
}

/// <summary>
/// Search the param table for a given stat, and return the value of that stat.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param, or false if the value is not found.</returns>
bool BattleObject::get_param_bool(std::string param) {
	return param_table.get_param_bool(param);
}

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or 0 if the value is not found.</returns>
int Fighter::get_param_int_special(std::string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return param_table.get_param_int(param + "_l");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return param_table.get_param_int(param + "_m");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return param_table.get_param_int(param + "_h");
	}
	else {
		return param_table.get_param_int(param + "_ex");
	}
}
/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or 0.0 if the value is not found.</returns>
float Fighter::get_param_float_special(std::string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return param_table.get_param_float(param + "_l");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return param_table.get_param_float(param + "_m");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return param_table.get_param_float(param + "_h");
	}
	else {
		return param_table.get_param_float(param + "_ex");
	}
}

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or false if the value is not found.</returns>
bool Fighter::get_param_bool_special(std::string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return param_table.get_param_bool(param + "_l");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return param_table.get_param_bool(param + "_m");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return param_table.get_param_bool(param + "_h");
	}
	else {
		return param_table.get_param_bool(param + "_ex");
	}
}

/// <summary>
/// Search the user's param table for a stat that's specific to the level of the special move they're in.
/// </summary>
/// <param name="param">: The param to search for.</param>
/// <returns>The value of the specified param for the current special level, or "" if the value is not found.</returns>
std::string Fighter::get_param_string_special(std::string param) {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
		return param_table.get_param_string(param + "_l");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
		return param_table.get_param_string(param + "_m");
	}
	else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
		return param_table.get_param_string(param + "_h");
	}
	else {
		return param_table.get_param_string(param + "_ex");
	}
}