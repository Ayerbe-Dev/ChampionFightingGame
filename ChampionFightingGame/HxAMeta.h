#pragma once
#include "HxAEnums.h"
#include <iostream>
#include <vector>
#include <typeindex>
#include <assert.h>

class HxAMetaData {
public:
	HxAMetaData();
	~HxAMetaData();
	void parse(std::ifstream* stream);
	std::type_index get_type_index();

	std::string name;
	unsigned data_length;
	HxAMetaDataType type;
	std::vector<void*> data;
};

#define EXTRACT_META(var_name, type, meta_data, index) type var_name = *(type*)meta_data.data[index];	\
if (typeid(type) != meta_data.get_type_index()) {														\
	std::cout << "Error: Tried to extract object of type " << typeid(type) << " from MetaData of type "	\
	<< meta_data.get_type_index() << "\n";																\
	assert(false);																						\
}

#define EXTRACT_META_NO_DECL(var_name, type, meta_data, index) var_name = *(type*)meta_data.data[index];\
if (typeid(type) != meta_data.get_type_index()) {														\
	std::cout << "Error: Tried to extract object of type " << typeid(type) << " from MetaData of type "	\
	<< meta_data.get_type_index() << "\n";																\
	assert(false);																						\
}