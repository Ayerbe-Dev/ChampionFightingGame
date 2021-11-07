#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "EricFireball.fwd.h"
#include "Eric.fwd.h"
#include "Eric.h"

class EricFireball : public ProjectileInstance {
public:
	EricFireball();
	EricFireball(int id, PlayerInfo* player_info, FighterInstanceAccessor *fighter_instance_accessor);

	Eric* eric;

	void loadEricFireballStatusFunctions();
	void loadEricFireballACMD();

	void (EricFireball::* eric_fireball_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (EricFireball::* eric_fireball_enter_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (EricFireball::* eric_fireball_exit_status[PROJECTILE_ERIC_FIREBALL_STATUS_MAX - PROJECTILE_STATUS_MAX])();

	void tickOnceProjectileUnique() override;
	void projectile_unique_status() override;
	void projectile_unique_enter_status() override;
	void projectile_unique_exit_status() override;

	virtual void status_default() override;
};