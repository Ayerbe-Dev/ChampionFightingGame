#pragma once
#include "PlayerInfo.h"
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Projectile.h"
#include "BattleObject.h"
#include "FighterAccessor.h"
#include "Battle.fwd.h"
#include "GameManager.h"
#include "UI.h"

void battle_main(GameManager *game_manager);

class Battle : public GameMenu {
public:
	Battle();
	~Battle();

	void load_battle();

	IObject* fighter_interface[2];
	Fighter* fighter[2];
	Stage stage;
	FighterAccessor* fighter_accessor;

	GameTextureNew health_texture[2];
	GameTextureNew health_bar_texture[2];

	//TODO: Edit the following 3 classes to work in OpenGL

//	ExBar ex_bar[2];
//	PlayerIndicator player_indicator[2];
//	GameTimer timer;

	//TODO: Create a class for the combo counter or otherwise some form of text rendering
};

void process_fighter_positions(Fighter* fighter[2]);
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