#include <iostream>
#include <functional>
#include <vector>
#include<string>
#include "PlayerInfo.h"
#include <SDL.h>
#include "Animation.h"
#include "Game.h"

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.

void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer) {
	//Marks down the addresses of all of the player's status functions and puts them all on their info table. 

	/*
		TODO: Make it so this only runs once, probably while initializing the player_info class but it could be somewhere else.Eventually there's
		going to be a version of this function for each character that may or may not overwrite the global one, so ideally they aren't constantly
		overwriting each other every frame.
	*/

	//Calls the looping status function for whatever the player's current status_kind is.
	player_info->playoutStatus();

	/*
		Get the player's inputs and increment the frame. 
	*/

	player_info->processInput();
	if (player_info->canStep()) {
		player_info->stepAnimation();
	}
	(*player_info).prev_stick_dir = (*player_info).get_stick_dir();
	if ((*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_F_WINDOW] --;
	}
	if ((*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] != 0) {
		(*player_info).chara_int[CHARA_INT_DASH_B_WINDOW] --;
	}
}


