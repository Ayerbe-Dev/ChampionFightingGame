#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <ostream>
#include <cstdint>

#define HXA_NAME_MAX_LENGTH 256

enum HXANodeType {
	HXA_NT_META_ONLY,
	HXA_NT_GEOMETRY,
	HXA_NT_IMAGE,
	HXA_NT_COUNT
};



enum HXALayerDataType {
	HXA_LDT_UINT8,
	HXA_LDT_INT32, 
	HXA_LDT_FLOAT, 
	HXA_LDT_DOUBLE, 
	HXA_LDT_COUNT
};

enum HXAImageType {
	HXA_IT_CUBE_IMAGE,
	HXA_IT_1D_IMAGE,
	HXA_IT_2D_IMAGE,
	HXA_IT_3D_IMAGE,
};

enum HXAMetaDataType{
	HXA_MDT_INT64,
	HXA_MDT_DOUBLE,
	HXA_MDT_NODE,
	HXA_MDT_TEXT,
	HXA_MDT_BINARY,
	HXA_MDT_META,
	HXA_MDT_COUNT
};

struct HXAMeta {
	std::string name;
	HXAMetaDataType type; 
	uint32_t array_length;
	union HXAMetaContents{
		uint64_t* int64_value;
		double* double_value;
		uint32_t* node_value;
		char* text_value;
		unsigned char* bin_value;
		//std::vector<HXAMeta> array_of_meta;
		void* array_of_meta;
	} value;
};

struct HXALayer {
	std::string name;
	uint8_t components; //how many data points in a vector ie, a vec 2 this would be 2
	HXALayerDataType type; // Stored in the file as a uint8.
	union {
		uint8_t* uint8_data;
		int32_t* int32_data;
		float* float_data;
		double* double_data;
	}data;
};

struct HXALayerStack {
	uint32_t layer_count;
	//std::vector<HXALayer> layers;
	HXALayer* layers;
};

struct HXANode {
	HXANodeType type;
	uint32_t meta_data_count;
	std::vector<HXAMeta> meta_data;

	union{
		struct{
			uint32_t vertex_count;
			HXALayerStack vertex_stack;

			uint32_t edge_corner_count;
			HXALayerStack corner_stack;
			HXALayerStack edge_stack;
			
			uint32_t face_count;
			HXALayerStack face_stack;

		}geometry;
		struct{
			HXAImageType type; 
			uint32_t resolution[3]; 
			HXALayerStack image_stack;
		}image;
	}content;
};

struct HXAFile {
	uint8_t version;
	uint32_t node_count;
	std::vector<HXANode> nodes;
};

std::vector<char> hxaCopyFile(const char* path);

HXAFile hxaLoad(const char* path);