#pragma once
#include "BattleObject.h"
#include "ProjectileConstants.h"
#include <glm/glm.hpp>
#include <iostream>
#include "ObjectManager.h"

#define ADD_PROJECTILE_STATUS(index, status_func) (status_script[index] = (void (Projectile::*)(void))status_func)
#define ADD_PROJECTILE_ENTRY_STATUS(index, status_func) (enter_status_script[index] = (void (Projectile::*)(void))(status_func))
#define ADD_PROJECTILE_EXIT_STATUS(index, status_func) (exit_status_script[index] = (void (Projectile::*)(void))(status_func))

class Fighter;

class Projectile: public BattleObject {
public:
	Projectile();
	~Projectile();

	/*
		PROJECTILE FUNCTIONS
	*/

	//Main

	void projectile_main();
	virtual void projectile_unique_main() {};
	void process_status();
	void process_animate();
	void process_post_animate();
	void process_position();

	void projectile_post();
	void process_post_position();

	void decrease_common_variables();

	//Setup
	
	void load_projectile();
	void load_model_shader();
	void load_anim_list();
	void load_projectile_status_scripts();
	virtual void load_projectile_unique_status_scripts() {};
	virtual void load_move_scripts() {};
	void load_sound_list();
	void set_default_vars();
	void load_collision_boxes();

	//Animation

