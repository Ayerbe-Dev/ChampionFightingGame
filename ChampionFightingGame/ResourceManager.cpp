#include "ResourceManager.h"
#include "GameManager.h"
#include "GLEW Helpers.h"

ResourceManager::ResourceManager() {

}

ResourceManager* ResourceManager::instance = nullptr;
ResourceManager* ResourceManager::get_instance() {
	if (instance == nullptr) {
		instance = new ResourceManager;
	}
	return instance;
}

void ResourceManager::load_model_resource(std::string dir) {
	if (!model_map.contains(dir)) {
		model_map[dir] = ModelResource();
		model_map[dir].model.init(dir);
		model_map[dir].user_count = 0;
	}
}

Model& ResourceManager::load_model(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model_resource(dir);
	}
	model_map[dir].user_count++;
	return model_map[dir].model;
}

void ResourceManager::unload_model_resource(std::string dir, bool strict) {
	if (model_map.contains(dir)) {
		if (model_map[dir].user_count == 0 || !strict) {
			model_map[dir].model.destroy();
			model_map.erase(dir);
		}
	}
}

void ResourceManager::unload_model(std::string dir) {
	if (model_map.contains(dir)) {
		model_map[dir].user_count--;
	}
}

void ResourceManager::load_texture_resource(std::string dir) {
	if (!texture_map.contains(dir)) {
		texture_map[dir] = TextureResource();
		texture_map[dir].texture = loadGLTexture(dir);
		texture_map[dir].user_count = 0;
	}
}

unsigned int ResourceManager::load_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture_resource(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture;
}

void ResourceManager::unload_texture_resource(std::string dir, bool strict) {
	if (texture_map.contains(dir)) {
		if (texture_map[dir].user_count == 0 || !strict) {
			glDeleteTextures(1, &texture_map[dir].texture);
			texture_map.erase(dir);
		}
	}
}

void ResourceManager::unload_texture(std::string dir) {
	if (texture_map.contains(dir)) {
		texture_map[dir].user_count--;
	}
}

void ResourceManager::unload_unused() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		if (it->second.user_count == 0) {
			it->second.model.destroy();
		}
	}
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		if (it->second.user_count == 0) {
			glDeleteTextures(1, &it->second.texture);
		}
	}
}

void ResourceManager::unload_all() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		it->second.model.destroy();
	}
	model_map.clear();
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		glDeleteTextures(1, &it->second.texture);
	}
	texture_map.clear();
}

void ResourceManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}