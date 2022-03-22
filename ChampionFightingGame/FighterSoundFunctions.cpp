#include "Fighter.h"

void Fighter::playCommonSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->playCommonSE(se, id);
}

void Fighter::playCharaSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->playCharaSE(se, id);
}

void Fighter::playVC(int vc) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->playVC(vc, id);
}

void Fighter::pauseCommonSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->pauseCommonSE(se, id);
}

void Fighter::pauseCharaSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->pauseCharaSE(se, id);
}

void Fighter::pauseVC(int vc) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->pauseVC(vc, id);
}

void Fighter::pauseSEAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->pauseSEAll(id);
}

void Fighter::pauseVCAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->pauseVCAll(id);
}

void Fighter::resumeSEAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->resumeSEAll(id);
}

void Fighter::resumeVCAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->resumeVCAll(id);
}

void Fighter::stopCommonSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->stopCommonSE(se, id);
}

void Fighter::stopCharaSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->stopCharaSE(se, id);
}

void Fighter::stopVC(int vc) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->stopVC(vc, id);
}

void Fighter::stopSEAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->stopSEAll(id);
}

void Fighter::stopVCAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->stopVCAll(id);
}

int Fighter::loadCommonSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	return sound_manager->loadCommonSE(se, id);
}

int Fighter::loadCharaSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	return sound_manager->loadCharaSE(se, id);
}

int Fighter::loadVC(int vc) {
	SoundManager* sound_manager = SoundManager::get_instance();
	return sound_manager->loadVC(vc, id);
}

void Fighter::unloadCommonSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->unloadCommonSE(se, id);
}

void Fighter::unloadCharaSE(int se) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->unloadCharaSE(se, id);
}

void Fighter::unloadVC(int vc) {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->unloadVC(vc, id);
}

void Fighter::unloadSEAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->unloadSEAll(id);
}

void Fighter::unloadVCAll() {
	SoundManager* sound_manager = SoundManager::get_instance();
	sound_manager->unloadVCAll(id);
}