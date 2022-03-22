#include "ProjectileInterface.h"
#include "Projectiles.h"

Projectile* create_projectile(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, BattleObjectManager* battle_object_manager) {
	ProjectileInterface projectile_interface(projectile_kind, id, player_info, owner, battle_object_manager);
	Projectile* ret = projectile_interface.get_projectile();
	return ret;
}

ProjectileInterface::ProjectileInterface() {}

ProjectileInterface::ProjectileInterface(int projectile_kind, int id, PlayerInfo* player_info, Fighter* owner, BattleObjectManager* battle_object_manager) {
	switch (projectile_kind) {
		case (PROJECTILE_KIND_ROY_FIREBALL):
		default: {
			projectile = new RoyFireball(id, player_info, battle_object_manager);
		}
		break;
		case (PROJECTILE_KIND_ERIC_FIREBALL): {
			projectile = new EricFireball(id, player_info, battle_object_manager);
		}
		break;
		case (PROJECTILE_KIND_PROJECTILE_TEMPLATE): {
			projectile = new ProjectileTemplate(id, player_info, battle_object_manager);
		} break;
	}
	projectile->owner = owner;
	projectile->owner_id = id;
}

ProjectileInterface::~ProjectileInterface() {}

Projectile* ProjectileInterface::get_projectile() {
	return projectile;
}