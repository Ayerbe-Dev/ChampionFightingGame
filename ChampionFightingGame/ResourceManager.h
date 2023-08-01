#pragma once
#include <iostream>
#include <map>
#include "Model.h"

struct ModelResource {
	ModelData model;
	int user_count;
};

struct TextureResource {
	unsigned int texture;
	int user_count;
};

class ResourceManager {
public:
	ResourceManager(ResourceManager& other) = delete;
	void operator=(const ResourceManager& other) = delete;

	void load_model(std::string dir);
	void init_gl_model(std::string dir);

	ModelData* get_model(std::string dir);
	ModelData* get_model_keep_user_count(std::string dir);
	void unload_model(std::string dir, bool strict = true);
	void unuse_model(std::string dir);

	void load_texture(std::string dir);
	unsigned int get_texture(std::string dir);
	unsigned int get_texture_keep_user_count(std::string dir);
	void unload_texture(std::string dir, bool strict = true);
	void unuse_texture(std::string dir);

	void unload_unused();
	void unload_all();

	static ResourceManager* get_instance();
	void destroy_instance();
private:
	std::map<std::string, ModelResource> model_map;
	std::map<std::string, TextureResource> texture_map;

	ResourceManager();
	static ResourceManager* instance;
};