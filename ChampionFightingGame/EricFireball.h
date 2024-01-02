#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "Eric.h"

class EricFireball : public Projectile {
public:
	EricFireball();
	EricFireball(int id, Player* player);

	void load_projectile_unique_status_scripts() override;
	void load_move_scripts() override;

	void projectile_unique_main() override;

	void unique_activate() override;
	void unique_deactivate() override;
};