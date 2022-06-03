#pragma once
#include "Player.h"
#include "Fighter.h"
#include "Projectile.h"
#include "BattleObject.h"
#include "GameManager.h"
#include "GameTexture.h"

class ThreadManager;
class Camera;

void battle_main();

class HealthBar {
public:
	HealthBar();
	void init(Fighter* fighter);
	void destroy();
	void process();
	void render();

	GameTexture health_texture;
	GameTexture bar_texture;
	float* health;
	float max_health;
};

class ExBar {
public:
	ExBar();
	void init(Fighter* fighter);
	void destroy();
	void process();
	void render();

	Fighter* fighter;
	GameTexture ex_texture;
	GameTexture ex_segment_texture;
	GameTexture bar_texture;
	float* ex;
	float max_ex;
	int num_bars;
	int prev_segments = 0;
};

class PlayerIndicator {
public:
	Fighter* fighter;
	GameTexture texture;
	std::string nametag;

	PlayerIndicator();
	PlayerIndicator(Fighter* fighter, std::string nametag = "");
};

class GameTimer {
public:
	GameTimer();
	GameTimer(int time);

	void init(int time);
	void process();
	void render();
	void flip_clock();

	bool time_up;
	int clock_mode;

	int deca_seconds;
	int seconds;
	int deca_frames;
	int frames;

	GameTexture second_texture;
	GameTexture deca_second_texture;
	GameTexture frame_texture;
	GameTexture deca_frame_texture;
	GameTexture clock;
};

class Battle : public GameMenu {
public:
	Battle();
	~Battle();

	void load_game_menu();
	void unload_game_menu();

	void process_main();
	void process_ui();
	void pre_process_fighter();
	void process_fighter();
	void post_process_fighter();
	void process_frame_pause();

	void process_background();

	void check_collisions();
	void check_projectile_collisions();
	void check_fighter_collisions();
	int get_event_hit_collide_player(Fighter* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox);
	int get_event_grab_collide_player(Fighter* attacker, Fighter* defender, Grabbox* grabbox, Hurtbox* hurtbox);
	int get_event_hit_collide_projectile(Projectile* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox);
	bool event_hit_collide_player();
	void event_grab_collide_player();
	void event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox);
	bool can_counterhit(Fighter* defender, Hitbox* hitbox);
	int get_damage_status(int hit_status, int situation_kind);

	void render_world();
	void render_ui();

	GameController debug_controller;

	const Uint8* keyboard_state;

	Fighter* fighter[2];
	Stage stage;
	BattleObjectManager* battle_object_manager;
	ThreadManager* thread_manager;

	Player* player[2];

	HealthBar health_bar[2];

	ExBar ex_bar[2];
	PlayerIndicator player_indicator[2];
	GameTimer timer;
	Camera *camera;

	//TODO: Create a class for the combo counter or otherwise some form of text rendering

	GameRect debug_rect[2];
	glm::vec2 debug_anchor[2] = { {0,0} };
	glm::vec2 debug_offset[2] = { {0,0} };

	bool visualize_boxes;
	bool pause;
	bool frame_pause;
};

void fighter_thread(void* fighter_arg);
void ui_thread(void* battle_arg);

//Demoknight TF2 has been evicted from UI.cpp as that file no longer exists

/*





																  .
																 }ob
																 00O
																~ZZl
																t00^
															   .00Q
															   {OZ
															   zZC
															  `OZn
															  jQO`
															  J0U                ___________________
															 :0wu               (  So, No Home?     )
															 J0m"               (___________________)
															"mwz               /
															+0Q?              /
														   ^pO0^     ^;      /
														   {hhh.   Zkhaak+
														   bakX   nUtJZdhm:
													 :    |QUY)!{l^``"i|ruUX'
													   .tfZwZbpbbwYx/rXUppzX)^"II
													  .OkhbmUumOwpOpqqdkbhabbkopwmhh)
													 ^vkkkpm|pqOQOdqkhkk*#%%%%#ahbXYo_
													'vphbdbpJhbbbbbbbbh#mnLmZOJfqQLXpx
												  .IxkqbdbwX)ZhhkkkkkkhaqYmmmmzxzcuuw[
											   r^?/ffYvqkkkkkkpudbkkkhaoW*88mrr/tttvZ
											  `)ffffjn)dkkkkkkkJtt/1<";](]0):!<~[nJX?
												.,[!^ `kkkkkkkhtttttttt///|(rmmwwwXjl
													   `Zwpddd0ffftttttttttfjJwqqqqw
														  l#*%WWWW*q0vffffffrqpqpaW>
														   *BBB&&&&&WW&&WWW8%8%8WW&j"
														  '##CXXLCYYYYQLYYYLd#hpwb#*
														   'wmmpkkwmmpkkwmmZ%WM##*].
														  0ahmmqppwmmqppwmZq&#h#ob.
														 XaoWmmmmmmmmmmmmmOp#Owdoac
														-hoC:mmmmmmmmmmmwwZaMMwC|`
															1qM##########MMM*pmQn
														   [wb*MMMMMMMMMMMMkkdqZU)
														  1wbkaaMMMMMMMWWpahkbdq0n
														 _qbhaaabLrxuUn  thkkbddmY)
														;mbbkkkk1         |bbdddpOu`
													   'mbbbkkk-           Odbbbbwzl
													  'Okkkkhh>            +kkkkkbOf
													  (hhhhhhI              khhhhhbCl
													 !Chhhhhku.             lhhhhhkp(.
													 {bkkkhhhb_              }hhhhhhZ/
													 lhhhkhhhh:              Chhhhhhhp(
													  )Xmhha*^              .vdhhhhhhkdt
														 J###d,             `Qbahhhhhhd!
														 Ohhhhx               zahhhhk?
														<phhhoY                }#M##~
													   .whhh##h                 o##*
												   :/Qb*#####aa+               [Jooh,
												  _p###*qQ+ '                  tkhhh~
													'`'                        tkhhh+
																			   Cbhhhf
																			  ?wkoooU
																			  lqqakqC)
																				]wo*#*hX
																				:UYOppm0^

















*/