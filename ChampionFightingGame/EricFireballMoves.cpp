#include "EricFireball.h"
#include "EricFireballConstants.h"

void EricFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("hover_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);		
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35),
				COLLISION_KIND_GROUND | COLLISION_KIND_PROJECTILE,
				HitResult().damage(50).meter(30).hit(8, 21).block(8, 21)
				.j_max(1).anims("heavy_mid", "heavy_high", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().ground(100.0, 100.0).frames(5), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(8, 0).j_start(1).j_inc(1).j_max(1),
				HIT_STATUS_LAUNCH, HitMove().launch(22.0, 1.3, 12.0, 22.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("hover_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35),
				COLLISION_KIND_GROUND | COLLISION_KIND_PROJECTILE,
				HitResult().damage(50).meter(30).hit(8, 21).block(8, 21)
				.j_max(1).anims("heavy_mid", "heavy_high", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().ground(100.0, 100.0).frames(5), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35), COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(8, 0).j_start(1).j_inc(1).j_max(1),
				HIT_STATUS_LAUNCH, HitMove().launch(22.0, 1.3, 12.0, 22.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("hover_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(50).meter(30).hit(8, 0).block(8, 21).j_start(1).j_inc(1)
				.j_max(1).anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH,
				HitMove().ground(0.0, 100.0).frames(5).launch(30.0, 1.3, 12.0, 8.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("hover_ex", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 35),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(50).hit(8, 0).block(8, 21).j_start(1).j_inc(1).j_max(1)
				.anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH,
				HitMove().ground(0.0, 100.0).frames(5).launch(22.0, 1.3, 12.0, 8.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("punched_l", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 25),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(30).meter(18).hit(10, 22).block(10, 10).j_start(1).j_inc(1)
				.j_max(4).anims("heavy_mid", "heavy_high", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(160.0, 160.0).air(80.0, 25.0).frames(9)
				.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("punched_m", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 25),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(40).meter(24).hit(10, 18).block(10, 10).j_start(1).j_inc(1)
				.j_max(4).anims("heavy_mid", "heavy_high", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(180.0, 180.0).air(80.0, 25.0).frames(9)
				.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("punched_h", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 25),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(50).meter(30).hit(10, 18).block(10, 10).j_start(1).j_inc(1)
				.j_max(4).anims("heavy_mid", "heavy_high", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(180.0, 180.0).air(80.0, 25.0).frames(9)
				.launch(10.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("punched_ex", [this]() {
		execute_frame(1, [this]() {
			push_function(&Projectile::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
			push_function(&Projectile::NEW_HURTBOX, 0, glm::vec2(-50, -35), glm::vec2(50, 35), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0,
				glm::vec2(-50, -35), glm::vec2(50, 25),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(30).hit(3, 18).block(3, 10).j_start(1).j_inc(1).j_max(4)
				.anims("heavy_mid", "heavy_high", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().ground(8.0, 8.0).air(3.0, 5.0).frames(9).launch(10.0, 1.3, 15.0, 8.0),
				HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
			push_function(&Projectile::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1,
				glm::vec2(-50, -35), glm::vec2(50, 15),
				COLLISION_KIND_GROUND | COLLISION_KIND_AIR | COLLISION_KIND_PROJECTILE,
				HitResult().damage(30).hit(10, 18).block(10, 10).j_start(1).j_inc(1)
				.j_max(5).anims("heavy_mid", "heavy_high", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(18.0, 18.0).air(8.0, 25.0).frames(9)
				.launch(14.0, 1.3, 15.0, 8.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", ""
			);
		});
	});
	script("hit", [this]() {

	});
}