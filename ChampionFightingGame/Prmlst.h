#pragma once
#include <fstream>
#include <any>

void parse_param_entry(std::ifstream& stream, std::string& name, int& type, std::any& value);
char get_prmlst_char(char key);