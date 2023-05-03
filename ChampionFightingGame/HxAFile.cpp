#include "HxAFile.h"
#include <string>
#include "ReadBytes.h"

HxAFile::HxAFile() {
	version_number = 0;
	node_count = 0;
}

HxAFile::HxAFile(std::string dir) {
	version_number = 0;
	node_count = 0;

	init(dir);
}

HxAFile::~HxAFile() {
	for (int i = 0; i < node_count; i++) {
		switch (nodes[i]->node_type) {
			case(HxANodeType::MetaOnly): {
				HxAMetaNode* node = (HxAMetaNode*)nodes[i];
				delete node;
			} break;
			case(HxANodeType::Geometry): {
				HxAGeometryNode* node = (HxAGeometryNode*)nodes[i];
				delete node;
			} break;
			case(HxANodeType::Image): {
				HxAImageNode* node = (HxAImageNode*)nodes[i];
				delete node;
			} break;
			default:
			case(HxANodeType::Unknown): {
				delete nodes[i];
			} break;
		}
	}
	nodes.clear();
}

void HxAFile::init(std::string dir) {
	std::ifstream stream;

	stream.open(dir, std::ios::binary);
	if (stream.fail()) {
		std::cout << "Failed to open file\n";
		stream.close();
		return;
	}

	if (!read_header(&stream)) {
		std::cout << "Failed to read header\n";
		stream.close();
		return;
	}

	for (int i = 0; i < node_count; i++) {
		HxANode* node = create_node(&stream);
		if (node == nullptr) {
			std::cout << "Failed to create Node from HxAFile!\n";
			return;
		}
		node->parse(&stream);
		nodes.push_back(node);
	}

	stream.close();
}

bool HxAFile::read_header(std::ifstream* stream) {
	std::string input;
	if (!read_bytes_to_string(stream, &input, 4)) {
		std::cout << "Failed to read 4 bytes\n";
		return false;
	}
	if (input[0] != 72 || input[1] != 120 || input[2] != 65 || input[3] != 0) {
		std::cout << "Failed to read \'HaX\\0\', first 4 bytes were: " << (int)input[0] << ", " 
			<< (int)input[1] << ", " << (int)input[2] << ", " << (int)input[3] << "\n";
		return false;
	}
	if (!read_bytes_to_string(stream, &input, 1)) {
		std::cout << "Failed to read version number\n";
		return false;
	}
	version_number = input[0];
	if (!read_bytes_to_string(stream, &input, 4)) {
		std::cout << "Failed to read node count\n";
		return false;
	}
	for (int i = 0; i < 4; i++) {
		node_count += (((unsigned)input[i]) & 0xFF) << (8 * i);
	}
	return true;
}