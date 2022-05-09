#include "BattleObject.h"
#include "SoundManager.h"

void BattleObject::play_se(std::string se, int vol) {
	sound_manager->play_sound(id, SOUND_KIND_SE, se, vol);
}

void BattleObject::play_vc(std::string vc, int vol) {
	sound_manager->play_sound(id, SOUND_KIND_VC, vc, vol);
}

void BattleObject::stop_se(std::string se) {
	sound_manager->stop_sound(id, SOUND_KIND_SE, se);
}

void BattleObject::stop_se_all() {
	sound_manager->stop_sound_all(id, SOUND_KIND_SE);
}

void BattleObject::stop_vc(std::string vc) {
	sound_manager->stop_sound(id, SOUND_KIND_VC, vc);
}

void BattleObject::stop_vc_all() {
	sound_manager->stop_sound_all(id, SOUND_KIND_VC);
}

void BattleObject::load_sound(std::string name) {
	sound_manager->load_sound(name);
}

void BattleObject::unload_sound(std::string name) {
	sound_manager->unload_sound(name);
}

void BattleObject::unload_all_sounds() {
	sound_manager->unload_all_sounds();
}