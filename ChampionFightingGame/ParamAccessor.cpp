#include "ParamAccessor.h"

ParamAccessor* ParamAccessor::instance = nullptr;

ParamAccessor::ParamAccessor() {
	params[PARAM_FIGHTER].load_params("resource/param/battle/fighter_common.yml");
	params[PARAM_RENDER].load_params("resource/param/battle/render_param.yml");
	params[PARAM_MENU].load_params("resource/param/menu/menu_param.yml");
	params[PARAM_GLOBAL].load_params("resource/param/global/global_param.yml");
}

ParamAccessor* ParamAccessor::get_instance() {
	if (instance == nullptr) {
		instance = new ParamAccessor;
	}

	return instance;
}

int get_param_int(std::string param_name, int param_table) {
	return ParamAccessor::get_instance()->params[param_table].get_param_int(param_name);
}

float get_param_float(std::string param_name, int param_table) {
	return ParamAccessor::get_instance()->params[param_table].get_param_float(param_name);
}

std::string get_param_string(std::string param_name, int param_table) {
	return ParamAccessor::get_instance()->params[param_table].get_param_string(param_name);
}

bool get_param_bool(std::string param_name, int param_table) {
	return ParamAccessor::get_instance()->params[param_table].get_param_bool(param_name);
}