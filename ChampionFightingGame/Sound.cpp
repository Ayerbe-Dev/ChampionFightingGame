#include "Sound.h"

SoundInfo::SoundInfo() {
	sound_kind = SOUND_KIND_MAX;
}

SoundInfo::SoundInfo(std::string name, int sound_kind, int chara_kind, int volume, int sound_type) {
	this->name = name;
	this->sound_kind = sound_kind;
	this->sound_type = sound_type;
	this->volume = volume;
	active = false;
	looped = false;
	switch (sound_kind) {
	case (SOUND_KIND_MUSIC): {
		this->dir = "resource/sound/bgm/" + name + ".wav";
		this->loop_dir = "resource/sound/bgm/" + name + "_loop.wav";
	} break;
	case (SOUND_KIND_SE): {
		switch (chara_kind) {
		default: {
			this->dir = "resource/sound/se/common/" + name + ".wav";
		} break;
		case (CHARA_KIND_ROY): {
			this->dir = "resource/sound/se/roy/" + name + ".wav";
		} break;
		case (CHARA_KIND_ERIC): {
			this->dir = "resource/sound/se/eric/" + name + ".wav";
		} break;
		case(CHARA_KIND_LEON): {
			this->dir = "resource/sound/se/leon/" + name + ".wav";
		} break;
		case(CHARA_KIND_CHAMELEON):
		{
			this->dir = "resource/sound/se/chameleon/" + name + ".wav";
		} break;
		case(CHARA_KIND_ANGELICA):
		{
			this->dir = "resource/sound/se/angelica/" + name + ".wav";
		} break;
		case(CHARA_KIND_NIGHTSABER):
		{
			this->dir = "resource/sound/se/nightsaber/" + name + ".wav";
		} break;
		case(CHARA_KIND_SULLY):
		{
			this->dir = "resource/sound/se/sully/" + name + ".wav";
		} break;
		case(CHARA_KIND_PRIEST):
		{
			this->dir = "resource/sound/se/priest/" + name + ".wav";
		} break;
		case(CHARA_KIND_AZIEL):
		{
			this->dir = "resource/sound/se/aziel/" + name + ".wav";
		} break;
		case(CHARA_KIND_BRUNO):
		{
			this->dir = "resource/sound/se/bruno/" + name + ".wav";
		} break;
		case(CHARA_KIND_TESSA):
		{
			this->dir = "resource/sound/se/tessa/" + name + ".wav";
		} break;
		case(CHARA_KIND_ALEJANDRO):
		{
			this->dir = "resource/sound/se/alejandro/" + name + ".wav";
		} break;
		case(CHARA_KIND_NORMAN):
		{
			this->dir = "resource/sound/se/norman/" + name + ".wav";
		} break;
		case(CHARA_KIND_ATLAS):
		{
			this->dir = "resource/sound/se/atlas/" + name + ".wav";
		} break;
		case(CHARA_KIND_JULIUS):
		{
			this->dir = "resource/sound/se/julius/" + name + ".wav";
		} break;
		case(CHARA_KIND_RAMONA):
		{
			this->dir = "resource/sound/se/ramona/" + name + ".wav";
		} break;
		case(CHARA_KIND_ZYAIR):
		{
			this->dir = "resource/sound/se/zyair/" + name + ".wav";
		} break;
		case(CHARA_KIND_VESUVIUS):
		{
			this->dir = "resource/sound/se/vesuvius/" + name + ".wav";
		} break;
		}
	} break;
	case (SOUND_KIND_VC): {
		switch (chara_kind) {
		default: {} break;
		case (CHARA_KIND_ROY):
		{
			this->dir = "resource/sound/vc/roy/" + name + ".wav";
		} break;
		case (CHARA_KIND_ERIC):
		{
			this->dir = "resource/sound/vc/eric/" + name + ".wav";
		} break;
		case(CHARA_KIND_LEON):
		{
			this->dir = "resource/sound/vc/leon/" + name + ".wav";
		} break;
		case(CHARA_KIND_CHAMELEON):
		{
			this->dir = "resource/sound/vc/chameleon/" + name + ".wav";
		} break;
		case(CHARA_KIND_ANGELICA):
		{
			this->dir = "resource/sound/vc/angelica/" + name + ".wav";
		} break;
		case(CHARA_KIND_NIGHTSABER):
		{
			this->dir = "resource/sound/vc/nightsaber/" + name + ".wav";
		} break;
		case(CHARA_KIND_SULLY):
		{
			this->dir = "resource/sound/vc/sully/" + name + ".wav";
		} break;
		case(CHARA_KIND_PRIEST):
		{
			this->dir = "resource/sound/vc/priest/" + name + ".wav";
		} break;
		case(CHARA_KIND_AZIEL):
		{
			this->dir = "resource/sound/vc/aziel/" + name + ".wav";
		} break;
		case(CHARA_KIND_BRUNO):
		{
			this->dir = "resource/sound/vc/bruno/" + name + ".wav";
		} break;
		case(CHARA_KIND_TESSA):
		{
			this->dir = "resource/sound/vc/tessa/" + name + ".wav";
		} break;
		case(CHARA_KIND_ALEJANDRO):
		{
			this->dir = "resource/sound/vc/alejandro/" + name + ".wav";
		} break;
		case(CHARA_KIND_NORMAN):
		{
			this->dir = "resource/sound/vc/norman/" + name + ".wav";
		} break;
		case(CHARA_KIND_ATLAS):
		{
			this->dir = "resource/sound/vc/atlas/" + name + ".wav";
		} break;
		case(CHARA_KIND_JULIUS):
		{
			this->dir = "resource/sound/vc/julius/" + name + ".wav";
		} break;
		case(CHARA_KIND_RAMONA):
		{
			this->dir = "resource/sound/vc/ramona/" + name + ".wav";
		} break;
		case(CHARA_KIND_ZYAIR):
		{
			this->dir = "resource/sound/vc/zyair/" + name + ".wav";
		} break;
		case(CHARA_KIND_VESUVIUS):
		{
			this->dir = "resource/sound/vc/vesuvius/" + name + ".wav";
		} break;
		}
	} break;
	}
}

Sound::Sound() {

}