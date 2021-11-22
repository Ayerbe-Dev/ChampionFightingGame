#pragma warning(disable : 4996)
#include "Fighter.h"

void Fighter::superInit(int id) {
	this->id = id;

	//these initial gamecoord values get overwritten almost immediately. why are we still here, just to suffer?

	if (id == 0) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0);
	}
	else if (id == 1) {
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0);
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
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		animation_table[i].faf = ymlChopInt(faf);
		animation_table[i].force_center = ymlChopInt(force_center);
		animation_table[i].move_dir = ymlChopInt(move_dir);
		load_anim_map(animation_table[i].anim_map, animation_table[i].name);
		loadAnimation(&animation_table[i]);
	}
	anim_list.close();
}

void Fighter::load_anim_map(SDL_Rect ret[MAX_ANIM_LENGTH], string anim_dir) {
	ifstream anim_map;
	anim_map.open(resource_dir + "/anims/" + anim_dir + ".yml");

	if (anim_map.fail()) { //If the map for that animation doesn't exist, don't worry about it. The code can still function without one, it just 
		//needs one if we want a spritesheet to be across multiple lines
		SDL_Rect empty_rect[MAX_ANIM_LENGTH];
		for (int i = 0; i < MAX_ANIM_LENGTH; i++) {
			empty_rect[i] = { 0, 0, 0, 0 };
		}
		ret = empty_rect;
	}
	else {
		int initial_index;
		anim_map >> initial_index;
		int i = initial_index;
		goto SKIP_READ_FRAME;
		while (anim_map >> i) {
			/*
				Regardless of how the animation is indexed, the anim_map's first character on each line will be a 0. Since peek() only looks at a
				character, not the whole string, and the number of 0s can vary, we can just read the entire segment in as an int, then skip over the
				first iteration of the loop. A bit scuffed but it's reliable.
			*/
			SKIP_READ_FRAME:

			i -= initial_index;
			string equal_sign;
			anim_map >> equal_sign; //It is followed by a single equal sign. We can use this to make sure we're looking at the right fields.
			//If for whatever reason it's missing, that means the file got messed up somewhere, so we'll just crash to the debug menu with the 
			if (equal_sign != "=") {
				char buffer[58];
				sprintf(buffer, "Error at line %d of %s.yml: Expected equal sign, found %s", i, anim_dir.c_str(), equal_sign.c_str());
				player_info->crash_reason = buffer;
				crash_to_debug = true;
				anim_map.close();
				return;
			}
			anim_map >> ret[i].x >> ret[i].y >> ret[i].w >> ret[i].h;
		}
	}
	anim_map.close();
}