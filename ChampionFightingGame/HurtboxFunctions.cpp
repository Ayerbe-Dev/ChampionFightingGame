#include "Object.h"

/// <summary>
/// Updates the position of a hurtbox relative to the object. 
/// </summary>
void Object::update_hurtbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (hurtboxes[i].id != -1) {
			hurtboxes[i].update_pos(this);
		}
	}
}

/// <summary>
/// Clear the hurtbox with the specified ID.
/// </summary>
/// <param name="id">: The ID of the hurtbox to clear.</param>
void Object::clear_hurtbox(int id) {
	hurtboxes[id].clear();
}

/// <summary>
/// Clear all active hurtboxes.
/// </summary>
void Object::clear_hurtbox_all() {
	for (int i = 0; i < 10; i++) {
		hurtboxes[i].clear();
	}
}

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
void Fighter::new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind) {
	if (id < 10) {
		hurtboxes[id] = Hurtbox(this, id, anchor, offset, hurtbox_kind, armor, intangible_kind);
	}
}