#pragma once
#include <vector>

//Vector Bool Pointer - Helper class used for getting/setting bools put into vectors using pointers
class VBP {
public:
	VBP() {
		ptr = nullptr;
		mask = 0;
	}

	VBP(std::vector<bool>::reference ref) {
		ptr = (unsigned*)ref._Getptr();
		mask = static_cast<std::_Vbase>(1) << ref._Myoff;
	}

	VBP& operator=(const VBP& other) {
		if (this != &other) {
			this->ptr = other.ptr;
			this->mask = other.mask;
		}

		return *this;
	}

	VBP& operator=(const bool& other) {
		if (ptr != nullptr && (bool)(*ptr & mask) != other) {
			*ptr ^= mask;
		}

		return *this;
	}

	bool operator==(const VBP& other) {
		return this->ptr == other.ptr && this->mask == other.mask;
	}

	bool operator==(const bool& other) {
		return ptr && (*ptr & mask) == other;
	}

	operator bool() {
		return ptr && *ptr & mask;
	}

	bool operator !() {
		return !ptr || !(*ptr & mask);
	}
private:
	unsigned* ptr;
	std::_Vbase mask;
};

bool get_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx);
void set_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx, bool value);