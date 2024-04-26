#include "FighterMovelist.h"
#include "Fighter.h"
#include "VectorBoolPointer.h"
#include "debug.h"

FighterMoveListEntry::FighterMoveListEntry() {
	name = "";
	status_kind = BATTLE_OBJECT_STATUS_NONE;
	input_kind = INPUT_KIND_NORMAL;
	required_buttons = 0;
	num_required_buttons = 0;
	charge_req = 0;
	special_level_setting = SPECIAL_LEVEL_SETTING_N;
	recover_crouching = false;
	meter_cost_normal = 0.0f;
	meter_cost_cancel = 0.0f;
	super_meter = false;
	disable_flag = VBP();

	hit_cancel_enabled = false;
	block_cancel_enabled = false;
	cancel_enabled = false;
	special_level = SPECIAL_LEVEL_N;
	buttons_pressed = 0;
}

FighterMoveListEntry::FighterMoveListEntry(std::string name, unsigned int status_kind, unsigned short required_buttons,
	unsigned char num_required_buttons, std::set<unsigned int> stick_dirs, bool recover_crouching, 
	VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
	bool super_meter, std::set<std::string> allowed_states) {
	this->name = name;
	this->status_kind = status_kind;
	this->input_kind = INPUT_KIND_NORMAL;
	this->required_buttons = required_buttons;
	this->num_required_buttons = num_required_buttons;
	this->valid_stick_dirs = stick_dirs;
	this->charge_req = 0;
	this->special_level_setting = SPECIAL_LEVEL_SETTING_N;
	this->recover_crouching = recover_crouching;
	this->disable_flag = disable_flag;
	this->meter_cost_normal = meter_cost_normal;
	this->meter_cost_cancel = meter_cost_cancel;
	this->super_meter = super_meter;
	this->allowed_states = allowed_states;

	this->hit_cancel_enabled = false;
	this->block_cancel_enabled = false;
	this->cancel_enabled = false;
	this->special_level = SPECIAL_LEVEL_N;
	this->buttons_pressed = 0;
}

FighterMoveListEntry::FighterMoveListEntry(std::string name, unsigned int status_kind, 
	InputKind input_kind, unsigned short required_buttons, unsigned char num_required_buttons, 
	int charge_req, SpecialLevelSetting special_level_setting, bool recover_crouching, 
	VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
	bool super_meter, std::set<std::string> allowed_states) {
	this->name = name;
	this->status_kind = status_kind;
	this->input_kind = input_kind;
	this->required_buttons = required_buttons;
	this->num_required_buttons = num_required_buttons;
	switch (input_kind) {
		case (INPUT_KIND_46): {
			valid_stick_dirs = { 6 };
		} break;
		case (INPUT_KIND_28): {
			valid_stick_dirs = { 7, 8, 9 };
		} break;
		default: {
			valid_stick_dirs = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		} break;
	}
	this->charge_req = charge_req;
	this->charge_req = 0;
	this->special_level_setting = special_level_setting;
	this->recover_crouching = recover_crouching;
	this->disable_flag = disable_flag;
	this->meter_cost_normal = meter_cost_normal;
	this->meter_cost_cancel = meter_cost_cancel;
	this->super_meter = super_meter;
	this->allowed_states = allowed_states;

	this->hit_cancel_enabled = false;
	this->block_cancel_enabled = false;
	this->cancel_enabled = false;
	this->special_level = SPECIAL_LEVEL_N;
	this->buttons_pressed = 0;
}

