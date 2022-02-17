#include "BattleObject.h"

/// <summary>
/// Updates the position of a grabbox relative to the object. 
/// </summary>
void BattleObject::update_grabbox_pos() {
	for (int i = 0; i < 10; i++) {
		if (grabboxes[i].id != -1) {
			grabboxes[i].update_pos(this);
		}
	}
}

/// <summary>
/// Clear the grabbox with the specified ID.
/// </summary>
/// <param name="id">: The ID of the grabbox to clear.</param>
void BattleObject::clear_grabbox(int id) {
	grabboxes[id].clear();
}

/// <summary>
/// Clear all active grabboxes.
/// </summary>
void BattleObject::clear_grabbox_all() {
	for (int i = 0; i < 10; i++) {
		grabboxes[i].clear();
	}
}

#include "Fighter.h"

/// <summary>
/// Creates a grabbox object for a Fighter. This grabbox will remain active until it is cleared, either manually or through changing statuses, and it 
/// will move to follow the player's position unless it is set to remain in a specific position.
/// </summary>
/// <param name="id">: A value of 0-9. Each ID can store exactly 1 grabbox, and overwriting a pre-existing ID will remove the grabbox originally mapped 
/// to that ID.</param>
/// <param name="anchor">: The bottom left corner of the grabbox.</param>
/// <param name="offset">: The top right corner of the grabbox.</param>
/// <param name="grabbox_kind">: Either NORMAL, NOTECH, HITSTUN, or HITSTUN_NOTECH. NOTECH marks a grabbox as untechable, while HITSTUN allows a grab 
/// to connect against an opponent who's currently in hitstun. Use NORMAL for regular throws, HITSTUN for air throws, and either of the NOTECH options
/// for command throws.</param>
/// <param name="situation_hit">: What situations the defender must be in for the move to connect. Options are: SITUATION_HIT_GROUND, SITUATION_HIT_AIR,
/// SITUATION_HIT_GROUND_AIR, SITUATION_HIT_DOWN, SITUATION_HIT_ALL. If the grabbox collides with a hurtbox but the defender is in the wrong state, all
/// future grabboxes will still be checked.</param>
/// <param name="attacker_status_if_hit">: The status to put the attacker in upon a successful hit.</param>
/// <param name="defender_status_if_hit">: The status to put the defender in upon a successful hit.</param>
/// <param name="use_player_pos">: If the anchor and offset coords are based on the player position, or a static point on the screen. True by default.</param>
void Fighter::new_grabbox(int id, vec2 anchor, vec2 offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	if (id < 10) {
		grabboxes[id].activate(this, id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
	}
}