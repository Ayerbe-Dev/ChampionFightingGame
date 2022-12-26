#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include <any>

class Param {
public:
	Param();
	Param(std::string name);
	std::string name;
	int type;
	std::any value;
};

class ParamTable {
public:
	ParamTable();
	void load_params(std::string resource_dir);
	void unload_params();

	int get_param_int(std::string param_name);
	float get_param_float(std::string param_name);
	std::string get_param_string(std::string param_name);
	bool get_param_bool(std::string param_name);
private:
	std::vector<Param> params;
	std::unordered_map<std::string, int> param_map;
};