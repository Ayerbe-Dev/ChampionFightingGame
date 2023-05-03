#pragma once
#include "Param.h"

int get_param_int(int param_table, std::string param_name);
float get_param_float(int param_table, std::string param_name);
std::string get_param_string(int param_table, std::string param_name);
bool get_param_bool(int param_table, std::string param_name);

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
	void destroy_instance();
private:
	ParamAccessor();
	static ParamAccessor* instance;
};