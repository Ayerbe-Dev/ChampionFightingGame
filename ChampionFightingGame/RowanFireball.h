#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Rowan.h"

class RowanFireball : public Projectile {
public:
	RowanFireball();
	RowanFireball(int id, Player* player);

	void loadRowanFireballStatusFunctions();
	void load_move_scripts() override;

	void projectile_unique_main() override;

	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;
	
	void status_rowan_fireball_hover();
	void enter_status_rowan_fireball_hover();
	void exit_status_rowan_fireball_hover();
	void status_rowan_fireball_punched();
	void enter_status_rowan_fireball_punched();
	void exit_status_rowan_fireball_punched();
	void status_rowan_fireball_kicked();
	void enter_status_rowan_fireball_kicked();
	void exit_status_rowan_fireball_kicked();
	void status_rowan_fireball_fall();
	void enter_status_rowan_fireball_fall();
	void exit_status_rowan_fireball_fall();
	void status_rowan_fireball_ground();
	void enter_status_rowan_fireball_ground();
	void exit_status_rowan_fireball_ground();
};