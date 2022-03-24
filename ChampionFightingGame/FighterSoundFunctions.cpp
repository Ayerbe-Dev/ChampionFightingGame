#include "Fighter.h"

void Fighter::playCommonSE(int se) {

	sound_manager->playCommonSE(se, id);
}

void Fighter::playCharaSE(int se) {

	sound_manager->playCharaSE(se, id);
}

void Fighter::playVC(int vc) {

	sound_manager->playVC(vc, id);
}

void Fighter::pauseCommonSE(int se) {

	sound_manager->pauseCommonSE(se, id);
}

void Fighter::pauseCharaSE(int se) {

	sound_manager->pauseCharaSE(se, id);
}

void Fighter::pauseVC(int vc) {

	sound_manager->pauseVC(vc, id);
}

void Fighter::pauseSEAll() {

	sound_manager->pauseSEAll(id);
}

void Fighter::pauseVCAll() {

	sound_manager->pauseVCAll(id);
}

void Fighter::resumeSEAll() {

	sound_manager->resumeSEAll(id);
}

void Fighter::resumeVCAll() {

	sound_manager->resumeVCAll(id);
}

void Fighter::stopCommonSE(int se) {

	sound_manager->stopCommonSE(se, id);
}

void Fighter::stopCharaSE(int se) {

	sound_manager->stopCharaSE(se, id);
}

void Fighter::stopVC(int vc) {

	sound_manager->stopVC(vc, id);
}

void Fighter::stopSEAll() {

	sound_manager->stopSEAll(id);
}

void Fighter::stopVCAll() {

	sound_manager->stopVCAll(id);
}

int Fighter::loadCommonSE(int se) {

	return sound_manager->loadCommonSE(se, id);
}

int Fighter::loadCharaSE(int se) {

	return sound_manager->loadCharaSE(se, id);
}

int Fighter::loadVC(int vc) {

	return sound_manager->loadVC(vc, id);
}

void Fighter::unloadCommonSE(int se) {

	sound_manager->unloadCommonSE(se, id);
}

void Fighter::unloadCharaSE(int se) {

	sound_manager->unloadCharaSE(se, id);
}

void Fighter::unloadVC(int vc) {

	sound_manager->unloadVC(vc, id);
}

void Fighter::unloadSEAll() {

	sound_manager->unloadSEAll(id);
}

void Fighter::unloadVCAll() {

	sound_manager->unloadVCAll(id);
}