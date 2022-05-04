#pragma once
#include "Param.h"

int get_param_int(std::string param_name, int param_table);
float get_param_float(std::string param_name, int param_table);
std::string get_param_string(std::string param_name, int param_table);
bool get_param_bool(std::string param_name, int param_table);

enum {
	PARAM_FIGHTER,
	PARAM_RENDER,
	PARAM_MENU,
	PARAM_GLOBAL,

	PARAM_MAX,
};

class ParamAccessor {
public:
	ParamAccessor(ParamAccessor& other) = delete;
	void operator=(const ParamAccessor& other) = delete;

	ParamTable params[PARAM_MAX];

	static ParamAccessor* get_instance();
private:
	ParamAccessor();
	static ParamAccessor* instance;
};