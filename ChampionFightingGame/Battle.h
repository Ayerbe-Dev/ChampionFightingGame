#pragma once
#include "Fighter.h"
#include "Projectile.h"
#include "BattleObject.h"
#include "GameManager.h"
#include "GameTexture.h"

enum TimerSetting {
	TIMER_SETTING_NORMAL,
	TIMER_SETTING_TRAINING,
	TIMER_SETTING_NONE,

	TIMER_SETTING_MAX
};

const int BATTLE_STATE_PRE_INTRO = 0;
const int BATTLE_STATE_INTRO = 1;
const int BATTLE_STATE_ROUND_START = 2;
const int BATTLE_STATE_BATTLE = 3;
const int BATTLE_STATE_KO = 4;
const int BATTLE_STATE_OUTRO = 5;

const int ROUND_WIN_DOUBLE_KO = 2;
const int ROUND_WIN_P2 = 1;
const int ROUND_WIN_P1 = 0;
const int ROUND_WIN_TIMEOUT = -1;

const int ROUND_ICON_NONE = 0;
const int ROUND_ICON_KO = 1;
const int ROUND_ICON_PERFECT = 2;
const int ROUND_ICON_EX_SUPER = 3;
const int ROUND_ICON_CHAMPION_SUPER = 4;
const int ROUND_ICON_TIMEOUT = 5;
const int ROUND_ICON_DOUBLE = 6;

class ThreadManager;
class Camera;

void battle_main();

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

	bool collision_kind_ground;
	bool collision_kind_air;
	bool collision_kind_down;
	bool collision_kind_projectile;
	bool collision_kind_soft_intangible;
	bool collision_kind_armor;

	/*
		TODO: Put the revamped HitResult fields here
	*/

	HitStatus hit_status;
	std::string custom_hit_status;

	bool force_stand;
	bool force_crouch;
	bool force_aerial;
	bool down_face_down;
	bool hard_knockdown;
	bool continue_launch;
	bool disable_hitstun_parry;
	bool crumple_on_ko;

	/*
		TODO: Put the revamped HitMove fields here
	*/

	CriticalCondition critical_condition;
	HitStatus critical_status;
	std::string custom_critical_status;

	bool critical_force_stand;
	bool critical_force_crouch;
	bool critical_force_aerial;
	bool critical_down_face_down;
	bool critical_hard_knockdown;
	bool critical_continue_launch;
	bool critical_disable_hitstun_parry;
	bool critical_crumple_on_ko;

	/*
		TODO: Put the revamped HitMove fields here too (but like the ones used on crit)
	*/

	int juggle_start;
	int juggle_increase;
	int juggle_max;
	HitHeight hit_height;
	float damage;
	float chip_damage;
	int damage_scale;
	float meter_gain;
	int hitlag;
	int blocklag;
	int hit_advantage;
	int block_advantage;
	float pushback_ground_hit;
	float pushback_ground_block;
	float pushback_air_x;
	float pushback_air_y;
	int pushback_frames;

	bool has_launch_target_pos;
	glm::vec3 launch_target_pos;
	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;
	DamageKind damage_kind;
	std::string hit_effect;
	std::string hit_sound;
};

class SimHurtbox : public Simbox {
public:
	SimHurtbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);
	HurtboxKind hurtbox_kind;
	int armor_hits;
	bool intangible_kind_high;
	bool intangible_kind_mid;
	bool intangible_kind_low;
	bool intangible_kind_aerial;
	bool intangible_kind_throw;
	bool intangible_kind_projectile;
	bool intangible_kind_invincible;
	bool intangible_kind_soft;
};

class SimGrabbox : public Simbox {
public:
	SimGrabbox();
	void print_start(BattleObject* battle_object);
	void print_end(BattleObject* object);

	bool grabbox_kind_notech;
	bool grabbox_kind_hitstun;
	
	bool collision_kind_ground;
	bool collision_kind_air;
	bool collision_kind_down;
	bool collision_kind_projectile;
	bool collision_kind_soft_intangible;
	bool collision_kind_armor;

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

const int TRAINING_FIELD_STARTUP = 0;
const int TRAINING_FIELD_DAMAGE = 1;
const int TRAINING_FIELD_COMBO_DAMAGE = 2;
const int TRAINING_FIELD_STUN_FRAMES = 3;
const int TRAINING_FIELD_AIRTIME = 4;
const int TRAINING_FIELD_FRAME_GAP = 5;
const int TRAINING_FIELD_JUGGLE_VALUE = 6;
const int TRAINING_FIELD_MAX = 7;

class TrainingInfo {
public:
	TrainingInfo();

	void init(Fighter* fighter, Font* font);
	void destroy();
	void render();

	GameTexture background_texture;

	const std::string field_names[TRAINING_FIELD_MAX] = {
		"Startup: ", "Damage: ", "Combo Damage: ", "Stun Frames: ", "Airtime: ", 
		"Frame Gap: ", "Juggle Value: "
	};
	GameTexture fields[TRAINING_FIELD_MAX];

	/* TODO: Replace current fields with the following:
	* Startup (Hit Frame)
	* Damage (Scale)
	* Combo Damage (Max Combo Damage)
	* Hitstun/Blockstun (Frame Advantage)
	* Airtime (Frame Advantage pre-landing)
	* Frame Gap (Frame Margin, I.E. how many frames until opponent becomes actionable)
	* Juggle Value (juggle_max)
	*/

	Fighter* fighter;

	InputVisualizer input_visualizer;
	std::list<InputVisualizer> mini_visualizers;
};

class Battle : public GameState {
public:
	Battle();
	~Battle();

	void load_world();
	void load_ui();

	void pre_event_process_main();
	void process_main();
	void render_main();

	void process_pre_intro();
	void process_intro();
	void process_round_start();
	void process_battle();
	void process_ko();
	void process_outro();

	void process_debug_boxes();
	void process_ui();
	void process_fighters();
	void post_process_fighters();
	void process_training();
	void process_collisions();

	void render_world();
	void render_ui();

	void event_start_press();
	void event_frame_pause_press();
	void event_frame_advance_press();
	void event_record_input_press();
	void event_replay_input_press();
	void event_switch_input_press();

	ObjectManager* object_manager;
	ThreadManager* thread_manager;

	Font combo_font;
	Font message_font;
	Font info_font;

	Fighter* fighter[2];
	Stage stage;
	Camera* camera;

	BattleText* combo_counter[2] = { nullptr };
	BattleText* combo_hit[2] = { nullptr };
	std::list<BattleText> texts[2];
	int ko_timer;
	int actionable_timer;

	MusicInstance* battle_music;

	TrainingInfo training_info[2];

	BattleObject* active_hitbox_object;
	int active_hitbox_object_index;
	std::map<std::string, HitboxSim> hitbox_sim;
	int curr_round;
	int round_count_setting;
	TimerSetting timer_setting;

	bool frame_pause;
	bool frame_advance;
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