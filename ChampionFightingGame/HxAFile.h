#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "HxAMeta.h"
#include "HxANode.h"

class HxAFile {
public:
	HxAFile();
	HxAFile(std::string dir);
	~HxAFile();
	void init(std::string dir);

private:
	bool read_header(std::ifstream* stream);

	int version_number;
	unsigned node_count;
	std::vector<HxANode*> nodes;
};
