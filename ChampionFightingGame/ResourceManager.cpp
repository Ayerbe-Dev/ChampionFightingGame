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

/// <summary>
/// Sets the current SRGB mode for loading textures.
/// </summary>
/// <param name="srgb">The SRGB mode.</param>
void ResourceManager::set_srgb(bool srgb) {
	this->srgb = srgb;
}

/// <summary>
/// Returns the current SRGB mode for loading textures.
/// </summary>
/// <returns>True if textures should be loaded in SRGB mode, false otherwise.</returns>
bool ResourceManager::is_srgb() const {
	return srgb;
}

/// <summary>
/// Loads the model at dir and sets its user count to 0.
/// </summary>
/// <param name="dir">The file address of the model to load.</param>
void ResourceManager::load_model(std::string dir) {
	if (!model_map.contains(dir)) {
		model_map[dir] = ModelResource();
		model_map[dir].model.load_model(dir);
		model_map[dir].user_count = 0;
	}
}

/// <summary>
/// Runs all of the OpenGL calls in the initialization process for the model at dir. This function
/// should only be called from the main thread.
/// </summary>
/// <param name="dir">The directory of a model which has been initialized but has no VAO/VBO, i.e. a model loaded in a thread.</param>
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

/// <summary>
/// Loads a model into the ResourceManager if it wasn't already loaded, then returns it without
/// increasing its user count.This can be used for a variety of reasons, such as having a model persist between scopes easily,
//	or accessing a ModelData before it's been finalized.
/// </summary>
/// <param name="dir">The location of the model file we load</param>
/// <returns>The ResourceManager's copy of the model.</returns>
ModelData* ResourceManager::get_model_keep_user_count(std::string dir) {
	if (!model_map.contains(dir)) {
		load_model(dir);
	}
	return &model_map[dir].model;
}

/// <summary>
/// Unloads the model at dir if either it isn't actively being used or strict is set to false.
/// </summary>
/// <param name="dir">The path to the model to unload.</param>
/// <param name="strict">True if the model must have a user count of 0, false otherwise.</param>
void ResourceManager::unload_model(std::string dir, bool strict) {
	if (model_map.contains(dir)) {
		if (model_map[dir].user_count == 0 || !strict) {
			model_map[dir].model.unload_model();
			model_map.erase(dir);
		}
	}
}

/// <summary>
/// Decreases the user count of the model at dir.
/// </summary>
/// <param name="dir">The model to decrease the user count for.</param>
void ResourceManager::unuse_model(std::string dir) {
	if (model_map.contains(dir)) {
		model_map[dir].user_count--;
	}
}

/// <summary>
/// Loads the texture file at dir into the ResourceManager and sets its user count to 0.
/// </summary>
/// <param name="dir">The path to the file to load.</param>
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

/// <summary>
/// Loads a texture file if it isn't already loaded, then increases the user count and returns the 
/// OpenGL texture ID. If the texture at dir contains multiple frames, only the first frame is returned.
/// </summary>
/// <param name="dir">The path to the texture to return.</param>
/// <returns>The GL Texture ID representing the texture.</returns>
unsigned int ResourceManager::get_texture(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture[0];
}

/// <summary>
/// Loads a texture file if it isn't already loaded, then returns the OpenGL texture ID without 
/// increasing the user count. If the texture at dir contains multiple frames, only the first frame 
/// is returned.
/// </summary>
/// <param name="dir">The path to the texture to return.</param>
/// <returns>The GL Texture ID representing the texture.</returns>
unsigned int ResourceManager::get_texture_keep_user_count(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	return texture_map[dir].texture[0];
}

/// <summary>
/// Loads a texture file if it isn't already loaded, then increases the user count and returns the 
/// vector of OpenGL texture IDs for each frame of the image.
/// </summary>
/// <param name="dir">The path to the texture to return.</param>
/// <returns>A vector of the GL Texture IDs representing the texture.</returns>
std::vector<unsigned int> ResourceManager::get_textures(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	texture_map[dir].user_count++;
	return texture_map[dir].texture;
}

