#pragma once
#include <vector>
#include "Button.h"
class Fighter;
class StageAsset;
class Projectile;
class Stage;

class BattleObjectManager {
public:
	BattleObjectManager(BattleObjectManager& other) = delete;
	void operator=(const BattleObjectManager& other) = delete;

	Fighter* fighter[2];
	std::vector<StageAsset*> stage_assets;
	std::vector<Projectile*> projectiles;

	std::vector<Button [BUTTON_MAX]> player_inputs[2];
	Stage* stage;

	bool allow_dec_var(int object_id);
	float get_time_multiplier(int object_id);
	void set_world_rate(int object_id, float world_rate);
	void reset_world_rate(int object_id);

	float world_frame;
	float world_rate;
	int real_time_id;

	static BattleObjectManager* get_instance();
	void destroy_instance();
private:
	BattleObjectManager();
	static BattleObjectManager* instance;
};