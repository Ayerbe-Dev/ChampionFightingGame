#pragma warning(disable : 4996)
#include "Projectile.h"
#include "RenderManager.h"
#include "BoxConstants.h"
#include "Fighter.h"
#include "FighterAccessor.h"
extern RenderManager g_rendermanager;

void Projectile::superInit() {
	load_stats();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_status_scripts();

	frame = 0.0;
	change_anim("default", 2, 0);
	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::load_model_shader() {
	shader.init("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.link_shader(&shader);
	has_model = get_param_bool("has_model");
	if (has_model) {
		model.load_model(resource_dir + "/model/model.dae");
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
			char buffer[56];
			sprintf(buffer, "Projectile %d's resource directory was incorrectly set!", projectile_kind);
			fighter_accessor->fighter[id]->player_info->crash_reason = buffer;

			fighter_accessor->fighter[id]->crash_to_debug = true;
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
}