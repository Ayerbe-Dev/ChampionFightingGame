#pragma once
#include "utils.h"
#include "Animation.h"
#include "GameCoordinate.h"
#include "BattleObject.fwd.h"
#include "Box.fwd.h"
#include "Box.h"
#include "ParamTable.h"
#include "PlayerInfo.fwd.h"

#include "RenderObject.h"

class MoveScript {
public:
	string name;
	function<void()> move_script{ []() {} };
	int id{ -1 };

	MoveScript();
	MoveScript(string name, function<void()> move_script, int id);
};

class BattleObject : public RenderObject {
public:
	BattleObject();

	int id;
	int object_type;
	vec3 prev_pos;

	float facing_dir{ 1.0 };
	bool facing_right = true;
	u32 status_kind{ FIGHTER_STATUS_WAIT };
	u32 situation_kind{ FIGHTER_SITUATION_GROUND };

	vec3 extra_rot = vec3(0.0);

	PlayerInfo* player_info;

	GameRect jostle_box;
	GameRect base_jostle_box;

	Animation animation_table[ANIM_TABLE_LENGTH];
	Animation* anim_kind;
	Animation* prev_anim_kind;

	float prev_anim_rate;
	float prev_anim_frame;

	float frame;
	float rate;
	float last_excute_frame{ 0 };
	int excute_count{ 0 };
	int attempted_excutes{ 0 };

	bool is_anim_end{ false };
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];

	void script(string name, function<void()> move_script);
	void wipe_scripts();
	MoveScript move_scripts[MOVE_SCRIPT_MAX];
	MoveScript move_script;
	
	bool multihit_connected[10] = {false};

	void update_hitbox_connect(int multihit_index);
	void update_hitbox_pos();
	bool is_hitbox_active(int multihit = -1);
	void clear_hitbox(int id);
	void clear_hitbox_all();
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	Param stat_table[PARAM_TABLE_LENGTH];
	Param param_table[PARAM_TABLE_LENGTH];

	void load_stats();
	void load_params();


	int get_param_int(string param, Param param_table[] = {});
	float get_param_float(string param, Param param_table[] = {});
	string get_param_string(string param, Param param_table[] = {});
	bool get_param_bool(string param, Param param_table[] = {});

	string resource_dir;

	void set_current_move_script(string anim_name);
	bool is_excute_frame(float frame);
	bool is_excute_wait(float frames);
};