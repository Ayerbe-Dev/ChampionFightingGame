#include "ProjectileInterface.h"
#include "Projectiles.h"

Projectile* create_projectile(int projectile_kind, Player* player, Fighter* owner) {
	owner->num_projectiles++;
	Projectile* projectile;
	switch (projectile_kind) {
		case (PROJECTILE_KIND_ROWAN_FIREBALL):
		default: {
			projectile = new RowanFireball(10 * (player->id + 1) + owner->num_projectiles, player);
		} break;
		case (PROJECTILE_KIND_ERIC_FIREBALL): {
			projectile = new EricFireball(10 * (player->id + 1) + owner->num_projectiles, player);
		} break;
		case (PROJECTILE_KIND_PROJECTILE_TEMPLATE): {
			projectile = new ProjectileTemplate(10 * (player->id + 1) + owner->num_projectiles, player);
		} break;
	}
	projectile->owner = owner;
	projectile->owner_id = player->id;
	return projectile;
}