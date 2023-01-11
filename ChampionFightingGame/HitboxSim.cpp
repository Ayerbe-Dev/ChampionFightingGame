#include "Battle.h"

HitboxSim::HitboxSim() {
	active_cat = 0;
	for (int i = 0; i < 3; i++) {
		active_box[i] = 0;
		for (int i2 = 0; i2 < 10; i2++) {
			anchor[i][i2] = glm::ivec2(0);
			offset[i][i2] = glm::ivec2(0);
		}
	}
}

void HitboxSim::init() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < 10; i2++) {
			boxes[i][i2].init();
		}
	}
	for (int i = 0; i < 10; i++) {
		boxes[0][i].set_rgba(glm::vec4(255, 0, 0, 127));
		boxes[1][i].set_rgba(glm::vec4(0, 255, 0, 127));
		boxes[2][i].set_rgba(glm::vec4(0, 0, 255, 127));
	}
}

void HitboxSim::destroy() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < 10; i2++) {
			boxes[i][i2].destroy();
		}
	}
}

void HitboxSim::render() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < 10; i2++) {
			boxes[i][i2].render();
		}
	}
}

void HitboxSim::print(Fighter* fighter) { //Prints the info required to generate a hitbox at the given
	//coordinates. Also makes some assumptions about the rest of the hitbox's behavior, and requests
	//user input to determine the rest.
	glm::ivec2 anchor = this->anchor[active_cat][active_box[active_cat]] - glm::vec2(fighter->pos);
	glm::ivec2 offset = this->offset[active_cat][active_box[active_cat]] - glm::vec2(fighter->pos);
	anchor.x *= fighter->facing_dir;
	offset.x *= fighter->facing_dir;
	bool command_loop = true;
	std::string command_input;
	float command_input_f;
	switch (active_cat) {
		case 0: { //Hitboxes
			int attack_strength;
			int attack_kind = fighter->fighter_int[FIGHTER_INT_ATTACK_KIND];
			switch (attack_kind) {
				case (ATTACK_KIND_LP):
				case (ATTACK_KIND_LK):
				case (ATTACK_KIND_CLP):
				case (ATTACK_KIND_CLK): {
					attack_strength = 1;
				} break;
				case (ATTACK_KIND_MP):
				case (ATTACK_KIND_MK):
				case (ATTACK_KIND_CMP):
				case (ATTACK_KIND_CMK): {
					attack_strength = 2;
				} break;
				case (ATTACK_KIND_HP):
				case (ATTACK_KIND_HK):
				case (ATTACK_KIND_CHP):
				case (ATTACK_KIND_CHK): {
					attack_strength = 3;
				} break;
				case (ATTACK_KIND_SUPER_236236):
				case (ATTACK_KIND_SUPER_214214): {
					attack_strength = 5;
				}
				default: {
					attack_strength = 4;
				}
			}
			int multihit;
			float damage;
			float chip_damage;
			float meter_gain;
			int damage_scale;
			std::string situation_hit;
			std::string attack_level;
			std::string attack_height;
			int hitlag;
			int blocklag;
			int hitstun;
			int blockstun;
			float hit_pushback;
			float block_pushback;
			std::string hit_status;
			std::string counterhit_status;
			std::string counterhit_type;
			int juggle_start;
			int juggle_increase;
			int juggle_max;
			std::string ko_kind;
			std::string continue_launch;
			std::string disable_hitstun_parry;
			std::string launch_info;
			damage = 20 + (10 * attack_strength);
			hitlag = 6 + (2 * attack_strength);
			blocklag = 4 + (4 * attack_strength);
			switch (attack_strength) {
				case 1: {
					continue_launch = "false";
					attack_level = "ATTACK_LEVEL_LIGHT";
					hit_pushback = 10.0;
					block_pushback = 10.0;
					damage_scale = 2;
					hit_status = "HIT_STATUS_NORMAL";
					counterhit_status = "HIT_STATUS_NORMAL";
					counterhit_type = "COUNTERHIT_TYPE_NORMAL";
					ko_kind = "KO_KIND_NORMAL";
				} break;
				case 2: {
					continue_launch = "false";
					attack_level = "ATTACK_LEVEL_MEDIUM";
					std::cout << "Hit Pushback?\n";
					std::cin >> hit_pushback;
					std::cout << "Block Pushback?\n";
					std::cin >> block_pushback;
					damage_scale = 1;
					hit_status = "HIT_STATUS_NORMAL";
					counterhit_status = "HIT_STATUS_NORMAL";
					counterhit_type = "COUNTERHIT_TYPE_NORMAL";
					ko_kind = "KO_KIND_NORMAL";
				} break;
				case 3: {
					continue_launch = "true";
					attack_level = "ATTACK_LEVEL_HEAVY";
					std::cout << "Hit Pushback?\n";
					std::cin >> hit_pushback;
					std::cout << "Block Pushback?\n";
					std::cin >> block_pushback;
					damage_scale = 1;
					if (attack_kind == ATTACK_KIND_CHK) {
						hit_status = "HIT_STATUS_KNOCKDOWN";
						counterhit_status = "HIT_STATUS_KNOCKDOWN";
					}
					else if (attack_kind == ATTACK_KIND_CHP) {
						hit_status = "HIT_STATUS_NORMAL";
						counterhit_status = "HIT_STATUS_LAUNCH";
					}
					else {
						std::cout << "Hit status? (name of the status, or \'no\' for default)\n";
						while (command_loop) {
							std::cin >> command_input;
							for (int i = 0; i < command_input.length(); i++) {
								command_input[i] = std::toupper(command_input[i]);
							}
							if (command_input == "NO" || command_input == "N") {
								hit_status = "HIT_STATUS_NORMAL";
							}
							else {
								if (!command_input.starts_with("HIT_STATUS")) {
									if (command_input.starts_with("STATUS")) {
										command_input = "HIT_" + command_input;
									}
									else {
										command_input = "HIT_STATUS_" + command_input;
									}
								}
								hit_status = command_input;
							}
							command_loop = false;
						}
						command_loop = true;
						std::cout << "Counterhit status? (name of the status, or \'no\' for default)\n";
						while (command_loop) {
							std::cin >> command_input;
							for (int i = 0; i < command_input.length(); i++) {
								command_input[i] = std::toupper(command_input[i]);
							}
							if (command_input == "NO" || command_input == "N") {
								counterhit_status = "HIT_STATUS_NORMAL";
							}
							else {
								if (!command_input.starts_with("HIT_STATUS")) {
									if (command_input.starts_with("STATUS")) {
										command_input = "HIT_" + command_input;
									}
									else {
										command_input = "HIT_STATUS_" + command_input;
									}
								}
								counterhit_status = command_input;
							}
							command_loop = false;
						}
						command_loop = true;
					}
					std::cout << "Counterhit Type? (name of the type, or \'no\' for default)\n";
					while (command_loop) {
						std::cin >> command_input;
						for (int i = 0; i < command_input.length(); i++) {
							command_input[i] = std::toupper(command_input[i]);
						}
						if (command_input == "NO" || command_input == "N") {
							counterhit_type = "COUNTERHIT_TYPE_NORMAL";
						}
						else {
							if (!command_input.starts_with("HIT_STATUS")) {
								if (command_input.starts_with("STATUS")) {
									command_input = "COUNTERHIT_" + command_input;
								}
								else {
									command_input = "COUNTERHIT_TYPE_" + command_input;
								}
							}
							counterhit_type = command_input;
						}
						command_loop = false;
					}
					command_loop = true;
					ko_kind = "KO_KIND_NORMAL";
				} break;
				case 4:
				case 5: {
					std::cout << "Damage?\n";
					std::cin >> damage;
					std::cout << "Damage Scale?\n";
					std::cin >> damage_scale;
					continue_launch = "true";
					attack_level = "ATTACK_LEVEL_HEAVY";
					std::cout << "Hit Pushback?\n";
					std::cin >> hit_pushback;
					std::cout << "Block Pushback?\n";
					std::cin >> block_pushback;
					std::cout << "Hit status? (name of the status, or \'no\' for default)\n";
					while (command_loop) {
						std::cin >> command_input;
						for (int i = 0; i < command_input.length(); i++) {
							command_input[i] = std::toupper(command_input[i]);
						}
						if (command_input == "NO" || command_input == "N") {
							hit_status = "HIT_STATUS_NORMAL";
						}
						else {
							if (!command_input.starts_with("HIT_STATUS")) {
								if (command_input.starts_with("STATUS")) {
									command_input = "HIT_" + command_input;
								}
								else {
									command_input = "HIT_STATUS_" + command_input;
								}
							}
							hit_status = command_input;
						}
						command_loop = false;
					}
					command_loop = true;
					std::cout << "Counterhit status? (name of the status, or \'no\' for default)\n";
					while (command_loop) {
						std::cin >> command_input;
						for (int i = 0; i < command_input.length(); i++) {
							command_input[i] = std::toupper(command_input[i]);
						}
						if (command_input == "NO" || command_input == "N") {
							counterhit_status = "HIT_STATUS_NORMAL";
						}
						else {
							if (!command_input.starts_with("HIT_STATUS")) {
								if (command_input.starts_with("STATUS")) {
									command_input = "HIT_" + command_input;
								}
								else {
									command_input = "HIT_STATUS_" + command_input;
								}
							}
							counterhit_status = command_input;
						}
						command_loop = false;
					}
					command_loop = true;
					std::cout << "Counterhit Type? (name of the type, or \'no\' for default)\n";
					while (command_loop) {
						std::cin >> command_input;
						for (int i = 0; i < command_input.length(); i++) {
							command_input[i] = std::toupper(command_input[i]);
						}
						if (command_input == "NO" || command_input == "N") {
							counterhit_type = "COUNTERHIT_TYPE_NORMAL";
						}
						else {
							if (!command_input.starts_with("HIT_STATUS")) {
								if (command_input.starts_with("STATUS")) {
									command_input = "COUNTERHIT_" + command_input;
								}
								else {
									command_input = "COUNTERHIT_TYPE_" + command_input;
								}
							}
							counterhit_type = command_input;
						}
						command_loop = false;
					}
					command_loop = true;
					if (attack_strength == 5) {
						std::cout << "Can KO? (yes/no)\n";
						while (command_loop) {
							std::cin >> command_input;
							if (command_input == "yes" || command_input == "y") {
								ko_kind = "KO_KIND_CHIP";
								command_loop = false;
							}
							else if (command_input == "no" || command_input == "n") {
								ko_kind = "KO_KIND_NONE";
								command_loop = false;
							}
							else {
								std::cout << "\'" << command_input << "\' unrecognized\n";
							}
						}
					}
					else {
						ko_kind = "KO_KIND_CHIP";
					}
				} break;
			}

			std::cout << "Multihit? (yes/no)\n";
			while (command_loop) {
				std::cin >> command_input;
				if (command_input == "yes" || command_input == "y") {
					multihit = 1;
					command_loop = false;
				}
				else if (command_input == "no" || command_input == "n") {
					multihit = 0;
					command_loop = false;
				}
				else {
					std::cout << "\'" << command_input << "\' unrecognized\n";
				}
			}
			command_loop = true;
			std::cout << "Chip Damage? (enter as % of damage)\n";
			std::cin >> command_input_f;
			chip_damage = (command_input_f / 100.0) * damage;
			meter_gain = damage * 0.8;
			std::cout << "Hits which situations? (ground/air/both/otg/all)\n";
			while (command_loop) {
				std::cin >> command_input;
				for (int i = 0; i < command_input.length(); i++) {
					command_input[i] = std::tolower(command_input[i]);
				}
				if (command_input == "ground" || command_input == "g") {
					situation_hit = "SITUATION_HIT_GROUND";
					command_loop = false;
				}
				else if (command_input == "air" || command_input == "a") {
					situation_hit = "SITUATION_HIT_AIR";
					command_loop = false;
				}
				else if (command_input == "both" || command_input == "b") {
					situation_hit = "SITUATION_HIT_GROUND_AIR";
					command_loop = false;
				}
				else if (command_input == "otg" || command_input == "o") {
					situation_hit = "SITUATION_HIT_OTG";
					command_loop = false;
				}
				else if (command_input == "all" || command_input == "a") {
					situation_hit = "SITUATION_HIT_ALL";
					command_loop = false;
				}
				else {
					std::cout << "\'" << command_input << "\' unrecognized\n";
				}
			}
			command_loop = true;
			std::cout << "Attack Height? (low/mid/overhead)\n";
			while (command_loop) {
				std::cin >> command_input;
				for (int i = 0; i < command_input.length(); i++) {
					command_input[i] = std::tolower(command_input[i]);
				}
				if (command_input == "low" || command_input == "l") {
					attack_height = "ATTACK_HEIGHT_LOW";
					command_loop = false;
				}
				else if (command_input == "mid" || command_input == "medium" || command_input == "m") {
					attack_height = "ATTACK_HEIGHT_MID";
					command_loop = false;
				}
				else if (command_input == "overhead" || command_input == "high" || command_input == "o" || command_input == "h") {
					attack_height = "ATTACK_HEIGHT_HIGH";
					command_loop = false;
				}
				else {
					std::cout << "\'" << command_input << "\' unrecognized\n";
				}
			}
			command_loop = true;
			if (attack_strength >= 4) {
				std::cout << "Hitstun?\n";
				std::cin >> hitstun;
				std::cout << "Blockstun?\n";
				std::cin >> blockstun;
			}
			else {
				std::cout << "Hit Advantage?\n";
				std::cin >> hitstun;
				hitstun -= fighter->get_frames_until_actionable();
				std::cout << "Block Advantage?\n";
				std::cin >> blockstun;
				blockstun -= fighter->get_frames_until_actionable();
			}
			std::cout << "Juggle Start?\n";
			std::cin >> juggle_start;
			std::cout << "Juggle Increase?\n";
			std::cin >> juggle_increase;
			std::cout << "Juggle Max?\n";
			std::cin >> juggle_max;

			if (continue_launch == "false" && hit_status != "HIT_STATUS_LAUNCH" && counterhit_status != "HIT_STATUS_LAUNCH") {
				launch_info = "10.0, 0.0, 0.0, 1.0";
			}
			else {
				std::cout << "Launch Info? (y initial speed, gravity, max fall speed, x speed)\n";
				float y_init, grav, fall, x_speed;
				std::cin.setf(std::ios::fixed, std::ios::floatfield);
				std::cin >> y_init >> grav >> fall >> x_speed;
				launch_info = std::to_string(y_init) + ", " + std::to_string(grav)
					+ ", " + std::to_string(fall) + ", " + std::to_string(x_speed);
			}

			std::cout.precision(1);
			std::cout << "Script: " << fighter->active_move_script.name << ", frame: " << fighter->frame << "\n";
			std::cout << "push_function(&Fighter::NEW_HITBOX, /*ID*/ " << active_box[active_cat] << 
				", /*Multihit ID*/ " << multihit << ", /*Damage*/ " << damage << ", /*Chip Damage*/ "
				<< chip_damage << ", /*Damage Scale*/ " << damage_scale << ", /*Meter Gain*/ " << 
				meter_gain << ", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2(" << offset.x << 
				", " << offset.y << "), " << situation_hit << ", " << attack_level << ", " <<
				attack_height << ", /*Hitlag*/ " << hitlag << ", /*Blocklag*/ " << blocklag << 
				", /*Hitstun*/ " << hitstun << ", /*Blockstun*/ " << blockstun << ", /*Hit Pushback*/ "
				<< hit_pushback << ", /*Block Pushback*/ " << block_pushback << ", /*Hit Status*/ " 
				<< hit_status << ", /*Counterhit Status*/ " << counterhit_status << ", " << 
				counterhit_type << ", /*Juggle Start*/ " << juggle_start << ", /*Juggle Increase*/ " <<
				juggle_increase << ", /*Juggle Max*/ " << juggle_max << ", CLANK_KIND_NORMAL, " << ko_kind
				<< ", /*Continue Launch*/ " << continue_launch << ", /*Disable Hitstun Parry*/ false"
				<< ", /*Launch Info*/ " << launch_info << ");\n";

		} break;
		case 1: { //Grabboxes
			std::string grabbox_kind;
			std::string situation_hit;
			std::string atk_status;
			std::string def_status;
			if (fighter->get_anim().starts_with("grab")) {
				if (fighter->situation_kind == FIGHTER_SITUATION_AIR) {
					grabbox_kind = "GRABBOX_KIND_HITSTUN";
				}
				else {
					grabbox_kind = "GRABBOX_KIND_NORMAL";
				}
				situation_hit = "SITUATION_HIT_GROUND_AIR";
				atk_status = "FIGHTER_STATUS_GRAB";
				def_status = "FIGHTER_STATUS_GRABBED";
			}
			else {
				std::cout << "Connects during hitstun? (yes/no)\n";
				while (command_loop) {
					std::cin >> command_input;
					for (int i = 0; i < command_input.length(); i++) {
						command_input[i] = std::tolower(command_input[i]);
					}
					if (command_input == "yes" || command_input == "y") {
						grabbox_kind = "GRABBOX_KIND_HITSTUN_NOTECH";
						command_loop = false;
					}
					else if (command_input == "no" || command_input == "n") {
						grabbox_kind = "GRABBOX_KIND_NOTECH";
						command_loop = false;
					}
					else {
						std::cout << "\'" << command_input << "\' unrecognized\n";
					}
				}
				command_loop = true;
				std::cout << "Hits which situations? (ground/air/both)\n";
				while (command_loop) {
					std::cin >> command_input;
					for (int i = 0; i < command_input.length(); i++) {
						command_input[i] = std::tolower(command_input[i]);
					}
					if (command_input == "ground" || command_input == "g") {
						situation_hit = "SITUATION_HIT_GROUND";
						command_loop = false;
					}
					else if (command_input == "air" || command_input == "a") {
						situation_hit = "SITUATION_HIT_AIR";
						command_loop = false;
					}
					else if (command_input == "both" || command_input == "b") {
						situation_hit = "SITUATION_HIT_GROUND_AIR";
						command_loop = false;
					}
					else {
						std::cout << "\'" << command_input << "\' unrecognized\n";
					}
				}
				command_loop = true;
				std::cout << "Custom attacker status? (name of the status, or \'no\')\n";
				while (command_loop) {
					std::cin >> command_input;
					for (int i = 0; i < command_input.length(); i++) {
						command_input[i] = std::toupper(command_input[i]);
					}
					if (command_input == "NO" || command_input == "N") {
						atk_status = "FIGHTER_STATUS_GRAB";
					}
					else {
						if (!command_input.starts_with("FIGHTER_STATUS") && !command_input.starts_with("CHARA_")) {
							if (command_input.starts_with("STATUS")) {
								command_input = "FIGHTER_" + command_input;
							}
							else {
								command_input = "FIGHTER_STATUS_" + command_input;
							}
						}
						atk_status = command_input;
					}
					command_loop = false;
				}
				command_loop = true;
				std::cout << "Custom defender status? (name of the status, or \'no\')\n";
				while (command_loop) {
					std::cin >> command_input;
					for (int i = 0; i < command_input.length(); i++) {
						command_input[i] = std::toupper(command_input[i]);
					}
					if (command_input == "NO" || command_input == "N") {
						def_status = "FIGHTER_STATUS_GRABBED";
					}
					else {
						if (!command_input.starts_with("FIGHTER_STATUS") && !command_input.starts_with("CHARA_")) {
							if (command_input.starts_with("STATUS")) {
								command_input = "FIGHTER_" + command_input;
							}
							else {
								command_input = "FIGHTER_STATUS_" + command_input;
							}
						}
						def_status = command_input;
					}
					command_loop = false;
				}
			}
			std::cout << "Script: " << fighter->active_move_script.name << ", frame: " << fighter->frame << "\n";
			std::cout << "push_function(&Fighter::NEW_GRABBOX, " << active_box[active_cat] <<
				", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2(" << offset.x << ", " <<
				offset.y << "), " << grabbox_kind << ", " << situation_hit << ", " << atk_status << ", "
				<< def_status << ");\n";
		} break;
		case 2: { //Hurtboxes
			std::cout << "Script: " << fighter->active_move_script.name << ", frame: " << fighter->frame << "\n";
			std::cout << "push_function(&Fighter::NEW_HURTBOX, " << active_box[active_cat] <<
				", glm::vec2(" << anchor.x << ", " << anchor.y << "), glm::vec2(" << offset.x << ", " <<
				offset.y << "), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);\n";
		} break;
	}
}