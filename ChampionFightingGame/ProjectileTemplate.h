#pragma once
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "ProjectileTemplate.fwd.h"
#include "CharaTemplate.fwd.h"
#include "CharaTemplate.h"

class ProjectileTemplate : public ProjectileInstance {
public:
	ProjectileTemplate();
	ProjectileTemplate(int id, PlayerInfo* player_info, FighterInstanceAccessor* fighter_instance_accessor);

	CharaTemplate* chara_template;

	void (ProjectileTemplate::* projectile_template_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (ProjectileTemplate::* projectile_template_enter_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX - PROJECTILE_STATUS_MAX])();
	void (ProjectileTemplate::* projectile_template_exit_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX - PROJECTILE_STATUS_MAX])();

	void loadProjectileTemplateStatusFunctions();
	void loadProjectileTemplateACMD();

	void tickOnceProjectileUnique() override;
	void projectile_unique_status() override;
	void projectile_unique_enter_status() override;
	void projectile_unique_exit_status() override;

	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;

	void projectile_template_status_template();
	void projectile_template_enter_status_template();
	void projectile_template_exit_status_template();
};