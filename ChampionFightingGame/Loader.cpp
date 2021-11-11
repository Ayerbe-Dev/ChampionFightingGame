#include "Loader.h"

GameLoader::GameLoader() {

}

GameLoader::GameLoader(PlayerInfo p1, PlayerInfo p2) {
	this->player_info[0] = p1;
	this->player_info[1] = p2;
}