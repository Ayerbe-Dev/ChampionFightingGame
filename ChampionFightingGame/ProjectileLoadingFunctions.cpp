#pragma warning(disable : 4996)
#include "Projectile.h"
#include "RenderManager.h"
#include "BoxConstants.h"
#include "Fighter.h"
#include "BattleObjectManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include <fstream>

void Projectile::init() {
	sound_manager->register_game_object(this);
	effect_manager->add_effect_caster(id);

	load_stats();
	load_params();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_projectile_unique_status_scripts();
	load_projectile_status_scripts();
	load_move_scripts();

	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::load_sound_list() {
	std::ifstream sound_stream;
	std::string sound_name;
	std::string sound_file;
	float volume_mod;
	sound_stream.open(resource_dir + "/vc/vc_list.yml");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s vc directory was incorrectly set!");
	}
	else {
		while (sound_stream >> sound_name) {
			sound_stream >> sound_file >> volume_mod;
			sound_manager->load_sound(sound_name, resource_dir + "/vc/" + sound_file, volume_mod);
		}
		sound_stream.close();
	}
	sound_stream.open(resource_dir + "/se/se_list.yml");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s se directory was incorrectly set!");
	}
	else {
		while (sound_stream >> sound_name) {
			sound_stream >> sound_file >> volume_mod;
			sound_manager->load_sound(sound_name, resource_dir + "/se/" + sound_file, volume_mod);
		}
		sound_stream.close();
	}
}

void Projectile::load_model_shader() {
	scale = glm::vec3(get_local_param_float("model_scale"));
	has_model = get_local_param_bool("has_model");
	if (has_model) {
		model.load_model(resource_dir + "/model/model.dae");
		model.load_textures();
		unsigned int flags = 0;
		if (model.has_skeleton) {
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
}

void Projectile::load_anim_list() {
	Model* model_ptr;
	if (has_model) {
		model_ptr = &model;
	}
	else {
		model_ptr = nullptr;
	}
	try {
		anim_table.load_anlst(resource_dir, model_ptr);
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s resource directory was incorrectly set!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void Projectile::load_projectile_status_scripts() {
	status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::status_default;
	enter_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::enter_status_default;
	exit_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::exit_status_default;

	status_script[PROJECTILE_STATUS_MOVE] = &Projectile::status_move;
	enter_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::enter_status_move;
	exit_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::exit_status_move;

	status_script[PROJECTILE_STATUS_HIT] = &Projectile::status_hit;
	enter_status_script[PROJECTILE_STATUS_HIT] = &Projectile::enter_status_hit;
	exit_status_script[PROJECTILE_STATUS_HIT] = &Projectile::exit_status_hit;
}

void Projectile::set_default_vars() {

}

void Projectile::init_boxes() {
	for (int i = 0; i < HITBOX_COUNT_MAX; i++) {
		hitboxes[i].init(this);
	}
	blockbox.init(this);
}