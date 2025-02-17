#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "Model.h"

struct ModelResource {
	ModelData model;
	int user_count;
};

struct TextureResource {
	std::vector<unsigned int> texture;
	int user_count;
};

class ResourceManager {
public:
	ResourceManager(ResourceManager& other) = delete;
	void operator=(const ResourceManager& other) = delete;

	void set_srgb(bool srgb);
	bool is_srgb() const;

	void load_model(std::string dir);
	void init_gl_model(std::string dir);
	ModelData* get_model(std::string dir);
	ModelData* get_model_keep_user_count(std::string dir);
	void unload_model(std::string dir, bool strict = true);
	void unuse_model(std::string dir);

	void load_texture(std::string dir);
	unsigned int get_texture(std::string dir);
	unsigned int get_texture_keep_user_count(std::string dir);
	std::vector<unsigned int> get_textures(std::string dir); //gif version
	std::vector<unsigned int> get_textures_keep_user_count(std::string dir);
	void unload_texture(std::string dir, bool strict = true);
	void unuse_texture(std::string dir);

	void unload_unused();
	void unload_all();

	void store_const_copy_addr(unsigned int texture_id, void* addr);
	bool is_tex_const_copied(unsigned int texture_id);
	bool is_tex_const_copied(unsigned int texture_id, void* addr);
	int get_num_const_copied_textures();

	static ResourceManager* get_instance();
	void destroy_instance();
private:
	std::map<std::string, ModelResource> model_map;
	std::map<std::string, TextureResource> texture_map;
	std::map<unsigned int, std::set<void*>> const_copy_addr;
	bool srgb;

	ResourceManager();
	static ResourceManager* instance;
};