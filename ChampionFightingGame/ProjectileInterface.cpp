#include "ProjectileInterface.h"
#include "Projectiles.h"

Projectile* create_projectile(int projectile_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	ProjectileInterface projectile_interface(projectile_kind, id, player_info, fighter_accessor);
	Projectile* ret = projectile_interface.get_projectile();
	return ret;
}

ProjectileInterface::ProjectileInterface() {}

ProjectileInterface::ProjectileInterface(int projectile_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	switch (projectile_kind) {
		case (PROJECTILE_KIND_ROY_FIREBALL):
		{
			projectile = new RoyFireball(id, player_info, fighter_accessor);
		}
		break;
		case (PROJECTILE_KIND_ERIC_FIREBALL):
		{
			projectile = new EricFireball(id, player_info, fighter_accessor);
		}
		break;
		case (PROJECTILE_KIND_PROJECTILE_TEMPLATE):
		{
			projectile = new ProjectileTemplate(id, player_info, fighter_accessor);
		} break;
	}
}

ProjectileInterface::~ProjectileInterface() {}

Projectile* ProjectileInterface::get_projectile() {
	return projectile;
}