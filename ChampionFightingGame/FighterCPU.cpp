#include "FighterCPU.h"
#include "Fighter.h"
#include "utils.h"
#include "ParamAccessor.h"

FighterCPU::FighterCPU() {
	owner = nullptr;
	opponent = nullptr;
	execution_error_margin = 2;

	reaction_time = 15;
	correct_prediction_time = 12;
	incorrect_prediction_time = 26;
	prediction_mode = false;
	hit_confirm_skill = 9;
	move_recognition_skill = 2;

	movement_error_margin = 0;
	move_range_error_margin = 0.0f;
	mental_stack_update_interval = 5;

	cautiousness = 3;
	impulse = 0;

	tilt_modifier = -3;
	comeback_factor = 1;
	locked_in = false;
	stamina = 60;

	cpu_mode = CPU_MODE_NEUTRAL;

	input_frames = 0;
}

void FighterCPU::init(Fighter* owner, Fighter* opponent) {
	this->owner = owner;
	this->opponent = opponent;
	states.resize(60);
	opponent_states.resize(60);
	add_action("wait", "wait", INPUT_KIND_NORMAL, 0, 5, { 5 }, {}, {}, 0.0f, true);
	add_action("crouch", "crouch", INPUT_KIND_NORMAL, 0, 1, { 1, 2, 3 }, { CP_TAG_EVADE_ATK }, {}, 0.0f, true);
	add_action("crouch_d", "crouch_d", {}, { "crouch" }, true);
	add_action("crouch_u", "crouch_u", {}, { "wait" }, true);
	add_action("walk_f", "walk_f", INPUT_KIND_NORMAL, 0, 6, { 6 }, {}, {}, 0.0f, true)
		.add_movement_info(owner->get_param_float("walk_f_speed"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	add_action("walk_b", "walk_b", INPUT_KIND_NORMAL, 0, 4, { 4 }, { CP_TAG_EVADE_ATK }, {}, 0.0f, true)
		.add_movement_info(-owner->get_param_float("walk_b_speed"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	add_action("dash_f", "dash_f", INPUT_KIND_66, 0, 6, { 6 }, {}, {}, 0.0f, false);
	add_action("dash_b", "dash_b", INPUT_KIND_44, 0, 4, { 4 }, { CP_TAG_EVADE_ATK }, {}, 0.0f, false);
//	add_action("jump_squat", "jump_squat", INPUT_KIND_NORMAL, 0, 8, { 7, 8, 9 }, { CP_TAG_EVADE_ATK }, { "jump_n", "jump_f", "jump_b" }, 0.0f, false);
}

CPUAction& FighterCPU::add_action(std::string script_name, std::string anim_name, InputKind input_kind, 
	unsigned short required_buttons, unsigned int pref_stick_dir, std::set<unsigned int> stick_dirs,
	std::vector<CPUTag> tags, std::vector<std::string> auto_followups, float resource_cost, 
	bool allow_interrupt) {
	actions[script_name].init(owner, script_name, anim_name, input_kind, required_buttons,
		pref_stick_dir, stick_dirs, tags, auto_followups, resource_cost, allow_interrupt, true);
	return actions[script_name];
}

CPUAction& FighterCPU::add_action(std::string script_name, std::string anim_name, std::vector<CPUTag> tags,
	std::vector<std::string> auto_followups, bool allow_interrupt) {
	actions[script_name].init(owner, script_name, anim_name, INPUT_KIND_NORMAL, 0, 0, {}, tags, 
		auto_followups, 0.0f, allow_interrupt, false
	);
	return actions[script_name];
}

void FighterCPU::process() {
	determine_states();
	if (input_frames) {
		input_frames--;
	}
	if (!input_frames) {
		switch (cpu_mode) {
			default:
			case (CPU_MODE_NEUTRAL): {
				process_neutral();
			} break;
			case (CPU_MODE_ATTACK_SUCCESS): {
				process_attack_success();
			} break;
			case (CPU_MODE_HITSTUN): {
				process_hitstun();
			} break;
			case (CPU_MODE_WAKEUP_BLOCKSTUN): {
				process_wakeup_blockstun();
			} break;
		}
	}
}

void FighterCPU::process_neutral() {
	int reaction_frames = reaction_time-1;
	if (prediction_mode) {
		reaction_frames = correct_prediction_time - 1; //TODO: Add the mental stack here
	}
	CPUFighterState state = states.newest(reaction_frames);
	CPUFighterState opp_state = opponent_states.newest(reaction_frames);

	if (!opp_state.action) return;

	if (state.action_kind != CPU_ACTION_KIND_NORMAL || opp_state.action_kind != CPU_ACTION_KIND_NORMAL) {
		switch (state.action_kind) {
			default:
			case (CPU_ACTION_KIND_NORMAL): {
				cpu_mode = CPU_MODE_ATTACK_SUCCESS;
			} break;
			case (CPU_ACTION_KIND_HITSTUN): {
				cpu_mode = CPU_MODE_HITSTUN;
			} break;
			case (CPU_ACTION_KIND_BLOCKSTUN):
			case (CPU_ACTION_KIND_WAKEUP): {
				cpu_mode = CPU_MODE_WAKEUP_BLOCKSTUN;
			} break;
		}
		return;
	}
	if (opp_state.action->active && !opp_state.attack_finished) {
		std::vector<CPUAction> responses;
		CPUFighterState present_state = states.newest();
		CPUFighterState opp_present_state = opponent_states.newest();
		int remaining_start_frames = (opp_state.action->startup - opp_state.frame) - reaction_frames - 1;
		int remaining_hit_frames = remaining_start_frames - 1;
		if (remaining_start_frames <= 0) {
			for (int i = reaction_frames; i--> 0;) {
				if (check_contact(states.newest(i), opponent_states.newest(i))) {
					cpu_mode = CPU_MODE_HITSTUN;
					return;
				}
			}
		}
		int hit_frame = get_hit_frame(*opp_present_state.action, opp_present_state.frame,
			opp_present_state.facing_dir, opp_present_state.base_pos, *present_state.action,
			present_state.frame, present_state.facing_dir, present_state.base_pos, false);
		if (hit_frame != -1) {
			remaining_hit_frames = (hit_frame - opp_state.frame) - reaction_frames;
			for (auto& a : actions) {
				CPUAction action = a.second;
				if (action.active && action.startup <= remaining_hit_frames) {
					int sim_hit_frame = get_hit_frame(action, 0, present_state.facing_dir, 
						present_state.pos, *opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir, 
						opp_present_state.base_pos, true
					) + 1;
					if (sim_hit_frame && (sim_hit_frame + get_frames_to_input(action) <= remaining_hit_frames)) {
						//TODO: Rather than just making sure our sim hit frame is fast enough to trade, we also
						//want to make sure that we won't be negative after a trade
						responses.push_back(action);
						continue;
					}
				}
				for (size_t i = 0, max = action.tags.size(); i < max; i++) {
					switch (action.tags[i]) {
						case (CP_TAG_EVADE_ATK): {
							int sim_hit_frame = get_hit_frame(*opp_present_state.action,
								opp_present_state.frame, opp_present_state.facing_dir, 
								opp_present_state.base_pos, action, 0, 
								present_state.facing_dir, present_state.pos, false
							);
							if (sim_hit_frame == -1 || sim_hit_frame > hit_frame) {
								responses.push_back(action);
								continue;
							}
						} break;
						case (CP_TAG_IGNORE_STRIKE): {
							if (opp_present_state.action->is_strike) {

							}
						} break;
						case (CP_TAG_IGNORE_THROW): {
							if (opp_present_state.action->is_throw) {
								responses.push_back(action);
								continue;
							}
						} break;
					}
				}
			}
			if (!responses.empty()) {
				execute_action(responses[rng(0, responses.size() - 1)]);
			}
			else {
				//We're going to get hit, and we haven't found a response that gets us out of this 
				//situation. Therefore, we're going to try blocking.
			}
			return;
		}
		else {
			if (present_state.action->allow_interrupt) {
				for (size_t i = 0, max = present_state.action->tags.size(); i < max; i++) {
					if (present_state.action->tags[i] == CP_TAG_EVADE_ATK) {
						int test_hit_frame = get_hit_frame(*opp_present_state.action, 
							opp_present_state.frame, opp_present_state.facing_dir, 
							opp_present_state.base_pos, actions["wait"], 0, present_state.facing_dir, 
							present_state.base_pos, false
						);
						if (test_hit_frame != -1) {
							execute_action(*present_state.action);
							return;
						}
						break;
					}
				}
			}
			if (!responses.empty()) {
				execute_action(responses[rng(0, responses.size() - 1)]);
				return;
			}
		}
	}
	//If we make it to here, then either the opponent isn't attacking, or we can't immediately respond
	//to them. This is the part where we start making reads and/or approaching.
}

void FighterCPU::process_attack_success() {
	CPUFighterState opponent_state = opponent_states.newest(reaction_time);
	if (opponent_state.action_kind == CPU_ACTION_KIND_NORMAL) {
		cpu_mode = CPU_MODE_NEUTRAL;
		return;
	}
}

void FighterCPU::process_hitstun() {
	CPUFighterState state = states.newest(reaction_time);
	if (state.action_kind == CPU_ACTION_KIND_NORMAL) {
		cpu_mode = CPU_MODE_NEUTRAL;
		return;
	}
}

void FighterCPU::process_wakeup_blockstun() {
	CPUFighterState state = states.newest(reaction_time);
	if (state.action_kind == CPU_ACTION_KIND_NORMAL) {
		cpu_mode = CPU_MODE_NEUTRAL;
		return;
	}
}

void FighterCPU::execute_action(CPUAction action) {
	CPUAction prev_action = curr_action;
	curr_action = action;
	owner->player->manual_seq.reset_idx();
	switch (action.input_kind) {
		case (INPUT_KIND_NORMAL): {
			if (action.required_buttons) { //If we require both a stick input and a button input, 
				//the button input may or may not come a few frames before or after the stick input
				if (action.stick_dirs.contains(owner->get_stick_dir())) {
					//If we're already holding the stick in the correct direction in advance,
					//no chance to mess up
					for (int i = 0, max = rng(1, execution_error_margin + 1); i < max; i++) {
						add_input(owner->get_stick_dir(), action.required_buttons);
					}
				}
				else {
					//We have a 1 in execution_error_margin * 5 chance to attempt to nail the input. If
					//we succeed on this chance, we have an execution_error_margin in 10 chance to
					//press the button too early
					int jf = rng(0, execution_error_margin * 5);
					if (jf) {
						for (int i = 0, max = jf / 5; i < max; i++) {
							add_input(action.pref_stick_dir, 0);
						}
					}
					else {
						if (rng(1, 10) <= execution_error_margin) {
							add_input(owner->get_stick_dir(), action.required_buttons);
						}
					}
					for (int i = 0, max = rng(1, execution_error_margin + 1); i < max; i++) {
						add_input(action.pref_stick_dir, action.required_buttons);
					}
				}
				for (int i = 0, max = rng(0, execution_error_margin); i < max; i++) {
					add_input(action.pref_stick_dir, 0);
				}
			}
			else { //Inputs that lack a button press are pretty hard to mess up
				add_input(action.pref_stick_dir, 0);
			}
		} break;
		case (INPUT_KIND_236): {

		} break;
		case (INPUT_KIND_214): {

		} break;
		case (INPUT_KIND_623): {

		} break;
		case (INPUT_KIND_41236): {

		} break;
		case (INPUT_KIND_63214): {

		} break;
		case (INPUT_KIND_632): {

		} break;
		case (INPUT_KIND_46): {

		} break;
		case (INPUT_KIND_28): {

		} break;
		case (INPUT_KIND_66): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(6, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(6, action.required_buttons);
			}
		} break;
		case (INPUT_KIND_44): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(4, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(4, action.required_buttons);
			}
		} break;
		case (INPUT_KIND_22): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(2, 0);
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				add_input(5, 0);
			}
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				add_input(2, action.required_buttons);
			}
		} break;
		case (INPUT_KIND_236236): {

		} break;
		case (INPUT_KIND_214214): {

		} break;
		case (INPUT_KIND_4646): {

		} break;
		default: {

		} break;
	}
	owner->player->manual_seq.reset_idx();
}

