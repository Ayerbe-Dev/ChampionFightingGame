#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "RoyFireball.fwd.h"
#include "Roy.fwd.h"
#include "Roy.h"

class RoyFireball : public ProjectileInstance {
public:
	RoyFireball();
	RoyFireball(SDL_Renderer* renderer, int id, FighterInstanceAccessor *fighter_instance_accessor);

	Roy* roy;

	void loadRoyFireballStatusFunctions();
	void loadRoyFireballACMD();

	void tickOnceProjectileUnique() override;

	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;
	
	void status_roy_fireball_hover() override;
	void enter_status_roy_fireball_hover() override;
	void exit_status_roy_fireball_hover() override;
	void status_roy_fireball_punched() override;
	void enter_status_roy_fireball_punched() override;
	void exit_status_roy_fireball_punched() override;
	void status_roy_fireball_kicked() override;
	void enter_status_roy_fireball_kicked() override;
	void exit_status_roy_fireball_kicked() override;
	void status_roy_fireball_fall() override;
	void enter_status_roy_fireball_fall() override;
	void exit_status_roy_fireball_fall() override;
	void status_roy_fireball_ground() override;
	void enter_status_roy_fireball_ground() override;
	void exit_status_roy_fireball_ground() override;
};