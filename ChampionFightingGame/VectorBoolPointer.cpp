#include "VectorBoolPointer.h"
#include <vector>

VBP::VBP() {
	ptr = nullptr;
	mask = 0;
}

VBP::VBP(std::vector<bool>::reference ref) {
	ptr = (unsigned*)ref._Getptr();
	mask = static_cast<std::_Vbase>(1) << ref._Myoff;
}

VBP& VBP::operator=(const VBP& other) {
	if (this != &other) {
		this->ptr = other.ptr;
		this->mask = other.mask;
	}
	
	return *this;
}

VBP& VBP::operator=(const bool& other){
	if (ptr != nullptr && (bool)(*ptr & mask) != other) {
		*ptr ^= mask;
	}

	return *this;
}

bool VBP::operator==(const VBP& other) {
	return this->ptr == other.ptr && this->mask == other.mask;
}

bool VBP::operator==(const bool& other) {
	return ptr && (*ptr & mask) == other;
}

VBP::operator bool() {
	return ptr && *ptr & mask;
}

bool VBP::operator !() {
	return !ptr || !(*ptr & mask);
}

bool get_value_at_vector_bool_addr(unsigned int* ptr, unsigned int mask) {
	mask %= (sizeof(std::_Vbase) * 8);
	/*
		Bool vectors internally store words of type std::_Vbase, so we mod our index
		to be within the bounds of the word size. 
	*/
	return ((*ptr) >> mask) & (unsigned)1;
}

void set_value_at_vector_bool_addr(unsigned int* ptr, unsigned int mask, bool value) {
	mask %= (sizeof(std::_Vbase) * 8);
	if ((((*ptr) >> mask) & (unsigned)1) == value) return;
	*ptr ^= ((std::_Vbase)1 << mask);
}