#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void RowanFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("hover", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 8, /*Blocklag*/ 12, /*Hitstun*/ 0, /*Blockstun*/ 21, /*Block Pushback Ground*/ 10.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 30.0, /*Launch Gravity*/ 1.5, /*Launch Max Fall Speed*/ 12.0, /*Launch Speed X*/ 8.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, -1, -1);
			}
			else {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 8, /*Blocklag*/ 12, /*Hitstun*/ 0, /*Blockstun*/ 21, /*Block Pushback Ground*/ 10.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 30.0, /*Launch Gravity*/ 1.5, /*Launch Max Fall Speed*/ 12.0, /*Launch Speed X*/ 8.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, -1, -1);
			}
		});
	});
	script("punched", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 3, /*Blocklag*/ 3, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 8.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 5.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
		});
	});
	script("kicked", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.8), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
			if (object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 3, /*Blocklag*/ 3, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 8.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 5.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
				push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 10, /*Blocklag*/ 10, /*Hitstun*/ 18, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 18.0, /*Block Pushback Ground*/ 18.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 9, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, -1, -1);
			}
		});
	});
	script("hit", [this]() {

	});
	script("deactivate", [this]() {

	});
}