void FighterCPU::add_input(unsigned int input_stick, unsigned int input_buttons) {
	input_frames++;
	owner->player->manual_seq.add_inputs(numpad_to_bits(input_stick) | input_buttons);
}

int FighterCPU::get_frames_to_input(CPUAction action) {
	int ret = -1;
	switch (action.input_kind) {
		case (INPUT_KIND_NORMAL): {
			if (action.required_buttons) {
				if (action.stick_dirs.contains(owner->get_stick_dir())) {
					for (int i = 0, max = rng(1, execution_error_margin + 1); i < max; i++) {
						ret++;
					}
				}
				else {
					int jf = rng(0, execution_error_margin * 5);
					if (jf) {
						for (int i = 0, max = jf / 5; i < max; i++) {
							ret++;
						}
					}
					ret++;
				}
			}
			else {
				ret++;
			}
		} break;
		case (INPUT_KIND_236): {

		} break;
		case (INPUT_KIND_214): {

		} break;
		case (INPUT_KIND_623): {

		} break;
		case (INPUT_KIND_41236): {

		} break;
		case (INPUT_KIND_63214): {

		} break;
		case (INPUT_KIND_632): {

		} break;
		case (INPUT_KIND_46): {

		} break;
		case (INPUT_KIND_28): {

		} break;
		case (INPUT_KIND_66): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			ret++;
		} break;
		case (INPUT_KIND_44): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			ret++;
		} break;
		case (INPUT_KIND_22): {
			for (int i = 0, max = (rng(0, execution_error_margin) / 2) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			for (int i = 0, max = rng(0, execution_error_margin) + 1; i < max; i++) {
				ret++;
			}
			ret++;
		} break;
		case (INPUT_KIND_236236): {

		} break;
		case (INPUT_KIND_214214): {

		} break;
		case (INPUT_KIND_4646): {

		} break;
		default: {

		} break;
	}
	return ret;
}

