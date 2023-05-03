#include "HxAMeta.h"
#include "HxANode.h"
#include "ReadBytes.h"

HxAMetaData::HxAMetaData() {
	data_length = 0;
	type = HxAMetaDataType::Unknown;
}

HxAMetaData::~HxAMetaData() {
	switch (type) {
		case (HxAMetaDataType::INT64): {
			for (unsigned i = 0; i < data_length; i++) {
				long long* d = (long long*)data[i];
				delete d;
			}
		} break;
		case (HxAMetaDataType::DOUBLE): {
			for (unsigned i = 0; i < data_length; i++) {
				double* d = (double*)data[i];
				delete d;
			}
		} break;
		case (HxAMetaDataType::NODE): {
			for (unsigned i = 0; i < data_length; i++) {
				switch (((HxANode*)data[i])->node_type) {
					case(HxANodeType::MetaOnly): {
						HxAMetaNode* node = (HxAMetaNode*)data[i];
						delete node;
					} break;
					case(HxANodeType::Geometry): {
						HxAGeometryNode* node = (HxAGeometryNode*)data[i];
						delete node;
					} break;
					case(HxANodeType::Image): {
						HxAImageNode* node = (HxAImageNode*)data[i];
						delete node;
					} break;
					default:
					case(HxANodeType::Unknown): {
						HxANode* node = (HxANode*)data[i];
						delete node;
					} break;
				}
			}
		} break;
		case (HxAMetaDataType::TEXT): {
			if (data_length != 0) {
				std::string* s = (std::string*)data[0];
				delete s;
			}
		} break;
		case (HxAMetaDataType::META): {
			for (unsigned i = 0; i < data_length; i++) {
				HxAMetaData* d = (HxAMetaData*)data[i];
				delete d;
			}
		} break;
		default:
		case (HxAMetaDataType::COUNT):
		case (HxAMetaDataType::Unknown): {

		} break;
	}
	data.clear();
}

void HxAMetaData::parse(std::ifstream* stream) {
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 1)) {
		std::cout << "Failed to read name length for HxA Meta!\n";
		return;
	}
	if (!read_bytes_to_string(stream, &this->name, bytes[0])) {
		std::cout << "Failed to read name for HxA Meta!\n";
		return;
	}
	if (!read_bytes_to_string(stream, &bytes, 5)) {
		std::cout << "Failed to read MetaDataType and data length for HxAMeta!\n";
		return;
	}
	type = (HxAMetaDataType)bytes[0];
	for (size_t i = 0; i < 4; i++) {
		data_length += (((unsigned)bytes[i+1]) & 0xFF) << (8 * i);
	}
	data.reserve(data_length);
	switch (type) {
		case (HxAMetaDataType::INT64): {
			for (unsigned _ = 0; _ < data_length; _++) {
				read_bytes_to_string(stream, &bytes, 8);
				long long* d = new long long;
				long long result = 0;
				for (size_t i = 0; i < 8; i++) {
					result += (((long long)bytes[i]) & 0xFF) << (8 * i);
				}
				*d = result;
				data.push_back(d);
			}
		} break;
		case (HxAMetaDataType::DOUBLE): {
			for (unsigned _ = 0; _ < data_length; _++) {
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
		case (HxAMetaDataType::NODE): {
			for (unsigned _ = 0; _ < data_length; _++) {
				HxANode* node = create_node(stream);
				if (node == nullptr) {
					std::cout << "Failed to create Node from HxAMetaData\n";
					return;
				}
				node->parse(stream);
				data.push_back(node);
			}
		} break;
		case (HxAMetaDataType::TEXT): {
			std::string* string = new std::string;
			read_bytes_to_string(stream, string, data_length);
			data.push_back(string);
		} break;
		case (HxAMetaDataType::META): {
			for (unsigned _ = 0; _ < data_length; _++) {
				HxAMetaData* meta_data = new HxAMetaData();
				meta_data->parse(stream);
				data.push_back(meta_data);
			}
		} break;
		default:
		case (HxAMetaDataType::COUNT):
		case (HxAMetaDataType::Unknown): {

		} break;
	}
}

std::type_index HxAMetaData::get_type_index() {
	switch (type) {
		case (HxAMetaDataType::INT64): {
			return std::type_index(typeid(long));
		} break;
		case (HxAMetaDataType::DOUBLE): {
			return std::type_index(typeid(double));
		} break;
		case (HxAMetaDataType::NODE): {
			return std::type_index(typeid(HxANode));
		} break;
		case (HxAMetaDataType::TEXT): {
			return std::type_index(typeid(std::string));
		} break;
		case (HxAMetaDataType::BINARY): {
			return std::type_index(typeid(unsigned char));
		} break;
		case (HxAMetaDataType::META): {
			return std::type_index(typeid(HxAMetaData));
		} break;
		case (HxAMetaDataType::COUNT):
		default: {
			return std::type_index(typeid(unsigned char));
		} break;
	}
}