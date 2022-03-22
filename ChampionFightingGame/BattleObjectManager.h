#pragma once
#include <vector>
class Fighter;
class StageAsset;
class Projectile;

class BattleObjectManager {
public:
	BattleObjectManager(BattleObjectManager& other) = delete;
	void operator=(const BattleObjectManager& other) = delete;

	Fighter* fighter[2];
	std::vector<StageAsset*> stage_assets;
	std::vector<Projectile*> projectiles;
	
	static BattleObjectManager* get_instance();

private:
	BattleObjectManager();
	static BattleObjectManager* instance;
};