#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Rowan.h"

class RowanFireball : public Projectile {
public:
	RowanFireball();
	RowanFireball(int id, Player* player);

	void load_projectile_unique_status_scripts() override;
	void load_move_scripts() override;

	void unique_process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) override;

	void projectile_unique_main() override;

	void status_activate() override;
	
	void status_rowan_fireball_hover();
	void enter_status_rowan_fireball_hover();
	void exit_status_rowan_fireball_hover();
	void status_rowan_fireball_punched();
	void enter_status_rowan_fireball_punched();
	void exit_status_rowan_fireball_punched();
	void status_rowan_fireball_kicked();
	void enter_status_rowan_fireball_kicked();
	void exit_status_rowan_fireball_kicked();
};