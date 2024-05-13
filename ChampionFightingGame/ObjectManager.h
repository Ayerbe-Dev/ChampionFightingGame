#pragma once
#include <vector>
#include <list>
#include "Button.h"
class Fighter;
class StageAsset;
class Projectile;
class Stage;
class GameObject;

class ObjectManager {
public:
	ObjectManager(ObjectManager& other) = delete;
	void operator=(const ObjectManager& other) = delete;

	Fighter* fighter[2];
	std::vector<StageAsset*> stage_assets;
	std::vector<Projectile*> projectiles;
	std::list<GameObject*> game_objects;
	GameObject* real_time_object;

	Stage* stage;

	void process();

	void register_game_object(GameObject* object);
	void unregister_game_object(GameObject* object);

	bool is_frame_elapsed();
	bool is_allow_realtime_process(GameObject* object);
	float get_world_rate(GameObject* object);
	void set_world_rate(GameObject* object, float world_rate);
	void reset_world_rate(GameObject* object);

	float world_frame;
	float world_rate;

	static ObjectManager* get_instance();
	void destroy_instance();
private:
	ObjectManager();
	static ObjectManager* instance;
};