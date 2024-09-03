#include "ResourceManager.h"
#include "ThreadManager.h"
#include "LoadTextures.h"
#include <glew/glew.h>

ResourceManager::ResourceManager() {
	srgb = false;
}

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::get_instance() {
	if (instance == nullptr) {
		instance = new ResourceManager;
	}
	return instance;
}

void ResourceManager::set_srgb(bool srgb) {
	this->srgb = srgb;
}

bool ResourceManager::is_srgb() {
	return srgb;
}

void ResourceManager::load_model(std::string dir) {
	if (!model_map.contains(dir)) {
		model_map[dir] = ModelResource();
		model_map[dir].model.load_model(dir);
		model_map[dir].user_count = 0;
	}
}

void ResourceManager::init_gl_model(std::string dir) {
	if (model_map.contains(dir)) {
		assert(ThreadManager::get_instance()->is_main_thread());
		model_map[dir].model.init_gl_meshes();
	}
}

/// <summary>
/// Loads a model into the ResourceManager if it wasn't already loaded, then increases its user
/// count and returns it. This function is called by other instances of the Model class.
/// </summary>
/// <param name="dir">The location of the model file we load</param>
/// <returns>The ResourceManager's copy of the model.</returns>
ModelData* ResourceManager::get_model(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	model_map[dir].user_count++;
	return &model_map[dir].model;
}

//Same as get_model(), only difference is the user count is not increased for this function.
//This can be used for a variety of reasons, such as having a model persist between scopes easily,
//or accessing a ModelData before it's been finalized.
ModelData* ResourceManager::get_model_keep_user_count(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	return &model_map[dir].model;
}

void ResourceManager::unload_model(std::string dir, bool strict) {
	if (model_map.contains(dir)) {
		if (model_map[dir].user_count == 0 || !strict) {
			model_map[dir].model.unload_model();
			model_map.erase(dir);
		}
	}
}

void ResourceManager::unuse_model(std::string dir) {
	if (model_map.contains(dir)) {
		model_map[dir].user_count--;
	}
}

void ResourceManager::load_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		texture_map[dir] = TextureResource();
		if (dir.ends_with(".gif")) {
			texture_map[dir].texture = loadGLTextures(dir);
		}
		else {
			texture_map[dir].texture.push_back(loadGLTexture(dir));
		}
		texture_map[dir].user_count = 0;
	}
}

unsigned int ResourceManager::get_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture[0];
}

unsigned int ResourceManager::get_texture_keep_user_count(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	return texture_map[dir].texture[0];
}

std::vector<unsigned int> ResourceManager::get_textures(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture;
}

std::vector<unsigned int> ResourceManager::get_textures_keep_user_count(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	return texture_map[dir].texture;
}

void ResourceManager::unload_texture(std::string dir, bool strict) {
	if (texture_map.contains(dir)) {
		if (texture_map[dir].user_count == 0 || !strict) {
			for (int i = 0, max = texture_map[dir].texture.size(); i < max; i++) {
				glDeleteTextures(1, &texture_map[dir].texture[i]);
			}
			texture_map.erase(dir);
		}
	}
}

void ResourceManager::unuse_texture(std::string dir) {
	if (texture_map.contains(dir)) {
		texture_map[dir].user_count--;
	}
}

void ResourceManager::unload_unused() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		if (it->second.user_count == 0) {
			it->second.model.unload_model();
			it = model_map.erase(it);
		}
	}
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		if (it->second.user_count == 0) {
			for (int i = 0, max = it->second.texture.size(); i < max; i++) {
				glDeleteTextures(1, &it->second.texture[i]);
			}
			texture_map.erase(it->first);
		}
	}
}

void ResourceManager::unload_all() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		it->second.model.unload_model();
	}
	model_map.clear();
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		for (int i = 0, max = it->second.texture.size(); i < max; i++) {
			glDeleteTextures(1, &it->second.texture[i]);
		}
	}
	texture_map.clear();
}

void ResourceManager::destroy_instance() {
	unload_all();
	if (instance != nullptr) {
		delete instance;
	}
}