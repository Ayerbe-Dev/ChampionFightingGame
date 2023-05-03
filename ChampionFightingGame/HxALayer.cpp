#include "HxALayer.h"
#include "ReadBytes.h"

HxALayer::HxALayer() {
	components = 0;
	type = HxALayerDataType::Unknown;
}

HxALayer::~HxALayer() {
	switch (type) {
		case (HxALayerDataType::UINT8): {
			for (size_t i = 0, max = data.size(); i < max; i++) {
				char* d = (char*)data[i];
				delete d;
			}
		} break;
		case (HxALayerDataType::INT32): {
			for (size_t i = 0, max = data.size(); i < max; i++) {
				int* d = (int*)data[i];
				delete d;
			}
		} break;
		case (HxALayerDataType::FLOAT): {
			for (size_t i = 0, max = data.size(); i < max; i++) {
				float* d = (float*)data[i];
				delete d;
			}
		} break;
		case (HxALayerDataType::DOUBLE): {
			for (size_t i = 0, max = data.size(); i < max; i++) {
				double* d = (double*)data[i];
				delete d;
			}
		} break;
		default:
		case (HxALayerDataType::Unknown): {

		} break;
	}
	data.clear();
}

void HxALayer::parse(std::ifstream* stream, unsigned int num_items) {
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 1)) {
		std::cout << "Failed to read name length for HxA Layer!\n";
		return;
	}
	if (!read_bytes_to_string(stream, &this->name, bytes[0])) {
		std::cout << "Failed to read name for HxA Layer!\n";
		return;
	}
	if (!read_bytes_to_string(stream, &bytes, 2)) {
		std::cout << "Failed to read layer components and type for HxA Layer!\n";
		return;
	}
	components = bytes[0];
	type = (HxALayerDataType)bytes[1];
	switch (type) {
		case (HxALayerDataType::UINT8): {
			for (unsigned _ = 0, max = (unsigned)components * num_items; _ < max; _++) {
				read_bytes_to_string(stream, &bytes, 1);
				char *d = new char;
				*d = bytes[0];
				data.push_back(d);
			}
		} break;
		case (HxALayerDataType::INT32): {
			for (unsigned _ = 0, max = (unsigned)components * num_items; _ < max; _++) {
				read_bytes_to_string(stream, &bytes, 4);
				int* d = new int;
				unsigned result = 0;
				for (size_t i = 0; i < 4; i++) {
					result += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
				}
				*d = result;
				data.push_back(d);
			}
		} break;
		case (HxALayerDataType::FLOAT): {
			for (unsigned _ = 0, max = (unsigned)components * num_items; _ < max; _++) {
				read_bytes_to_string(stream, &bytes, 4);
				float* d = new float;
				unsigned result = 0;
				for (size_t i = 0; i < 4; i++) {
					result += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
				}
				memcpy(d, &result, sizeof(unsigned int));
				data.push_back(d);
			}
		} break;
		case (HxALayerDataType::DOUBLE): {
			for (unsigned _ = 0, max = (unsigned)components * num_items; _ < max; _++) {
				read_bytes_to_string(stream, &bytes, 8);
				double* d = new double;
				unsigned long long result = 0;
				for (size_t i = 0; i < 8; i++) {
					result += (((unsigned long long)bytes[i]) & 0xFF) << (8 * i);
				}
				memcpy(d, &result, sizeof(unsigned long long));
				data.push_back(d);
			}
		} break;
		default:
		case (HxALayerDataType::Unknown): {

		} break;
	}
}

std::type_index HxALayer::get_type_index() {
	switch (type) {
		case (HxALayerDataType::UINT8): {
			return std::type_index(typeid(char));
		} break;
		case (HxALayerDataType::INT32): {
			return std::type_index(typeid(int));
		} break;
		case (HxALayerDataType::FLOAT): {
			return std::type_index(typeid(float));
		} break;
		case (HxALayerDataType::DOUBLE): {
			return std::type_index(typeid(double));
		} break;
		default: {
			return std::type_index(typeid(unsigned char));
		} break;
	}
}

HxALayerStack::HxALayerStack() {
	layer_count = 0;
}

HxALayerStack::~HxALayerStack() {
	layers.clear();
	layer_map.clear();
}

void HxALayerStack::parse(std::ifstream* stream, unsigned int num_items) {
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 4)) {
		std::cout << "Failed to read number of layers for HxA Layer Stack!\n";
		return;
	}
	for (unsigned i = 0; i < 4; i++) {
		layer_count += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
	}
	for (unsigned i = 0; i < layer_count; i++) {
		layers.emplace_back();
		layers.back().parse(stream, num_items);
		layer_map[layers.back().name] = i;
	}
}

HxALayer& HxALayerStack::find(std::string layer_name) {
	if (!layer_map.contains(layer_name)) {
		std::cout << "ERROR: Layer " << layer_name << " does not exist in stack!\n";
		assert(false);
	}
	return layers[layer_map[layer_name]];
}