void FighterCPU::determine_states() {
	CPUFighterState new_state;
	CPUFighterState new_opp_state;
	std::string script_name = opponent->move_script.name;
	new_opp_state.frame = opponent->object_int[FIGHTER_INT_EXTERNAL_FRAME];
	new_opp_state.anim_frame = opponent->frame;
	new_opp_state.facing_dir = opponent->facing_dir;
	new_opp_state.pos = glm::vec2(opponent->get_pos());
	new_opp_state.base_pos = new_opp_state.pos;
	new_opp_state.speed = glm::vec2(
		opponent->object_float[BATTLE_OBJECT_FLOAT_X_SPEED],
		opponent->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED]
	);
	if (opponent->cpu.actions.contains(script_name)) {
		new_opp_state.action = &opponent->cpu.actions[script_name];
		new_opp_state.base_pos -= new_opp_state.action->frame_data[new_opp_state.anim_frame].pos_offset * glm::vec2(opponent->get_scale_vec());
	}
	else {
		new_opp_state.action = &opponent->cpu.actions["wait"];
	}
	if (opponent->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		new_opp_state.action_kind = CPU_ACTION_KIND_HITSTUN;
	}
	else {
		new_opp_state.action_kind = CPU_ACTION_KIND_NORMAL;
	}
	new_opp_state.attack_finished = opponent->object_flag[FIGHTER_FLAG_ATTACK_HIT] || opponent->object_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	opponent_states.insert(new_opp_state);
	script_name = owner->move_script.name;
	new_state.frame = owner->object_int[FIGHTER_INT_EXTERNAL_FRAME];
	new_state.anim_frame = owner->frame;
	new_state.facing_dir = owner->facing_dir;
	new_state.pos = glm::vec2(owner->get_pos());
	new_state.base_pos = new_state.pos;
	new_state.speed = glm::vec2(
		owner->object_float[BATTLE_OBJECT_FLOAT_X_SPEED],
		owner->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED]
	);
	if (actions.contains(script_name)) {
		new_state.action = &actions[script_name];
		new_state.base_pos -= new_state.action->frame_data[new_state.frame].pos_offset * glm::vec2(owner->get_scale_vec());
	}
	else {
		new_state.action = &actions["wait"];
	}
	if (owner->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		new_state.action_kind = CPU_ACTION_KIND_HITSTUN;
	}
	else {
		new_state.action_kind = CPU_ACTION_KIND_NORMAL;
	}
	new_state.attack_finished = owner->object_flag[FIGHTER_FLAG_ATTACK_HIT] || owner->object_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
	states.insert(new_state);
}

