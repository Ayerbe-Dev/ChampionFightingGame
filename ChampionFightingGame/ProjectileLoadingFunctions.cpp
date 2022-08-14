#pragma warning(disable : 4996)
#include "Projectile.h"
#include "RenderManager.h"
#include "BoxConstants.h"
#include "Fighter.h"
#include "BattleObjectManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"

void Projectile::super_init() {
	sound_manager = SoundManager::get_instance();
	sound_manager->add_sound_player(id);
	effect_manager = EffectManager::get_instance();
	effect_manager->add_effect_caster(id);

	load_stats();
	load_params();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_status_scripts();
	load_move_scripts();

	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::load_model_shader() {
	scale = glm::vec3(0.05 * get_local_param_float("model_scale"));
	has_model = get_local_param_bool("has_model");
	if (has_model) {
		model.load_model(resource_dir + "/model/model.dae");
		model.load_textures();
		shader.init("vertex_main.glsl", "fragment_main.glsl", "geometry_main.glsl");
		shadow_shader.init("vertex_shadow.glsl", "fragment_shadow.glsl");
		outline_shader.init("vertex_outline.glsl", "fragment_outline.glsl", "geometry_outline.glsl");
		shader.use();
		shader.set_int("shadow_map", 0);
		shader.set_int("material.diffuse", 1);
		shader.set_int("material.specular", 2);
		shader.set_float("brightness_mul", 1.0);
		shader.set_bool("has_skeleton", model.has_skeleton);
		shadow_shader.use();
		shadow_shader.set_bool("has_skeleton", model.has_skeleton);
		outline_shader.use();
		outline_shader.set_bool("has_skeleton", model.has_skeleton);
		render_manager->link_shader(&shader);
		render_manager->link_shader(&shadow_shader);
		render_manager->link_shader(&outline_shader);
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
		anim_table.load_animations_no_faf(resource_dir, model_ptr);
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

void Projectile::load_status_scripts() {
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