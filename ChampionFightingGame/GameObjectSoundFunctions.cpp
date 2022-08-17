#include "GameObject.h"
#include "SoundManager.h"

void GameObject::play_se(std::string se, int vol) {
	sound_manager->play_sound(id_sound, SOUND_KIND_SE, se, vol);
}

void GameObject::play_vc(std::string vc, int vol) {
	sound_manager->play_sound(id_sound, SOUND_KIND_VC, vc, vol);
}

void GameObject::stop_se(std::string se) {
	sound_manager->stop_sound(id_sound, SOUND_KIND_SE, se);
}

void GameObject::stop_se_all() {
	sound_manager->stop_sound_all(id_sound, SOUND_KIND_SE);
}

void GameObject::stop_vc(std::string vc) {
	sound_manager->stop_sound(id_sound, SOUND_KIND_VC, vc);
}

void GameObject::stop_vc_all() {
	sound_manager->stop_sound_all(id_sound, SOUND_KIND_VC);
}

void GameObject::load_sound(std::string name) {
	sound_manager->load_sound(name);
}

void GameObject::unload_sound(std::string name) {
	sound_manager->unload_sound(name);
}

void GameObject::unload_all_sounds() {
	sound_manager->unload_all_sounds();
}