#pragma warning(disable : 4996)
#include "Fighter.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include "Anlst.h"

void Fighter::init() {
	player->controller.reset_buffer();
	sound_manager->register_game_object(this);
	effect_manager->add_effect_caster(id);

	stage = battle_object_manager->stage;

	if (id == 0) {
		pos = glm::vec3(stage->start_pos.x, 0.0f, 0.0f);
		facing_right = true;
		internal_facing_right = true;
		facing_dir = 1.0;
	}
	else {
		pos = glm::vec3(stage->start_pos.y, 0.0f, 0.0f);
		facing_right = false;
		internal_facing_right = false;
		facing_dir = -1.0;
	}

	load_stats();
	load_params();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_chara_status_scripts();
	load_fighter_status_scripts();
	load_move_scripts();
	load_sound_list();
	load_fighter_effects();
	load_chara_effects();
	set_default_vars();

	change_status(FIGHTER_STATUS_WAIT, false, false);
}

void Fighter::load_sound_list() {
	std::ifstream sound_stream;
	std::string sound_name;
	std::string sound_file;
	float volume_mod;
	sound_stream.open(resource_dir + "/vc/vc_list.yml");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s vc directory was incorrectly set!");
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
		GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s se directory was incorrectly set!");
	}
	else {
		while (sound_stream >> sound_name >> volume_mod) {
			sound_stream >> sound_file;
			sound_manager->load_sound(sound_name, resource_dir + "/se/" + sound_file, volume_mod);
		}
		sound_stream.close();
	}
}

void Fighter::load_fighter_effects() {

}

void Fighter::load_model_shader() {
	scale = glm::vec3(get_local_param_float("model_scale"));
	model.load_model_instance(resource_dir + "/model/m" + std::to_string(player->alt_costume) + "/model.dae");
	model.load_textures("c" + std::to_string(player->alt_color));
	unsigned int flags = 0;
	if (model.has_skeleton()) {
		flags |= SHADER_FEAT_HAS_BONES;
	}
	shader = shader_manager->get_shader("model", "model", "model", SHADER_FEAT_DIM_MUL | flags);
	shadow_shader = shader_manager->get_shader("shadow", "shadow", "", flags);
	outline_shader = shader_manager->get_shader("3d_outline", "3d_outline", "3d_outline", flags);
	shader->use();
	shader->set_int("shadow_map", 0);
	shader->set_int("material.diffuse", 1);
	shader->set_int("material.specular", 2);
}

void Fighter::load_anim_list() {
	try {
		anim_table.load_anlst(resource_dir + "/anims/battle", model.get_skeleton());
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s animation directory was incorrectly set!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
	std::ifstream camera_stream;
	camera_stream.open(resource_dir + "/cam_anims/anim_list.anlst", std::ios::binary);
	if (camera_stream.fail()) {
		GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s camera directory was incorrectly set!");
	}
	std::string name;
	std::string filename;
	while (!camera_stream.eof()) {
		parse_anlst_entry(camera_stream, name, filename);
		render_manager->camera.load_camera_anim(name, resource_dir + "/cam_anims/" + filename + ".fbx");
	}
}

void Fighter::set_default_vars() {
	base_jostle_anchor = glm::vec2(get_local_param_float("jostle_x0"), get_local_param_float("jostle_y0"));
	base_jostle_offset = glm::vec2(get_local_param_float("jostle_x1"), get_local_param_float("jostle_y1"));
	reset_jostle_dimensions();
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_local_param_float("health");
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_NONE;
}

void Fighter::init_boxes() {
	jostle_box.init();
	jostle_box.set_rgba(glm::vec4(0, 0, 0, 204));
	for (int i = 0; i < 10; i++) {
		hitboxes[i].init(this);
		hurtboxes[i].init(this);
		grabboxes[i].init(this);
	}
	blockbox.init(this);
}