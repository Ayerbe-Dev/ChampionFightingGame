#include "BattleObject.h"

void BattleObject::SET_RATE(ScriptArg args) {
	if (args.args.front().type() != typeid(float)) {
		std::cout << args.args.front().type().name() << "\n";
	}
	UNWRAP(rate, float);
	set_rate(rate);
}

void BattleObject::SET_FRAME(ScriptArg args) {
	UNWRAP(frame, float);
	set_rate(frame);
}

void BattleObject::NEW_BLOCKBOX(ScriptArg args) {
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	new_blockbox(anchor, offset);
}

void BattleObject::PLAY_SE(ScriptArg args) {
	UNWRAP(se, std::string);
	play_se(se);
}

void BattleObject::PLAY_VC(ScriptArg args) {
	UNWRAP(vc, std::string);
	play_vc(vc);
}

void BattleObject::NEW_EFFECT(ScriptArg args) {
	int overload = 3;
	std::any bone_container;
	std::any offset_container;
	UNWRAP(name, std::string);
	UNWRAP(pos, glm::vec3);
	UNWRAP(rot, glm::vec3);
	UNWRAP(scale, glm::vec3);
	UNWRAP(rgba, glm::vec4);
	if (args.args.front().type() != typeid(glm::vec3)) {
		if (args.args.front().type() == typeid(int)) {
			overload = 1;

		}
		else if (args.args.front().type() == typeid(std::string)) {
			overload = 2;
		}
		bone_container = args.get_arg();
		offset_container = args.get_arg();
	}
	else {
		overload = 0;
	}
	UNWRAP(pos_frame, glm::vec3);
	UNWRAP(rot_frame, glm::vec3);
	UNWRAP(scale_frame, glm::vec3);
	UNWRAP(rgba_frame, glm::vec4);
	float rate = 1.0;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0;
	UNWRAP_NO_DECL(frame);
	switch (overload) {
		case (0): {
			new_effect(name, pos, rot, scale, rgba, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
		} break;
		case (1): {
			int bone_id = std::any_cast<int>(bone_container);
			glm::vec3 bone_offset = std::any_cast<glm::vec3>(offset_container);
			new_effect(name, pos, rot, scale, rgba, bone_id, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
		} break;
		case (2): {
			std::string bone_name = std::any_cast<std::string>(bone_container);
			glm::vec3 bone_offset = std::any_cast<glm::vec3>(offset_container);
			new_effect(name, pos, rot, scale, rgba, bone_name, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
		} break;
		default: {
			GameManager::get_instance()->add_crash_log("ERROR: Arg 6 of NEW_EFFECT in script " 
				+ active_move_script.name + " was neither int, string, nor glm::vec3. How you managed "
				+ "to get here without crashing is beyond me.");
		} break;
	}
}

void BattleObject::NEW_EFFECT_NO_FOLLOW(ScriptArg args) {
	UNWRAP(name, std::string);
	UNWRAP(pos, glm::vec3);
	UNWRAP(rot, glm::vec3);
	UNWRAP(scale, glm::vec3);
	UNWRAP(rgba, glm::vec4);
	UNWRAP(pos_frame, glm::vec3);
	UNWRAP(rot_frame, glm::vec3);
	UNWRAP(scale_frame, glm::vec3);
	UNWRAP(rgba_frame, glm::vec4);
	float rate = 1.0;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0;
	UNWRAP_NO_DECL(frame);
	new_effect_no_follow(name, pos, rot, scale, rgba, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::CLEAR_EFFECT(ScriptArg args) {
	UNWRAP(name, std::string);
	int instance_id = 0;
	UNWRAP_NO_DECL(instance_id);
	clear_effect(name, instance_id);
}

void BattleObject::CLEAR_EFFECT_ALL(ScriptArg args) {
	clear_effect_all();
}

void BattleObject::PRINT_MSG_FROM_SCRIPT(ScriptArg args) {
	UNWRAP(message, std::string);
	std::cout << message;
}