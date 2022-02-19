#include "Projectile.h"
#include "RenderManager.h"
extern RenderManager g_rendermanager;

void Projectile::superInit() {
	load_stats();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_status_scripts();

	change_anim("default", 2, 0);
	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::load_model_shader() {
	shader.init("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&shader);
	has_model = get_param_bool("has_model");
	if (has_model) {
		model.load_model(resource_dir + "/model/model.dae");
	}
}

void Projectile::load_anim_list() {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");
	Model* model_ptr;
	if (has_model) {
		model_ptr = &model;
	}
	else {
		model_ptr = nullptr;
	}

	if (anim_list.fail()) {
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string name;
	string path;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path;
		name = ymlChopString(name);
		Animation anim(name, path, model_ptr);
		animation_table[i] = anim;
	}
	anim_list.close();
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

void Projectile::load_stats() {
	ifstream stats_table;
	stats_table.open(resource_dir + "/param/stats.yml");

	if (stats_table.fail()) {
		cerr << "Could not open stats table!" << endl;
		exit(1);
	}

	string stat;
	for (int i = 0; stats_table >> stat; i++) {
		stat_table[i].stat = stat;
		stats_table >> stat_table[i].type;
		switch (stat_table[i].type) {
			case(PARAM_TYPE_INT):
			{
				stats_table >> stat_table[i].value_i;
			} break;
			case(PARAM_TYPE_FLOAT):
			{
				stats_table >> stat_table[i].value_f;
			} break;
			case(PARAM_TYPE_STRING):
			{
				stats_table >> stat_table[i].value_s;
			} break;
			case (PARAM_TYPE_BOOL):
			{
				stats_table >> stat_table[i].value_b;
			} break;
			default:
			{
				stats_table >> stat_table[i].value_i;
			} break;
		}
	}

	stats_table.close();
}

void Projectile::set_default_vars() {

}

void Projectile::init_boxes() {
	for (int i = 0; i < HITBOX_COUNT_MAX; i++) {
		hitboxes[i].init(this);
	}
}