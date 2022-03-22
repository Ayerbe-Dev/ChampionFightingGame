#pragma once
#include "Projectile.h"

Projectile* create_projectile(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, BattleObjectManager* battle_object_manager);

class ProjectileInterface {
public:
	ProjectileInterface();
	ProjectileInterface(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, BattleObjectManager* battle_object_manager);
	~ProjectileInterface();
	Projectile* get_projectile();
private:
	Projectile* projectile = nullptr;
};
