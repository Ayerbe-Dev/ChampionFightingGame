#include "FighterCPU.h"
#include "Fighter.h"
#include "utils.h"
#include "ParamAccessor.h"

FighterCPU::FighterCPU() {
	owner = nullptr;
	opponent = nullptr;
	execution_error_margin = 0;

	reaction_time = 15;
	correct_prediction_time = 12;
	incorrect_prediction_time = 26;
	prediction_mode = false;
	hit_confirm_skill = 9;
	block_recognition_skill = 2;

	movement_error_margin = 0;
	move_range_error_margin = 0.0f;
	mental_stack_update_interval = 5;
	decision_update_interval = 10;

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
	add_action("wait", "wait", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 5, { 5 }, {}, {}, 0.0f, true);
	add_action("crouch", "crouch", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 1, { 1, 2, 3 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, true);
	add_action("crouch_d", "crouch_d", {}, { "crouch" }, true);
	add_action("crouch_u", "crouch_u", {}, { "wait" }, true);
	add_action("walk_f", "walk_f", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 6, { 6 }, {}, {}, 0.0f, true)
		.add_movement_info(owner->get_param_float("walk_f_speed"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	add_action("walk_b", "walk_b", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 4, { 4 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, true)
		.add_movement_info(-owner->get_param_float("walk_b_speed"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	add_action("dash_f", "dash_f", FIGHTER_CONTEXT_GROUND, INPUT_KIND_66, 0, 6, { 6 }, {}, {}, 0.0f, false);
	add_action("dash_b", "dash_b", FIGHTER_CONTEXT_GROUND, INPUT_KIND_44, 0, 4, { 4 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, false);
	add_action("parry_start_high", "parry_start_high", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		8, { 7, 8, 9 }, {}, {}, 0.0f, false);
	add_action("parry_start_mid", "parry_start_mid", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		5, { 4, 5, 6 }, {}, {}, 0.0f, false);
	add_action("parry_start_low", "parry_start_low", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		2, { 1, 2, 3 }, {}, {}, 0.0f, false);
	add_action("jump_squat", "jump_squat", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 8, { 7, 8, 9 }, { CPU_TAG_EVADE_ATK }, { "jump_n", "jump_f", "jump_b" }, 0.0f, false);
	add_action("jump_n", "jump_n", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 8, { 8 }, { }, { "fall" }, 0.0f, false)
		.add_movement_info(0.0f, 0.0f, 0.0f, owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"));
	add_action("jump_f", "jump_f", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 9, { 9 }, { }, { "fall" }, 0.0f, false)
		.add_movement_info(owner->get_param_float("jump_x_speed"), 0.0f, 0.0f, owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"));
	add_action("jump_b", "jump_b", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 7, { 7 }, { }, { "fall" }, 0.0f, false)
		.add_movement_info(-owner->get_param_float("jump_x_speed"), 0.0f, 0.0f, owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"));
	add_action("fall", "fall", {}, {}, true)
		.add_movement_info(0.0f, 0.0f, 0.0f, 0.0f, owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"));
	add_action("landing", "landing", {}, {}, false);
}

CPUAction& FighterCPU::add_action(std::string script_name, std::string anim_name, int context, 
	InputKind input_kind, unsigned short required_buttons, unsigned int pref_stick_dir, 
	std::set<unsigned int> stick_dirs, std::vector<CPUTag> tags, std::vector<std::string> auto_followups, 
	float resource_cost, bool allow_interrupt) {
	actions[script_name].init(owner, script_name, anim_name, context, input_kind, required_buttons,
		pref_stick_dir, stick_dirs, tags, auto_followups, resource_cost, allow_interrupt, true);
	return actions[script_name];
}

CPUAction& FighterCPU::add_action(std::string script_name, std::string anim_name, std::vector<CPUTag> tags,
	std::vector<std::string> auto_followups, bool allow_interrupt) {
	actions[script_name].init(owner, script_name, anim_name, FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 
		0, 0, {}, tags, auto_followups, 0.0f, allow_interrupt, false
	);
	return actions[script_name];
}

void FighterCPU::process() {
	determine_states();
	if (input_frames) input_frames--;
	if (curr_decision.frames) curr_decision.frames--;
	switch (cpu_mode) {
		default:
		case (CPU_MODE_NEUTRAL): {
			process_neutral();
		} break;
		case (CPU_MODE_ATTACK): {
			process_attack();
		} break;
		case (CPU_MODE_HITSTUN): {
			process_hitstun();
		} break;
		case (CPU_MODE_WAKEUP_BLOCKSTUN): {
			process_wakeup_blockstun();
		} break;
	}
}

void FighterCPU::process_neutral() {
	int curr_reaction_time = reaction_time-1;
	if (prediction_mode) {
		curr_reaction_time = correct_prediction_time - 1; //TODO: Add the mental stack here
	}
	CPUFighterState state = states.newest(curr_reaction_time);
	CPUFighterState opp_state = opponent_states.newest(curr_reaction_time);
	if (!opp_state.action) return;
	CPUFighterState present_state = states.newest();
	if (opp_state.action->active && !opp_state.actionable) {
		curr_decision = CPUDecision(CPU_DECISION_MODE_NONE, this);
		std::vector<CPUAction> responses;
		int remaining_start_frames = (opp_state.action->startup - opp_state.frame) - curr_reaction_time - 1;
		int opp_remaining_hit_frames = remaining_start_frames - 1;
		if (remaining_start_frames <= 0) {
			for (int i = curr_reaction_time; i--> 0;) {
				if (check_contact(states.newest(i), opponent_states.newest(i))) {
					cpu_mode = CPU_MODE_HITSTUN;
					return;
				}
			}
		}
		CPUFighterState opp_present_state = opponent_states.newest();
		int opp_hit_frame = get_hit_frame(*opp_state.action, opp_present_state.frame,
			opp_present_state.facing_dir, opp_present_state.base_pos, *present_state.action,
			present_state.frame, present_state.facing_dir, present_state.base_pos, false);
		if (opp_hit_frame != -1) {
			opp_remaining_hit_frames = (opp_hit_frame - opp_state.frame) - curr_reaction_time;
			for (auto& a : actions) {
				CPUAction action = a.second;
				if (action.active && action.startup <= opp_remaining_hit_frames) {
					int sim_hit_frame = get_hit_frame(action, 0, present_state.facing_dir, 
						present_state.pos, *opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir, 
						opp_present_state.base_pos, true
					) + 1;
					int sim_opp_hit_frame = get_hit_frame(*opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, action, 0, present_state.facing_dir,
						present_state.pos, false
					);
					int sim_opp_remaining_hit_frames = (sim_opp_hit_frame - opp_state.frame) - curr_reaction_time;
					if (sim_hit_frame && (sim_hit_frame + get_frames_to_input(action) <= sim_opp_remaining_hit_frames
						|| sim_opp_hit_frame == -1)) {
						//TODO: Add code to limit how - we're allowed to be on trade
						responses.push_back(action);
						continue;
					}
				}
				for (size_t i = 0, max = action.tags.size(); i < max; i++) {
					switch (action.tags[i]) {
						case (CPU_TAG_EVADE_ATK): {
							int sim_opp_hit_frame = get_hit_frame(*opp_present_state.action,
								opp_present_state.frame, opp_present_state.facing_dir, 
								opp_present_state.base_pos, action, 0, 
								present_state.facing_dir, present_state.pos, false
							);
							if (sim_opp_hit_frame == -1 || sim_opp_hit_frame > opp_hit_frame) {
								responses.push_back(action);
								continue;
							}
						} break;
						case (CPU_TAG_IGNORE_STRIKE): {
							if (opp_present_state.action->is_strike) {

							}
						} break;
						case (CPU_TAG_IGNORE_THROW): {
							if (opp_present_state.action->is_throw) {
								responses.push_back(action);
								continue;
							}
						} break;
					}
				}
			}
			if (!responses.empty()) {
				CPUAction future_action = responses[rng(0, responses.size() - 1)];
				if (future_action.active) {
					cpu_mode = CPU_MODE_ATTACK;
				}
				execute_action(future_action);
			}
			else {
				int parry_odds = opp_remaining_hit_frames * 2;
				//TODO: If the move is safe on block, multiply parry_odds by 2
				//TODO: Test if this actually beats crossups, low key might end up being too good at it 
				//if it does
				bool try_parry = rng(1, 20) <= parry_odds && !opp_present_state.action->is_throw;
				bool success_block = false;
				switch (owner->get_stick_dir()) {
					case 4: {
						if (opp_present_state.action->frame_data[opp_hit_frame].hitboxes[0].hit_height
							!= HIT_HEIGHT_LOW) {
							success_block = rng(0, 9);
						}
						else {
							success_block = rng(0, block_recognition_skill) >= opp_state.frame;
						}
					} break;
					case 1: {
						if (opp_present_state.action->frame_data[opp_hit_frame].hitboxes[0].hit_height
							!= HIT_HEIGHT_HIGH) {
							success_block = rng(0, 9);
						}
						else {
							success_block = rng(0, block_recognition_skill) >= opp_state.frame;
						}
					} break;
					default: {
						if (!try_parry) {
							success_block = rng(0, block_recognition_skill) >= opp_state.frame;
						}
					} break;
				}
				int block_frames = opp_remaining_hit_frames + rng(0, execution_error_margin * 2) + 1;
				if (success_block) {
					switch (opp_present_state.action->frame_data[opp_hit_frame].hitboxes[0].hit_height) {
						case (HIT_HEIGHT_HIGH): {
							if (try_parry) {
								execute_action(actions["parry_start_high"]);
							}
							else {
								for (int i = 0; i < block_frames; i++) {
									add_input(4, 0);
								}
							}
						} break;
						default:
						case (HIT_HEIGHT_MID): {
							if (try_parry) {
								execute_action(actions["parry_start_mid"]);
							}
							else {
								for (int i = 0; i < block_frames; i++) {
									if (owner->get_stick_dir() > 3) {
										add_input(4, 0);
									}
									else {
										add_input(1, 0);
									}
								}
							}
						} break;
						case (HIT_HEIGHT_LOW): {
							if (try_parry) {
								execute_action(actions["parry_start_low"]);
							}
							else {
								for (int i = 0; i < block_frames; i++) {
									add_input(1, 0);
								}
							}
						} break;
					}
				}
				else {
					if (try_parry) {
						switch (rng(0, 2)) {
							case 0: {
								execute_action(actions["parry_start_high"]);
							} break;
							case 1: {
								execute_action(actions["parry_start_mid"]);
							} break;
							case 2: {
								execute_action(actions["parry_start_low"]);
							} break;
						}
					}
					else {
						int height = rng(0, 1) * 3 + 1;
						for (int i = 0; i < block_frames; i++) {
							add_input(height, 0);
						}
					}
				}
				owner->player->manual_seq.reset_idx();
			}
			return;
		}
		else {
			//If we see a move and conclude that walking backwards lets us dodge it, next frame we will 
			//conclude that "at our current action, we'll dodge it, so no need to do anything." It's 
			//important that we make sure to keep walking backwards if stopping will make us get hit
			if (present_state.action->allow_interrupt) {
				for (size_t i = 0, max = present_state.action->tags.size(); i < max; i++) {
					if (present_state.action->tags[i] == CPU_TAG_EVADE_ATK) {
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
			int recovery_frames = opp_state.action->total - opp_state.frame - curr_reaction_time - 1;
			for (auto& a : actions) {
				CPUAction action = a.second;
				if (action.active && action.startup <= recovery_frames) {
					int sim_hit_frame = get_hit_frame(action, 0, present_state.facing_dir,
						present_state.pos, *opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, true
					) + 1;
					int sim_opp_hit_frame = get_hit_frame(*opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, action, 0, present_state.facing_dir,
						present_state.pos, false
					);
					int sim_opp_remaining_hit_frames = (sim_opp_hit_frame - opp_state.frame) - curr_reaction_time;
					if (sim_hit_frame && sim_opp_hit_frame == -1 
						&& (sim_hit_frame + get_frames_to_input(action) <= recovery_frames)) {
						responses.push_back(action);
						continue;
					}
				}
			}
			if (!responses.empty()) {
				CPUAction future_action = responses[rng(0, responses.size() - 1)];
				if (future_action.active) {
					cpu_mode = CPU_MODE_ATTACK;
				}
				execute_action(future_action);
				return;
			}
		}
	}
	if (input_frames) return;
	switch (curr_decision.mode) {
		default:
		case CPU_DECISION_MODE_NONE: {
			if (!curr_decision.frames) {
				int next_decision_mode = rng(CPU_DECISION_MODE_NONE, CPU_DECISION_MODE_JUMP_BUTTON);
				curr_decision = CPUDecision((CPUDecisionMode)next_decision_mode, this);
			}
		} break;
		case CPU_DECISION_MODE_WALK_BUTTON: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * state.speed 
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(curr_decision.action.attack_range_x[0] * state.facing_dir, curr_decision.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_decision.action.attack_range_x[1] * state.facing_dir, curr_decision.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(opp_state.action->defense_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(opp_state.action->defense_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				execute_action(curr_decision.action);
				cpu_mode = CPU_MODE_ATTACK;
				curr_decision = CPUDecision(CPU_DECISION_MODE_NONE, this);
				return;
			}
			if (!curr_decision.frames) {
				if (rng(1, 5) == 1) {
					owner->player->manual_seq.reset_idx();
					for (int i = 0, max = rng(1, 20); i < max; i++) {
						buffer_action(actions["crouch"]);
					}
					owner->player->manual_seq.reset_idx();
				}
				else {
					execute_action(actions["walk_f"]);
				}
				curr_decision.frames = rng(1, 20);
			}
			else {
				execute_action(actions["walk_f"]);
			}
		} break;
		case CPU_DECISION_MODE_WALK_COUNTERPOKE: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time -1 + movement_error) * state.speed
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(curr_decision.action.attack_range_x[0] * state.facing_dir, curr_decision.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_decision.action.attack_range_x[1] * state.facing_dir, curr_decision.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			float steps = owner->get_param_float("walk_f_speed")
				* rng(2, 7) * state.facing_dir;
			glm::vec2 sim_c1 = c1 + glm::vec2(steps, 0);
			glm::vec2 sim_c2 = c2 + glm::vec2(steps, 0);

			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time -1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(opp_state.action->defense_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(opp_state.action->defense_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;

			float opp_steps = opponent->get_param_float("walk_f_speed")
				* rng(2, 7) * opp_state.facing_dir;
			glm::vec2 sim_opp_c1 = opp_c1 + glm::vec2(opp_steps, 0);
			glm::vec2 sim_opp_c2 = opp_c2 + glm::vec2(opp_steps, 0);

			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				execute_action(actions["walk_b"]);
			}
			else if (is_rect_collide(sim_c1, sim_c2, sim_opp_c1, sim_opp_c2)) {
				std::vector<std::string> buffer_options;
				for (auto& a : actions) {
					if (curr_decision.action.hit_cancel_options.contains(a.first)) {
						buffer_options.push_back(a.first);
					}
				}
				owner->player->manual_seq.reset_idx();
				buffer_action(curr_decision.action);
					
				if (!buffer_options.empty()) {
					for (int i = 0; i < curr_decision.action.startup - 1; i++) {
						add_input(owner->get_stick_dir(), 0);
					}
					buffer_action(actions[buffer_options[rng(0, buffer_options.size() - 1)]]);
				}
				owner->player->manual_seq.reset_idx();
				cpu_mode = CPU_MODE_ATTACK;
				curr_decision = CPUDecision(CPU_DECISION_MODE_NONE, this);
			}
			else {
				execute_action(actions["walk_f"]);
			}
		} break;
		case CPU_DECISION_MODE_WALK_SHIMMY: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * state.speed
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(state.action->defense_range_x[0] * state.facing_dir, curr_decision.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(state.action->defense_range_x[1] * state.facing_dir, curr_decision.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(curr_decision.action.attack_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(curr_decision.action.attack_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				curr_decision.frames = rng(1, 10);
			}
			else if (!curr_decision.frames) {
				if (rng(0, 3)) {
					execute_action(actions["walk_f"]);
				}
				else {
					int next_decision_mode = rng(CPU_DECISION_MODE_NONE, CPU_DECISION_MODE_DASH_BUTTON);
					while (next_decision_mode == CPU_DECISION_MODE_WALK_SHIMMY) {
						next_decision_mode = rng(CPU_DECISION_MODE_NONE, CPU_DECISION_MODE_DASH_BUTTON);
					}
					curr_decision = CPUDecision((CPUDecisionMode)next_decision_mode, this);
				}
			}
			if (curr_decision.frames) {
				execute_action(actions["walk_b"]);
			}
		} break;
		case CPU_DECISION_MODE_DASH_BUTTON: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = present_state.pos;
			glm::vec2 c1 = glm::vec2(curr_decision.action.attack_range_x[0] * state.facing_dir, curr_decision.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_decision.action.attack_range_x[1] * state.facing_dir, curr_decision.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(opp_state.action->defense_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(opp_state.action->defense_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				execute_action(curr_decision.action);
				cpu_mode = CPU_MODE_ATTACK;
				curr_decision = CPUDecision(CPU_DECISION_MODE_NONE, this);
			}
			else if (present_state.frames_until_bufferable < get_frames_to_input(actions["dash_f"])) {
				execute_action(actions["dash_f"]);
			}
		} break;
		case CPU_DECISION_MODE_JUMP_BUTTON: {
			if (present_state.context == FIGHTER_CONTEXT_GROUND 
				&& owner->status_kind != FIGHTER_STATUS_JUMPSQUAT) {

				int fh_time = owner->calc_airtime(owner->get_param_float("jump_y_init_speed"));
				int sh_time = owner->calc_airtime(owner->get_param_float("jump_y_init_speed_s"));
				float f_speed = owner->get_param_float("jump_x_speed");

				float sim_pos_x_fh = (fh_time * f_speed * present_state.facing_dir) + present_state.pos.x;
				float sim_pos_x_sh = (sh_time * f_speed * present_state.facing_dir) + present_state.pos.x;

				glm::vec2 target_dist_x = curr_decision.target_dist_x + glm::vec2(present_state.pos.x);
				switch (curr_decision.misc) {
					case CPU_JUMP_KIND_SHORTHOP_FORWARD: {
						target_dist_x = curr_decision.target_dist_x + glm::vec2(sim_pos_x_sh);
						if (is_between(target_dist_x[0], opp_state.pos.x, target_dist_x[1])) {
							owner->player->manual_seq.reset_idx();
							for (int i = 0; i < 2; i++) {
								add_input(9, 0);
							}
							for (int i = 0; i < 3; i++) {
								add_input(6, 0);
							}
							owner->player->manual_seq.reset_idx();
							return;
						}
					} break;
					case CPU_JUMP_KIND_FULLHOP_FORWARD: {
						target_dist_x = curr_decision.target_dist_x + glm::vec2(sim_pos_x_fh);
						if (is_between(target_dist_x[0], opp_state.pos.x, target_dist_x[1])) {
							owner->player->manual_seq.reset_idx();
							for (int i = 0; i < 5; i++) {
								add_input(9, 0);
							}
							owner->player->manual_seq.reset_idx();
							return;
						}
					} break;
					case CPU_JUMP_KIND_NEUTRAL: {
						if (is_between(target_dist_x[0], opp_state.pos.x, target_dist_x[1])) {
							owner->player->manual_seq.reset_idx();
							for (int i = 0; i < 5; i++) {
								add_input(8, 0);
							}
							owner->player->manual_seq.reset_idx();
							return;
						}
//						if (is_between(target_dist_x[0], opp_state.pos.x, target_dist_x[1])) {
//							owner->player->manual_seq.reset_idx();
//							for (int i = 0; i < 2; i++) {
//								add_input(8, 0);
//							}
//							for (int i = 0; i < 3; i++) {
//								add_input(5, 0);
//							}
//							owner->player->manual_seq.reset_idx();
//							return;
//						}
					} break;
				}
				owner->player->manual_seq.reset_idx();
				if (opp_state.pos.x < std::min(target_dist_x[0], target_dist_x[1]) == (opp_state.facing_dir == 1.0f)) {
					add_input(6, 0);
				}
				else {
					add_input(4, 0);
				}
				owner->player->manual_seq.reset_idx();
			}
			else {
				CPUFighterState opp_present_state = opponent_states.newest();
				int sim_hit_frame = get_hit_frame(curr_decision.action, 0, present_state.facing_dir,
					present_state.pos, *opp_present_state.action, opp_present_state.frame, 
					opp_present_state.facing_dir, opp_present_state.pos, true
				);
				if (sim_hit_frame != -1) {
					execute_action(curr_decision.action);
					cpu_mode = CPU_MODE_ATTACK;
					curr_decision = CPUDecision(CPU_DECISION_MODE_NONE, this);
					return;
				}
			}
		} break;
		case CPU_DECISION_MODE_ADVANCE: {

		} break;
		case CPU_DECISION_MODE_SPECIAL: {

		} break;
	}
}

void FighterCPU::process_attack() {
	CPUFighterState state = states.newest();
	if (state.actionable) {
		cpu_mode = CPU_MODE_NEUTRAL;
		process_neutral();
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
	owner->player->manual_seq.reset_idx();
	buffer_action(action);
	owner->player->manual_seq.reset_idx();
}

void FighterCPU::buffer_action(CPUAction action) {
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
					//Our odds of attempting to justframe the input are 1 / (execution_error * 5 + 1)
					//If we choose to attempt it, we have an execution_error / 10 chance to mess up
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
			else {
				//If we only have to worry about stick inputs, they should be pretty free.
				//TODO: Add travel time into the stick movement
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
}

void FighterCPU::determine_states() {
	Fighter* fighter[2] = { owner, opponent };
	CPUFighterState new_state[2];
	for (int i = 0; i < 2; i++) {
		std::string script_name = fighter[i]->move_script.name;
		new_state[i].context = fighter[i]->fighter_context;
		new_state[i].frame = fighter[i]->object_int[FIGHTER_INT_EXTERNAL_FRAME];
		new_state[i].anim_frame = fighter[i]->frame;
		new_state[i].facing_dir = fighter[i]->facing_dir;
		new_state[i].pos = glm::vec2(fighter[i]->get_pos());
		new_state[i].base_pos = new_state[i].pos;
		new_state[i].speed = glm::vec2(
			fighter[i]->object_float[BATTLE_OBJECT_FLOAT_X_SPEED],
			fighter[i]->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED]
		);
		if (fighter[i]->cpu.actions.contains(script_name)) {
			new_state[i].action = &opponent->cpu.actions[script_name];
			new_state[i].base_pos.x -= new_state[i].action->frame_data[new_state[i].frame].pos_offset * opponent->get_scale_vec().x * new_state[i].facing_dir;
		}
		else {
			new_state[i].action = &opponent->cpu.actions["wait"];
		}
		if (fighter[i]->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
			if (fighter[i]->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
				new_state[i].action_kind = CPU_ACTION_KIND_BLOCKSTUN;
			}
			else {
				new_state[i].action_kind = CPU_ACTION_KIND_HITSTUN;
			}
		}
		else {
			new_state[i].action_kind = CPU_ACTION_KIND_NORMAL;
		}
		new_state[i].actionable = new_state[i].frame >= new_state[i].action->total;
		new_state[i].frames_until_bufferable = std::max(new_state[i].action->total - new_state[i].frame 
			- get_global_param_int(PARAM_FIGHTER, "buffer_window"), 0);
	}
	states.insert(new_state[0]);
	opponent_states.insert(new_state[1]);
}

int FighterCPU::get_hit_frame(CPUAction atk_action, int atk_frame, float atk_facing_dir, 
	glm::vec2 atk_base_pos, CPUAction def_action, int def_frame, float def_facing_dir, 
	glm::vec2 def_base_pos, bool this_atk) {
	FighterCPU *attacker = this;
	FighterCPU *defender = &opponent->cpu;
	if (!this_atk) std::swap(attacker, defender);
	if (atk_action.fighter_context == FIGHTER_CONTEXT_AIR) {
		if (atk_action.x_speed == 0.0f) {
			atk_action.x_speed = attacker->owner->object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * attacker->owner->facing_dir;
		}
		if (atk_action.y_speed == 0.0f) {
			atk_action.y_speed = attacker->owner->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED];
		}
		if (atk_action.y_accel == 0.0f) {
			atk_action.y_accel = attacker->owner->object_float[FIGHTER_FLOAT_CURRENT_GRAVITY];
		}
		if (atk_action.y_max == 0.0f) {
			atk_action.y_max = attacker->owner->object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX];
		}
	}
	if (def_action.fighter_context == FIGHTER_CONTEXT_AIR) {
		if (def_action.x_speed == 0.0f) {
			def_action.x_speed = defender->owner->object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * defender->owner->facing_dir;
		}
		if (def_action.y_speed == 0.0f) {
			def_action.y_speed = defender->owner->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED];
		}
		if (def_action.y_accel == 0.0f) {
			def_action.y_accel = defender->owner->object_float[FIGHTER_FLOAT_CURRENT_GRAVITY];
		}
		if (def_action.y_max == 0.0f) {
			def_action.y_max = defender->owner->object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX];
		}
	}
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

void FighterCPU::add_input(unsigned int input_stick, unsigned int input_buttons) {
	input_stick = numpad_to_bits(input_stick);
	switch (input_buttons) {
		case BUTTON_2L_BIT: {
			if (rng(0, 10) < execution_error_margin) {
				unsigned int early_button = rng(0, 1) ? BUTTON_LP_BIT : BUTTON_LK_BIT;
				input_frames++;
				owner->player->manual_seq.add_inputs(input_stick | early_button);
				if (rng(3, 10) < execution_error_margin) {
					input_frames++;
					owner->player->manual_seq.add_inputs(input_stick | early_button);
				}
			}
		} break;
		case BUTTON_2M_BIT: {
			if (rng(0, 10) < execution_error_margin) {
				unsigned int early_button = rng(0, 1) ? BUTTON_MP_BIT : BUTTON_MK_BIT;
				input_frames++;
				owner->player->manual_seq.add_inputs(input_stick | early_button);
				if (rng(3, 10) < execution_error_margin) {
					input_frames++;
					owner->player->manual_seq.add_inputs(input_stick | early_button);
				}
			}
		} break;
		case BUTTON_2H_BIT: {
			if (rng(0, 10) < execution_error_margin) {
				unsigned int early_button = rng(0, 1) ? BUTTON_HP_BIT : BUTTON_HK_BIT;
				input_frames++;
				owner->player->manual_seq.add_inputs(input_stick | early_button);
				if (rng(3, 10) < execution_error_margin) {
					input_frames++;
					owner->player->manual_seq.add_inputs(input_stick | early_button);
				}
			}
		} break;
		case BUTTON_3P_BIT: {
			if (rng(0, 10) < execution_error_margin) {
				unsigned int early_button = (rng(0, 1) ? BUTTON_LP_BIT : BUTTON_HP_BIT);
				input_frames++;
				owner->player->manual_seq.add_inputs(input_stick | early_button);
				if (rng(3, 10) < execution_error_margin) {
					input_frames++;
					owner->player->manual_seq.add_inputs(input_stick | early_button);
				}
			}
		} break;
		case BUTTON_3K_BIT: {
			if (rng(0, 10) < execution_error_margin) {
				unsigned int early_button = (rng(0, 1) ? BUTTON_LK_BIT : BUTTON_HK_BIT);
				input_frames++;
				owner->player->manual_seq.add_inputs(input_stick | early_button);
				if (rng(3, 10) < execution_error_margin) {
					input_frames++;
					owner->player->manual_seq.add_inputs(input_stick | early_button);
				}
			}
		} break;
		case BUTTON_6B_BIT:
		default: {
			//CPUs are allowed to always nail taunt inputs, that's fine
		} break;
	}
	input_frames++;
	owner->player->manual_seq.add_inputs(input_stick | input_buttons);
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
	fighter_context = FIGHTER_CONTEXT_GROUND;
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

void CPUAction::init(Fighter* owner, std::string script_name, std::string anim_name, int fighter_context,
	InputKind input_kind, unsigned short required_buttons, unsigned int pref_stick_dir, 
	std::set<unsigned int> stick_dirs, std::vector<CPUTag> tags, std::vector<std::string> auto_followups, 
	float resource_cost, bool allow_interrupt, bool manual_input) {
	this->name = script_name;
	this->fighter_context = fighter_context;
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
	if (!anim) return;
	MoveScript script = owner->move_script_table.get_script(script_name);
	script.activate();
	ScriptArg args;
	std::map<int, CPUHitbox> hitboxes;
	std::map<int, CPUHurtbox> hurtboxes;
	std::map<int, CPUGrabbox> grabboxes;
	std::deque<std::string> hit_cancel_options;
	std::deque<std::string> block_cancel_options;
	std::deque<std::string> cancel_options;

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
				unsigned int condition_kind = sf.script_funcs.front().condition_kind;
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

					if (condition_kind == SCRIPT_CONDITION_KIND_NONE) {
						attack_range_x[0] = std::min(attack_range_x[0], hitbox.anchor.x);
						attack_range_x[1] = std::max(attack_range_x[1], hitbox.offset.x);
						attack_range_y[0] = std::min(attack_range_y[0], hitbox.anchor.y);
						attack_range_y[1] = std::max(attack_range_y[1], hitbox.offset.y);
					}
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

					if (condition_kind == SCRIPT_CONDITION_KIND_NONE) {
						attack_range_x[0] = std::min(attack_range_x[0], grabbox.anchor.x);
						attack_range_x[1] = std::max(attack_range_x[1], grabbox.offset.x);
						attack_range_y[0] = std::min(attack_range_y[0], grabbox.anchor.y);
						attack_range_y[1] = std::max(attack_range_y[1], grabbox.offset.y);
					}

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

					if (condition_kind == SCRIPT_CONDITION_KIND_NONE) {
						defense_range_x[0] = std::min(defense_range_x[0], hurtbox.anchor.x);
						defense_range_x[1] = std::max(defense_range_x[1], hurtbox.offset.x);
						defense_range_y[0] = std::min(defense_range_y[0], hurtbox.anchor.y);
						defense_range_y[1] = std::max(defense_range_y[1], hurtbox.offset.y);
					}

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
				if (func == &Fighter::ENABLE_CANCEL) {
					UNWRAP(option, std::string);
					UNWRAP(kind, CancelKind);
					switch (kind) {
						case CANCEL_KIND_HIT: {
							this->hit_cancel_options.insert(option);
							hit_cancel_options.push_back(option);
						} break;
						case CANCEL_KIND_BLOCK: {
							this->block_cancel_options.insert(option);
							block_cancel_options.push_back(option);
						} break;
						case CANCEL_KIND_CONTACT: {
							this->hit_cancel_options.insert(option);
							this->block_cancel_options.insert(option);
							hit_cancel_options.push_back(option);
							block_cancel_options.push_back(option);

						} break;
						case CANCEL_KIND_ANY: {
							this->cancel_options.insert(option);
							cancel_options.push_back(option);
						} break;
					}
				}
				if (func == &Fighter::DISABLE_CANCEL) {
					UNWRAP(option, std::string);
					UNWRAP(kind, CancelKind);
					switch (kind) {
					case CANCEL_KIND_HIT: {
						for (std::deque<std::string>::iterator it = hit_cancel_options.begin(); 
							it != hit_cancel_options.end(); 
							it++) {
							if (*it == option) {
								hit_cancel_options.erase(it);
								break;
							}
						}
					} break;
					case CANCEL_KIND_BLOCK: {
						for (std::deque<std::string>::iterator it = block_cancel_options.begin();
							it != block_cancel_options.end();
							it++) {
							if (*it == option) {
								block_cancel_options.erase(it);
								break;
							}
						}
					} break;
					case CANCEL_KIND_CONTACT: {
						for (std::deque<std::string>::iterator it = hit_cancel_options.begin();
							it != hit_cancel_options.end();
							it++) {
							if (*it == option) {
								hit_cancel_options.erase(it);
								break;
							}
						}
						for (std::deque<std::string>::iterator it = block_cancel_options.begin();
							it != block_cancel_options.end();
							it++) {
							if (*it == option) {
								block_cancel_options.erase(it);
								break;
							}
						}
					} break;
					case CANCEL_KIND_ANY: {
						for (std::deque<std::string>::iterator it = cancel_options.begin();
							it != cancel_options.end();
							it++) {
							if (*it == option) {
								cancel_options.erase(it);
								break;
							}
						}
					} break;
					}
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
			block_cancel_options, cancel_options
		));
	}
	if (!active) std::swap(startup, recovery);
	attack_range_x += glm::vec2(min_trans_offset, max_trans_offset) * glm::vec2(owner->get_scale_vec().x);
	defense_range_x += glm::vec2(min_trans_offset, max_trans_offset) * glm::vec2(owner->get_scale_vec().x);
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
	context = FIGHTER_CONTEXT_GROUND;
	action_kind = CPU_ACTION_KIND_NORMAL;
	frame = 0;
	anim_frame = 0.0f;
	facing_dir = 1.0f;
	pos = glm::vec2(0.0f);
	base_pos = glm::vec2(0.0f);
	speed = glm::vec2(0.0f);
	actionable = false;
	frames_until_bufferable = 0;
}

CPUFrameData::CPUFrameData(float pos_offset, std::map<int, CPUHitbox> hitboxes,
	std::map<int, CPUHurtbox> hurtboxes, std::map<int, CPUGrabbox> grabboxes,
	std::deque<std::string> hit_cancel_options,
	std::deque<std::string> block_cancel_options,
	std::deque<std::string> cancel_options) {
	this->pos_offset = pos_offset;
	this->hitboxes = hitboxes;
	this->hurtboxes = hurtboxes;
	this->grabboxes = grabboxes;
	this->hit_cancel_options = hit_cancel_options;
	this->block_cancel_options = block_cancel_options;
	this->cancel_options = cancel_options;
}

CPUDecision::CPUDecision() {
	mode = CPU_DECISION_MODE_NONE;
	frames = 0;
	target_dist_x = glm::vec3(0.0);
	misc = -1;
}

CPUDecision::CPUDecision(CPUDecisionMode mode, FighterCPU* owner) {
	this->mode = mode;
	frames = 0;
	target_dist_x = glm::vec3(0.0);
	misc = -1;
	std::vector<CPUAction> actions;
	switch (mode) {
		case CPU_DECISION_MODE_NONE: {
			frames = owner->decision_update_interval;
		} break;
		case CPU_DECISION_MODE_WALK_BUTTON:
		case CPU_DECISION_MODE_DASH_BUTTON: {
			for (const auto& a : owner->actions) {
				CPUAction action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_GROUND 
					&& action.input_kind == INPUT_KIND_NORMAL) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_DECISION_MODE_WALK_COUNTERPOKE: {
			for (const auto& a : owner->actions) {
				CPUAction action = a.second;
				if (action.active 
					&& action.fighter_context == FIGHTER_CONTEXT_GROUND
					&& action.input_kind == INPUT_KIND_NORMAL
					&& action.attack_range_x[1] >= 250.0f) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_DECISION_MODE_WALK_SHIMMY: {
			for (const auto& a : owner->opponent->cpu.actions) {
				CPUAction action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_GROUND
					&& action.input_kind == INPUT_KIND_NORMAL
					&& action.attack_range_x[1] >= 250.0f) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_DECISION_MODE_JUMP_BUTTON: {
			for (const auto& a : owner->actions) {
				CPUAction action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_AIR
					&& action.input_kind == INPUT_KIND_NORMAL) {
					actions.push_back(action);
				}
			}

			//The JUMP decision is special in that we also need to determine a range threshold we're
			//supposed to try to reach before jumping. In order to figure this out, we need to already
			//know what action to pick, so we're going to decide it now instead of breaking out of the
			//switch statement.

			this->action = actions[rng(0, actions.size() - 1)];

			Fighter* fighter = owner->owner;
			CPUFighterState present_state = owner->states.newest();
			int fh_time = fighter->calc_airtime(fighter->get_param_float("jump_y_init_speed"));
			int sh_time = fighter->calc_airtime(fighter->get_param_float("jump_y_init_speed_s"));
			float f_speed = fighter->get_param_float("jump_x_speed");
			float fh_range_f = f_speed * fh_time * present_state.facing_dir;
			float sh_range_f = f_speed * sh_time * present_state.facing_dir;

			int movement_error = rng(-owner->movement_error_margin, owner->movement_error_margin);
			glm::vec2 f_atk_immediate_range = (action.attack_range_x
				+ glm::vec2(f_speed * action.startup)) * glm::vec2(present_state.facing_dir);

			glm::vec2 sf_atk_range(
				action.attack_range_x[0] * present_state.facing_dir,
				action.attack_range_x[1] * present_state.facing_dir + sh_range_f
			);

			glm::vec2 ff_atk_range(
				action.attack_range_x[0] * present_state.facing_dir,
				action.attack_range_x[1] * present_state.facing_dir + fh_range_f
			);

			glm::vec2 n_atk_range = (action.attack_range_x * glm::vec2(present_state.facing_dir));

			bool crossup_check = false;
			bool rising_check = false;
			bool spacing_check = false;
			std::vector<int> jump_types;
			std::vector<glm::vec2> jump_distances;
			for (size_t i = 0, max = action.tags.size(); i < max; i++) {
				switch (action.tags[i]) {
				case CPU_TAG_AIR_ATK_CROSSUP: {
					jump_distances.push_back(action.attack_range_x * glm::vec2(present_state.facing_dir));
					jump_types.push_back(CPU_JUMP_KIND_SHORTHOP_FORWARD);
				} break;
				case CPU_TAG_AIR_ATK_RISING: {
					jump_distances.push_back(action.attack_range_x * glm::vec2(present_state.facing_dir));
					jump_distances.push_back(f_atk_immediate_range);
					jump_types.push_back(CPU_JUMP_KIND_NEUTRAL);
					jump_types.push_back(CPU_JUMP_KIND_SHORTHOP_FORWARD);
				} break;
				default: {

				} break;
				}
			}

			jump_distances.push_back(sf_atk_range);
			jump_distances.push_back(ff_atk_range);
			jump_types.push_back(CPU_JUMP_KIND_SHORTHOP_FORWARD);
			jump_types.push_back(CPU_JUMP_KIND_FULLHOP_FORWARD);

			int selection = rng(0, jump_distances.size() - 1);
			target_dist_x = jump_distances[selection];
			misc = jump_types[selection]; 

			return;

		} break;
		case CPU_DECISION_MODE_ADVANCE: {

		} break;
		case CPU_DECISION_MODE_SPECIAL: {

		} break;
		default: {

		} break;
	}
	if (!actions.empty()) {
		this->action = actions[rng(0, actions.size() - 1)];
	}
	else {
		this->action = CPUAction();
	}
}