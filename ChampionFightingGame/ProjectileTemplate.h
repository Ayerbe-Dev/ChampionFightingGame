#pragma once
#include "Projectile.h"
#include "BattleObject.h"
#include "CharaTemplate.h"

class ProjectileTemplate : public Projectile {
public:
	ProjectileTemplate();
	ProjectileTemplate(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager);

	void loadProjectileTemplateStatusFunctions();
	void load_move_scripts() override;

	void projectile_unique_main() override;
	
	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;

	void projectile_template_status_template();
	void projectile_template_enter_status_template();
	void projectile_template_exit_status_template();
};