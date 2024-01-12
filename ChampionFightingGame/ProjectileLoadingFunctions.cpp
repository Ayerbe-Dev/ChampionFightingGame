#pragma warning(disable : 4996)
#include "Projectile.h"
#include "RenderManager.h"
#include "Fighter.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include <fstream>
#include "Sndlst.h"

void Projectile::load_projectile() {
	effect_manager->add_effect_caster(id);

	stage = object_manager->stage;

	load_stats();
	load_params();
	load_model_shader();
	load_collision_boxes();
	load_anim_list();
	load_projectile_unique_status_scripts();
	load_projectile_status_scripts();
	load_move_scripts();
	load_sound_list();

	change_status(PROJECTILE_STATUS_NONE, false);
}

void Projectile::load_sound_list() {
	std::ifstream sound_stream;
	std::string name;
	std::string file;
	sound_stream.open(resource_dir + "/vc/vc_list.sndlst");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s vc directory was incorrectly set!");
	}
	else {
		while (!sound_stream.eof()) {
			parse_sndlst_entry(sound_stream, name, file);
			if (name == "") {
				break;
			}
			sound_player.load_sound(name, resource_dir + "/vc/" + file);
		}
		sound_stream.close();
	}
	sound_stream.open(resource_dir + "/se/se_list.sndlst");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s se directory was incorrectly set!");
	}
	else {
		while (!sound_stream.eof()) {
			parse_sndlst_entry(sound_stream, name, file);
			if (name == "") {
				break;
			}
			sound_player.load_sound(name, resource_dir + "/se/" + file);

		}
		sound_stream.close();
	}
}

void Projectile::load_model_shader() {
	scale = glm::vec3(get_local_param_float("model_scale"));
	has_model = get_local_param_bool("has_model");
	if (has_model) {
		model.load_model_instance(resource_dir + "/model/model.dae");
		model.load_textures();
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
}

void Projectile::load_anim_list() {
	try {
		if (has_model) {
			anim_table.load_anlst(resource_dir + "/anims", model.get_skeleton());
		}
		else {
			anim_table.load_anlst(resource_dir + "/anims", Skeleton());
		}
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Projectile " + std::to_string(projectile_kind) + "\'s animation directory has no anim list!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void Projectile::set_default_vars() {

}

void Projectile::load_collision_boxes() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].init(this);
		hurtboxes[i].init(this);
		grabboxes[i].init(this);
		pushboxes[i].init(this);
	}
	blockbox.init(this);
}