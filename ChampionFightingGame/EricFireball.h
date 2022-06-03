#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Eric.h"

class EricFireball : public Projectile {
public:
	EricFireball();
	EricFireball(int id, Player* player);

	void loadEricFireballStatusFunctions();
	void load_move_scripts() override;

	void projectile_unique_main() override;

	virtual void status_default() override;
};