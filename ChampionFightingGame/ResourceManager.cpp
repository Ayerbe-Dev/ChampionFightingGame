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
		model_map[dir].model.init(dir);
		model_map[dir].user_count = 0;
	}
}

/// <summary>
/// Loads a model into the ResourceManager if it wasn't already loaded, then increases its user
/// count and returns it. This function is called by other instances of the Model class.
/// </summary>
/// <param name="dir">The location of the model file we load</param>
/// <returns>The ResourceManager's copy of the model.</returns>
Model& ResourceManager::get_model(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	model_map[dir].user_count++;
	return model_map[dir].model;
}

/// <summary>
/// Loads a model into the ResourceManager if it wasn't already loaded, then returns it. 
/// Unlike get_model(), this function only increases the user count if it's currently 0. Essentially
/// it's designed to let us maintain a model between different scopes without incorrectly increasing
/// the user count.
/// This function is called by other instances of the Model class.
/// </summary>
/// <param name="dir">The location of the model file we load</param>
/// <returns>The ResourceManager's copy of the model.</returns>
Model& ResourceManager::get_used_model(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	if (model_map[dir].user_count == 0) {
		model_map[dir].user_count++;
	}
	return model_map[dir].model;
}

void ResourceManager::unload_model(std::string dir, bool strict) {
	if (model_map.contains(dir)) {
		if (model_map[dir].user_count == 0 || !strict) {
			model_map[dir].model.destroy();
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
		texture_map[dir].texture = loadGLTexture(dir);
		texture_map[dir].user_count = 0;
	}
}

unsigned int ResourceManager::get_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
		std::cout << "Loading texture " << dir << " with ID " << texture_map[dir].texture << "\n";
	}
	else {
		std::cout << "Using texture " << dir << " with ID " << texture_map[dir].texture << "\n";
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture;
}

unsigned int ResourceManager::get_used_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
		std::cout << "Loading texture " << dir << " with ID " << texture_map[dir].texture << "\n";
	}
	else {
		std::cout << "Using texture " << dir << " with ID " << texture_map[dir].texture << "\n";
	}
	if (texture_map[dir].user_count == 0) {
		texture_map[dir].user_count++;
	}
	return texture_map[dir].texture;
}

void ResourceManager::unload_texture(std::string dir, bool strict) {
	if (texture_map.contains(dir)) {
		if (texture_map[dir].user_count == 0 || !strict) {
			std::cout << "Unloading texture " << dir << " with ID " << texture_map[dir].texture << "\n";
			glDeleteTextures(1, &texture_map[dir].texture);
			texture_map.erase(dir);
		}
		else {
			std::cout << "Keeping texture " << dir << " loaded because it still has " 
				<< texture_map[dir].user_count << " users\n";
		}
	}
}

void ResourceManager::unuse_texture(std::string dir) {
	if (texture_map.contains(dir)) {
		std::cout << "Unusing texture " << dir << " with ID " << texture_map[dir].texture << "\n";
		texture_map[dir].user_count--;
	}
}

void ResourceManager::unload_unused() {
	for (std::map<std::string, ModelResource>::iterator it = model_map.begin(); it != model_map.end(); it++) {
		if (it->second.user_count == 0) {
			it->second.model.destroy();
			it = model_map.erase(it);
		}
	}
	for (std::map<std::string, TextureResource>::iterator it = texture_map.begin(); it != texture_map.end(); it++) {
		if (it->second.user_count == 0) {
			std::cout << "Unloading unused texture " << it->first << " with ID " << it->second.texture << "\n";
			glDeleteTextures(1, &it->second.texture);
			texture_map.erase(it->first);
		}
		else {
			std::cout << "Texture " << it->first << " still has " << it->second.user_count << " active users, keeping ID "
				<< it->second.texture << "\n";
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
	unload_all();
	if (instance != nullptr) {
		delete instance;
	}
}