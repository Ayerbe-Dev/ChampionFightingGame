#include "ProjectileInterface.h"
#include "Projectiles.h"

Projectile* create_projectile(int projectile_kind, int id, Player* player, Fighter* owner) {
	owner->num_projectiles++;
	ProjectileInterface projectile_interface(projectile_kind, id, player, owner);
	Projectile* ret = projectile_interface.get_projectile();
	return ret;
}

ProjectileInterface::ProjectileInterface() {}

ProjectileInterface::ProjectileInterface(int projectile_kind, int id, Player* player, Fighter* owner) {
	switch (projectile_kind) {
		case (PROJECTILE_KIND_ROWAN_FIREBALL):
		default: {
			projectile = new RowanFireball(10 * (id + 1) + owner->num_projectiles, player);
		}
		break;
		case (PROJECTILE_KIND_ERIC_FIREBALL): {
			projectile = new EricFireball(10 * (id + 1) + owner->num_projectiles, player);
		}
		break;
		case (PROJECTILE_KIND_PROJECTILE_TEMPLATE): {
			projectile = new ProjectileTemplate(10 * (id + 1) + owner->num_projectiles, player);
		} break;
	}
	projectile->owner = owner;
	projectile->owner_id = id;
}

ProjectileInterface::~ProjectileInterface() {}

Projectile* ProjectileInterface::get_projectile() {
	return projectile;
}