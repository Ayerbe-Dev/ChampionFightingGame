#pragma once
#include "PlayerInfo.h"

void set_status_functions(PlayerInfo* player_info);
void tickOnce(PlayerInfo* player_info, SDL_Renderer* renderer);
void check_attack_connections(PlayerInfo player_info[2], SDL_Renderer* renderer, bool visualize_boxes);

void status_wait(PlayerInfo* player_info);
void enter_status_wait(PlayerInfo* player_info);
void exit_status_wait(PlayerInfo* player_info);
void status_walkf(PlayerInfo* player_info);
void enter_status_walkf(PlayerInfo* player_info);
void exit_status_walkf(PlayerInfo* player_info);
void status_walkb(PlayerInfo* player_info);
void enter_status_walkb(PlayerInfo* player_info);
void exit_status_walkb(PlayerInfo* player_info);
void status_dash(PlayerInfo* player_info);
void enter_status_dash(PlayerInfo* player_info);
void exit_status_dash(PlayerInfo* player_info);
void status_dashb(PlayerInfo* player_info);
void enter_status_dashb(PlayerInfo* player_info);
void exit_status_dashb(PlayerInfo* player_info);
void status_crouchd(PlayerInfo* player_info);
void enter_status_crouchd(PlayerInfo* player_info);
void exit_status_crouchd(PlayerInfo* player_info);
void status_crouch(PlayerInfo* player_info);
void enter_status_crouch(PlayerInfo* player_info);
void exit_status_crouch(PlayerInfo* player_info);
void status_crouchu(PlayerInfo* player_info);
void enter_status_crouchu(PlayerInfo* player_info);
void exit_status_crouchu(PlayerInfo* player_info);
void status_jumpsquat(PlayerInfo* player_info);
void enter_status_jumpsquat(PlayerInfo* player_info);
void exit_status_jumpsquat(PlayerInfo* player_info);
void status_jump(PlayerInfo* player_info);
void enter_status_jump(PlayerInfo* player_info);
void exit_status_jump(PlayerInfo* player_info);
void status_attack(PlayerInfo* player_info);
void enter_status_attack(PlayerInfo* player_info);
void exit_status_attack(PlayerInfo* player_info);
void status_hitstun(PlayerInfo* player_info);
void enter_status_hitstun(PlayerInfo* player_info);
void exit_status_hitstun(PlayerInfo* player_info);
void status_blockstun(PlayerInfo* player_info);
void enter_status_blockstun(PlayerInfo* player_info);
void exit_status_blockstun(PlayerInfo* player_info);