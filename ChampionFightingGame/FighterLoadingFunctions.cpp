#pragma warning(disable : 4996)
#include "Fighter.h"
#include "WindowManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "ShaderManager.h"
#include "Anlst.h"

void Fighter::load_fighter() {
	player->controller.reset_buffer();
	effect_manager->add_effect_caster(id);

	stage = object_manager->stage;

	if (id == 0) {
		set_pos(glm::vec3(stage->start_pos.x, 0.0f, 0.0f));
		facing_right = true;
		internal_facing_right = true;
		facing_dir = 1.0;
	}
	else {
		set_pos(glm::vec3(stage->start_pos.y, 0.0f, 0.0f));
		facing_right = false;
		internal_facing_right = false;
		facing_dir = -1.0;
	}

	load_params();
	load_model_shader();
	load_collision_boxes();
	load_anim_list();
	load_chara_status_scripts();
	load_fighter_status_scripts();
	load_battle_object_status_scripts();
	load_move_list();
	load_move_scripts();
	cpu.init(this, object_manager->fighter[!id]);
	load_cpu_move_info();
	load_sounds();
	load_effect_list();
	load_chara_effects();
	set_default_vars();

	change_status(BATTLE_OBJECT_STATUS_NONE, false);
}

void Fighter::load_sounds() {
	load_sound_list("vc", resource_dir);
	load_sound_list("vc", "resource/chara/common");
	load_sound_list("se", resource_dir);
	load_sound_list("se", "resource/chara/common");
}

void Fighter::load_effect_list() {

}

void Fighter::load_model_shader() {
	set_scale(glm::vec3(get_param_float("model_scale")));
	model.load_model_instance(resource_dir + "/model/m" + std::to_string(player->alt_costume) + "/model.fbx");
	model.load_textures("c" + std::to_string(player->alt_color));
	init_shader();
}

void Fighter::load_anim_list() {
	try {
		anim_table.load_anlst(resource_dir + "/anims/battle", model.get_skeleton());
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s animation directory has no anim list!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
	std::ifstream camera_stream;
	camera_stream.open(resource_dir + "/cam_anims/anim_list.anlst", std::ios::binary);
	if (camera_stream.fail()) {
		GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s camera directory has no anim list!");
	}
	std::string name;
	std::string filename;
	while (!camera_stream.eof()) {
		parse_anlst_entry(camera_stream, name, filename);
		if (name == "") {
			break;
		}
		window_manager->camera.load_camera_anim(name, resource_dir + "/cam_anims/" + filename);
	}
	camera_stream.close();
}

void Fighter::set_default_vars() {
	object_float[FIGHTER_FLOAT_HEALTH] = get_param_float("health");
	object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = object_float[FIGHTER_FLOAT_HEALTH];
	object_float[FIGHTER_FLOAT_DAMAGE_SCALE] = 1.0f;
	object_float[FIGHTER_FLOAT_PREV_DAMAGE_SCALE] = 1.0f;
	object_float[FIGHTER_FLOAT_DAMAGE_SCALE_UI] = 1.0f;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
}

void Fighter::load_collision_boxes() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].init(this);
		hurtboxes[i].init(this);
		grabboxes[i].init(this);
		pushboxes[i].init(this);
	}
	blockbox.init(this);
}