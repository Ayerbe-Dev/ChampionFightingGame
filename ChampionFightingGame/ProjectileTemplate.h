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
	ProjectileTemplate(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);

	CharaTemplate* chara_template;

	void loadProjectileTemplateStatusFunctions();
	void loadProjectileTemplateACMD();

	void tickOnceProjectileUnique() override;

	void status_default() override;
	void status_hit() override;
	void enter_status_hit() override;

	void projectile_template_status_template() override;
	void projectile_template_enter_status_template() override;
	void projectile_template_exit_status_template() override;
};