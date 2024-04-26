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

	void unique_activate() override;
	void unique_deactivate() override;
	
	void status_move() override;
	void enter_status_move() override;
	void exit_status_move() override;

	void status_rowan_fireball_hover();
	void enter_status_rowan_fireball_hover();
	void exit_status_rowan_fireball_hover();
};