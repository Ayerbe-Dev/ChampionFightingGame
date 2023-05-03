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
	ParamTable(std::string resource_dir);
	void load_params(std::string resource_dir);
	void unload_params();

	int get_param_int(std::string param_name);
	int get_param_int(std::string param_name, std::string sub_table_name);
	int get_param_int(std::string param_name, int sub_table_index);
	int get_param_int(std::string param_name, std::initializer_list<std::any> sub_tables);

	float get_param_float(std::string param_name);
	float get_param_float(std::string param_name, std::string sub_table_name);
	float get_param_float(std::string param_name, int sub_table_index);
	float get_param_float(std::string param_name, std::initializer_list<std::any> sub_tables);

	std::string get_param_string(std::string param_name);
	std::string get_param_string(std::string param_name, std::string sub_table_name);
	std::string get_param_string(std::string param_name, int sub_table_index);
	std::string get_param_string(std::string param_name, std::initializer_list<std::any> sub_tables);

	bool get_param_bool(std::string param_name);
	bool get_param_bool(std::string param_name, std::string sub_table_name);
	bool get_param_bool(std::string param_name, int sub_table_index);
	bool get_param_bool(std::string param_name, std::initializer_list<std::any> sub_tables);

	ParamTable get_param_table(std::string param_name);
	ParamTable get_param_table(int param_index);

	int get_param_type(std::string param_name);

	void add_param(Param param, int index);
	bool load_failed();
private:
	int get_param_int(std::string param_name, std::list<std::any> sub_tables);
	float get_param_float(std::string param_name, std::list<std::any> sub_tables);
	std::string get_param_string(std::string param_name, std::list<std::any> sub_tables);
	bool get_param_bool(std::string param_name, std::list<std::any> sub_tables);

	std::vector<Param> params;
	std::unordered_map<std::string, int> param_map;
	bool load_fail;
};