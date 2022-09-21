#pragma once
#include "Model.h"
#include <iostream>;
#include <map>;

struct ModelResource {
	Model model;
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

	void load_model_resource(std::string dir);
	Model load_model(std::string dir);
	void unload_model_resource(std::string dir, bool strict = true);
	void unload_model(std::string dir);

	void load_texture_resource(std::string dir);
	unsigned int load_texture(std::string dir);
	void unload_texture_resource(std::string dir, bool strict = true);
	void unload_texture(std::string dir);

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