bool FighterMoveListEntry::check_input(Fighter* fighter) {
	float meter = super_meter ? fighter->object_float[FIGHTER_FLOAT_SUPER_METER] : fighter->object_float[FIGHTER_FLOAT_EX_METER];
	float cost = (fighter->is_enable_free_cancel()) ? meter_cost_normal : meter_cost_cancel;
	short buffer_code = fighter->player->controller.get_buffer_code();
	if (special_level_setting == SPECIAL_LEVEL_SETTING_N && meter < cost) return false;
	if (required_buttons) {
		if (std::popcount((unsigned)(buffer_code & required_buttons)) < num_required_buttons) return false;
		buttons_pressed = buffer_code & required_buttons;
		if ((fighter->player->controller.get_buffer_lockout_code() & buttons_pressed) == buttons_pressed) return false;
	}
	if (!allowed_states.empty() && !allowed_states.contains(fighter->object_string[FIGHTER_STRING_MOVE_KIND])) return false;
	if (input_kind == INPUT_KIND_NORMAL) {
		if (!valid_stick_dirs.contains(fighter->get_stick_dir())) return false;	
	}
	else {
		if (!valid_stick_dirs.contains(fighter->get_buffer_stick_dir())) return false;
		switch (input_kind) {
			default:
			case (INPUT_KIND_NORMAL): {
				return false;
			} break;
			case (INPUT_KIND_236): {
				if (fighter->object_int[FIGHTER_INT_236_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_214): {
				if (fighter->object_int[FIGHTER_INT_214_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_623): {
				if (fighter->object_int[FIGHTER_INT_623_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_41236): {
				if (fighter->object_int[FIGHTER_INT_41236_STEP] != 5) return false;
			} break;
			case (INPUT_KIND_63214): {
				if (fighter->object_int[FIGHTER_INT_63214_STEP] != 5) return false;
			} break;
			case (INPUT_KIND_632): {
				if (fighter->object_int[FIGHTER_INT_632_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_46): {
				if (fighter->object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] < charge_req) return false;
			} break;
			case (INPUT_KIND_28): {
				if (fighter->object_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] < charge_req) return false;
			} break;
			case (INPUT_KIND_66): {
				if (fighter->object_int[FIGHTER_INT_66_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_44): {
				if (fighter->object_int[FIGHTER_INT_44_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_22): {
				if (fighter->object_int[FIGHTER_INT_22_STEP] != 3) return false;
			} break;
			case (INPUT_KIND_236236): {
				if (fighter->object_int[FIGHTER_INT_236236_STEP] != 6) return false;
			} break;
			case (INPUT_KIND_214214): {
				if (fighter->object_int[FIGHTER_INT_214214_STEP] != 6) return false;
			} break;
			case (INPUT_KIND_4646): {
				if (fighter->object_int[FIGHTER_INT_BACK_CHARGE_FRAMES] < charge_req
					|| fighter->object_int[FIGHTER_INT_4646_STEP] != 4) return false;
			} break;
		}
		switch (special_level_setting) {
			default:
			case (SPECIAL_LEVEL_SETTING_L_M_H_EX): {
				if (std::popcount((unsigned)buttons_pressed) > 1 && meter >= cost) {
					special_level = SPECIAL_LEVEL_EX;
				}
				else {
					cost = 0.0f;
					[[fallthrough]];
					case (SPECIAL_LEVEL_SETTING_L_M_H): {
						if (buttons_pressed & (BUTTON_HP_BIT | BUTTON_HK_BIT)) {
							special_level = SPECIAL_LEVEL_H;
						}
						else if (buttons_pressed & (BUTTON_MP_BIT | BUTTON_MK_BIT)) {
							special_level = SPECIAL_LEVEL_M;
						}
						else {
							special_level = SPECIAL_LEVEL_L;
						}
					} break;
				}
			} break;
			case (SPECIAL_LEVEL_SETTING_N_EX): {
				if (std::popcount((unsigned)buttons_pressed) > 1 && meter >= cost) {
					special_level = SPECIAL_LEVEL_EX;
				}
				else {
					cost = 0.0f;
					[[fallthrough]];
					case (SPECIAL_LEVEL_SETTING_N): {
						special_level = SPECIAL_LEVEL_N;
					} break;
				}
			} break;
		}
	}

	return true;
}

bool FighterMoveListEntry::try_change_status(Fighter* fighter) {
	if (disable_flag
	|| !((fighter->is_enable_free_cancel())
	|| (hit_cancel_enabled && fighter->object_flag[FIGHTER_FLAG_ATTACK_HIT])
	|| (block_cancel_enabled && fighter->object_flag[FIGHTER_FLAG_ATTACK_BLOCKED])
	|| cancel_enabled)) {
		return false;
	}
	fighter->object_string[FIGHTER_STRING_MOVE_KIND] = name;
	//Make sure that we can't re-buffer any actions as we transtion into them
	switch (input_kind) {
		default:
		case (INPUT_KIND_NORMAL):
		case (INPUT_KIND_46): 
		case (INPUT_KIND_28): {
			//46 and 28 are both handled by the fact that their inputs use buffer_stick_dir, which
			//resets on status change
		} break;
		case (INPUT_KIND_236): {
			fighter->object_int[FIGHTER_INT_236_STEP] = 0;
		} break;
		case (INPUT_KIND_214): {
			fighter->object_int[FIGHTER_INT_214_STEP] = 0;
		} break;
		case (INPUT_KIND_623): {
			fighter->object_int[FIGHTER_INT_623_STEP] = 0;
		} break;
		case (INPUT_KIND_41236): {
			fighter->object_int[FIGHTER_INT_41236_STEP] = 0;
		} break;
		case (INPUT_KIND_63214): {
			fighter->object_int[FIGHTER_INT_63214_STEP] = 0;
		} break;
		case (INPUT_KIND_632): {
			fighter->object_int[FIGHTER_INT_632_STEP] = 0;
		} break;
		case (INPUT_KIND_66): {
			fighter->object_int[FIGHTER_INT_66_STEP] = 0;
		} break;
		case (INPUT_KIND_44): {
			fighter->object_int[FIGHTER_INT_44_STEP] = 0;
		} break;
		case (INPUT_KIND_22): {
			fighter->object_int[FIGHTER_INT_22_STEP] = 0;
		} break;
		case (INPUT_KIND_236236): {
			fighter->object_int[FIGHTER_INT_236236_STEP] = 0;
		} break;
		case (INPUT_KIND_214214): {
			fighter->object_int[FIGHTER_INT_214214_STEP] = 0;
		} break;
		case (INPUT_KIND_4646): {
			fighter->object_int[FIGHTER_INT_4646_STEP] = 0;
		} break;
	}
	if (super_meter) {
		fighter->object_float[FIGHTER_FLOAT_SUPER_METER] = 0.0f;
	}
	else if (special_level == SPECIAL_LEVEL_EX
		|| special_level_setting == SPECIAL_LEVEL_SETTING_L_M_H
		|| special_level_setting == SPECIAL_LEVEL_SETTING_N) {
		if (fighter->is_enable_free_cancel() || cancel_enabled) {
			fighter->spend_ex(meter_cost_normal);
		}
		else {
			fighter->spend_ex(meter_cost_cancel);
		}
	}
	fighter->object_int[FIGHTER_INT_SPECIAL_LEVEL] = special_level;
	fighter->change_status(status_kind);
	fighter->player->controller.set_buffer_lockout_code(buttons_pressed);
	return true;
}

FighterMoveList::FighterMoveList() {
	move_map[""] = 0;
	move_list.push_back(FighterMoveListEntry());
}

void FighterMoveList::add_movelist_entry(std::string name, unsigned int status_kind, 
	unsigned short required_buttons, unsigned char num_required_buttons, 
	std::set<unsigned int> stick_dirs, bool recover_crouching, 
	VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
	bool super_meter, std::set<std::string> allowed_states) {
	move_map[name] = move_list.size();
	move_list.push_back(FighterMoveListEntry(name, status_kind, required_buttons, num_required_buttons, stick_dirs,
		recover_crouching, disable_flag, meter_cost_normal, meter_cost_cancel, super_meter, 
		allowed_states
	));
}

void FighterMoveList::add_movelist_entry(std::string name, unsigned int status_kind, 
	InputKind input_kind, unsigned short required_buttons, unsigned char num_required_buttons, 
	int charge_req, SpecialLevelSetting special_level_setting, bool recover_crouching, 
	VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
	bool super_meter, std::set<std::string> allowed_states) {
	move_map[name] = move_list.size();
	move_list.push_back(FighterMoveListEntry(name, status_kind, input_kind, required_buttons, 
		num_required_buttons, charge_req, special_level_setting, recover_crouching,
		disable_flag, meter_cost_normal, meter_cost_cancel, super_meter, allowed_states
	));
}

void FighterMoveList::check_inputs(Fighter* fighter) {
	for (size_t i = 0, max = move_list.size(); i < max; i++) {
		if (!buffered_moves.contains(i) && move_list[i].check_input(fighter)) {
			buffered_moves.insert(i);
		}
	}
}

void FighterMoveList::try_changing_to_buffered_status(Fighter* fighter) {
	bool free_cancel = fighter->is_enable_free_cancel();
	for (std::set<int>::iterator it = buffered_moves.begin(); it != buffered_moves.end();) {
		if (((move_list[*it].cancel_enabled && move_list[*it].input_kind == INPUT_KIND_NORMAL)
			|| free_cancel) && !move_list[*it].check_input(fighter)) {
			it = buffered_moves.erase(it);
		}
		else {
			it++;
		}
	}
	for (std::set<int>::reverse_iterator it = buffered_moves.rbegin(), max = buffered_moves.rend(); it != max; it++) {
		if (move_list[*it].try_change_status(fighter)) return;
	}
}

bool FighterMoveList::is_curr_move_recover_crouching(Fighter* fighter) {
	return (get_move(fighter->object_string[FIGHTER_STRING_MOVE_KIND]).recover_crouching);
}

bool FighterMoveList::is_enable_cancel(Fighter* fighter, std::string name) {
	FighterMoveListEntry move = get_move(name);
	return move.cancel_enabled
		|| (fighter->is_enable_free_cancel() && move.allowed_states.contains(fighter->object_string[FIGHTER_STRING_MOVE_KIND]))
		|| (fighter->object_flag[FIGHTER_FLAG_ATTACK_HIT] && move.hit_cancel_enabled)
		|| (fighter->object_flag[FIGHTER_FLAG_ATTACK_BLOCKED] && move.block_cancel_enabled);
}

void FighterMoveList::enable_cancel(std::string name, CancelKind cancel_kind) {
#ifdef DEBUG 
	if (!move_map.contains(name)) return;
#endif
	FighterMoveListEntry& move = get_move(name);
	switch (cancel_kind) {
		case (CANCEL_KIND_HIT): {
			move.hit_cancel_enabled = true;
		} break;
		case (CANCEL_KIND_BLOCK): {
			move.block_cancel_enabled = true;
		} break;
		case (CANCEL_KIND_CONTACT): {
			move.hit_cancel_enabled = true;
			move.block_cancel_enabled = true;
		} break;
		case (CANCEL_KIND_ANY): {
			move.cancel_enabled = true;
		} break;
	}
}

void FighterMoveList::disable_cancel(std::string name, CancelKind cancel_kind) {
	FighterMoveListEntry& move = get_move(name);
	switch (cancel_kind) {
		case (CANCEL_KIND_HIT): {
			move.hit_cancel_enabled = false;
		} break;
		case (CANCEL_KIND_BLOCK): {
			move.block_cancel_enabled = false;
		} break;
		case (CANCEL_KIND_CONTACT): {
			move.hit_cancel_enabled = false;
			move.block_cancel_enabled = false;
		} break;
		case (CANCEL_KIND_ANY): {
			move.cancel_enabled = false;
		} break;
	}
}

void FighterMoveList::disable_all_cancels() {
	for (size_t i = 0, max = move_list.size(); i < max; i++) {
		move_list[i].cancel_enabled = false;
		move_list[i].hit_cancel_enabled = false;
		move_list[i].block_cancel_enabled = false;
		move_list[i].special_level = SPECIAL_LEVEL_N;
	}
	buffered_moves.clear();
}

void FighterMoveList::swap_buffers(std::string a, std::string b) {
	if (!move_map.contains(a) || !move_map.contains(b)) return;
	int idx_a = move_map[a];
	int idx_b = move_map[b];
	FighterMoveListEntry& move_a = move_list[idx_a];
	FighterMoveListEntry& move_b = move_list[idx_b];
	std::swap(move_a.special_level, move_b.special_level);
	std::swap(move_a.buttons_pressed, move_b.buttons_pressed);
	if (buffered_moves.contains(idx_a) != buffered_moves.contains(idx_b)) {
		if (buffered_moves.contains(idx_a)) {
			buffered_moves.erase(idx_a);
			buffered_moves.insert(idx_b);
		}
		else {
			buffered_moves.erase(idx_b);
			buffered_moves.insert(idx_a);
		}
	}
}

bool FighterMoveList::is_buffered(std::string name) {
	return move_map.contains(name) && buffered_moves.contains(move_map[name]);
}

FighterMoveListEntry& FighterMoveList::get_move(std::string name) {
#ifdef DEBUG
	if (!move_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("ERROR: Move " + name + " was not in the movelist");
		return move_list[0];
	}
#endif
	return move_list[move_map[name]];
}