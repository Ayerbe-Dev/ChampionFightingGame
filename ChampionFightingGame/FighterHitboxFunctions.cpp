#include "Fighter.h"

/// <summary>
/// Creates a hitbox object for a Fighter. This hitbox will remain active until it is cleared, either manually or through changing statuses, and it will
/// move to follow the player's position unless it is set to remain in a specific position.
/// </summary>
/// <param name="id">: A value of 0-9. Each ID can store exactly 1 hitbox, and overwriting a pre-existing ID will remove the hitbox originally mapped 
/// to that ID.</param>
/// <param name="multihit">: A secondary ID. When a hitbox connects, all hitboxes that share the multihit value will be marked as having successfully 
/// hit.</param>
/// <param name="damage">: How much damage the move deals on hit.</param>
/// <param name="chip_damage">: How much damage the move deals on block.</param>
/// <param name="counterhit_damage_mul">: How much the move's damage is multiplied by on counterhit. 1.2 is the standard value.</param>
/// <param name="scale">: How much damage scaling is added if the opponent is hit during a combo. Not added to the first hit of a combo.</param>
/// <param name="anchor">: The bottom left corner of the hitbox.</param>
/// <param name="offset">: The top right corner of the hitbox.</param>
/// <param name="hitbox_kind">: Just make it HITBOX_KIND_NORMAL for now</param>
/// <param name="meter_gain_on_hit">: How much meter the attacker will gain if their attack hits.</param>
/// <param name="meter_gain_on_counterhit">: How much meter the attacker will gain if their attack is a counterhit.</param>
/// <param name="meter_gain_on_block">: How much meter the attacker will gain if their attack is blocked. This param / 2 is used if an armored opponent 
/// is hit.</param>
/// <param name="situation_hit">: What situations the defender must be in for the move to connect. Options are: SITUATION_HIT_GROUND, SITUATION_HIT_AIR,
/// SITUATION_HIT_GROUND_AIR, SITUATION_HIT_DOWN, SITUATION_HIT_ALL. If the hitbox collides with a hurtbox but the defender is in the wrong state, all
/// future hitboxes will still be checked.</param>
/// <param name="hitlag">: Period where both players are frozen after an attack hits.</param>
/// <param name="hitstun">: Period where the defender cannot act after getting hit.</param>
/// <param name="blocklag">: Period where both players are frozen after an attack is blocked.</param>
/// <param name="blockstun">: Period where the defender cannot act after blocking an attack.</param>
/// <param name="unblockable">: Whether or not a move will ignore the opponent's block. Note: Unblockable moves can still be parried.</param>
/// <param name="attack_height">: If the move has to be blocked high/low, and what direction it needs to be parried in.</param>
/// <param name="attack_level">: If the move is Light, Medium, or Heavy. Determines what hitstun animation the defender is put in and which hitbox has
/// priority in the event of a trade.</param>
/// <param name="hit_pushback">: How far the opponent is pushed back after getting hit.</param>
/// <param name="block_pushback">: How far the opponent is pushed back after blocking a move.</param>
/// <param name="Notes on both forms of pushback">: Pushback is applied during the first half of hitstun/blockstun, odd numbers rounded up. If the 
/// defender is pushed into a wall during their pushback, the attacker is pushed backwards instead. If a move hits/is blocked in the air, pushback is
/// applied both up and away from the attacker.</param>
/// <param name="clank_kind">: What to do if two people hit each other at the same time with hitboxes of the same Attack Level. NORMAL: Both get hit, 
/// CLANK: Both go into a clank animation and get pushed away, CONTINUE: The other player goes into a clank animation, while your hitbox gets canceled
/// but the animation for your current move continues.</param>
/// <param name="juggle_start">: If the defender's juggle value is below this one, theirs is set to this.</param>
/// <param name="juggle_increase">: Otherwise, it is increased by this much.</param>
/// <param name="max_juggle">: The highest juggle value that an aerial opponent can have before this hitbox automatically whiffs.</param>
/// <param name="hit_status">: What status to put the defender in if the move hits normally. Passing HIT_STATUS_NORMAL results in the regular damage
/// animation based on the defender's situation.</param>
/// <param name="counterhit_status">: What status to put the defender in on counterhit.</param>
/// <param name="counterhit_type">: The situations under which this hitbox can counterhit. NONE: Can't counterhit, NORMAL: Can counterhit if the 
/// defender's counterhitable flag is on, AERIAL: Can counterhit if the defender's counterhit flag is on and they're in the air.</param>
/// <param name="launch_init_y">: The initial vertical speed to set the opponent to. Used only if they're either already in the air or in a status that
/// will put them there.</param>
/// <param name="launch_gravity_y">: Opponent's gravity is set to this value during the launch status.</param>
/// <param name="launch_max_fall_speed">: Opponent's max fall speed is set to this value during the launch status.</param>
/// <param name="launch_speed_x">: The horizontal speed to set the opponent to.</param>
/// <param name="continue_launch">: Whether or not the move can extend an opponent's launch status even if its hit status isn't HIT_STATUS_LAUNCH.
/// Generally this value should be true for heavy normals, false for light normals, and case-by-case for medium normals.</param>
/// <param name="can_chip_ko">: Whether or not the chip damage for this move is able to KO the defender. Generally should be false for normals, true for
/// specials and supers.</param>
/// <param name="can_ko">: Whether or not this move can kill at all. Should only be false during some cutscene supers.</param>
/// <param name="use_player_pos">: If the anchor and offset coords are based on the player position, or a static point on the screen. True by default.</param>
void Fighter::new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, glm::vec2 anchor,
	glm::vec2 offset, int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit,
	int hitlag, int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback,
	int clank_kind, int juggle_start, int juggle_increase, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y,
	float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool can_ko, bool use_player_pos) {
	if (id < 10) {
		hitboxes[id].activate(this, id, multihit, damage, chip_damage, counterhit_damage_mul, scale, anchor, offset, hitbox_kind, meter_gain_on_hit,
			meter_gain_on_counterhit, meter_gain_on_block, situation_hit, hitlag, hitstun, blocklag, blockstun, unblockable, attack_height,
			attack_level, hit_pushback, block_pushback, clank_kind, juggle_start, juggle_increase, max_juggle, hit_status, counterhit_status,
			counterhit_type, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x, continue_launch, can_chip_ko, can_ko, use_player_pos);
	}
}