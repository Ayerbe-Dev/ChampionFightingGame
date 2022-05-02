#pragma once
class EffectManager {
public:
	EffectManager(EffectManager& other) = delete;
	void operator=(const EffectManager& other) = delete;

	static EffectManager* get_instance();
private:
	EffectManager();
	static EffectManager* instance;
};