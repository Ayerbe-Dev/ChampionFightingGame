#pragma once
#include "RenderObject.h"
#include "Animation.h"
#include "MoveScript.h"
#include "Box.h"
#include "Param.h"
#include "FighterStatus.h"
#include "FighterAttribute.h"
#include "FighterInt.h"
#include "FighterFloat.h"
#include "FighterFlag.h"

class PlayerInfo;
class BattleObjectManager;
class SoundManager;

class BattleObject : public RenderObject {
public:
	BattleObject();

	int id;
	int object_type;
	glm::vec3 prev_pos;

	float facing_dir{ 1.0 };
	bool facing_right = true;
	bool internal_facing_right = true;
	unsigned int status_kind{ FIGHTER_STATUS_WAIT };
	unsigned int situation_kind{ FIGHTER_SITUATION_GROUND };

	glm::vec3 extra_rot = glm::vec3(0.0);

	PlayerInfo* player_info;

	GameRect jostle_box;
	glm::vec2 base_jostle_anchor;
	glm::vec2 base_jostle_offset;

	AnimationTable anim_table;
	Animation* anim_kind;
	Animation* prev_anim_kind;
	glm::vec3 prev_anim_offset;

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

	void script(std::string name, std::function<void()> move_script);
	void wipe_scripts();

	MoveScriptTable move_script_table;
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

	ParamTable stats;
	ParamTable params;

	void load_stats();
	void load_params();

	int get_param_int(std::string param);
	int get_param_int(std::string param, ParamTable param_table);
	float get_param_float(std::string param);
	float get_param_float(std::string param, ParamTable param_table);
	std::string get_param_string(std::string param);
	std::string get_param_string(std::string param, ParamTable param_table);
	bool get_param_bool(std::string param);
	bool get_param_bool(std::string param, ParamTable param_table);

	std::string resource_dir;

	void set_current_move_script(std::string anim_name);
	bool is_excute_frame(float frame);
	bool is_excute_wait(float frames);

	BattleObjectManager* battle_object_manager;
	SoundManager* sound_manager;
};