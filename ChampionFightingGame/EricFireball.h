#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "EricFireball.fwd.h"

class EricFireball : public ProjectileInstance {
public:
	EricFireball();
	EricFireball(SDL_Renderer* renderer, int id, FighterInstanceAccessor *fighter_instance_accessor);

	void loadEricFireballStatusFunctions();
	void loadEricFireballACMD();

	virtual void status_default() override;
};