#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "EricFireball.fwd.h"

class EricFireball : public ProjectileInstance {
public:
	EricFireball();
	EricFireball(SDL_Renderer* renderer, int id);
};