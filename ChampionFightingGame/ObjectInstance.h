#pragma once
#include "utils.h"
#include "Animation.h"
#include "GameCoordinate.h"
#include "ObjectInstance.fwd.h"
#include "Box.fwd.h"
#include "Box.h"

class ObjectInstance {
public:
	int id{ -1 };
	int object_type;
	GameCoordinate pos;
	GameCoordinate prevpos;
	bool facing_right{ true };
	float facing_dir{ 1.0 };
	u32 status_kind{ CHARA_STATUS_WAIT };
	u32 situation_kind{ CHARA_SITUATION_GROUND };

	Animation* anim_kind;
	Animation animation_table[ANIM_TABLE_LENGTH];

	int frame;
	int render_frame;
	int ticks;
	int max_ticks;
	int last_excute_frame;
	int excute_count;
	bool is_anim_end{ false };
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];

	string resource_dir;
	SDL_Texture* base_texture;
	SDL_Rect base_rect;
	SDL_Rect frame_rect;

	function<void()> move_script;
	virtual void set_current_move_script(string anim_name);
	bool is_excute_frame(int excute_count, int frame);
	bool is_excute_wait(int excute_count, int frames);
};