int FighterCPU::get_hit_frame(CPUAction atk_action, int atk_frame, float atk_facing_dir, 
	glm::vec2 atk_base_pos, CPUAction def_action, int def_frame, float def_facing_dir, 
	glm::vec2 def_base_pos, bool this_atk) {
	FighterCPU *attacker = this;
	FighterCPU *defender = &opponent->cpu;
	if (!this_atk) std::swap(attacker, defender);
	glm::vec2 atk_speed_accel = glm::vec2(atk_action.x_speed * atk_facing_dir, atk_action.y_speed);
	glm::vec2 atk_speed_total = atk_speed_accel;
	glm::vec2 def_speed_accel = glm::vec2(def_action.x_speed * def_facing_dir, def_action.y_speed);
	glm::vec2 def_speed_total = def_speed_accel;
	for (int atk_active = atk_action.active + atk_action.startup - 1;
		atk_frame < atk_active; atk_frame++, def_frame++) {
		if (atk_frame >= atk_action.frame_data.size()) {
			atk_frame = 0;
			if (!atk_action.auto_followups.empty()) {
				atk_action = attacker->actions[atk_action.auto_followups[0]];
			}
			else {
				return -1;
			}
		}
		if (def_frame >= def_action.frame_data.size()) {
			def_frame = 0;
			if (!def_action.auto_followups.empty()) {
				def_action = defender->actions[def_action.auto_followups[0]];
			}
		}
		CPUFrameData atk_frame_data = atk_action.frame_data[atk_frame];
		CPUFrameData def_frame_data = def_action.frame_data[def_frame];
		atk_speed_accel += glm::vec2(atk_action.x_accel * atk_facing_dir, atk_action.y_accel);
		def_speed_accel += glm::vec2(def_action.x_accel * def_facing_dir, def_action.y_accel);
		atk_speed_accel.x = clampf(-atk_action.x_max, atk_speed_accel.x, atk_action.x_max);
		def_speed_accel.x = clampf(-def_action.x_max, def_speed_accel.x, def_action.x_max);
		if (atk_action.y_max != 0.0f) atk_speed_accel.y = std::max(-atk_action.y_max, atk_speed_accel.y);
		if (def_action.y_max != 0.0f) def_speed_accel.y = std::max(-def_action.y_max, def_speed_accel.y);
		atk_speed_total += atk_speed_accel;
		def_speed_total += def_speed_accel;
		glm::vec2 atk_pos = atk_base_pos + glm::vec2(atk_frame_data.pos_offset * attacker->owner->get_scale_vec().x * atk_facing_dir, 0.0f)
			+ atk_speed_total;
		glm::vec2 def_pos = def_base_pos + glm::vec2(def_frame_data.pos_offset * defender->owner->get_scale_vec().x * def_facing_dir, 0.0f)
			+ def_speed_total;
		if (check_contact(def_frame_data, def_pos, def_facing_dir, atk_frame_data, atk_pos, atk_facing_dir)) {
			return atk_frame;
		}
	}
	return -1;
}

