#include "Fighter.h"

/// <summary>
/// Creates a hurtbox object for a Fighter. This hurtbox will remain active until it is cleared, either manually or through changing statuses, and it 
/// will move to follow the player's position.
/// </summary>
/// <param name="id">: A value of 0-9. Each ID can store exactly 1 hurtbox, and overwriting a pre-existing ID will remove the hurtbox originally mapped 
/// to that ID.</param>
/// <param name="anchor">: The bottom left corner of the hurtbox.</param>
/// <param name="offset">: The top right corner of the hurtbox.</param>
/// <param name="hurtbox_kind">: Options are NORMAL, COUNTER, RIGHT_OF_WAY. NORMAL hurtboxes can be hit by anything, COUNTER hurtboxes will cause the 
/// defender to go into a counter event if a hitbox connects with them, and RIGHT_OF_WAY is used by Leon while he has Right of Way.</param>
/// <param name="armor">: Whether or not the hitbox has armor.</param>
/// <param name="intangible_kind">: What kinds of hitboxes this hurtbox is immune to. Options are: NORMAL (immune to all attacks but not grabs or 
/// projectiles), HIGH, MID, and LOW (immune to attacks that match the height), PROJECTILE (immune to projectiles), THROW (immune to throws), NONE
/// (immune to nothing), ALL (immune to everything), and INVINCIBLE (same as ALL except the attacker also goes into hitlag).</param>
void Fighter::new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, int hurtbox_kind, bool armor, int intangible_kind) {
	if (id < 10) {
		hurtboxes[id].activate(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}