#pragma once
#include <vector>

//Vector Bool Pointer - Helper class used for getting/setting bools put into vectors using pointers
class VBP {
public:
	VBP();
	VBP(std::vector<bool>::reference ref);

	VBP& operator=(const VBP& other);
	VBP& operator=(const bool& other);

	bool operator==(const VBP& other);
	bool operator==(const bool& other);
	operator bool();
	bool operator !();
private:
	unsigned* ptr;
	std::_Vbase mask;
};

bool get_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx);
void set_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx, bool value);