#pragma once
#include "BattleConstants.h"
#include "Player.h"
#include "Fighter.h"
#include "Projectile.h"
#include "BattleObject.h"
#include "GameManager.h"
#include "GameTexture.h"

class ThreadManager;
class Camera;

void battle_main();

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
	void reset();
	void process();
	void render();
	void flip_clock();

	bool time_up;
	int clock_mode;

	int deca_seconds;
	int seconds;
	int deca_frames;
	int frames;

	int max_time;

	GameTexture second_texture;
	GameTexture deca_second_texture;
	GameTexture frame_texture;
	GameTexture deca_frame_texture;
	GameTexture clock;
};

class BattleMeter {
public:
	BattleMeter();

	void init(Fighter* fighter, int num_rounds);
	void destroy();
	void process();
	void render();

	GameTexture health_texture;
	GameTexture partial_health_texture;
	GameTexture combo_health_texture;
	GameTexture health_border;
	GameTexture ex_texture;
	GameTexture ex_segment_texture;
	GameTexture ex_border;
	std::vector<GameTexture> round_counter;

	float* health;
	float* partial_health;
	float max_health;
	const unsigned* ended_hitstun;
	const unsigned* disable_hitstun_parry;
	int* post_hitstun_timer;
	float* ex;
	float max_ex;
	int num_bars;
	float prev_segments;
	int wins;
};

class BattleText : public GameTexture {
public:
	BattleText();

	void init (Font* font, std::string text, int duration, Fighter* fighter, glm::vec2 pos);
	void update(std::string text, int duration);

	Font* font;

	int duration;
};

class Simbox {
public:
	Simbox();
	void render(BattleObject* active_object);
	virtual void print_start(BattleObject* battle_object) = 0;
	virtual void print_end(BattleObject* object) = 0;
	void update_rect(BattleObject* object);
	float frame_start;
	float frame_end;
	GameRect rect;
	int id;
	glm::vec2 anchor;
	glm::vec2 offset;
	glm::vec2 relative_anchor;
	glm::vec2 relative_offset;
	glm::vec2 prev_anchor;
	glm::vec2 prev_offset;
	glm::vec2 prev_relative_anchor;
	glm::vec2 prev_relative_offset;
};

class SimHitbox : public Simbox {
public:
	SimHitbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);

	int multihit;
	float damage;
	float chip_damage;
	int damage_scale;
	float meter_gain;
	HitKind hit_kind;
	AttackLevel attack_level;
	AttackHeight attack_height;
	int hitlag;
	int blocklag;
	int hit_advantage;
	int block_advantage;
	float hit_pushback;
	float block_pushback;
	HitStatus hit_status;
	HitStatus counterhit_status;
	CounterhitType counterhit_type;
	int juggle_start;
	int juggle_increase;
	int juggle_max;
	ClankKind clank_kind;
	DamageKind damage_kind;
	bool continue_launch;
	bool disable_hitstun_parry;
	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;
};

class SimHurtbox : public Simbox {
public:
	SimHurtbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);
	HurtboxKind hurtbox_kind;
	bool armor;
	IntangibleKind intangible_kind;
};

class SimGrabbox : public Simbox {
public:
	SimGrabbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);
	GrabboxKind grabbox_kind;
	HitKind hit_kind;
	std::string attacker_status;
	std::string defender_status;
};

class SimPushbox : public Simbox {
public:
	SimPushbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);
};

class HitboxSim {
public:
	HitboxSim();

	void destroy();
	void render(BattleObject* active_object);
	void update(BattleObject* active_object);
	void print(BattleObject* active_object);
	void print_all(BattleObject* active_object);

	Simbox* active_box;

	std::vector<SimHitbox> hitboxes;
	std::vector<SimHurtbox> hurtboxes;
	std::vector<SimGrabbox> grabboxes;
	std::vector<SimPushbox> pushboxes;
};

class InputVisualizer {
public:
	InputVisualizer();
	void init(Fighter* fighter, Font* font, bool keep_frames);
	void render();

	Fighter* fighter;
	Font* font;

	GameTexture background;
	GameTexture buttons[6];
	GameTexture stick[9];
	GameTexture num_frames;

	short input_code;
	bool keep_frames;
	int frame_timer;
};

class TrainingInfo {
public:
	TrainingInfo();

	void init(Fighter* fighter, Font* font);
	void destroy();
	void render();

	GameTexture hit_frame;
	GameTexture background_texture;
	GameTexture damage;
	GameTexture combo_damage;
	GameTexture stun_frames;
	GameTexture frame_advantage;

	Fighter* fighter;

	InputVisualizer input_visualizer;
	std::list<InputVisualizer> mini_visualizers;
};

class Battle : public GameState {
public:
	Battle();
	~Battle();

	void process_main();
	void render_main();

	void process_pre_intro();
	void process_intro();
	void process_battle();
	void process_ko();
	void process_outro();

	void process_debug_boxes();
	void process_ui();
	void pre_process_fighter();
	void process_fighter();
	void post_process_fighter();
	void process_frame_pause();
	void process_training();

	void process_background();

	void check_collisions();
	void check_projectile_collisions();
	void check_fighter_collisions();
	int get_event_hit_collide_player(Fighter* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox);
	int get_event_grab_collide_player(Fighter* attacker, Fighter* defender, Grabbox* grabbox, Hurtbox* hurtbox);
	int get_event_hit_collide_projectile(Projectile* attacker, Fighter* defender, Hitbox* hitbox, Hurtbox* hurtbox);
	void event_hit_collide_player();
	bool event_grab_collide_player();
	void event_hit_collide_projectile(Fighter* p1, Fighter* p2, Projectile* p1_projectile, Hitbox* p1_hitbox);
	int can_counterhit(Fighter* defender, Hitbox* hitbox);
	int get_damage_status(Fighter* defender, Hitbox* hitbox, int counterhit_val);

	void render_world();
	void render_ui();

	BattleObjectManager* battle_object_manager;
	ThreadManager* thread_manager;

	Font combo_font;
	Font message_font;
	Font info_font;

	Player* player[2];
	Fighter* fighter[2];
	Stage stage;
	Camera* camera;
	BattleState state;

	BattleMeter meters[2];
	BattleText* combo_counter[2] = { nullptr };
	BattleText* combo_hit[2] = { nullptr };
	std::list<BattleText> texts[2];
	int num_rounds;
	int ko_timer;
	bool sudden_death;

	PlayerIndicator player_indicator[2];
	GameTimer timer;

	TrainingInfo training_info[2];

	GameController debug_controller;

	BattleObject* active_hitbox_object;
	int active_hitbox_object_index;
	std::map<std::string, HitboxSim> hitbox_sim;

	bool visualize_boxes;
	bool frame_pause;
};

void gamestate_battle_fighter_thread(void* fighter_arg);
void gamestate_battle_ui_thread(void* battle_arg);

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