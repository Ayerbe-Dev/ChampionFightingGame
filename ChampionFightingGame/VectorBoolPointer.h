#pragma once
#include <vector>

//A container class used for getting/setting bools put into vectors using pointers
class VBP {
public:
	VBP() {
		ptr = nullptr;
		mask = 0;
	}

	/// <summary>
	/// Constructs a VBP using an element of a bool vector.
	/// </summary>
	/// <param name="ref">The element of a bool vector we want to access by pointer</param>
	VBP(std::vector<bool>::reference ref) {
		ptr = (unsigned*)ref._Getptr();

		//std::_Vbase uses an offset variable for faster vector access. The class member is called
		//std::_Vbase._Myoff.

		//There is a function called std::_Vbase._Mask(), which returns this->_Myoff. 
		//Between _Mask() and _Myoff, one member is public and the other is protected.

		mask = static_cast<std::_Vbase>(1) << ref._Myoff; //<-- Guess which is which

		//Get the STL devs the hell out of the kitchen, they cannot cook
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