	void change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);

	//Status

	void change_status(unsigned int new_status_kind, bool call_end_status = true) override;

	//Data Functions

	void set_int(int target, int val);
	void inc_int(int target);
	void dec_int(int target);
	void set_float(int target, float val);
	void set_flag(int target, bool val);
	void set_string(int target, std::string val);

	//Collision Functions

	bool is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) override;
	bool is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) override;

	void process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) override;
	void process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) override;

	virtual void unique_process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender){};

	void process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender);
	virtual void unique_process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {};

	//Script Functions
	template<typename ...T>
	void push_function(void (Projectile::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.function_calls.push((void (BattleObject::*)(ScriptArg))function);
		active_script_frame.function_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_true(void (Projectile::* function)(ScriptArg), T... args) {
		if (active_script_condition == nullptr) {
			GameManager::get_instance()->add_crash_log("ERROR: Tried to push conditional function to script "
				+ active_move_script.name + " without declaring a condition.");
			return;
		}
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_condition->true_calls.push((void (BattleObject::*)(ScriptArg))function);
		active_script_condition->true_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_false(void (Projectile::* function)(ScriptArg), T... args) {
		if (active_script_condition == nullptr) {
			GameManager::get_instance()->add_crash_log("ERROR: Tried to push conditional function to script "
				+ active_move_script.name + " without declaring a condition.");
			return;
		}
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_condition->false_calls.push((void (BattleObject::*)(ScriptArg))function);
		active_script_condition->false_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}


	//Script Wrappers
	void SET_INT(ScriptArg args);
	void SET_FLOAT(ScriptArg args);
	void SET_FLAG(ScriptArg args);
	void SET_STRING(ScriptArg args);

	void ADD_POS(ScriptArg args);
	void SET_POS(ScriptArg args);

	void CHANGE_STATUS(ScriptArg args);

	//Activation

	void activate();
	void deactivate();
	virtual void unique_activate();
	virtual void unique_deactivate();

	//Status Scripts

	virtual void status_none();
	virtual void enter_status_none();
	virtual void exit_status_none();

	int projectile_kind;
	std::string projectile_name;
	int owner_id;

	Fighter* owner;

	bool active = false;

	std::vector<int> projectile_int;
	std::vector<float> projectile_float;
	std::vector<bool> projectile_flag;
	std::vector<std::string> projectile_string;

	std::vector<void (Projectile::*)(void)> status_script;
	std::vector<void (Projectile::*)(void)> enter_status_script;
	std::vector<void (Projectile::*)(void)> exit_status_script;
};

/*
:::::::::::::::::::::::::::::c:cccc::::::::::::::::::::::::::::::::::::c::::::ccc:cxKXXKKK00OO0KKKK0OOOkxdllccclllodxoc:c:::cccccc:cccccccccc:::cccc:ccccccccccc:::::cccccccc:::::::::ccccc:::c:::cccccclllloooooddxxddoollcc:::::ccc:::::::::::
:::::::::::::::::::::::::::::c:::::::::::::::::::::::::::::::::::::::::c::::::c:ccoOXXKKK00OkkO0KKK00OOkxdollcclllloxdl::::::::::cccccccccccccccccccccccccccccc:::cccccccccccccccc:::::ccccllooodddxxxxkkkkkkkkkkkkkkkkkxxdoollcccc::ccc::::::::
:::::::::::::::::::::::::::::c:::::::::::::::::::::::::::::::::::::::::ccc:cc::::cxKXKK00OkxxxxkO0000OOkxdolllcclllldxoc:::cclooollc::cccccccccccccccccccccccccccccccccccccccccc:::ccloodxxkOOOO0000OOO000000000OOOOOkkxxdddddollllcccc:::::::::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::cccc:::::cccccccc:::::oOKKKK0OkkxdoodkO0000Okxdollccccccclddl:cldkO00Okkxoc:cccc::cccccccc::::cc::ccc:::ccccccccc:::::clodxxkkOO00000000OOO0000000000OOOOkkkxxxdddolccccccccc::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::ccccc::ccccccccccc::ccco0KKKK0kkxdlcccoxO000Okxdollccccc::;clllldO00000Okxdoc:ccc:::ccccc::::::c:::::cccc:ccccccc:cccclooddxkOOOO0000000000OOOO0000OOOkkOOOkkkkkxxdoollcc:::::::::ccc:
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::cccccccccccccccccc::c:cd0KKK0Okxdolc::coxO00Okxdollcccc::;,,;cloxkO000Okdollcc:::ccc::::::::cccccccccccccc:::::::cccllolloxkOOkxdddxxxkkkOOOOOOOOOOOOOOOOOOOOkkkkxxddoollc:::::cccc:::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::cccccccccccccccccc:::::cx0KK00Oxdolc:;;;:lxOOOkxdolccc:::;,,'';:lxkOOOkxxolc:::::ccc::::::::ccccccccccc::::::::::::ccllccldkOkxoccclodxxxkkkOOOOOOOOOOOOOOOOOOOOkkkkxxdooolc:;;;:cccc:c
:::::::::::::::::::::::::::::::::::::::::::::::::::::::cccccc::::::::cccccc:::::cx0KK0Okxoolc:;,'';lxkkxxdolc::::;;,''',;cdxkkkxddolc::::cc:::::::c:::ccc:::::::::::::::::ccc::::coxxdlccodxkkOOOOOOOOO0000OOOOOOOOOOOOOOOkkkkkxxdoolc;,,;::cccc
:::::::::::::::::::::::::::::::::::::::::::::::::::::::cccc:::::::::::ccccc:::::cd0KK0Okxoolc:;,..';ldxxddlc:;;;;;,,''.',:oxkkkxdollc:::::cccclcccclloodollcccc:::::::::c::;;;;;:lool:codxkkkOOOOOOO00000000000000OOOOOOOOOkkkkkxdool:;'',;:clol
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::ccccc:::::cd0KK0Okxoolc:;'...';ldddolc:;;;;;,,''.',;coxxxxdollc::;;:ldxxkkkkkO00000kxdlcccccllcclll;'';;::cll::cdxkkkkOOOOOOOO00000000000000000OOOOOOkkkxxxdolc:;,'',;cooo
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::o0KK00kxoolc:;'....':looolc:;;;;;;;,,''',:odxxxdollcc:;;;:oO000KKKXXXXXK00OxxdddxkkkOOOo;';:cclllccldxkkkkOOOOOOOO0000000000000000000000OOOkxxxdolccc:;,''';clo
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::ccccc::::::oOKK00Oxdlcc:,'....':ldddollccccclcc:;,,;:lodxxdollc:;,,,,:d0K0KKKXXXXXKKKKKKKKKKKKKXX0l,,:cccc::loddxxkkkkOOOOOOO00000000000KK0000000O00OOkxddolc:cll:,'..';cl
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::cccc:::::::lkKK0OOxdlc:;,'....':oxkxxddddddddool:;;,;:ldxxdollc:;,''',:xKKKKKKXXXXXXXXXXXXXXKKXXKx;';ccc:c:coddddxxkkkkOOOOO00000KKKKKKKKKKK00000000OOkxxddlccodoc,'...,:c
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::lk000Okxol:;,''....':oxOOOkkxxxxxxddlc:;,,;coxxddolc;,''''',ckKKKKKKXXXXXXXXXXXXXKKXXKd,';clcc::clodddxxxxxkkkOO000000KKKKKKKKKKK00000000OOkkkxdolodxoc;'...',;
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::c::::::::::lk0K0Okxol:;,,'''..';lxOOOOOOkkkxxxdol:;;,,;lddddol:;'''....,cOXKKKKXXXXXXXXXXXXKKKKXKx,.,ccc::::cloddodddxxkkkO00000000000000000000000000OOOkxdoooxxdl;,''''',
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::lkKK0Okxolc:;,''''.',cdOOOOOOkkkxxxdol::;'',:ldddol:,''.....',o0KKKKKXXXXKKKKKKKKKKKKKk:.,;::;;:cccoddooddxxxkkkOOOOOOOO0000000000KK000000OOkkxddodddoc;;,,,,,;
::::::::::::::::::::::::c:::::::::::::::::::::::::::::::::::::::::::::::::::c::::lkKK0Okxolc:;,,''...':okOOOOOkkkxxxdol:;;''',:lddol:,'......'';x0KKKKKKKKKKKKKKKKKKKKKOl'';;;;,;:::ldxxdddxxxkkkOOOOOOOOOO00000000KKKK0000Okkkxxdddddlc:cc:;;,;
:::::::::::::::::::::::cc::::::::::::c::::::::::::::::::::::::::::::::::::ccc::::lkK0Okxxdlc:;,,''....,lxOOOOOkkkxxddol:;;,'.',:lool:,'......'',ckKKKKKKKKKKKKKKKKKKKKK0d,.',,,;;:;;:lodddxxxxkkOOOOOOOOOOO000OO0000KKKK00OOOOOOkkxxxddlloooc;,'
:::::::::::::::::::::::cc:::::::::cc:::::::::::::::::::::::::::::::::::::::cooooookKK0Okxdolc:;;,,'...,cxOOOOOkkkxxdool::;,'...,:clc:,'......''';o0KKKKKKKKKKKKKKKKKKKKKk:..',,;;;;;;cloooodddxkOOOOOOOOOOO0OOOOOO0000000OkkOOOkkkxxdddollddo:'.
:::::::::::::::::::::::::::::::::::::::::::::::cc:::::::::::::::::::::cccoxOKKKKKKKXXXK0Okkxolcc::;,'.':okOO0OOkkxxddolc::,''...,:cc:,'......''',ckKKKKKKKKKKKKKKKKKKKKKOl...',,,;;;:coooooooddxkkkkkkkkOOOOOOOOOOOO00Okxddxxxxxxddodolc;;cll:,.
:::::::::::::::::::::::::::::::::::::::::::::::c::::::::::::::::::::::cd0XXXXXKKKKXNNXKK00Okxddolc:,'.',lxkOOOkkxxxddoolc:;,'...',:cc:;,'..'',,,,;oOKKKKKKKKKKKKKKKKKKKKKx;..',,;;;;:coooddddddxxxxkkkkkkxxxdddxkkxdxxdollllcc:cccccllc;,',;:;'.
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::cc::c:cxKNNNXK00O00XNNNXKK00Okxxdol:;'..':dkOOOkxxxxdddolcc;,'....,cloll::;,,,;:::;cx0KKKKKKKKKKKKKKKKKKKKOc..',,,,;;:cooodddddxxxxkkkkkxxdolc::ldxdlc:;,,''',:looooloolc;,,;;,'.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::cxKNNXXK0OkkkOKNWNNXKK0OOkxdooc;,'.';lxkOOkxxxxxdddolc:,'....,:odddolc:;;::::::lOKKKKKKKKKKKKKKKKKKKK0d,.',,''',;coooooddxxxxxxddlc:;,''..';oxxolc:,'.,:cc:;,''..'',;:::;,''
::::::::::::::::::::::ccccc::::cccc:::::::::::::::::::::::::::::::::lOXNXKK0kkxxxkKNWWNXXK00Okkxxdl:,''',coxkkkxxdddxddolc:;,'...':ldxxdolc:;;::::;:d0KKKKKKKKKKKKKKKKKKKKOl''''...';:llllllllllllc;,'.........,cxOkdoc:;:c:'....'',,,,,;ccc:;,,
c:::ccc:::::::::::::::ccccc::::cccc:::::::::::::::::::::::::::::::::o0XXXK0OkxdooxOXWWWNXKK00Okkxdoc;''.';cdxxxddooddddolc:;,''..',cdxxxdlc:::::::;;ckK00KKKKKKKKKKKKKKKKK0d;.''...',:ccllc;,,;:c:;;,;;:;;;;:::coxOOkdlclllc:lddxxxxxxddddddoc:;
ccccccc:::::::::::::::ccccc::::cccc:::::::::::::::::::::::::::::::::oKNXK0OkxdollokKNWWNXKKK0OOkxdol:,'..';ldxddooodxxdolc:;,,''..';ldxxdoc:::::::;;:dOKKKKKKKKKKKKKKKKKKKKk:...''..';:llolc::cc:,........',:lodxkOOOkdoodxxxxkkkkkkkkxxxdddocc:
ccccccc:::::::::::::::ccccc::::cccc:::::::::::::::::::::::::::::::::dKNXK0Okxdolclx0NWWNXXKK0OOkxdol:;'...,coxxxdddxkkxdoc:;,,'....':odxdoc::;;;;;;;;cx0KKKKKK00KKKKKKKKKKK0d;...''''';:lodolc:,....',;;cclodxxxxkkOOkxdoodxkOOOOOkkkkkkkxdolccc
c:c:ccc:::::ccc::cc:ccccccccccccccccccc::::::::c::::::::::::::::::::dXNXK0Okxdlc:coOXNWNXXK00OOkxxdlc;,'..';lxxxxxxxkkkdol:,,,''....;ldddoc:;;;;;:;;;:oOKKK00000000KKKKKKKK0x:,''';;;,,;cloool:,;coooddooodxkkkxdxkkOOkdlloxkOOO000000OOkxdolllc
::::::::::ccccccccccccccccccccc::cccccc::::::::::::::::::::::::::::cxXXXK0Oxdol:;;cxKNNNNXK0OOkkkxdoc:,''.';ldxkkxxxkkkddl:;,,''....':lddol:;;;;:::;;:cd0K00000000000KKKK00xlc;'',:cc::cloooooooodxxxxxxxxkOOkkxddxxkOOxolloxkOO0000000Okxdolllc
::::::ccccccccccccccccccccccccccccccccccccccc::::cc:::::::::::::ccclkXXKK0kxdlc:;,:oOXNWNXK0Okkkkxdol:;'''';lxOOOOkkOOOkxdoc:;,''....,coooc:;;;;:cc:::cok0000000000000KKK0Oc'','',::::::cooddddxxxxxkkkkOOO0OkxddddxkOOkxoc::okOO00000OOkxolccc:
::::::ccccccccccccccccccccccccccccccccccccccccccclllllcclllloooodxkOKXXK0Okxolc:,,;lkKNNXK0OkkkOkxxdlc;,''';ok0KXKKKKXK0Okxol:;,'....';lolc:;;;:clcc::ccdO0000000000000000Od;;;,..,;;;;;:looddddxxkkO00KK00OOkdollodkOOOxoc;,;ldxxkkkkkkxdoc::;,
:::ccccccccccccccccccccccccccccccllllllllloodddxxkkkkOkkkOOOO0000KKKXXXK0Okxolc;,,;cxKNNXKOkkkkkkkxdoc:;,,,;lk0XXNNXXXXK0Oxdlc:;''....,:llcc:;;:lollccccok00000000000000000Odooc,..'''',;clooooddxkkO0KKK0OOkxocllloxkOOkdc:;:cclooddxxxdoc:;,,'
::ccccccccccccccccccccccccccccclodxxxxxxxxxkOO0000KKKKKKKKKKKKKKKKKKXXXK0Okxolc:;,;cd0XNXKOkkkOOOkxdoc:;;,,;lxOKXNNXNNXK0Okxolc:,'....';cllcc::coooolccloxOO0000000000000000Oxoc'......',;clooooodxkkO000OkkdoooddoooxkOkoc;;;;;coooooooolc:,''.
::cccccccccccccccccccccccccccccllodxxdddooodxxkkkOOOOOOOOOOO000KKKKKXXXK0Okxolc:;,,:okKNNXK0OOO0Okxdoc::;;,;cdOKXNNXXXXK00kxdoc:;,'...';loddooodxxxdoolllokOOOOOOO00OO000000Oxo;.........,;:lolloodxxkkOkxoc::clllllcccc:,.....':oxxdolllcc:;'..
::cccccccccccccccccccccccccccccccclllcccccccllllloooooooooooddxkkkkOKXXK0Okxolc:;,,;lxKNNNXK00000Okxolc:::;;:okKXNNXXXXK00Okxolc:,,'''':lxkkxxkkOOOkkxdooodkOOkOkkOOOOOOOOOOOkdc,.,;'.....',:lllllodddxdolc:clc;''......     ..';lxkkxdollc:;'..
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccclllcd0XXK0Okxolc:;,,;cxKNNNXXKKKK00Okxdoolc:;:lx0XNNNNNXXKKOkxdolc:;,'',:dk00000KKXXK0Oxddoodxxxxxxkkxxkkkkkkkkxdl:cc;.......,:cccclloooolccoxxdc,'....    ....';:ldxkkxdoool;,..
ccccccccccccccccccccccccccccccccccccccccccccccccc:::::ccc::::::::::o0XXXK0Okxoolc:;:lkKNWWNNNNXXXK0OOkxdolc::cdOKNNNNNXXXK0Okxdol::;,,;lxOKKKKXXNNWNX0kxddoooooooodoodddddxxxxxxdolc,........,,;::ccllllcloxxkkxdoollc:;,,,;;::clodddxxddddoc;'.
ccccccccccccccccccccccccccccccccccccccccccccccccc::::::c:::::::::::o0XNNXXK0kxxdolcclkKNWWWWWWWNNXK0Okxxdol:::okKXNNNNNXXXK0Okxdlc:;;,:ox0KXXXNNWWWWNK0kddoolcccllllloooooddddddddol;.........',;:::cccccldxxkkxxkxxxxdoooodoooodoolloooodxxoc,'
::cccccccccccccccccccccccccccccccccccccccccccccc::::::ccc::::::::::o0XNNNNXKOkkxdolclx0XNWWWWWWNNXK0Okkxdolc::lx0XNNNNNNXXK0Okxdocc:;;:lx0XNNNNWWWWWNX0kxdoolcccccccccccclllllllloolc,.........',;;:::c:cldxxxxxxxxxxddoddddoloolc:;;:cclodxxo:'
c:ccccccccccccccccccccccccccccccccccccccccccccccccc:::cccccccc:::::o0NNNNNXK0OOkxdolldkKNWWWWWNNNXK0OOkxxdoc::cdOKXNNNNXXKK00Okxolc::;;cdOXNNNWWWWWWNX0Oxdooolcc:cccccc::ccccccccccccc;,'........',,;;::clllllllooooollccc:;''........',:odxxdc;
cccccccccccccccccccccccccccccccccccccccccccccccccccc:cccc:cccc:::::o0NWWNNNX0OOkkxxdoox0XWWWWWWNNXKK00OkkxdolccokKXXXXXKKKKK0Okxolc::;;cdOXNNWWWWWWWWNKOxdoooolc::ccccc:cccccccccccccccc:'.........'';;::::::;;;;;;,,'......      ...',;codxxdl;
cccccccccccccccccccccccccccccccccccccccccccccccccccccccc::ccc::c:::o0NWWWNNXK0OOkkxdoldkKNWWWWWWWWWWNNNNNXXKK000KNNNNNXKKK000Okxdlc::::cdOXNWWWWWWWWWNX0kdolllolc:ccccccccc:::::::ccccccc;'.........',;::;,,'......     ....'.'';:clooooddxxxdl:
cccccccccccccccccccccccccccccccccccccccccccccccccccccccc:::cc::c:::lONWWWNNXK00OOkxxdodkXWMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWNNXK0Oxdlcc:::cokXNWWWWWWWWWWX0kxoollodoc:::ccc:::::::::cccc:cc::;'........'';ccc;,..........',;cclllodddddoooddxdxxdl;
cccccccccccccccccccccccccccccccccccccccccccccc::cccccccc:::cccccc::lONWWXKKKKK00OOkkO0KNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWNX0kdllccccokKNWWWWWWWWNNKOddoolllodl:::cccc::cccc::ccccc::::::;........',coollc;;:cccclloooooollcllllllodddooool:,
cccccccccccccccccccccccc:;;;::ccccccccccccccccc::ccccccccc::cccccc:lkXKkl::oOKKKXXNNWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWNKOxdooox0XWWWWWWWXOoll::;:cllloddlcc::ccccccccc:ccccccc:ccc:;'......',:looooolollcccllcc;;;,',;:::cloddolccc:,.
ccccccccccccccccccccc:;;,cdl;;:ccccccccccccccc:;;;;:cccccc:::::::::colc:cxd:ckXWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNXKKKXNWWWWWWWXd:coclkxc;:clodxxl::::::::::::::::ccc::cc::::,.....'',:llooooollc::;;,,,,,,,;;;:clddxdolc:;,..
ccccccccccccccc::ccc:;;looOKxc;:ccccccccccccc:,:ol;;::c:;;::::::;:::;:dOxkKOcckOxx0NWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNXKKKKKKKKKKk:,o00xd0Oc,:llodxd:;:::::::::::::;;;:cccccccc:,....''',;::::::cccc::;;;;::;,col::oxxxdoc:,'...
cccccccccc:cccc:::cc:,;xKOox0d;;:ccccccccccc:,cONKo,;:c;,lO0000000000kxOKkxko;;;cccoONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMW0occcccccccccc:::oOkcco:;clloddl;lO000000000000x:,:ccccccc::;,,;;;;:::cclol:;:cllllclldd:,xN0c;oxxxdoc;'.. .
cccccc::cc::::ccccc:;,,;oKOl:;;::ccccccccc:;;l0NKd;;:cc;,lk0000000000kccxxc::;,l0XOl:o0WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNx:o0XXXXXXXXXXXXXXXXx;,clooooddl;lk00000000000Od:,:cccc:c:;,,o0KKXXXXXXNNNNOc,coodddddxd:,xWOc;looddl:,..  .
cccccc::::;;:;;:::;;cxx:,:c;;:ccccccccccc:;:dXNOc;;:ccc:;;:::::::::llllc::lOKx:ckXWNkccxXWMMMMWX00XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNk::oxxxxxxxxxxxxxONWO::dxddddddo::::;;;;;;;;;;;;,;:ccccc:'..,lO0OOkkxxxd0WWO:,codddddddo:,xW0:,;:ccc:,..   .
ccccccccc;,o0kc,;:;;dXNO:,;:ccccccccccc:;;o0NXx:;:cc:;;:;;;;;;;;;;:lllolccod0X0d:ckNWO::kWMMMWOc::lkXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXkdoooooooooooo:;lKNx:lxkxxddoc::::::;;;;;;:::;;:;;:ccc;.. ..',''''..',c0NO:,;clooodool:;,xWO:,,,;;,..     .
cccccccc:,:OWXo,;::;;xNWO:,:c::cccccc;;;lONWKo;;:cc:,:xkkkkkkkkkkkkkkkkkkko,lKWW0o:ldl:dXWMMWOcck0o;oXMMMMMMMMMWXOOOOO0XNWMMMMMMMMMMMMMWWMMMMWWMWWXo:xNXl;oOOkkxdl;lOKKKKKKKKKKKKKKKKk:,::'.          ...,c0NOc'..',;:ccc:;,,,xWXxc;,''..      .
cccccccc;,oXWk;;:cc:;;xNWk:;:cccc::;;:d0NNWW0:;:ccc:,cOKKKKKKKKXWWNXKKKKKXx;cKWMWNOolo0NMMMW0c:kNXd;dNMMMMMMMMW0c:oddl:lxKWMMMMMMMMMMMMMMMMMMMMMMWO:c0WOc:k00OOkxl:cdkkkkkkkkkkkkkOXWKl''..            .,l0NOc'.  ....'''...',xWWWNKOdl;'..  ..,
ccccccc:,c0WKl,:cccc:,;kNNk;;:::;;:lkKNNOokW0:;:cccc;;::::::::;lKNk::lllllllkNMMMMWWNWWMMMNOc:kNNx;oKWMMMMMMMMWO:lKWW0c:o0WMMMMMMMMMMMMMMMMMMMMMW0l:kNXo;dKKK00Okdl::::::clc:;;;;,c0Nk;..            ..;dXWXd,'..............,xWKkk0XNNKko:'..'l
cccccc:;;xNNx;;:cccc::,c0WXd;;;,:xKNWKkl;,xN0:;:cccccc:::cc:c:,cKNx;oKXXXXXNWMMMMMMMMMMMW0o:l0NXd:oKWMMMMMMMMMWO:lXWMKc:o0WMMMMMMMMMMMMMMMMMMMMNOl:xNNd;l0XXXKK0Okxdoooodk0Odc::;;dNXo,..          ..'cONNNWXkc'....       ..,xW0:,;cdOXWNd;'.:k
cccccc;;dXNk:;:ccccccc;,oXWKc;;,:kKOo:;;;;xN0:;:ccccccccccccc:,lXXo:xNMMMMMMMMMMMMMMWWXOocckXWKo:dKWMMMMMMMMMMWO::dxxo;:d0WMMMMMMMMMMMMMMMMMWNKdclONXx:l0NNXXXXK0OkxdooodxOOxc;,,lKNk;'.          .':xXN0olkXWKd;'...........,xW0:'...,:od:,,cxK
cccc:;;dXW0c,:cccccccc:;;xWWk;;:;;:;;::c;;xN0:;:ccccccccccccl:;xN0:c0WMMMMMMWWNXXK0kxoccoOXWXkc:xNMMMMMMMMMMMMMNOdoooodOKNMMMMMMMMMMMMMMMWNKkoclkXNKo:o0NNNNNNXXK0OkxdoooxkOxc,,oKNO:'..       ..':d0NXx:'',lONW0l,,codoolll;,xWO:;:::cc::::lxkO
ccc:;;dXWKl,;cccccccccc:,lKWKl,:c:::cccc;;xN0:;:cccccccccccc:;oXNx;oXMMMMMMMKdcclllodx0XWWXkl:o0WMMMMMMMMMMMMMMMMMMMWWWMMMMMMMMMMMMMMWN0xdlcld0NWXxccxXWNNNNNNNXK00kxdooodxdc:ckXNk:'..      ..':d0NNOl,...';:oKWNk::dOOOOOkc,xW0c:dkkkkkxddddoo
ccc:;;lOOl,;cccccccccccc;;oxo:;:cccccccc;;xW0:;:cccccccccc:;;oKNO:l0WMMMMMMM0::kKXNWWWNKkoccd0NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:;oOKNWN0dccdKWWWNNNNNNXXK0Okxdlc:::cokXNKd;'...     ..,o0NNOl,...'codl:ckNNO:;oOOOOkc,xW0c:dkxxxxxdoolcc
lccc::;;;;:ccccccccccccc:;;;;::ccccccccc;;xW0:;:ccccccccc:,;dXNk:cOWMMMMMMMMXo:d0OkxdolcldOXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKo:dKKOdlclkKWMMMWNXKK0000OOkxdl:,cdOKNNKxc,....      ..,ckkl;...'cdkOOOkc:oxo:cxOOOkxc,xNOc:odddoollcc:;,
ccccccc::ccccccccccccccccccccccccccccccc;,oOd;;ccccccccccc;;lkd:l0WMMMMMMMMMW0l::clodk0KNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXx::clokKNMMMMMMWX0kkOOOOOOOkxoc;ckXKkoc:;'.......     ..',...,cdxOO00KKOdc:cdkOOOkkxc,col;:cllcc:;,,''.'
cccccccccc:::::cc:cccccccccccc:::ccccc:::;;;;,:clcccccccccloc;:oOXXXXXXXXNNWMNKkk0XNWMMMMMMWNXXXNWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNNXKOkO0XXXXNNWWMWN0kxxkO000KK0Okdl:;:::ldko;..........     ..,lxkO0OOkkxxkOOOO0OOkxxdol:;;;:::;;;,''.....'
ccc:;;:;;;;:;;;;;;;;:ccc:;;;;;;;;;;;;;:;;;;;;;;;:cccccccc;;cc::ccccccccccccxOdcclccoKWMMMMNklccccoKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNOlcccccccllcccloxKNN0Oxl:ccccoOK0kxdlccloxOOxc'..':cl:,.    .'lxO00OdlccloolccoxOkxl::::::::;;;;;;;;;,,,'...
ccc;,o0KKKKKKKKKK0xc;:oxl,l0KKKKd;,:x0KKKKKKKKOdc;:ccccc:,ckKKKKKKKKKKKKKk:cl,cOK0d;oXMMMMKl:xK0o:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl;d0KKKKKKKKKK0xc:dKNKk::kKOc;xKOkxdolclodkOOd;..;xXNOc.. .'cdO0K0xc:d0XNNNX0xc:ldo:;xKKKKKKKKKKKKKKKKKkc'..
llc;,xWMMMMMMMMMMMWXd;lko,dWMMMWO;,:OWMMMMMMMMMWXx:;:cll:,lKWMMMMMMMMMMMMKccl,lXMMKl:kNMMMKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWMMMMMMMMMMMWKo;oKNOcc0MXl,x0OxdollllldxkOkl,,:OMMXl'.':dO0KK0d;c0WMMMMMMMWXd;:c::OWMMMMMMMMWMMMMMMW0c'.
ccc;,xWWX0000000KNWWNd;ll,oNWMWWk;,:OWWX00000KNWMWOc;:cl:,lKWNK0000000000x:cl,cXMMWO:lKWMMKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWWX0000000KNWMXo:xX0cc0MXl,okxdoolllloodxkkd:;oXMMWk:;ok0KKKKk:cOWWWXOkk0NWWNd;;;;d000000KNMWK000000x:'.
llc;,xWWk;;::::::l0WW0c;:,;dXWNk:,,:OWWx;:::::lONMW0c;:l:,lKWXo:coooooooolokx;cXMMMXo:xNMMKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNk::looolco0WW0c:O0cc0MXl,oxdolccccclodxkkd:;kWMMWKl;dKKXKK0l;xWWXd:::::lOWWKl,,',,,,,,;lKWXo;,,;;:;;c;.
llc:;xWWx,;cllcc:,lKWNd,::,c0WNd;;;:OWWx,:lllc;;kNMWk;;c:,lKWXo:kXNNWNNNNNWWO,cXMMMW0:c0WMKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:oKNNNXx:lXMXl;xOccKMXl,okdlc:;;;;:codkko;c0WWWWNd;oKXXXKkc:0WNd;cdddl;c0WWx;'...   .'lKWXo'...,;:lxo'
llc;,xWWx,;lllllc;:OWWx,;c;c0WNo;;;:OWWx,:llooc;:kNMXo;::,lXWXocOWMMMMMMMMMMO;cXMNNWNx;oXMKccOWNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dNMMMMKc:0MNd:dkcc0MXl,dOkdoc:;,,,;:codc;dNWK0NWO:cOXXXKx:lXWKl;odool:,dNW0c'.     .'lKWXo'. ..,cdOx;
llc;,xWWx,;lllllc;;kWWx,:c;c0WNd;:;:OWWx,:looooc,lKWWk;::,lXWXocOWMMMMMMMMMMO;cXMKOXWKc:OWKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dNMMMMKc;OWNd:dkcc0MXl,dOkdolcc:;;,,,;:;:OWNxoKWXl;xXXXKd:oXWKl;ldol:;,cxkd:'.     .'lKWXo'.  ..:oxd:
ddo:,xWWx,:lllllc;:OWWd,:c;c0WNd;:;:OWWx,:looool;;kWM0:;:,lXWXocOWMMMMMMMMMMO;cXMKdkNWk;lXKcc0WNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dNMMMW0c:0MXo;xkcc0MXl,okxollcccccc:;;,,oKWKl;kWNx;o0XXKx:lKWNd;:lc;'..'.....      .'lKWXo'.    .':l:
Okxc,xWWx,:lllll:,oXWXl;cc;c0WNd;:;:OWWx,:looooo:,oNMXo;;,lKWXo:kXNNNNNNNNWWO;cXMKllKWXo:k0cc0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:oKNNNKd:dXMKc;kOcc0MXl,oxdoolccccccc::;,xWWO:,dNW0c:kKK0x::OWWXxc;,'.....          .'lKWXo'.      .;:
Okxc,xWWx;;;;::;:oKWNx;:lc;c0WNo;:;:OWWx,:ooooooc,lXMNd;;,lXWXo;:cccccccccxXO;cXMKl;dNWO:cxcc0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNk::clllc:dKWNx:oKOcc0MXl,okdollc::;;;;::;:0MNd,,c0MNo,o0Okxl;oKWMWXkl,...            .'lKWXo'.       .'
OOOl,xWWX0000000KNWNk::odl;c0WNo;:;:OWWx,:odddddl,cKWNx;;,lXWNX000000000kccOk;lXMKl,c0WXd:c:c0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWWNK000000KNWXx:c0NO:c0MXl,oxolcc:;;;:codl;dXWKc,;;kWWk:cxOxdo:;l0NMMWNOo;..           .'lKWXo'.        .
OOkc,xWMMMMMMMMMMMW0c,lOko;c0WNd;:;:OWWx,:ddddddl;c0WWx;,,lXWMMMMMMMMMMMXocOk;cXMKl,,dXW0c,,c0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWMMMMMMMMMMMWO:;xNNO:c0MXl,ldoc::::cldxkxc;OWNk:co:oXWKl,oxdoll:;;oONMMWN0o;'.         .'lKWXo'.
kkxc,xWMNKKKKKKKXNWW0l;okd;c0WNd;:::OWWx,cddxddxo;c0WWx;,'lXMWXKKKK00K0KkccOk;lXMKl,,:OWNx;'c0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWWNKKK0KKKXWMWOc:kXO:c0MXl,oxdolodxkkkxxl;lKMXo,ox::OWNd;coolcc:;'';lkXWMWN0d;..       .'lKWXo'.
ddd:,xWWk;::ccccco0WWKl;ll;c0WNd;c::OWWx,cxkkkkkd;c0WWx;;,lXMXd::ccllooollxXO;cXMKc:c;lXWKl,:0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNkc:lllolcoKWW0c:kO:c0MXl,oOkkxkOkxoooo:;xNWKl;cc::kWW0c;llc:;,.....,ckXWMWNOc'.      .'lKWXo'.
ddd:,xWWx,:odddol;c0WWk;:l;c0WNd;c::OWWx,lkOOOOOd;lKWNd:;,lKWXl;codkKXNNNNWWO;lXMKccxl:xNWk;:0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:oKNNNXx:cKMNx:oxccKMXl,okkxddollloxd::OWWNXKKKKKNWMXo,::;,...  ....'cxXWMWk:..     .'lKWXo'.
xxd:,xWWx,cdxxxxdc,dNWKc:c:c0WNd;c::OWWx,ckkkkkkl,oXMXo::,lXWKl;lddxOXWMMMMWO;cXMKccOO:c0WXocOWNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dXMMWWXo:xNWKc:occKMXl,lddollllodO0d;oXMMMMMMMMMMMMNx;,,''''.....   .,cOWMNo'.     .'lKWXo'.
xxdc,xWWx,cdxxxxxl,oXWXo;c;c0WNd;c::OWWx,cxkkkkdc;xNM0c:c,lXWKl:oxxxxk0XWWWWO,cXMKccOXd;dNWOo0WNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dXWWWWNx:oXMXc,l:cKMXl,lxxxxxxkO0K0c;kWWX0OOOOOOOKWWKc''',lkxc,.     .'c0WWx,..    .'lKWXo'.
xkxc,xWWx,cxxxkkko,oXWXo;::c0WNd;c::OWWx,cxkkkko;c0WWk;:l,lXWKl:oxxxxxkkOKNWO;lXMKccOW0:c0WXOKWNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dXWWWWNk:oXMXl,l:c0MXl,d0KKK000KKKk:c0MNd,;cccc:,lKMNo,'';kWNx,.      .,xWWk;..    .'lKWXo'.
kkxc,xWWx,cxkkkkkl,oNMXl;c:c0WNd;c::OWNx,ckkkkxc;xNMXo;lo,cKWKl:okkkkkkkxdkKk;lXMKcc0WNx;oXWNNWNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:dXWWWWXd:xNMKc;l:c0MXl;kNXXXKKKKK0o;dNWKl;d00Okd:;kWWO:'';xWWO:.      .;kWWx;.     .'lKWXo'....
kkxc,xWWx,cxkkkko::OWW0::c;c0WNo;c::OWWx,ckkkdc;dXWWO::xo,lKWKl:okkkkkkkkxddo,cXMKlc0WMKl:kWMMMNx:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNx:oKNNNNk::0MWOccd:c0MXl;kXXXKKKKKKk:;OWNk:ckOOxdo:;oXWXl'',dNWXo'.    .,lKWNo'.     .'lKWXo'. ....
kkxc,xWWx,;:ccc:;ckWMWx,;;;oKWNk:;,:OWWx,;ccc:cxXWWKc;okd,lKWKo;:ccccccccc:::,lXMKcc0WMWk:lKWMMNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:OWNk::lllol:cONMNd;dx:c0MXl,:lllllllll:;oXMXl,lddol:;,,:0MNx;,,:0WW0l,....,l0WWKc'.     .'lKWXo'.  .......
kkkc,xWWXOOOkkOOOXWMW0c;;,oXWMWWk;,:OWWXOOOOO0XWMWKo;lkOd,lXMNKOOOOOOOOOOko,,'lXMKcc0WMMXo:xNMMNd:xNMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWMXl:OWWXOkOOOOO0XWMWO:cOO:c0MWKOOOOOOOOOOko:xWW0c,:c:;,....;xNW0c'',oXMMN0xddxOXWWXd,.      .'lKWXo'.    .....
OOkc,xWWMMMMMMMMMMMW0c;oc,dWMMMWO;,:OWMMMMMMMMMMNOl;okOOd,lXMMMMMMMMMMMMMWO;''lXMKccOWWWWO:c0WMNd:xNMMMMMMMMMMMMMMMMMMMMMMMMWWWWWWWXl:OWMMMMMMMMMMMMNOc:kXO:cKMMMMMMMMMMMMMWOoOMNd,''.....  .,lKWNo,'.,lKWMMMMWWWMWXd;.       .'lKWXo'.        .
OOkc,dXXXXNNNXXXXKOd::okl,oXXXXXx;,:xXXXXXXXXX0ko:cdkOOOd,c0XXXXXXXXXXXXXXx;,,c0NOc:ONWWNKd:oKXKo:xNWMMMMWWWWWWWWWWWWWWWWWWWWWWWWWWXl;xXXXXNNNXXXXKOd:cOXNO::OXXXXNXXNNXXXXXkx0X0c'..        .;xXKx;...':d0NWWWWNKkl,.        .'cOX0l'.
OOOo:clllllllllllc:cokOOd::cllllc:::cllllllllcccldkOOOOOxc:cllllllllllllllc:cc:clc:oKNWNNX0oclllclOXNXXXXXXXXKKKKKKXXNNNWWWWWWWWWWWNkcclllllllllllllokKNNX0o:cllcccccccccccc:;::;,..         .';::;'.   ..,cloolc;'..          .,;::,.
OOOOkxxxxxxxxxxxxxkOOOOOOkxxxxxxxkkkxxxxxxxxxxkOOOOOOOOOOkxxxxxxxxxxxxxxxxxkkkxxxk0XNNWNNNNXK0000KXXKKK0000000000KKKXNNNNNWWWWWWWWWWNXKKKKKKKKKK0KKXNNNNXKK0kxddoollc::;;;,''.....            ....         .......
O
*/