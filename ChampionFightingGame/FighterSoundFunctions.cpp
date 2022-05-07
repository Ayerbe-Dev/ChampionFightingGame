#include "Fighter.h"
#include "SoundManager.h"

void Fighter::play_se(std::string se, int vol) {
	sound_manager->play_sound(id, SOUND_KIND_SE, se, vol);
}

void Fighter::play_vc(std::string vc, int vol) {
	sound_manager->play_sound(id, SOUND_KIND_VC, vc, vol);
}

void Fighter::stop_se(std::string se) {
	sound_manager->stop_sound(id, SOUND_KIND_SE, se);
}

void Fighter::stop_se_all() {
	sound_manager->stop_sound_all(id, SOUND_KIND_SE);
}

void Fighter::stop_vc(std::string vc) {
	sound_manager->stop_sound(id, SOUND_KIND_VC, vc);
}

void Fighter::stop_vc_all() {
	sound_manager->stop_sound_all(id, SOUND_KIND_VC);
}

void Fighter::load_sound(std::string name) {
	sound_manager->load_sound(name);
}

void Fighter::unload_sound(std::string name) {
	sound_manager->unload_sound(name);
}

void Fighter::unload_all_sounds() {
	sound_manager->unload_all_sounds();
}