#include "GameObject.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "GameManager.h"

void GameObject::load_model(std::string resource_dir, std::string texture_dir) {
	this->resource_dir = resource_dir;
	model.load_model_instance(resource_dir);
	if (texture_dir != "") {
		model.load_textures(texture_dir);
	}
}

void GameObject::load_used_model(std::string resource_dir, std::string texture_dir) {
	this->resource_dir = resource_dir;
	model.load_used_model_instance(resource_dir);
	if (texture_dir != "") {
		model.load_textures(texture_dir);
	}
}

void GameObject::init_shader() {
	unsigned int flags = 0;
	if (model.has_skeleton()) {
		flags |= SHADER_FEAT_HAS_BONES;
	}
	shader = shader_manager->get_shader("model", "model", "model", flags);
	shadow_shader = shader_manager->get_shader("shadow", "shadow", "", flags);
	outline_shader = shader_manager->get_shader("3d_outline", "3d_outline", "3d_outline", flags);
	shader->use();
	shader->set_int("shadow_map", 0);
	shader->set_int("material.diffuse", 1);
	shader->set_int("material.specular", 2);
}

void GameObject::load_anim_table(std::string anim_dir) {
	try {
		anim_table.load_anlst(anim_dir, model.get_skeleton());
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log(anim_dir + " is not a valid anim directory!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void GameObject::load_anim_table_unloaded_model(std::string anim_dir, std::string model_filename) {
	if (!model.is_loaded()) {
		ModelData* model_data = ResourceManager::get_instance()->get_model_keep_user_count(model_filename);
		try {
			anim_table.load_anlst(anim_dir, model_data->skeleton);
		}
		catch (std::runtime_error err) {
			if (err.what() == "Anim List Missing") {
				GameManager::get_instance()->add_crash_log(anim_dir + " is not a valid anim directory!");
			}
			else {
				std::cout << err.what() << "\n";
			}
		}
	}
	else {
		load_anim_table(anim_dir);
	}
}

void GameObject::load_anim_single(std::string name, std::string anim_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp) {
	anim_table.load_anim_single(name, anim_filename, end_frame, flag_move, flag_no_hitlag_interp, model.get_skeleton());
}

void GameObject::load_anim_single_unloaded_model(std::string name, std::string anim_filename, std::string model_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp) {
	if (!model.is_loaded()) {
		ModelData* model_data = ResourceManager::get_instance()->get_model_keep_user_count(model_filename);
		anim_table.load_anim_single(name, anim_filename, end_frame, flag_move, flag_no_hitlag_interp, model_data->skeleton);
	}
	else {
		load_anim_single(name, anim_filename, end_frame, flag_move, flag_no_hitlag_interp);
	}
}