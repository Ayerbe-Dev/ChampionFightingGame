#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <assert.h>
#include <iostream>
#include <typeindex>
#include "HxAEnums.h"

class HxALayer {
public:
	HxALayer();
	~HxALayer();
	void parse(std::ifstream* stream, unsigned int num_items);
	std::type_index get_type_index();

	std::string name;
	char components;
	HxALayerDataType type;
	std::vector<void*> data;
};

class HxALayerStack {
public:
	HxALayerStack();
	~HxALayerStack();
	void parse(std::ifstream* stream, unsigned int num_items);
	HxALayer& find(std::string layer_name);

	unsigned layer_count;
	std::vector<HxALayer> layers;
	std::map<std::string, unsigned int> layer_map;
};

#define EXTRACT_LAYER(var_name, type, layer, index) type var_name = *(type*)layer.data[index];			\
if (typeid(type) != layer.get_type_index()) {															\
	std::cout << "Error: Tried to extract object of type " << typeid(type) << " from Layer of type "	\
	<< layer.get_type_index() << "\n";																	\
	assert(false);																						\
}

#define EXTRACT_LAYER_NO_DECL(var_name, type, layer, index) var_name = *(type*)layer.data[index];		\
if (typeid(type) != layer.get_type_index()) {															\
	std::cout << "Error: Tried to extract object of type " << typeid(type) << " from Layer of type "	\
	<< layer.get_type_index() << "\n";																	\
	assert(false);																						\
}