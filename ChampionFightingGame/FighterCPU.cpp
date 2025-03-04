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
	hit_confirm_skill = 0;
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
	add_action("wait", "wait", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 5, { 5 }, {}, {}, 0.0f, true, true);
	add_action("crouch", "crouch", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 1, { 1, 2, 3 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, true, true);
	add_action("crouch_d", "crouch_d", {}, { "crouch" }, true);
	add_action("crouch_u", "crouch_u", {}, { "wait" }, true);
	
	add_action("walk_f", "walk_f", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 6, { 6 }, {}, {}, 0.0f, true, true)
		.add_movement_info(owner->get_param_float("walk_f_speed"), 0.0f, 0.0f, 0.0f, {}, {});
	
	add_action("walk_b", "walk_b", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 4, { 4 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, true, true)
		.add_movement_info(-owner->get_param_float("walk_b_speed"), 0.0f, 0.0f, 0.0f, {}, {});
	
	add_action("dash_f", "dash_f", FIGHTER_CONTEXT_GROUND, INPUT_KIND_66, 0, 6, { 6 }, {}, {}, 0.0f, false, true)
		.add_movement_info(owner->get_param_float("walk_f_speed"), 0.0f, 0.0f, 0.0f, {
			{owner->get_param_int("dash_f_start_frame"), owner->get_param_float("dash_f_speed")},
			{owner->get_param_int("dash_f_stop_frame"), (owner->get_param_float("walk_f_speed") + owner->get_param_float("dash_f_speed")) / 2.0f}
		}, {});
	
	add_action("dash_b", "dash_b", FIGHTER_CONTEXT_GROUND, INPUT_KIND_44, 0, 4, { 4 }, { CPU_TAG_EVADE_ATK }, {}, 0.0f, false, true)
		.add_movement_info(owner->get_param_float("walk_b_speed"), 0.0f, 0.0f, 0.0f, {
			{owner->get_param_int("dash_b_start_frame"), owner->get_param_float("dash_b_speed")},
			{owner->get_param_int("dash_b_stop_frame"), (owner->get_param_float("walk_b_speed") + owner->get_param_float("dash_b_speed")) / 2.0f}
		}, {});
	add_action("parry_start_high", "parry_start_high", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		8, { 7, 8, 9 }, {}, {}, 0.0f, false, true);
	add_action("parry_start_mid", "parry_start_mid", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		5, { 4, 5, 6 }, {}, {}, 0.0f, false, true);
	add_action("parry_start_low", "parry_start_low", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT | BUTTON_MK_BIT,
		2, { 1, 2, 3 }, {}, {}, 0.0f, false, true);
	add_action("jump_squat", "jump_squat", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, 0, 8, { 7, 8, 9 }, { CPU_TAG_EVADE_ATK }, { "jump_n", "jump_f", "jump_b" }, 0.0f, false, true);
	add_action("jump_n", "jump_n", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 8, { 8 }, { }, { "fall" }, 0.0f, false, false)
		.add_movement_info(0.0f, owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"), {}, {});
	add_action("jump_f", "jump_f", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 9, { 9 }, { }, { "fall" }, 0.0f, false, false)
		.add_movement_info(owner->get_param_float("jump_x_speed"), owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"), {}, {});
	add_action("jump_b", "jump_b", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, 0, 7, { 7 }, { }, { "fall" }, 0.0f, false, false)
		.add_movement_info(-owner->get_param_float("jump_x_speed"), owner->get_param_float("jump_y_init_speed"), owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"), {}, {});
	add_action("fall", "fall", {}, {}, true)
		.add_movement_info(0.0f, 0.0f, owner->get_param_float("gravity"), owner->get_param_float("max_fall_speed"), {}, {});
	add_action("landing", "landing", {}, {}, false);
}

CPUAction& FighterCPU::add_action(std::string script_name, std::string anim_name, int context, 
	InputKind input_kind, unsigned short required_buttons, unsigned int pref_stick_dir, 
	std::set<unsigned int> stick_dirs, std::vector<CPUTag> tags, std::vector<std::string> auto_followups, 
	float resource_cost, bool allow_interrupt, bool allow_from_idle) {
	actions[script_name].init(owner, script_name, anim_name, context, input_kind, required_buttons,
		pref_stick_dir, stick_dirs, tags, auto_followups, resource_cost, allow_interrupt, allow_from_idle);
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
	if (curr_neutral.frames) curr_neutral.frames--;
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
		curr_neutral = CPUNeutral(CPU_NEUTRAL_MODE_NONE, this);
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
		int opp_hit_frame = get_hit_data(*opp_state.action, opp_present_state.frame,
			opp_present_state.facing_dir, opp_present_state.base_pos, *present_state.action,
			present_state.frame, present_state.facing_dir, present_state.base_pos, false, false).frame;
		if (opp_hit_frame != -1) {
			opp_remaining_hit_frames = (opp_hit_frame - opp_state.frame) - curr_reaction_time;
			for (auto& a : actions) {
				CPUAction action = a.second;
				if (action.active && action.startup <= opp_remaining_hit_frames) {
					int sim_hit_frame = get_hit_data(action, 0, present_state.facing_dir, 
						present_state.pos, *opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir, 
						opp_present_state.base_pos, true, false
					).frame + 1;
					int sim_opp_hit_frame = get_hit_data(*opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, action, 0, present_state.facing_dir,
						present_state.pos, false, false
					).frame;
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
							int sim_opp_hit_frame = get_hit_data(*opp_present_state.action,
								opp_present_state.frame, opp_present_state.facing_dir, 
								opp_present_state.base_pos, action, 0, 
								present_state.facing_dir, present_state.pos, false, false
							).frame;
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
						int test_hit_frame = get_hit_data(*opp_present_state.action, 
							opp_present_state.frame, opp_present_state.facing_dir, 
							opp_present_state.base_pos, actions["wait"], 0, present_state.facing_dir, 
							present_state.base_pos, false, false
						).frame;
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
					int sim_hit_frame = get_hit_data(action, 0, present_state.facing_dir,
						present_state.pos, *opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, true, false
					).frame + 1;
					int sim_opp_hit_frame = get_hit_data(*opp_present_state.action,
						opp_present_state.frame, opp_present_state.facing_dir,
						opp_present_state.base_pos, action, 0, present_state.facing_dir,
						present_state.pos, false, false
					).frame;
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
	switch (curr_neutral.mode) {
		default:
		case CPU_NEUTRAL_MODE_NONE: {
			if (!curr_neutral.frames) {
				int next_neutral_mode = rng(CPU_NEUTRAL_MODE_NONE, CPU_NEUTRAL_MODE_JUMP_BUTTON);
				curr_neutral = CPUNeutral((CPUNeutralMode)next_neutral_mode, this);
			}
		} break;
		case CPU_NEUTRAL_MODE_WALK_BUTTON: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * state.speed 
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(curr_neutral.action.attack_range_x[0] * state.facing_dir, curr_neutral.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_neutral.action.attack_range_x[1] * state.facing_dir, curr_neutral.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(opp_state.action->defense_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(opp_state.action->defense_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				execute_action(curr_neutral.action);
				cpu_mode = CPU_MODE_ATTACK;
				curr_neutral = CPUNeutral(CPU_NEUTRAL_MODE_NONE, this);
				return;
			}
			if (!curr_neutral.frames) {
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
				curr_neutral.frames = rng(1, 20);
			}
			else {
				execute_action(actions["walk_f"]);
			}
		} break;
		case CPU_NEUTRAL_MODE_WALK_COUNTERPOKE: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time -1 + movement_error) * state.speed
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(curr_neutral.action.attack_range_x[0] * state.facing_dir, curr_neutral.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_neutral.action.attack_range_x[1] * state.facing_dir, curr_neutral.action.attack_range_y[1])
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

			if (is_rect_collide(c1, c2, opp_c1, opp_c2) || is_rect_collide(sim_c1, sim_c2, sim_opp_c1, sim_opp_c2)) {
				std::vector<std::string> buffer_options;
				for (auto& a : actions) {
					if (curr_neutral.action.hit_cancel_options.contains(a.first)) {
						buffer_options.push_back(a.first);
					}
				}
				owner->player->manual_seq.reset_idx();
				buffer_action(curr_neutral.action);
					
				if (!buffer_options.empty()) {
					for (int i = 0; i < curr_neutral.action.startup - 1; i++) {
						add_input(owner->get_stick_dir(), 0);
					}
					CPUAction action = actions[buffer_options[rng(0, buffer_options.size() - 1)]];
					buffer_action(action);
				}
				owner->player->manual_seq.reset_idx();
				cpu_mode = CPU_MODE_ATTACK;
				curr_neutral = CPUNeutral(CPU_NEUTRAL_MODE_NONE, this);
			}
			else {
				execute_action(actions["walk_f"]);
			}
		} break;
		case CPU_NEUTRAL_MODE_WALK_SHIMMY: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * state.speed
				+ state.base_pos;
			glm::vec2 c1 = glm::vec2(state.action->defense_range_x[0] * state.facing_dir, curr_neutral.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(state.action->defense_range_x[1] * state.facing_dir, curr_neutral.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(curr_neutral.action.attack_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(curr_neutral.action.attack_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				curr_neutral.frames = rng(1, 10);
			}
			else if (!curr_neutral.frames) {
				if (rng(0, 3)) {
					execute_action(actions["walk_f"]);
				}
				else {
					int next_neutral_mode = rng(CPU_NEUTRAL_MODE_NONE, CPU_NEUTRAL_MODE_DASH_BUTTON);
					while (next_neutral_mode == CPU_NEUTRAL_MODE_WALK_SHIMMY) {
						next_neutral_mode = rng(CPU_NEUTRAL_MODE_NONE, CPU_NEUTRAL_MODE_DASH_BUTTON);
					}
					curr_neutral = CPUNeutral((CPUNeutralMode)next_neutral_mode, this);
				}
			}
			if (curr_neutral.frames) {
				execute_action(actions["walk_b"]);
			}
		} break;
		case CPU_NEUTRAL_MODE_DASH_BUTTON: {
			float move_range_error = rng_f(-move_range_error_margin, move_range_error_margin);
			int movement_error = rng(-movement_error_margin, movement_error_margin);
			glm::vec2 sim_pos = present_state.pos;
			glm::vec2 c1 = glm::vec2(curr_neutral.action.attack_range_x[0] * state.facing_dir, curr_neutral.action.attack_range_y[0])
				+ move_range_error + sim_pos;
			glm::vec2 c2 = glm::vec2(curr_neutral.action.attack_range_x[1] * state.facing_dir, curr_neutral.action.attack_range_y[1])
				+ move_range_error + sim_pos;
			glm::vec2 opp_sim_pos = glm::vec2(curr_reaction_time - 1 + movement_error) * opp_state.speed
				+ opp_state.base_pos;
			glm::vec2 opp_c1 = glm::vec2(opp_state.action->defense_range_x[0] * opp_state.facing_dir, opp_state.action->defense_range_y[0])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			glm::vec2 opp_c2 = glm::vec2(opp_state.action->defense_range_x[1] * opp_state.facing_dir, opp_state.action->defense_range_y[1])
				+ glm::vec2(move_range_error) + opp_sim_pos;
			if (is_rect_collide(c1, c2, opp_c1, opp_c2)) {
				if (present_state.frames_until_bufferable <= get_frames_to_input(curr_neutral.action)) {
					execute_action(curr_neutral.action);
					cpu_mode = CPU_MODE_ATTACK;
					curr_neutral = CPUNeutral(CPU_NEUTRAL_MODE_NONE, this);
				}
			}
			else if (present_state.frames_until_bufferable <= get_frames_to_input(actions["dash_f"])) {
				execute_action(actions["dash_f"]);
			}
		} break;
		case CPU_NEUTRAL_MODE_JUMP_BUTTON: {
			if (present_state.context == FIGHTER_CONTEXT_GROUND 
				&& owner->status_kind != FIGHTER_STATUS_JUMPSQUAT) {

				int fh_time = owner->calc_airtime(owner->get_param_float("jump_y_init_speed"));
				int sh_time = owner->calc_airtime(owner->get_param_float("jump_y_init_speed_s"));
				float f_speed = owner->get_param_float("jump_x_speed");

				float sim_pos_x_fh = (fh_time * f_speed * present_state.facing_dir) + present_state.pos.x;
				float sim_pos_x_sh = (sh_time * f_speed * present_state.facing_dir) + present_state.pos.x;

				glm::vec2 target_dist_x = curr_neutral.target_dist_x + glm::vec2(present_state.pos.x);
				switch (curr_neutral.misc) {
					case CPU_JUMP_KIND_SHORTHOP_FORWARD: {
						target_dist_x = curr_neutral.target_dist_x + glm::vec2(sim_pos_x_sh);
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
						target_dist_x = curr_neutral.target_dist_x + glm::vec2(sim_pos_x_fh);
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
				int sim_hit_frame = get_hit_data(curr_neutral.action, 0, present_state.facing_dir,
					present_state.pos, *opp_present_state.action, opp_present_state.frame, 
					opp_present_state.facing_dir, opp_present_state.pos, true, false
				).frame;
				if (sim_hit_frame != -1) {
					execute_action(curr_neutral.action);
					cpu_mode = CPU_MODE_ATTACK;
					curr_neutral = CPUNeutral(CPU_NEUTRAL_MODE_NONE, this);
					return;
				}
			}
		} break;
		case CPU_NEUTRAL_MODE_ADVANCE: {

		} break;
		case CPU_NEUTRAL_MODE_SPECIAL: {

		} break;
	}
}

void FighterCPU::process_attack() {
	CPUFighterState present_state = states.newest();
	CPUFighterState opp_present_state = opponent_states.newest();
	if (present_state.frame == 0) {
		curr_attack.allow_confirm = false;
	}
	if (curr_attack.mode == CPU_ATTACK_MODE_NONE) {
		curr_attack = CPUAttack((CPUAttackMode)rng(CPU_ATTACK_MODE_NONE + 1, CPU_ATTACK_MODE_CORNER), this);
	}
	if (input_frames) return;
	if (present_state.action->is_strike) {
		//We're allowed to pick followup options for strikes every time we land a hit
		int opp_hitlag = opponent->object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES];
		if (opp_hitlag) {
			if (opp_hitlag == opponent->object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]) {
				curr_attack.update_action(find_followup_actions());
			}
		}
	}
	else if (present_state.action->is_throw) {
		//TODO: Figure out when to pick followup options for throws
	}
	else {
		//We're allowed to pick followup options for non-attacks exactly once, usually their first
		//active frame but projectiles might also end up using this check.

		//If we're in the attack state using something that isn't an attack, our action must
		//inherently be part of a sequence which will lead back into an attack anyway
		if (!curr_attack.allow_confirm) {
			std::vector<CPUAction> followups = find_followup_actions();
			if (!followups.empty()) {
				curr_attack.update_action(followups);
				curr_attack.auto_confirmed = true;
				curr_attack.allow_confirm = true;
			}
		}
	}
	if (curr_attack.allow_confirm) {
		bool confirm = (curr_attack.confirmed && owner->object_flag[FIGHTER_FLAG_ATTACK_HIT]) || curr_attack.auto_confirmed;
		if (confirm) {
			int frames = get_frames_to_input(curr_attack.action) + 1;
			bool can_execute = present_state.frames_until_bufferable <= frames;
			if (!can_execute) {
				if (present_state.action->frame_data.size() > present_state.frame + frames) {
					CPUFrameData fd = present_state.action->frame_data[present_state.frame + frames];
					for (size_t i = 0, max = fd.cancel_options.size(); i < max && !can_execute; i++) {
						if (fd.cancel_options[i] == curr_attack.action.name) {
							can_execute = true;
						}
					}
					for (size_t i = 0, max = fd.hit_cancel_options.size(); i < max && !can_execute; i++) {
						if (fd.hit_cancel_options[i] == curr_attack.action.name) {
							can_execute = true;
						}
					}
				}
				else {
					can_execute = true;
				}
			}
			if (can_execute) {
				std::cout << "Executing attack " << curr_attack.action.name << "\n";
				execute_action(curr_attack.action);
				return;
			}
		}
		else if (present_state.frame >= curr_attack.action.startup - 1) {
			int curr_reaction_time = reaction_time - 1;
			if (prediction_mode) {
				curr_reaction_time = correct_prediction_time - 1;
			}


			int confirmability = 0;
			CPUAction* action = present_state.action;
			for (int i = 0, max = action->tags.size(); i < max; i++) {
				if (action->tags[i] == CPU_TAG_HIT_CONFIRM) {
					confirmability = 1;
					break;
				}
				if (action->tags[i] == CPU_TAG_MULTIHIT_CONFIRM) {
					confirmability = 2;
					break;
				}
			}

			switch (confirmability) {
				case 0: {
					if (rng(0, 11 - impulse) == 0) {
						curr_attack.auto_confirmed = true;
					}
				} break;
				case 1: {
					if (rng(0, std::max(curr_reaction_time - (hit_confirm_skill - 5), 0)) == 0) {
						curr_attack.confirmed = true;
					}
					else if (present_state.action->hit_cancel_options.contains(curr_attack.action.name)
						&& (owner->object_flag[FIGHTER_FLAG_ATTACK_HIT] || owner->object_flag[FIGHTER_FLAG_ATTACK_BLOCKED])
						&& !owner->object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
						if (rng(0, hit_confirm_skill * 2) == 0) {
							curr_attack.auto_confirmed = true;
						}
					}
				} break;
				case 2: {
					if (rng(0, curr_reaction_time - (hit_confirm_skill - 5)) == 0) {
						curr_attack.confirmed = true;
					}
					else {
						
						std::vector<CPUAction> confirm_options;
						for (CPUAction& action : find_followup_actions()) {
							if ((action.startup < present_state.action->hit_advantage &&
								action.startup < present_state.action->block_advantage + 3
								+ (10 - rng(0, cautiousness)))
							|| present_state.action->cancel_options.contains(action.name)
							|| (present_state.action->hit_cancel_options.contains(action.name) &&
								present_state.action->block_cancel_options.contains(action.name))) {
								int startup = curr_attack.action.startup;
								if (!startup) {
									startup = curr_attack.action.total;
									for (int i = 0, max = curr_attack.action.frame_data.size(); i < max; i++) {
										//TODO: Account for auto followups and moves with multiple whiff cancel options
										if (!curr_attack.action.frame_data[i].cancel_options.empty()) {
											startup = this->actions[curr_attack.action.frame_data[i].cancel_options[0]].startup;
										}
									}
								}
								if (action.cancel_options.contains(curr_attack.action.name)
								|| action.hit_cancel_options.contains(curr_attack.action.name)
								|| startup < action.hit_advantage) {
									std::cout << action.name << " is a confirm option for " << present_state.action->name << "\n";
									confirm_options.push_back(action);
								}
							}
						}
						if (!confirm_options.empty()) {
							execute_action(confirm_options[rng(0, confirm_options.size() - 1)]);
						}
					}
				} break;
			}
		}
	}
	if (present_state.actionable) {
		if (curr_attack.allow_confirm) {
			if (curr_attack.confirmed || curr_attack.auto_confirmed) {
				std::cout << "???\n";
			}
			else {
				std::cout << "We became fully actionable without ever confirming our hit\n";
			}
		}
		else {
			std::cout << "We became fully actionable but never found a valid confirm option\n";
		}

		curr_attack = CPUAttack(CPU_ATTACK_MODE_NONE, this);
		cpu_mode = CPU_MODE_NEUTRAL;
		process_neutral();
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
				add_input(action.pref_stick_dir, 0);
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
			new_state[i].action = &fighter[i]->cpu.actions[script_name];
			new_state[i].base_pos.x -= new_state[i].action->frame_data[new_state[i].frame].pos_offset * opponent->get_scale_vec().x * new_state[i].facing_dir;
		}
		else {
			new_state[i].action = &fighter[i]->cpu.actions["wait"];
		}
		if (fighter[i]->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
			if (fighter[i]->status_kind == FIGHTER_STATUS_BLOCKSTUN) {
				new_state[i].action_kind = CPU_ACTION_KIND_BLOCKSTUN;
			}
			else if (fighter[i]->object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_HITSTUN) {
				new_state[i].action_kind = CPU_ACTION_KIND_HITSTUN;
			}
			else {
				new_state[i].action_kind = CPU_ACTION_KIND_NORMAL; //Landing
			}
		}
		else {
			new_state[i].action_kind = CPU_ACTION_KIND_NORMAL;
		}
		new_state[i].actionable = new_state[i].frame >= new_state[i].action->total;
		new_state[i].frames_until_bufferable = std::max(new_state[i].action->total - new_state[i].frame, 0);
	}
	states.insert(new_state[0]);
	opponent_states.insert(new_state[1]);
}

CPUHitData FighterCPU::get_hit_data(CPUAction atk_action, int atk_frame, float atk_facing_dir, 
	glm::vec2 atk_base_pos, CPUAction def_action, int def_frame, float def_facing_dir, 
	glm::vec2 def_base_pos, bool caller_is_attacker, bool defender_in_hitstun) {
	CPUHitData ret;
	FighterCPU *attacker = this;
	FighterCPU *defender = &opponent->cpu;
	if (!caller_is_attacker) std::swap(attacker, defender);
	float atk_x_speed, atk_y_speed, atk_y_accel, atk_y_max;
	glm::vec2 atk_speed_accel, atk_speed_total;
	init_action_speed_vars(atk_action, attacker->owner, &atk_x_speed, &atk_y_speed, &atk_y_accel, 
		&atk_y_max, &atk_speed_accel, &atk_speed_total);
	float def_x_speed, def_y_speed, def_y_accel, def_y_max;
	glm::vec2 def_speed_accel, def_speed_total;
	init_action_speed_vars(def_action, defender->owner, &def_x_speed, &def_y_speed, &def_y_accel,
		&def_y_max, &def_speed_accel, &def_speed_total);
	for (int atk_active = atk_action.active + atk_action.startup - 1;
		atk_frame < atk_active; atk_frame++, def_frame++) {
		if (atk_frame >= atk_action.frame_data.size()) {
			atk_frame = 0;
			if (!atk_action.auto_followups.empty()) {
				atk_action = attacker->actions[atk_action.auto_followups[0]];
				init_action_speed_vars(atk_action, attacker->owner, &atk_x_speed, &atk_y_speed, &atk_y_accel,
					&atk_y_max, &atk_speed_accel, &atk_speed_total);
			}
			else {
				return ret;
			}
		}
		if (def_frame >= def_action.frame_data.size() && !defender_in_hitstun) {
			def_frame = 0;
			if (!def_action.auto_followups.empty()) {
				def_action = defender->actions[def_action.auto_followups[0]];
				init_action_speed_vars(def_action, defender->owner, &def_x_speed, &def_y_speed, &def_y_accel,
					&def_y_max, &def_speed_accel, &def_speed_total);
			}
		}
		CPUFrameData atk_frame_data = atk_action.frame_data[atk_frame];
		CPUFrameData def_frame_data;
		if (defender_in_hitstun) {
			def_frame_data = def_action.frame_data[0];
		}
		else {
			def_frame_data = def_action.frame_data[def_frame];
		}
		atk_speed_accel.y += atk_y_accel;
		def_speed_accel.y += def_y_accel;
		if (atk_y_max != 0.0f) atk_speed_accel.y = std::max(-atk_y_max, atk_speed_accel.y);
		if (def_y_max != 0.0f) def_speed_accel.y = std::max(-def_y_max, def_speed_accel.y);
		if (atk_action.movement_info.x_speed_changes.contains(atk_frame)) {
			atk_speed_accel.x = atk_action.movement_info.x_speed_changes[atk_frame];
		}
		if (atk_action.movement_info.y_speed_changes.contains(atk_frame)) {
			atk_speed_accel.y = atk_action.movement_info.y_speed_changes[atk_frame];
		}
		if (def_action.movement_info.x_speed_changes.contains(def_frame)) {
			def_speed_accel.x = def_action.movement_info.x_speed_changes[def_frame];
		}
		if (def_action.movement_info.y_speed_changes.contains(def_frame)) {
			def_speed_accel.y = def_action.movement_info.y_speed_changes[def_frame];
		}
		atk_speed_total += atk_speed_accel;
		def_speed_total += def_speed_accel;
		glm::vec2 atk_pos = atk_base_pos + glm::vec2(atk_frame_data.pos_offset * attacker->owner->get_scale_vec().x * atk_facing_dir, 0.0f)
			+ atk_speed_total;
		glm::vec2 def_pos = def_base_pos + glm::vec2(def_frame_data.pos_offset * defender->owner->get_scale_vec().x * def_facing_dir, 0.0f)
			+ def_speed_total;
		int hitbox_id;
		if (check_contact(def_frame_data, def_pos, def_facing_dir, atk_frame_data, atk_pos, atk_facing_dir, &hitbox_id)) {
			ret.frame = atk_frame;
			ret.hitstun = atk_frame_data.hitboxes[hitbox_id].hit_result.hitstun;
			ret.pos = atk_pos;
			ret.opp_pos = def_pos;
			ret.pushback_frames = atk_frame_data.hitboxes[hitbox_id].hit_move.pushback_frames;
			ret.pushback_total.x = atk_frame_data.hitboxes[hitbox_id].hit_move.pushback_hit;
			break;
		}
	}
	return ret;
}

glm::vec2 FighterCPU::get_end_pos(CPUAction action, glm::vec2 base_pos) {
	float x_speed, y_speed, y_accel, y_max;
	glm::vec2 speed_accel, speed_total, pos;
	init_action_speed_vars(action, owner, &x_speed, &y_speed, &y_accel,
		&y_max, &speed_accel, &speed_total);
	pos = base_pos;
	for (int i = 0, max = action.frame_data.size(); i < max; i++) {
		CPUFrameData fd = action.frame_data[i];
		speed_accel.y += y_accel;
		if (y_max != 0.0f) speed_accel.y = std::max(-y_max, speed_accel.y);
		if (action.movement_info.x_speed_changes.contains(i)) {
			speed_accel.x = action.movement_info.x_speed_changes[i];
		}
		if (action.movement_info.y_speed_changes.contains(i)) {
			speed_accel.y = action.movement_info.y_speed_changes[i];
		}
		speed_total += speed_accel;
		pos = base_pos + glm::vec2(fd.pos_offset * owner->get_scale_vec().x * states.newest().facing_dir, 0.0f)
			+ speed_total;
	}
	return pos;
}

void FighterCPU::init_action_speed_vars(CPUAction action, Fighter* owner, float* x_speed, float* y_speed, float* y_accel, float* y_max, glm::vec2* speed_accel, glm::vec2* speed_total) {
	*x_speed = action.movement_info.x_speed;
	*y_speed = action.movement_info.y_speed;
	*y_accel = action.movement_info.y_accel;
	*y_max = action.movement_info.y_max;
	if (action.fighter_context == FIGHTER_CONTEXT_AIR) {
		if (*x_speed == 0.0f) {
			*x_speed = owner->object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * owner->facing_dir;
		}
		if (*y_speed == 0.0f) {
			*y_speed = owner->object_float[BATTLE_OBJECT_FLOAT_Y_SPEED];
		}
		if (*y_accel == 0.0f) {
			*y_accel = owner->object_float[FIGHTER_FLOAT_CURRENT_GRAVITY];
		}
		if (*y_max == 0.0f) {
			*y_max = owner->object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX];
		}
	}
	*speed_accel = glm::vec2(*x_speed * owner->facing_dir, *y_speed);
	*speed_total = *speed_accel;

}

bool FighterCPU::check_contact(CPUFighterState& state, CPUFighterState& opp_state) {
	if (!state.action || !opp_state.action) return false;

	int hitbox_id = 0;
	return check_contact(state.action->frame_data[state.frame], state.pos, state.facing_dir,
		opp_state.action->frame_data[opp_state.frame], opp_state.pos, opp_state.facing_dir, &hitbox_id);
}

bool FighterCPU::check_contact(CPUFrameData& frame_data, glm::vec2 pos, float facing_dir, 
	CPUFrameData& opp_frame_data, glm::vec2 opp_pos, float opp_facing_dir, int* hitbox_id) {
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
				*hitbox_id = hit.first;
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

std::vector<CPUAction> FighterCPU::find_followup_actions() {
	CPUFighterState present_state = states.newest();
	CPUFighterState opp_present_state = opponent_states.newest();

	int remaining_hitstun = opponent->object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES];
	glm::vec2 opp_pos = opp_present_state.base_pos;
	glm::vec2 pushback = glm::vec2(opponent->object_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME], 0.0);
	int pushback_frames = opponent->object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	std::vector<CPUAction> ret = find_followup_actions_rec(*present_state.action, present_state.frame,
		remaining_hitstun, present_state.action->total - present_state.frame - 1,
		get_end_pos(*present_state.action, present_state.base_pos), present_state.pos,
		opp_present_state.base_pos, pushback, pushback_frames
	);
	std::cout << "Found " << ret.size() << " possible followups for " << present_state.action->name << "\n";
	return ret;
}

std::vector<CPUAction> FighterCPU::find_followup_actions_rec(CPUAction curr_action, int frame,
	int remaining_hitstun, int recovery, glm::vec2 pos_link, glm::vec2 pos_cancel, glm::vec2 opp_pos,
	glm::vec2 pushback, int pushback_frames) {
	CPUFighterState present_state = states.newest();
	CPUFighterState opp_present_state = opponent_states.newest();
	CPUAction opp_sim_action = *opp_present_state.action;
	if (pushback_frames > 0) {
		opp_sim_action.movement_info.x_speed = pushback.x * -1;
		opp_sim_action.movement_info.x_speed_changes = { {pushback_frames, 0.0f} };
	}

	std::cout << "Looking for followups to " << curr_action.name << ": " << remaining_hitstun << "f hitstun, "
		<< recovery << "f recovery, ";
	if (remaining_hitstun - recovery >= 0) {
		std::cout << "+";
	}
	std::cout << (remaining_hitstun - recovery) << "\n";
	std::vector<CPUAction> ret;
	for (auto& a : actions) {
		std::string name = a.first;
		CPUAction& action = a.second;

		if (curr_action.name == "jump_squat") {
			if (action.fighter_context != FIGHTER_CONTEXT_AIR) continue;
		}
		else if (curr_action.fighter_context != action.fighter_context) continue;

		if (action.is_strike) {
			if (action.allow_from_idle && (remaining_hitstun - recovery) >= action.startup) {
				std::cout << curr_action.name << " -> " << name << ", Startup: " << action.startup
					<< " (link):";
				CPUHitData hit_data = get_hit_data(action, 0, present_state.facing_dir, pos_link,
					opp_sim_action, opp_present_state.frame, opp_present_state.facing_dir,
					opp_pos, true, true
				);
				if (hit_data.frame != -1) {
					std::cout << " Hits\n";
					ret.push_back(action);
					continue;
				}
				else {
					std::cout << " Whiffs\n";
				}
			}
			if (curr_action.hit_cancel_options.contains(name)
			|| curr_action.cancel_options.contains(name)) {
				std::cout << curr_action.name << " -> " << name << ", Startup: " << action.startup
					<< " (cancel):";
				CPUHitData hit_data = get_hit_data(action, 0, present_state.facing_dir, pos_cancel,
					opp_sim_action, opp_present_state.frame, opp_present_state.facing_dir,
					opp_pos, true, true
				);
				if (hit_data.frame != -1) {
					std::cout << " Hits\n";
					ret.push_back(action);
				}
				else {
					std::cout << " Whiffs\n";
				}
			}	
		}
		else if (action.is_throw) {

		}
		else {
			if (action.total <= 0) continue;
			if (curr_action.cancel_options.contains(name)
			|| curr_action.hit_cancel_options.contains(name)) {
				int cancel_frame = 0;
				for (int i = 0; i < curr_action.total && cancel_frame == 0; i++) {
					for (int i2 = 0, max2 = curr_action.frame_data[i].hit_cancel_options.size(); i2 < max2; i2++) {
						if (curr_action.frame_data[i].hit_cancel_options[i2] == name) {
							cancel_frame = i;
							break;
						}
					}
					for (int i2 = 0, max2 = curr_action.frame_data[i].cancel_options.size(); i2 < max2; i2++) {
						if (curr_action.frame_data[i].cancel_options[i2] == name) {
							cancel_frame = i;
							break;
						}
					}
				}
				std::vector<CPUAction> followups = find_followup_actions_rec(
					action, 0, remaining_hitstun - cancel_frame, action.total, pos_cancel,
					pos_cancel, opp_pos, pushback, pushback_frames - cancel_frame
				);
				if (!followups.empty()) {
					std::cout << curr_action.name << " -> " << name << " is a legal cancel. It has "
					<< followups.size() << " valid followup options\n";
					ret.push_back(action);
				}
				else {
					std::cout << curr_action.name << " -> " << name << " is a legal cancel, but has no "
						<< "valid followup options\n";
				}
			}
			else if (action.total < remaining_hitstun - recovery) {
				bool auto_followup = false;
				for (int i = 0; i < curr_action.auto_followups.size(); i++) {
					if (curr_action.auto_followups[i] == name) {
						auto_followup = true;
						break;
					}
				}
				if (action.allow_from_idle || auto_followup) {
					std::vector<CPUAction> followups = find_followup_actions_rec(
						action, 0, remaining_hitstun - recovery, action.total, get_end_pos(action, pos_link),
						get_end_pos(action, pos_link), opp_pos, pushback, pushback_frames - recovery
					);
					if (!followups.empty()) {
						std::cout << curr_action.name << " -> " << name << " is a legal link. It has "
							<< followups.size() << " valid followup options\n";
						ret.push_back(action);
					}
					else {
						std::cout << curr_action.name << " -> " << name << " is a legal link, but has no "
							<< "valid followup options\n";
					}
				}
			}
		}
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

	hitstun = 0;
	blockstun = 0;
	hit_advantage = 0;
	block_advantage = 0;

	is_strike = false;
	is_throw = false;

	attack_range_x = glm::vec2(0.0f);
	attack_range_y = glm::vec2(0.0f);
	defense_range_x = glm::vec2(0.0f);
	defense_range_y = glm::vec2(0.0f);

	resource_cost = 0.0f;
	allow_interrupt = false;
	allow_from_idle = true;
}

void CPUAction::init(Fighter* owner, std::string script_name, std::string anim_name, int fighter_context,
	InputKind input_kind, unsigned short required_buttons, unsigned int pref_stick_dir, 
	std::set<unsigned int> stick_dirs, std::vector<CPUTag> tags, std::vector<std::string> auto_followups, 
	float resource_cost, bool allow_interrupt, bool allow_from_idle) {
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
	this->allow_from_idle = allow_from_idle;
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

		float trans_offset = anim->get_trans_offset(frame);
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

					hitstun = std::max(hitbox.hit_result.hitstun, hitstun);
					blockstun = std::max(hitbox.hit_result.blockstun, blockstun);
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
	hitstun--;
	blockstun--;
	hit_advantage = hitstun - recovery;
	block_advantage = blockstun - recovery;
}

void CPUAction::add_movement_info(float x_speed, float y_speed, float y_accel, float y_max,
	std::map<int, float> x_speed_changes, std::map<int, float> y_speed_changes) {
	this->movement_info.x_speed = x_speed;
	this->movement_info.y_speed = y_speed;
	this->movement_info.y_accel = y_accel;
	this->movement_info.y_max = y_max;
	this->movement_info.x_speed_changes = x_speed_changes;
	this->movement_info.y_speed_changes = y_speed_changes;
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

CPUFrameData::CPUFrameData() {
	this->pos_offset = 0.0f;
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

CPUNeutral::CPUNeutral() {
	mode = CPU_NEUTRAL_MODE_NONE;
	frames = 0;
	target_dist_x = glm::vec3(0.0);
	misc = -1;
}

CPUNeutral::CPUNeutral(CPUNeutralMode mode, FighterCPU* owner) {
	this->mode = mode;
	frames = 0;
	target_dist_x = glm::vec3(0.0);
	misc = -1;
	std::vector<CPUAction> actions;
	switch (mode) {
		case CPU_NEUTRAL_MODE_NONE: {
			frames = owner->decision_update_interval;
		} break;
		case CPU_NEUTRAL_MODE_WALK_BUTTON:
		case CPU_NEUTRAL_MODE_DASH_BUTTON: {
			for (auto& a : owner->actions) {
				CPUAction &action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_GROUND 
					&& action.input_kind == INPUT_KIND_NORMAL
					&& action.allow_from_idle) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_NEUTRAL_MODE_WALK_COUNTERPOKE: {
			for (auto& a : owner->actions) {
				CPUAction &action = a.second;
				if (action.active 
					&& action.fighter_context == FIGHTER_CONTEXT_GROUND
					&& action.input_kind == INPUT_KIND_NORMAL
					&& action.allow_from_idle
					&& action.attack_range_x[1] >= 240.0f) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_NEUTRAL_MODE_WALK_SHIMMY: {
			for (auto& a : owner->opponent->cpu.actions) {
				CPUAction& action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_GROUND
					&& action.input_kind == INPUT_KIND_NORMAL
					&& action.attack_range_x[1] >= 250.0f) {
					actions.push_back(action);
				}
			}
		} break;
		case CPU_NEUTRAL_MODE_JUMP_BUTTON: {
			for (auto& a : owner->actions) {
				CPUAction &action = a.second;
				if (action.active && action.fighter_context == FIGHTER_CONTEXT_AIR
					&& action.input_kind == INPUT_KIND_NORMAL) {
					actions.push_back(action);
				}
			}

			//The JUMP decision is special in that we also need to determine a range threshold we're
			//supposed to try to reach before jumping. In order to figure this out, we need to already
			//know what action to pick, so we're going to decide it now instead of breaking out of the
			//switch statement.

			if (!actions.empty()) {
				this->action = actions[rng(0, actions.size() - 1)];
			}

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
		case CPU_NEUTRAL_MODE_ADVANCE: {

		} break;
		case CPU_NEUTRAL_MODE_SPECIAL: {

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

CPUAttack::CPUAttack() {
	mode = CPU_ATTACK_MODE_NONE;
	allow_confirm = false;
	confirmed = false;
	auto_confirmed = false;
}

CPUAttack::CPUAttack(CPUAttackMode mode, FighterCPU* owner) {
	this->mode = mode;
	allow_confirm = false;
	confirmed = false;
	auto_confirmed = false;
}

void CPUAttack::update_action(std::vector<CPUAction> actions) {
	if (!actions.empty()) {
		std::cout << "Updating action: ";
		this->action = actions[rng(0, actions.size() - 1)];
		std::cout << action.name << "\n";
		allow_confirm = true;
		return;
	}
	confirmed = false;
	auto_confirmed = false;
	allow_confirm = false;
}