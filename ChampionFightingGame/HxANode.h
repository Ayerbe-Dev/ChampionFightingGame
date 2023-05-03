#pragma once
#include <vector>
#include <fstream>
#include "HxALayer.h"
#include "HxAMeta.h"
#include "HxAEnums.h"

class HxANode {
public:
	HxANode();
	~HxANode();
	void parse(std::ifstream* stream);

	HxANodeType node_type;
	unsigned metadata_count;
	std::vector<HxAMetaData> metadata;
};

class HxAMetaNode : public HxANode {
public:
	HxAMetaNode();
	~HxAMetaNode();
	void parse(std::ifstream* stream);
};

class HxAGeometryNode : public HxANode {
public:
	HxAGeometryNode();
	~HxAGeometryNode();
	void parse(std::ifstream* stream);

	unsigned vertex_count;
	HxALayerStack vertex_stack;

	unsigned edge_corner_count;
	HxALayerStack corner_stack;
	HxALayerStack edge_stack;
	
	unsigned face_count;
	HxALayerStack face_stack;
};

class HxAImageNode : public HxANode {
public:
	HxAImageNode();
	~HxAImageNode();
	void parse(std::ifstream* stream);
};

HxANode* create_node(std::ifstream* stream);