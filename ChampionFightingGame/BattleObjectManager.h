#pragma once
#include <vector>
class Fighter;
class StageAsset;
class Projectile;

class BattleObjectManager {
public:
	Fighter* fighter[2];
	std::vector<StageAsset*> stage_assets;
	std::vector<Projectile*> projectiles;

	BattleObjectManager();
};