/// <summary>
/// Loads a texture file if it isn't already loaded, then returns the vector of OpenGL texture IDs for 
/// each frame of the image without increasing the user count.
/// </summary>
/// <param name="dir">The path to the texture to return.</param>
/// <returns>A vector of the GL Texture IDs representing the texture.</returns>
std::vector<unsigned int> ResourceManager::get_textures_keep_user_count(std::string dir) {
	if (!texture_map.contains(dir)) {
		load_texture(dir);
	}
	return texture_map[dir].texture;
}

/// <summary>
/// Unloads the GL texture(s) at dir if either it isn't actively being used or strict is set to false.
/// </summary>
/// <param name="dir">The path to the texture to unload.</param>
/// <param name="strict">True if the texture must have a user count of 0, false otherwise.</param>
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

/// <summary>
/// Decreases the user count for the texture file at dir.
/// </summary>
/// <param name="dir">The texture file for which to decrease the user count.</param>
void ResourceManager::unuse_texture(std::string dir) {
	if (texture_map.contains(dir)) {
		texture_map[dir].user_count--;
	}
}

/// <summary>
/// Unloads all models and textures which currently have a user count of 0.
/// </summary>
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

/// <summary>
/// Unloads all resources, even ones which are still actively being used.
/// </summary>
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

/// <summary>
/// Stores the address and (first) GL Texture ID of a ScreenTexture, ScreenText, WorldTexture or 
/// WorldText which is currently being used to const copy construct another one of itself. This 
/// information is required in order to prevent the VAO, VBO and GL Texture ID(s) from being destroyed
/// when 'this' goes out of scope, since we can't mark 'this' as unloaded when it is considered const.
/// </summary>
/// <param name="tex">The (first) GL Texture ID, which will be passed between instances of this object
/// during the construction process.</param>
/// <param name="addr">The address of this specific instance.</param>
void ResourceManager::store_const_copy_addr(unsigned int tex, void* addr) {
	const_copy_addr[tex].insert(addr);
}

/// <summary>
/// Returns whether or not the GL Texture ID tex is currently stored. If it is, that implies that 'this'
/// is going to go out of scope by the time control returns to the function that originally loaded this
/// texture, meaning any copy constructors which are passing this Texture ID along should behave like
/// move constructors instead, i.e. Have the copy constructed object inherit the load status and mark
/// the original as unloaded so it can't be destroyed on destruction rather than mark the copy constructed
/// object as unloaded so it can't accidentally unload the original while it's still active.
/// </summary>
/// <param name="tex">The GL Texture ID to check.</param>
/// <returns>Whether or not a const copy constructor is still in scope.</returns>
bool ResourceManager::is_tex_const_copied(unsigned int tex) {
	return const_copy_addr.contains(tex);
}

/// <summary>
/// Checks whether or not the GL Texture ID tex is currently being stored, but also checks if the
/// specific address addr is being stored. If the first check passes, this returns true no matter what,
/// indicating that destructors for the calling object should not unload any GL Texture IDs, VAOs, or 
/// VBOs. If the second check also passes, addr will stop being stored. If this was the last stored 
/// instance of tex, destructors will be allowed to function normally after the one for 'this' returns.
/// </summary>
/// <param name="tex">The GL Texture ID to check.</param>
/// <param name="addr">The address of the object to check.</param>
/// <returns>Whether or not a const copy constructor was still in scope before this call.</returns>
bool ResourceManager::is_tex_const_copied(unsigned int tex, void* addr) {
	if (const_copy_addr.contains(tex)) {
		if (const_copy_addr[tex].contains(addr)) {
			const_copy_addr[tex].erase(addr);
			if (const_copy_addr[tex].empty()) {
				const_copy_addr.erase(tex);
			}
		}
		return true;
	}
	return false;
}

/// <summary>
/// Gets the current number of const copy constructors still in scope. Useful for debugging.
/// </summary>
/// <returns>The number of GL Texture IDs which are currently not allowed to clear.</returns>
int ResourceManager::get_num_const_copied_textures() {
	return const_copy_addr.size();
}

void ResourceManager::destroy_instance() {
	unload_all();
	if (instance != nullptr) {
		delete instance;
	}
}