bool FighterCPU::check_contact(CPUFighterState& state, CPUFighterState& opp_state) {
	if (!state.action || !opp_state.action) return false;

	return check_contact(state.action->frame_data[state.frame], state.pos, state.facing_dir,
		opp_state.action->frame_data[opp_state.frame], opp_state.pos, opp_state.facing_dir);
}

bool FighterCPU::check_contact(CPUFrameData& frame_data, glm::vec2 pos, float facing_dir, 
	CPUFrameData& opp_frame_data, glm::vec2 opp_pos, float opp_facing_dir) {
	std::map<int, CPUHurtbox> hurtboxes = frame_data.hurtboxes;
	std::map<int, CPUHitbox> hitboxes = opp_frame_data.hitboxes;
	if (!hitboxes.size() || !hurtboxes.size()) return false;

	for (const auto& hit : hitboxes) {
		CPUHitbox hitbox = hit.second;
		hitbox.anchor /= glm::vec2(opponent->get_scale_vec());
		hitbox.offset /= glm::vec2(opponent->get_scale_vec());
		hitbox.anchor.x *= opp_facing_dir;
		hitbox.offset.x *= opp_facing_dir;
		hitbox.anchor += opp_pos / glm::vec2(opponent->get_scale_vec());
		hitbox.offset += opp_pos / glm::vec2(opponent->get_scale_vec());

		for (const auto& hurt : hurtboxes) {
			CPUHurtbox hurtbox = hurt.second;
			hurtbox.anchor /= glm::vec2(owner->get_scale_vec());
			hurtbox.offset /= glm::vec2(owner->get_scale_vec());
			hurtbox.anchor.x *= facing_dir;
			hurtbox.offset.x *= facing_dir;
			hurtbox.anchor += pos / glm::vec2(owner->get_scale_vec());
			hurtbox.offset += pos / glm::vec2(owner->get_scale_vec());

			if (is_rect_collide(hitbox.anchor, hitbox.offset, hurtbox.anchor, hurtbox.offset)) {
				return true;
			}
		}
	}
	return false;
}

