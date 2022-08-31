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

	void load_model(std::string dir); //Actually loads a model
	void load_model_instance(std::string dir, Model* ret); //Copies a loaded model onto the return
	void unload_model(std::string dir, bool strict = true); //Unloads a model, and either crashes to Debug
	//or warns in cout if anything is using it
	void unload_model_instance(std::string dir); //Decreases user count for a model, called by 
	//Model.unload_model_resource();
	void unload_all_models(); //Clears the entire model map

	void load_texture(std::string dir); //Loads texture
	unsigned int load_texture_instance(std::string dir); //"Copies" a loaded texture and returns it
	//(thanks to OpenGL, all this really needs to do is return a texture ID)
	void unload_texture(std::string dir, bool strict = true); //Unloads a texture, see above
	void unload_texture_instance(std::string dir); //Wild guess
	void unload_all_textures(); //i delete da texture

	static ResourceManager* get_instance();
	void destroy_instance();
private:
	std::map<std::string, ModelResource> model_map;
	std::map<std::string, TextureResource> texture_map;

	ResourceManager();
	static ResourceManager* instance;
};