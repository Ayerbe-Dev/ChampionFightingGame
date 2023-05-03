#pragma once
#include <fstream>
#include <iostream>

bool read_bytes_to_string(std::ifstream* stream, std::string* string, unsigned num_bytes);