#pragma warning(disable : 4996)
#include "Fighter.h"
#include "RenderManager.h"
extern RenderManager g_rendermanager;

void Fighter::superInit(int id) {
	this->id = id;

	if (id == 0) {
		pos = vec3(-200, FLOOR_GAMECOORD, 0);
	}
	else if (id == 1) {
		pos = vec3(200, FLOOR_GAMECOORD, 0);
	}
	load_stats();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_status_scripts();
	set_default_vars();

	change_anim("wait", 2, 0);
	status_kind = FIGHTER_STATUS_WAIT;
}

void Fighter::loadFighterSounds() {

}

void Fighter::load_model_shader() {
	scale = vec3(0.05 * get_param_float("model_scale"));
	shader.init("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&shader);
	model.load_model(resource_dir + "/model/model.dae");
}

void Fighter::load_anim_list() {
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		anim_list.close();
		char buffer[55];
		sprintf(buffer, "Character %d's resource directory was incorrectly set!", chara_kind);
		player_info->crash_reason = buffer;

		crash_to_debug = true;

		return;
	}

	string name;
	string path;
	string faf;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> faf;
		name = ymlChopString(name);
		path = ymlChopString(path);
		Animation anim(name, resource_dir + "/anims/" + path, &model);
		anim.faf = ymlChopInt(faf);
		animation_table[i] = anim;
	}
	anim_list.close();
}

void Fighter::set_default_vars() {
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_param_float("health");
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

void Fighter::init_boxes() {
	jostle_box.init();
	jostle_box.bind_scale(&scale);
	jostle_box.set_rgba(vec4(0, 0, 0, 127));
	for (int i = 0; i < 10; i++) {
		hitboxes[i].init(this);
		hurtboxes[i].init(this);
		grabboxes[i].init(this);
	}
}