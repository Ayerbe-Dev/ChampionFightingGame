#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "RoyFireball.fwd.h"

class RoyFireball : public ProjectileInstance {
public:
	RoyFireball();
	RoyFireball(SDL_Renderer* renderer, int id);

	void loadRoyFireballStatusFunctions();
	void loadRoyFireballACMD();

	virtual void status_default() override;
};