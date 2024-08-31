#include "VectorBoolPointer.h"

/// <summary>
/// Returns the value of an index of a bool vector by its pointer. This function should 
/// usually be avoided in favor of using VBPs.
/// </summary>
/// <param name="ptr">A pointer to the element of the vector we're trying to access</param>
/// <param name="idx">The index of the vector we're trying to access</param>
/// <returns></returns>
bool get_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx) {
	idx %= (sizeof(std::_Vbase) * 8);
	/*
		Bool vectors internally store words of type std::_Vbase, so we mod our index
		to be within the bounds of the word size.
	*/
	return ((*ptr) >> idx) & (unsigned)1;
}

/// <summary>
/// Sets the value of an index of a bool vector by its pointer. This function should 
/// usually be avoided in favor of using VBPs.
/// </summary>
/// <param name="ptr">A pointer to the element of the vector we're trying to set</param>
/// <param name="idx">The index of the vector we're trying to set</param>
/// <param name="value">The value to set this element to</param>
void set_value_at_vector_bool_addr(unsigned int* ptr, unsigned int idx, bool value) {
	idx %= (sizeof(std::_Vbase) * 8);
	if ((((*ptr) >> idx) & (unsigned)1) == value) return;
	*ptr ^= ((std::_Vbase)1 << idx);
}