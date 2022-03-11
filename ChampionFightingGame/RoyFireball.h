#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Roy.h"

class RoyFireball : public Projectile {
public:
	RoyFireball();
	RoyFireball(int id, PlayerInfo* player_info, FighterAccessor *fighter_accessor);

	void loadRoyFireballStatusFunctions();
	void loadRoyFireballACMD();

	void projectile_unique_main() override;

	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;
	
	void status_roy_fireball_hover();
	void enter_status_roy_fireball_hover();
	void exit_status_roy_fireball_hover();
	void status_roy_fireball_punched();
	void enter_status_roy_fireball_punched();
	void exit_status_roy_fireball_punched();
	void status_roy_fireball_kicked();
	void enter_status_roy_fireball_kicked();
	void exit_status_roy_fireball_kicked();
	void status_roy_fireball_fall();
	void enter_status_roy_fireball_fall();
	void exit_status_roy_fireball_fall();
	void status_roy_fireball_ground();
	void enter_status_roy_fireball_ground();
	void exit_status_roy_fireball_ground();
};