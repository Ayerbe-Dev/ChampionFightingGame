#pragma once
#include "Music.h"
#include "SE.h"
#include "VC.h"

enum {
	SOUND_KIND_SE,
	SOUND_KIND_VC,
	SOUND_KIND_MUSIC,

	SOUND_KIND_MAX,
};

enum {
	SOUND_TYPE_NORMAL,
	SOUND_TYPE_LOOP,

	SOUND_TYPE_MAX,
};