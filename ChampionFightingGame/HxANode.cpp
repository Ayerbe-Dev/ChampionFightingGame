#include "HxANode.h"
#include "ReadBytes.h"

HxANode::HxANode() {
	node_type = HxANodeType::Unknown;
	metadata_count = 0;
}

HxANode::~HxANode() {
	metadata.clear();
}

void HxANode::parse(std::ifstream* stream) {
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 4)) {
		std::cout << "Failed to read Metadata Count for HxANode!\n";
		return;
	}
	for (size_t i = 0; i < 4; i++) {
		metadata_count += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
	}
	metadata.reserve(metadata_count);
	for (size_t _ = 0; _ < metadata_count; _++) {
		metadata.emplace_back();
		metadata.back().parse(stream);
	}
}

HxAMetaNode::HxAMetaNode() {
	node_type = HxANodeType::MetaOnly;
}

HxAMetaNode::~HxAMetaNode() {

}

void HxAMetaNode::parse(std::ifstream* stream) {
	HxANode::parse(stream);
}

HxAGeometryNode::HxAGeometryNode() {
	node_type = HxANodeType::Geometry;
	vertex_count = 0;
	edge_corner_count = 0;
	face_count = 0;
}

HxAGeometryNode::~HxAGeometryNode() {
	
}

void HxAGeometryNode::parse(std::ifstream* stream) {
	HxANode::parse(stream);
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 4)) {
		std::cout << "Failed to read Vertex Count!\n";
		return;
	}
	for (size_t i = 0; i < 4; i++) {
		vertex_count += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
	}
	vertex_stack.parse(stream, vertex_count);

	if (!read_bytes_to_string(stream, &bytes, 4)) {
		std::cout << "Failed to read Edge Corner Count!\n";
		return;
	}
	for (size_t i = 0; i < 4; i++) {
		edge_corner_count += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
	}
	edge_stack.parse(stream, edge_corner_count);
	corner_stack.parse(stream, edge_corner_count);

	if (!read_bytes_to_string(stream, &bytes, 4)) {
		std::cout << "Failed to read Face Count!\n";
		return;
	}
	for (size_t i = 0; i < 4; i++) {
		face_count += (((unsigned)bytes[i]) & 0xFF) << (8 * i);
	}
	face_stack.parse(stream, face_count);
}

HxAImageNode::HxAImageNode() {
	node_type = HxANodeType::Image;
}

HxAImageNode::~HxAImageNode() {

}

void HxAImageNode::parse(std::ifstream* stream) {
	HxANode::parse(stream);
}

HxANode* create_node(std::ifstream* stream) {
	HxANode* ret;
	std::string bytes;
	if (!read_bytes_to_string(stream, &bytes, 1)) {
		std::cout << "Failed to read Node Type!\n";
		return nullptr;
	}
	HxANodeType type = (HxANodeType)bytes[0];
	switch (type) {
	case(HxANodeType::MetaOnly): {
		ret = new HxAMetaNode;
	} break;
	case(HxANodeType::Geometry): {
		ret = new HxAGeometryNode;
	} break;
	case(HxANodeType::Image): {
		ret = new HxAImageNode;
	} break;
	default:
	case(HxANodeType::Unknown): {
		ret = new HxANode;
	} break;
	}
	return ret;
}