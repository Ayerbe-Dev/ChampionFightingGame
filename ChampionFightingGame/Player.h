#pragma once
#include "Stage.h"
#include "GameController.h"
#include "InputSequence.h"

class PlayerInfo;

const int INPUT_MODE_POLL = 0;
const int INPUT_MODE_RECORD_SEQ = 1;
const int INPUT_MODE_PLAY_SEQ = 2;
const int INPUT_MODE_REPLAY = 3;
const int INPUT_MODE_ATLAS_REWIND = 4;
const int INPUT_MODE_ROLLBACK = 5;

const int PLAYER_KIND_PLAYER = 0;
const int PLAYER_KIND_CPU = 1;
const int PLAYER_KIND_DUMMY = 2;

class Player {
public:
	Player();
	Player(int id);

	void load_player(int index);
	void set_alt_for_chara();

	void poll_controller_menu();
	void poll_controller_fighter();

	GameController controller;
	int id;
	int chara_kind;
	int alt_costume;
	int alt_color;
	int control_type;
	StageInfo stage_info;
	std::string name;
	PlayerInfo* player_info;

	InputSequence manual_seq;
	InputSequence replay_seq;

	int rollback_frames;
	int input_mode;

	int player_kind;
};