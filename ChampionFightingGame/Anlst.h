#pragma once
#include <fstream>

void parse_anlst_entry(std::ifstream& stream, std::string& name, std::string& filename, int& end_frame,
	bool& flag_move, bool& flag_no_hitlag_interp, bool& flag_unused);
void parse_anlst_entry(std::ifstream& stream, std::string& name, std::string& filename);
char get_anlst_char(char key);