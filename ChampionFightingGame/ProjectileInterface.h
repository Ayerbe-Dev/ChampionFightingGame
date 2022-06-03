#pragma once
#include "Projectile.h"

Projectile* create_projectile(int projectile_kind, int id, Player* player, Fighter* owner);

class ProjectileInterface {
public:
	ProjectileInterface();
	ProjectileInterface(int projectile_kind, int id, Player* player, Fighter* owner);
	~ProjectileInterface();
	Projectile* get_projectile();
private:
	Projectile* projectile = nullptr;
};
