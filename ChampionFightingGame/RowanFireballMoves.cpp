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
			switch (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
				case (SPECIAL_LEVEL_EX): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 50.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, 
						/*Hitlag*/ 8, /*Hitstun*/ 0, /*Blocklag*/ 12, /*Blockstun*/ 21,
						HIT_STATUS_LAUNCH, HitResult().ground(0.0, 10.0).frames(5)
						.launch(30.0, 1.3, 12.0, 8.0), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, 
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
				case (SPECIAL_LEVEL_H): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 50.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, 
						/*Hitlag*/ 8, /*Hitstun*/ 0, /*Blocklag*/ 12, /*Blockstun*/ 21,
						HIT_STATUS_LAUNCH, HitResult().ground(0.0, 10.0).frames(5)
						.launch(30.0, 1.3, 12.0, 8.0), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, 
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
				case (SPECIAL_LEVEL_M):
				case (SPECIAL_LEVEL_L): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 50.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, 
						/*Hitlag*/ 8, /*Hitstun*/ 0, /*Blocklag*/ 12, /*Blockstun*/ 21,
						HIT_STATUS_LAUNCH, HitResult().ground(0.0, 10.0).frames(5)
						.launch(22.0, 1.3, 12.0, 22.0), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID,  
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
			}
		});
	});
	script("punched", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			switch (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
				case (SPECIAL_LEVEL_EX): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 25), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, 
						/*Hitlag*/ 3, /*Hitstun*/ 18, /*Blocklag*/ 3, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(8.0, 8.0).air(3.0, 5.0).frames(9)
						.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_DISABLE_HITSTUN_PARRY 
						| HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1, 
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, 
						"", ""
					);
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, 
						glm::vec2(-50, -35), glm::vec2(50, 25), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, 
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, 
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
				case (SPECIAL_LEVEL_H): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 25),
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
						/*Damage*/ 50.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0,
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, 
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, 
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
				case (SPECIAL_LEVEL_M): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 25), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 40.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0,
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID,
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
				case (SPECIAL_LEVEL_L): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 25), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0,
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
						/*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID,
						DAMAGE_KIND_CHIP, "", ""
					);
				} break;
			}
		});
	});
	script("kicked", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.8), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			switch (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL]) {
				case (SPECIAL_LEVEL_EX): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, 
						/*Hitlag*/ 3, /*Hitstun*/ 18, /*Blocklag*/ 3, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(8.0, 8.0).air(3.0, 5.0).frames(9)
						.launch(10.0, 1.3, 15.0, 1.0), HIT_FLAG_DISABLE_HITSTUN_PARRY 
						| HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1, 
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, 
						"", ""
					);
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, 
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 1.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1, 
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP,
						"", ""
					);
				} break;
				case (SPECIAL_LEVEL_H): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35),
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 50.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, 
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 1.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1,
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP,
						"", ""
					);
				} break;
				case (SPECIAL_LEVEL_M): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35), 
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE, 
						/*Damage*/ 40.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, 
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 1.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1,
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP,
						"", ""
					);
				} break;
				case (SPECIAL_LEVEL_L): {
					push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, 
						glm::vec2(-50, -35), glm::vec2(50, 35),
						COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
						/*Damage*/ 30.0, /*Chip Damage*/ 0.2f, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, 
						/*Hitlag*/ 10, /*Hitstun*/ 18, /*Blocklag*/ 10, /*Blockstun*/ 10,
						HIT_STATUS_NORMAL, HitResult().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
						.launch(10.0, 1.3, 15.0, 1.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, /*Juggle Start*/ 1,
						/*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP,
						"", ""
					);
				} break;
			}
		});
	});
	script("hit", [this]() {

	});
}