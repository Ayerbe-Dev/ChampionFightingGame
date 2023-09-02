#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "CharaTemplate.h"

class ProjectileTemplate : public Projectile {
public:
	ProjectileTemplate();
	ProjectileTemplate(int id, Player* player);

	void load_projectile_unique_status_scripts() override;
	void load_move_scripts() override;

	void projectile_unique_main() override;
	
	void status_activate() override;

	void projectile_template_status_template();
	void projectile_template_enter_status_template();
	void projectile_template_exit_status_template();
};