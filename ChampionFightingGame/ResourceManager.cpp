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

void ResourceManager::load_model(std::string dir) {
	if (!model_map.contains(dir)) {
		model_map[dir] = ModelResource();
		model_map[dir].model.load_model_resource(dir);
		model_map[dir].user_count = 0;
	}
}

void ResourceManager::unload_model(std::string dir, bool strict) {
	if (model_map.contains(dir)) {
		if (model_map[dir].user_count == 0) {
			model_map[dir].model.unload_model();
			model_map.erase(dir);
		}
		else if (strict) {
			GameManager::get_instance()->add_crash_log("ERROR: Cannot unload model " + dir 
				+ " while " + std::to_string(model_map[dir].user_count) + " instances are still in use");
		}
		else {
			std::cout << "WARNING: Model " << dir << " unloaded while " << model_map[dir].user_count << " instances are still in use.\n";
		}
	}
}

void ResourceManager::load_model_instance(std::string dir, Model* ret) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	model_map[dir].user_count++;
	model_map[dir].model.copy(ret);
}

void ResourceManager::unload_model_instance(std::string dir) {
	if (model_map.contains(dir)) {
		model_map[dir].user_count--;
	}
}

void ResourceManager::unload_all_models() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		it->second.model.destroy();
	}
	model_map.clear();
}

void ResourceManager::load_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		texture_map[dir] = TextureResource();
		texture_map[dir].texture = loadGLTexture(dir);
		texture_map[dir].user_count = 0;
	}
}

void ResourceManager::unload_texture(std::string dir, bool strict) {
	if (texture_map.contains(dir)) {
		if (texture_map[dir].user_count == 0) {
			glDeleteTextures(1, &texture_map[dir].texture);
			texture_map.erase(dir);
		}
		else if (strict) {
			GameManager::get_instance()->add_crash_log("ERROR: Cannot unload texture " + dir
				+ " while " + std::to_string(texture_map[dir].user_count) + " instances are still in use");
		}
		else {
			std::cout << "WARNING: Texture " << dir << " unloaded while " << texture_map[dir].user_count << " instances are still in use.\n";
		}
	}
}

unsigned int ResourceManager::load_texture_instance(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture;
}

void ResourceManager::unload_texture_instance(std::string dir) {
	if (texture_map.contains(dir)) {
		texture_map[dir].user_count--;
	}
}

void ResourceManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}

void ResourceManager::unload_all_textures() {
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		glDeleteTextures(1, &it->second.texture);
	}
	texture_map.clear();
}