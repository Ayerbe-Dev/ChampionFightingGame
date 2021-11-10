#include "Loader.h"

GameLoader::GameLoader() {

}

GameLoader::GameLoader(PlayerInfo player_info[2]) {
	this->player_info[0] = player_info[0];
	this->player_info[1] = player_info[1];
}