unsigned int FighterCPU::numpad_to_bits(unsigned int numpad_dir) {
	if (owner->facing_right) {
		switch (numpad_dir) {
			case (1): {
				return BUTTON_DOWN_BIT | BUTTON_LEFT_BIT;
			} break;
			case (2): {
				return BUTTON_DOWN_BIT;
			} break;
			case (3): {
				return BUTTON_DOWN_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (4): {
				return BUTTON_LEFT_BIT;
			} break;
			case (5): {
				return 0;
			} break;
			case (6): {
				return BUTTON_RIGHT_BIT;
			} break;
			case (7): {
				return BUTTON_UP_BIT | BUTTON_LEFT_BIT;
			} break;
			case (8): {
				return BUTTON_UP_BIT;
			} break;
			case (9): {
				return BUTTON_UP_BIT | BUTTON_RIGHT_BIT;
			} break;
		}
	}
	else {
		switch (numpad_dir) {
			case (1): {
				return BUTTON_DOWN_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (2): {
				return BUTTON_DOWN_BIT;
			} break;
			case (3): {
				return BUTTON_DOWN_BIT | BUTTON_LEFT_BIT;
			} break;
			case (4): {
				return BUTTON_RIGHT_BIT;
			} break;
			case (5): {
				return 0;
			} break;
			case (6): {
				return BUTTON_LEFT_BIT;
			} break;
			case (7): {
				return BUTTON_UP_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (8): {
				return BUTTON_UP_BIT;
			} break;
			case (9): {
				return BUTTON_UP_BIT | BUTTON_LEFT_BIT;
			} break;
		}
	}
}

CPUAction::CPUAction() {
	name = "";

	input_kind = INPUT_KIND_NORMAL;
	required_buttons = 0;
	pref_stick_dir = 5;
	stick_dirs = { 5 };

	startup = 0;
	active = 0;
	recovery = 0;
	total = 0;

	x_speed = 0.0f;
	x_accel = 0.0f;
	x_max = 0.0f;

	y_speed = 0.0f;
	y_accel = 0.0f;
	y_max = 0.0f;

	is_strike = false;
	is_throw = false;

	attack_range_x = glm::vec2(0.0f);
	attack_range_y = glm::vec2(0.0f);
	defense_range_x = glm::vec2(0.0f);
	defense_range_y = glm::vec2(0.0f);

	resource_cost = 0.0f;
	allow_interrupt = false;
	manual_input = true;
}

void CPUAction::init(Fighter* owner, std::string script_name, std::string anim_name, InputKind input_kind,
	unsigned short required_buttons, unsigned int pref_stick_dir, std::set<unsigned int> stick_dirs,
	std::vector<CPUTag> tags, std::vector<std::string> auto_followups, float resource_cost,
	bool allow_interrupt, bool manual_input) {
	this->name = script_name;
	this->input_kind = input_kind;
	this->required_buttons = required_buttons;
	this->stick_dirs = stick_dirs;
	this->pref_stick_dir = pref_stick_dir;
	this->tags = tags;
	this->auto_followups = auto_followups;
	this->resource_cost = resource_cost;
	this->allow_interrupt = allow_interrupt;
	this->manual_input = manual_input;
	Animation* anim = owner->anim_table.get_anim(anim_name, false);
	MoveScript script = owner->move_script_table.get_script(script_name);
	script.activate();
	ScriptArg args;
	std::map<int, CPUHitbox> hitboxes;
	std::map<int, CPUHurtbox> hurtboxes;
	std::map<int, CPUGrabbox> grabboxes;
	std::vector<std::string> hit_cancel_options;
	std::vector<std::string> block_cancel_options;
	std::vector<std::string> whiff_cancel_options;

	bool found_startup = false;
	bool found_active = false;

	float min_trans_offset = 0.0f;
	float max_trans_offset = 0.0f;
	for (float frame = 0.0f, rate = 1.0f, max = (float)(anim->length); frame < max; frame += rate) {

		float trans_offset = anim->get_trans_offset(frame, owner->model.get_skeleton());
		min_trans_offset = std::min(min_trans_offset, trans_offset);
		max_trans_offset = std::max(max_trans_offset, trans_offset);

		while (script.has_next_frame(frame)) {
			ScriptFrame sf = script.get_next_frame();
			while (!sf.script_funcs.empty()) {
				void (BattleObject:: * func)(ScriptArg) = sf.script_funcs.front().function_call;
				ScriptArg args = sf.script_funcs.front().function_args;
				sf.script_funcs.pop();
				if (func == &BattleObject::SET_RATE) {
					UNWRAP_NO_DECL(rate);
					continue;
				}
				if (func == &BattleObject::NEW_HITBOX) {
					is_strike = true;
					if (!found_startup) {
						startup++;
					}
					found_startup = true;
					CPUHitbox hitbox;
					UNWRAP(id, int);
					args.pop();
					UNWRAP_NO_DECL(hitbox.anchor);
					UNWRAP_NO_DECL(hitbox.offset);
					UNWRAP_NO_DECL(hitbox.collision_kind);
					UNWRAP_NO_DECL(hitbox.hit_result);
					if (WRAPPED_TYPE(int)) {
						UNWRAP_NO_DECL(hitbox.custom_hit_status);
					}
					else {
						UNWRAP_NO_DECL(hitbox.hit_status);
					}
					UNWRAP_NO_DECL(hitbox.hit_move);
					UNWRAP_NO_DECL(hitbox.hit_flags);
					UNWRAP_NO_DECL(hitbox.critical_condition);
					if (WRAPPED_TYPE(int) || WRAPPED_TYPE(HitStatus)) {
						if (WRAPPED_TYPE(int)) {
							UNWRAP_NO_DECL(hitbox.custom_critical_status);
						}
						else {
							UNWRAP_NO_DECL(hitbox.critical_status);
						}
						UNWRAP_NO_DECL(hitbox.critical_hit_move);
						UNWRAP_NO_DECL(hitbox.critical_hit_flags);
					}
					UNWRAP_NO_DECL(hitbox.hit_height);

					attack_range_x[0] = std::min(attack_range_x[0], hitbox.anchor.x);
					attack_range_x[1] = std::max(attack_range_x[1], hitbox.offset.x);
					attack_range_y[0] = std::min(attack_range_y[0], hitbox.anchor.y);
					attack_range_y[1] = std::max(attack_range_y[1], hitbox.offset.y);

					hitboxes[id] = hitbox;

					continue;
				}
				if (func == &BattleObject::NEW_GRABBOX) {
					is_throw = true;
					if (!found_startup) {
						startup++;
					}
					found_startup = true;
					CPUGrabbox grabbox;
					UNWRAP(id, int);
					UNWRAP_NO_DECL(grabbox.anchor);
					UNWRAP_NO_DECL(grabbox.offset);
					UNWRAP_NO_DECL(grabbox.grabbox_kind);
					UNWRAP_NO_DECL(grabbox.collision_kind);
					UNWRAP_NO_DECL(grabbox.attacker_status_if_hit);
					UNWRAP_NO_DECL(grabbox.defender_status_if_hit);

					attack_range_x[0] = std::min(attack_range_x[0], grabbox.anchor.x);
					attack_range_x[1] = std::max(attack_range_x[1], grabbox.offset.x);
					attack_range_y[0] = std::min(attack_range_y[0], grabbox.anchor.y);
					attack_range_y[1] = std::max(attack_range_y[1], grabbox.offset.y);

					grabboxes[id] = grabbox;

					continue;
				}
				if (func == &BattleObject::NEW_HURTBOX) {
					CPUHurtbox hurtbox;
					UNWRAP(id, int);
					UNWRAP_NO_DECL(hurtbox.anchor);
					UNWRAP_NO_DECL(hurtbox.offset);
					UNWRAP_NO_DECL(hurtbox.hurtbox_kind);
					UNWRAP_NO_DECL(hurtbox.armor_hits);
					UNWRAP_NO_DECL(hurtbox.intangible_kind);

					defense_range_x[0] = std::min(defense_range_x[0], hurtbox.anchor.x);
					defense_range_x[1] = std::max(defense_range_x[1], hurtbox.offset.x);
					defense_range_y[0] = std::min(defense_range_y[0], hurtbox.anchor.y);
					defense_range_y[1] = std::max(defense_range_y[1], hurtbox.offset.y);

					hurtboxes[id] = hurtbox;

					continue;
				}
				if (func == &BattleObject::CLEAR_HITBOX) {
					UNWRAP(id, int);
					hitboxes.erase(id);
					continue;
				}
				if (func == &BattleObject::CLEAR_HITBOX_ALL) {
					hitboxes.clear();
					continue;
				}
				if (func == &BattleObject::CLEAR_HURTBOX) {
					UNWRAP(id, int);
					hurtboxes.erase(id);
					continue;
				}
				if (func == &BattleObject::CLEAR_HURTBOX_ALL) {
					hurtboxes.clear();
					continue;
				}
				if (func == &BattleObject::CLEAR_GRABBOX) {
					UNWRAP(id, int);
					grabboxes.erase(id);
					continue;
				}
				if (func == &BattleObject::CLEAR_GRABBOX_ALL) {
					grabboxes.clear();
					continue;
				}
			}
		}
		if (hitboxes.empty() && grabboxes.empty()) {
			if (!found_startup) {
				startup++;
			}
			else if (frame < (float)(anim->faf) || anim->faf == -1) {
				recovery++;
			}
		}
		else {
			active += recovery + 1;
			recovery = 0;
		}
		if (frame < (float)(anim->faf) || anim->faf == -1) {
			total++;
		}

		frame_data.push_back(CPUFrameData(
			trans_offset, hitboxes, hurtboxes, grabboxes, hit_cancel_options,
			block_cancel_options, whiff_cancel_options
		));
	}
	attack_range_x += glm::vec2(min_trans_offset, max_trans_offset);
	defense_range_x += glm::vec2(min_trans_offset, max_trans_offset);
}

void CPUAction::add_movement_info(float x_speed, float x_accel, float x_max, float y_speed, 
	float y_accel, float y_max) {
	this->x_speed = x_speed;
	this->x_accel = x_accel;
	if (x_max == 0.0f) {
		this->x_max = abs(x_speed);
	}
	else {
		this->x_max = x_max;
	}
	this->y_speed = y_speed;
	this->y_accel = y_accel;
	this->y_max = y_max;
}

CPUFighterState::CPUFighterState() {
	action = nullptr;
	action_kind = CPU_ACTION_KIND_NORMAL;
	frame = 0;
	anim_frame = 0.0f;
	facing_dir = 1.0f;
	pos = glm::vec2(0.0f);
	base_pos = glm::vec2(0.0f);
	speed = glm::vec2(0.0f);
	attack_finished = false;
}

CPUFrameData::CPUFrameData(float pos_offset, std::map<int, CPUHitbox> hitboxes,
	std::map<int, CPUHurtbox> hurtboxes, std::map<int, CPUGrabbox> grabboxes,
	std::vector<std::string> hit_cancel_options,
	std::vector<std::string> block_cancel_options,
	std::vector<std::string> whiff_cancel_options) {
	this->pos_offset = pos_offset;
	this->hitboxes = hitboxes;
	this->hurtboxes = hurtboxes;
	this->grabboxes = grabboxes;
	this->hit_cancel_options = hit_cancel_options;
	this->block_cancel_options = block_cancel_options;
	this->whiff_cancel_options = whiff_cancel_options;
}