#pragma once
#include "PlayerInfo.h"
#include "FighterInstance.h"

int game_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]);

void tickOnce(FighterInstance* fighter_instance, SDL_Renderer* renderer);
void check_attack_connections(FighterInstance *p1, FighterInstance *p2, SDL_Renderer* renderer, bool visualize_boxes);
int get_event_hit_collide_player(FighterInstance* attacker, FighterInstance* defender, Hitbox *hitbox, Hurtbox *hurtbox);
void event_hit_collide_player(FighterInstance *p1, FighterInstance *p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox);
bool can_counterhit(FighterInstance* defender, Hitbox* hitbox);
int get_damage_status(int hit_status, int situation_kind);

class IFighter {
public:
	IFighter(int chara_id, SDL_Renderer* renderer, int id);
	~IFighter();
	FighterInstance* get_fighter();
private:
	FighterInstance* fighter_instance;
};