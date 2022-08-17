#include "BattleObject.h"

/// <summary>
/// Load all entries for this object's stats table.
/// </summary>
void BattleObject::load_stats() {
	stats.load_params(resource_dir + "/param/stats.yml");
}

/// <summary>
/// Load all entries for this object's param table.
/// </summary>
void BattleObject::load_params() {
	params.load_params(resource_dir + "/param/params.yml");
}