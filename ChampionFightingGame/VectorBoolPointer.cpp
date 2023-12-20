#include "VectorBoolPointer.h"
#include <vector>

bool get_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx) {
	idx %= (sizeof(std::_Vbase) * 8);
	/*
		Bool vectors internally store words of type std::_Vbase, so we mod our index
		to be within the bounds of the word size. 
	*/
	return ((*ptr) >> idx) & (unsigned)1;
}

void set_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx, bool value) {
	idx %= (sizeof(std::_Vbase) * 8);
	*ptr &= ((std::_Vbase)value << idx);
}