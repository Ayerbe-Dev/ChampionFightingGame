#include "Fighter.h"
extern SoundManager g_soundmanager;

void Fighter::playCommonSE(int se) {
	g_soundmanager.playCommonSE(se, id);
}

void Fighter::playCharaSE(int se) {
	g_soundmanager.playCharaSE(se, id);
}

void Fighter::playVC(int vc) {
	g_soundmanager.playVC(vc, id);
}

void Fighter::pauseCommonSE(int se) {
	g_soundmanager.pauseCommonSE(se, id);
}

void Fighter::pauseCharaSE(int se) {
	g_soundmanager.pauseCharaSE(se, id);
}

void Fighter::pauseVC(int vc) {
	g_soundmanager.pauseVC(vc, id);
}

void Fighter::pauseSEAll() {
	g_soundmanager.pauseSEAll(id);
}

void Fighter::pauseVCAll() {
	g_soundmanager.pauseVCAll(id);
}

void Fighter::resumeSEAll() {
	g_soundmanager.resumeSEAll(id);
}

void Fighter::resumeVCAll() {
	g_soundmanager.resumeVCAll(id);
}

void Fighter::stopCommonSE(int se) {
	g_soundmanager.stopCommonSE(se, id);
}

void Fighter::stopCharaSE(int se) {
	g_soundmanager.stopCharaSE(se, id);
}

void Fighter::stopVC(int vc) {
	g_soundmanager.stopVC(vc, id);
}

void Fighter::stopSEAll() {
	g_soundmanager.stopSEAll(id);
}

void Fighter::stopVCAll() {
	g_soundmanager.stopVCAll(id);
}

int Fighter::loadCommonSE(int se) {
	return g_soundmanager.loadCommonSE(se, id);
}

int Fighter::loadCharaSE(int se) {
	return g_soundmanager.loadCharaSE(se, id);
}

int Fighter::loadVC(int vc) {
	return g_soundmanager.loadVC(vc, id);
}

void Fighter::unloadCommonSE(int se) {
	g_soundmanager.unloadCommonSE(se, id);
}

void Fighter::unloadCharaSE(int se) {
	g_soundmanager.unloadCharaSE(se, id);
}

void Fighter::unloadVC(int vc) {
	g_soundmanager.unloadVC(vc, id);
}

void Fighter::unloadSEAll() {
	g_soundmanager.unloadSEAll(id);
}

void Fighter::unloadVCAll() {
	g_soundmanager.unloadVCAll(id);
}