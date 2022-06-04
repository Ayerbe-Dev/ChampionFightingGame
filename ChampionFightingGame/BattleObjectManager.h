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

	bool counters_can_move();

	float world_frame;
	float world_rate;
	
	static BattleObjectManager* get_instance();

private:
	BattleObjectManager();
	static BattleObjectManager* instance;
};