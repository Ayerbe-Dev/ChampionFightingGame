#pragma once
#include "PlayerInfo.h"
#include "Menu.h"

void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer);

int game_main(SDL_Renderer* renderer, PlayerChoice *p1_choice, PlayerChoice *p2_choice);
void check_attack_connections(PlayerInfo *p1, PlayerInfo *p2, SDL_Renderer* renderer, bool visualize_boxes);
int get_event_hit_collide_player(PlayerInfo* attacker, PlayerInfo* defender, Hitbox *hitbox, Hurtbox *hurtbox);
void event_hit_collide_player(PlayerInfo *p1, PlayerInfo *p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox);
bool can_counterhit(PlayerInfo* defender, Hitbox* hitbox);
int get_damage_status(int hit_status, int situation_kind);

class IFighter {
public:
	IFighter(int chara_id, SDL_Renderer* renderer, int id);
	~IFighter();
	PlayerInfo* get_fighter();
private:
	PlayerInfo* player_info;
};