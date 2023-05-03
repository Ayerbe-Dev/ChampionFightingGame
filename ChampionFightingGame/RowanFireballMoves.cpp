#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void RowanFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("hover", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
		});
	});
	script("punched", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 40.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 50.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
				push_function(&Projectile::NEW_HITBOX, 1, 1, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
		});
	});
	script("kicked", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.8), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 40.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 50.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
				push_function(&Projectile::NEW_HITBOX, 1, 1, 30.0, 5.0, 1, 15.0, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, DAMAGE_KIND_CHIP_KO, true, false, 10.0, 0.0, 0.0, 1.0);
			}
		});
	});
	script("hit", [this]() {

	});
	script("fall", [this]() {

	});
	script("ground", [this]() {

	});
}