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

	void status_default() override;
	void status_move() override;
};