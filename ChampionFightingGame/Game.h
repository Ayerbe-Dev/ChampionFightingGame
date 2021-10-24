#pragma once
#include "PlayerInfo.h"
#include "FighterInstance.h"
#include "ProjectileInstance.h"
#include "ObjectInstance.h"
#include "FighterInstanceAccessor.h"

int game_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]);

void tickOnceFighter(FighterInstance* fighter_instance);
void tickOnceProjectile(ProjectileInstance* projectile_instance);
void decrease_common_fighter_variables(FighterInstance* fighter_instance);
void decrease_common_projectile_variables(ProjectileInstance* projectile_instance);
void check_attack_connections(FighterInstance *p1, FighterInstance *p2, SDL_Renderer* renderer, bool visualize_boxes);
int get_event_hit_collide_player(FighterInstance* attacker, FighterInstance* defender, Hitbox *hitbox, Hurtbox *hurtbox);
int get_event_grab_collide_player(FighterInstance* attacker, FighterInstance* defender, Grabbox *grabbox, Hurtbox *hurtbox);
int get_event_hit_collide_projectile(ProjectileInstance* attacker, FighterInstance* defender, Hitbox* hitbox, Hurtbox* hurtbox);
bool event_hit_collide_player(FighterInstance *p1, FighterInstance *p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox);
void event_grab_collide_player(FighterInstance *p1, FighterInstance *p2, Grabbox* p1_grabbox, Grabbox* p2_grabbox);
bool can_counterhit(FighterInstance* defender, Hitbox* hitbox);
int get_damage_status(int hit_status, int situation_kind);

class IObject {
public:
	IObject(int object_type, int object_kind, SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);
	~IObject();
	FighterInstance* get_fighter();
	ProjectileInstance* get_projectile();
private: 
	FighterInstance* fighter_instance;
	ProjectileInstance* projectile_instance;
};