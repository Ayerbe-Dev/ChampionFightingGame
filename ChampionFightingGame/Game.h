#pragma once
#include "PlayerInfo.h"

void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer);

void check_attack_connections(PlayerInfo *p1, PlayerInfo *p2, SDL_Renderer* renderer, bool visualize_boxes);
int get_event_hit_collide_player(PlayerInfo* attacker, PlayerInfo* defender, Hitbox *hitbox, Hurtbox *hurtbox);
void event_hit_collide_player(PlayerInfo *p1, PlayerInfo *p2, Hitbox* p1_hitbox, Hitbox* p2_hitbox);
bool can_counterhit(PlayerInfo* defender, Hitbox* hitbox);
int get_damage_status(int hit_status, int situation_kind);