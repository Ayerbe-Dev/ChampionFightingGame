#pragma warning(disable : 4996)
#include "Fighter.h"

void Fighter::superInit(int id) {
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?

	if (id == 0) {
		pos = vec3(200, 0, 0);
	}
	else if (id == 1) {
		pos = vec3(-200, 0, 0);
	}

	if (!crash_to_debug) {
		load_anim_list();
	}
	if (!crash_to_debug) {
		load_stats();
	}
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_param_float("health");
	loadStatusScripts();
	pos.y = FLOOR_GAMECOORD;
	if (!crash_to_debug) {
		change_anim("wait", 2, 0);
	}
	status_kind = FIGHTER_STATUS_WAIT;
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

void Fighter::loadFighterSounds() {

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
	string frame_count;
	string faf;
	string force_center;
	string move_dir;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> frame_count >> faf >> force_center >> move_dir;
		Animation3D anim(name, path, model);
		anim.faf = ymlChopInt(faf);
		animation_table.push_back(anim);
	}
	anim_list.close();
}