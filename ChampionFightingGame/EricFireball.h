#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Eric.h"

class EricFireball : public Projectile {
public:
	EricFireball();
	EricFireball(int id, PlayerInfo* player_info, FighterAccessor *fighter_accessor);

	Eric* eric;

	void loadEricFireballStatusFunctions();
	void loadEricFireballACMD();

	void (EricFireball::* eric_fireball_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (EricFireball::* eric_fireball_enter_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (EricFireball::* eric_fireball_exit_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();

	void projectile_unique_main() override;
	void projectile_unique_status() override;
	void projectile_unique_enter_status() override;
	void projectile_unique_exit_status() override;

	virtual void status_default() override;
};