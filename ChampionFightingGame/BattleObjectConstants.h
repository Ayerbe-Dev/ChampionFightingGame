#pragma once
enum BattleObjectType {
	BATTLE_OBJECT_TYPE_FIGHTER,
	BATTLE_OBJECT_TYPE_PROJECTILE,

	BATTLE_OBJECT_TYPE_MAX
};

enum IncomingCollisionKind {
	INCOMING_COLLISION_KIND_NONE,
	INCOMING_COLLISION_KIND_HIT,
	INCOMING_COLLISION_KIND_BLOCKED,
	INCOMING_COLLISION_KIND_PARRIED,
	INCOMING_COLLISION_KIND_HITSTUN_PARRIED,
	INCOMING_COLLISION_KIND_ARMORED,
	INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED,
	INCOMING_COLLISION_KIND_INVINCIBILITY,
	INCOMING_COLLISION_KIND_COUNTER,
	INCOMING_COLLISION_KIND_MAX
};