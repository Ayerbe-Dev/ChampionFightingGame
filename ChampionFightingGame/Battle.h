#pragma once
#include "PlayerInfo.h"
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Projectile.h"
#include "BattleObject.h"
#include "FighterAccessor.h"
#include "Battle.fwd.h"
#include "GameManager.h"

void battle_main(GameManager *game_manager);

void decrease_common_fighter_variables(Fighter* fighter);
void decrease_common_projectile_variables(Projectile* projectile);
void check_attack_connections(Fighter *p1, Fighter *p2, bool visualize_boxes, bool check);
int get_event_hit_collide_player(Fighter* attacker, Fighter* defender, Hitbox *hitbox, Hurtbox *hurtbox);
int get_event_grab_collide_player(Fighter* attacker, Fighter* defender, Grabbox *grabbox, Hurtbox *hurtbox);
int get_event_hit_collide_projectile(Projectile* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox);
bool event_hit_collide_player(Fighter *p1, Fighter *p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox);
void event_grab_collide_player(Fighter *p1, Fighter *p2, Grabbox* p1_grabbox, Grabbox* p2_grabbox);
void event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox);
bool can_counterhit(Fighter* defender, Hitbox* hitbox);
int get_damage_status(int hit_status, int situation_kind);
void cleanup(IObject* p1, IObject* p2);
SDL_Rect getRenderPos(Fighter* fighter, bool force_center);

class IObject {
public:
	IObject();
	IObject(int object_type, int object_kind, int id, PlayerInfo *player_info, FighterAccessor* fighter_accessor);
	~IObject();
	Fighter* get_fighter();
	Projectile* get_projectile();
private: 
	Fighter* fighter{};
	Projectile* projectile{};
};