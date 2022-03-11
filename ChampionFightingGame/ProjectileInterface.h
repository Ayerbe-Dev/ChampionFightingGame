#pragma once
#include "Projectile.h"

Projectile* create_projectile(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, FighterAccessor* fighter_accessor);

class ProjectileInterface {
public:
	ProjectileInterface();
	ProjectileInterface(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, FighterAccessor* fighter_accessor);
	~ProjectileInterface();
	Projectile* get_projectile();
private:
	Projectile* projectile = nullptr;
};
