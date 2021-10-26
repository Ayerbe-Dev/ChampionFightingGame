#pragma once
#include "utils.h"
#include "Animation.h"
#include "GameCoordinate.h"
#include "ObjectInstance.fwd.h"
#include "Box.fwd.h"
#include "Box.h"
#include "ParamTable.h"

class MoveScript {
public:
	string name;
	function<void()> move_script{ []() {} };
	int id{ -1 };

	MoveScript();
	MoveScript(string name, function<void()> move_script, int id);
};

class ObjectInstance {
public:
	ObjectInstance();

	int id;
	int object_type;
	GameCoordinate pos;
	GameCoordinate prevpos;
	bool facing_right{ true };
	float facing_dir{ 1.0 };
	u32 status_kind{ CHARA_STATUS_WAIT };
	u32 situation_kind{ CHARA_SITUATION_GROUND };

	double angle{ 0 };

	Animation* anim_kind;
	Animation* prev_anim_kind;
	Animation animation_table[ANIM_TABLE_LENGTH];
	int prev_anim_max_ticks;
	int prev_anim_frame;
	int prev_anim_render_frame;

	int frame;
	int render_frame;
	int ticks;
	int max_ticks;
	int last_excute_frame{ 0 };
	int highest_successful_excute{ 1 };
	int excute_count{ 0 };
	int attempted_excutes{ 0 };

	bool is_anim_end{ false };
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];

	void script(string name, function<void()> move_script);
	MoveScript move_scripts[MOVE_SCRIPT_MAX];

	void update_hitbox_connect(int multihit_index);
	void update_hitbox_pos(bool add_window_width = true);
	void clear_hitbox(int id);
	void clear_hitbox_all();
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	Param param_table[PARAM_TABLE_LENGTH];
	Param unique_param_table[PARAM_TABLE_LENGTH];

	void load_params();
	void load_unique_params();


	int get_param_int(string param, Param param_table[] = {});
	float get_param_float(string param, Param param_table[] = {});
	string get_param_string(string param, Param param_table[] = {});
	bool get_param_bool(string param, Param param_table[] = {});

	string resource_dir;
	SDL_Texture* base_texture;
	SDL_Rect base_rect;
	SDL_Rect frame_rect;

	function<void()> move_script;
	void set_current_move_script(string anim_name);
	bool is_excute_frame(int frame);
	bool is_excute_wait(int frames);
};