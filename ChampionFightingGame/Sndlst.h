#pragma once
#include <fstream>

void parse_sndlst_entry(std::ifstream& stream, std::string& name, std::string& filename, float& volume_mod);
void parse_sndlst_entry(std::ifstream& stream, std::string& name, std::string& filename);
char get_sndlst_char(char key);